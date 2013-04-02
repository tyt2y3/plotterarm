This folder contains the embedded C program for the STM32 board that controls the robotic plotter arm.
- //(deprecated) `data.c` is where the plotting operations are put. several example graphics are included. now data is transfered on the fly via serial port
- `lcd.c`, `lcd.h` is the manufacturer-provided utility to print on on-board lcd
- `log.c` uses the lcd as an output terminal
- `main.c` is the program entry point
- `motor.c` is a class to control servo motors
- `plotter.c` is the application that controls a plotter arm
- `pwm.c` is the STM32 setup of PWM output
- `protocol.c` is the data transfer protocol
- `serial.c` provides an interface to send and receive data via USARTy serial port.
