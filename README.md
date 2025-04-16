# Sinusoidal_Pulse_Width_Modulation_Pico_Example
Example project for a pico which will generate three phase sinwave with a 120 degree offset.

# Inital build

To get started run init.sh
``` bash
bash init.sh
```

# Building

To build after that
1)
``` bash
cd build
```
2)
``` bash
make
```

put the Pico into bootloader mode using the button on top, while connecting it to your computer.

after running the setup script, go into the newly created build folder and copy the file "spwm_demo.uf2" onto the device
