// spwm.h - Header file for SPWM implementation on Raspberry Pi Pico
#ifndef SPWM_H
#define SPWM_H

#include "stdint.h"
typedef unsigned int uint;

// Function to initialize SPWM with configurable PWM-capable pins
void spwm_init(uint pin_a, uint pin_b, uint pin_c);

// Function to start SPWM output
void spwm_start();

// Function to stop SPWM output
void spwm_stop();

// Function to update the PWM frequency dynamically
void spwm_set_frequency(uint32_t new_freq);

#endif // SPWM_H

/*
// example usage, including optional changing of the frequency

#include "pico/stdlib.h"
#include "spwm.h"

#define PIN_A 2  // Select PWM-capable pins
#define PIN_B 3
#define PIN_C 4

int main() {
    stdio_init_all(); // Initialize standard I/O (optional, for debugging)

    // Initialize SPWM on selected pins
    spwm_init(PIN_A, PIN_B, PIN_C);

    // Start SPWM
    spwm_start();

    // Change frequency every second
    uint32_t new_freq = 15000;
    while (true) {
        sleep_ms(1000); // Wait 1 second
        new_freq = (new_freq == 15000) ? 20000 : 15000; // Toggle between 15kHz and 20kHz
        spwm_set_frequency(new_freq);
    }

    return 0;
}

*/