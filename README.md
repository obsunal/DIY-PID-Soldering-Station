# DIY-PID-Soldering-Station

Soldering Iron/Heat Gun Controller

The goal was take a some knockoff known good brand soldering iron (JCB, Hakko, Weller...) and make an overengineered control box for it.

In the end I want to feature;
- Two devices, most likely one of them being hot air (the one where they have blower inside the station.)
- PID control with double mode agressive and laid-back configurations
- AutoTune algorithm for PID.
- Stand idle mode
- Shutdown time
- Amount of power delivered
- Has Prusa like interface (rotary encoder with couple of buttons)
- Interface featuring variable control (eg. idle temp, PID variables...)
- Can be reprogrammed somewhat easily (via bootloader and/or ICSP headder)
Bonus points
- Feature selectable control methods
- Deployed multiple platforms (PIC, AVR, Xtensa-ESP32, ARM-MSP STM32)
