// spwm.c - SPWM implementation for Raspberry Pi Pico
#include "spwm.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/timer.h"
#include <math.h>

#define DEFAULT_PWM_FREQ 20000  // Default 20 kHz PWM frequency
#define SINE_TABLE_SIZE 256
#define SYSTEM_CLOCK 125000000 // 125 MHz Pico clock

static uint32_t pwm_freq = DEFAULT_PWM_FREQ;
static uint32_t timer_interval_us;
static uint8_t sine_table[SINE_TABLE_SIZE];
static uint slice_a, slice_b, slice_c;
static uint chan_a, chan_b, chan_c;
static repeating_timer_t spwm_timer;
static uint16_t index = 0;

// Configurable PWM output pins
static uint gpio_a = 2; // Changeable to any valid PWM-capable pin
static uint gpio_b = 3; // Changeable to any valid PWM-capable pin
static uint gpio_c = 4; // Changeable to any valid PWM-capable pin

/*
 * Valid PWM-capable GPIO pins on Raspberry Pi Pico:
 * - GPIO 0 - GPIO 15
 * - GPIO 16, 17 (only on certain boards with exposed pins)
 * Each PWM slice has two channels (A & B), so ensure selected pins belong to different channels.
 */

// Generate sine lookup table
void generate_sine_table() {
    for (int i = 0; i < SINE_TABLE_SIZE; i++) {
        sine_table[i] = (uint8_t)((sin(2 * M_PI * i / SINE_TABLE_SIZE) * 0.5 + 0.5) * 255);
    }
}

// Timer ISR callback
bool spwm_timer_callback(struct repeating_timer *t) {
    uint16_t index_a = index;
    uint16_t index_b = (index + SINE_TABLE_SIZE / 3) % SINE_TABLE_SIZE;
    uint16_t index_c = (index + 2 * SINE_TABLE_SIZE / 3) % SINE_TABLE_SIZE;
    
    pwm_set_gpio_level(gpio_a, sine_table[index_a]);
    pwm_set_gpio_level(gpio_b, sine_table[index_b]);
    pwm_set_gpio_level(gpio_c, sine_table[index_c]);
    
    index = (index + 1) % SINE_TABLE_SIZE;
    return true;
}

// Initialize PWM and sine table
void spwm_init(uint pin_a, uint pin_b, uint pin_c) {
    gpio_a = pin_a;
    gpio_b = pin_b;
    gpio_c = pin_c;
    
    generate_sine_table();
    gpio_set_function(gpio_a, GPIO_FUNC_PWM);
    gpio_set_function(gpio_b, GPIO_FUNC_PWM);
    gpio_set_function(gpio_c, GPIO_FUNC_PWM);
    
    slice_a = pwm_gpio_to_slice_num(gpio_a);
    slice_b = pwm_gpio_to_slice_num(gpio_b);
    slice_c = pwm_gpio_to_slice_num(gpio_c);
    
    chan_a = pwm_gpio_to_channel(gpio_a);
    chan_b = pwm_gpio_to_channel(gpio_b);
    chan_c = pwm_gpio_to_channel(gpio_c);
    
    pwm_set_wrap(slice_a, 255);
    pwm_set_wrap(slice_b, 255);
    pwm_set_wrap(slice_c, 255);
    
    pwm_set_enabled(slice_a, false);
    pwm_set_enabled(slice_b, false);
    pwm_set_enabled(slice_c, false);
    
    timer_interval_us = 1000000 / (pwm_freq * SINE_TABLE_SIZE);
}

// Start SPWM output
void spwm_start() {
    pwm_set_enabled(slice_a, true);
    pwm_set_enabled(slice_b, true);
    pwm_set_enabled(slice_c, true);
    add_repeating_timer_us(-timer_interval_us, spwm_timer_callback, NULL, &spwm_timer);
}

// Stop SPWM output
void spwm_stop() {
    pwm_set_enabled(slice_a, false);
    pwm_set_enabled(slice_b, false);
    pwm_set_enabled(slice_c, false);
    cancel_repeating_timer(&spwm_timer);
}

// Update PWM frequency dynamically
void spwm_set_frequency(uint32_t new_freq) {
    pwm_freq = new_freq;
    timer_interval_us = 1000000 / (pwm_freq * SINE_TABLE_SIZE);

    // Stop the current timer
    cancel_repeating_timer(&spwm_timer);

    // Start a new timer with the updated interval
    add_repeating_timer_us(-timer_interval_us, spwm_timer_callback, NULL, &spwm_timer);
}
