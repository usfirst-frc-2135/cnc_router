
# cnc_router - K2CNC router software and settings including 2135 customizations

## Repository structure

Since K2CNC has gone out of business, there is no online reference to what the default settings should be for the KG-3925HD-5 and KFLOP controller. The model number stands for 39" x 25" cutting surface with a heavy duty (HD) table and 5" Z height.

The KFLOP and KMotion541 (was KMotion434) directories were copied from our Dell PC that controls the router. Very little has been changed from the original installation other than some notes. The router is now going through an upgrade to a Beelink
mini-PC on Windows 11.

The K2_*.c files are located in the KFLOP directory per the original installation. These four files are what were installed by the K2 CNC Manufacturing company when it was first built. The black controller box (Rev. 3.5) was manufactured by K2CNC and consists of a KFLOP with an interface board. The Dell PC connects to the KFLOP controller through a USB 2.0 cable. The KFLOP is a multithreaded OS consisting of 7 threads. The three K2*.c programs run in these three threads using a time-slice OS.

In order to install the programs, use the KMotion.exe software provided by Dynomotion (not KMotionCNC.exe which controls the router movements). KMotion provides low level tuning and configuration access to the KFLOP board. It can also be used to update the firmware in the KFLOP and download these programs. Any time the firmware is updated in the KFLOP, these K2*.c programs should be re-downloaded and flashed into the KFLOP as well.

Note that each program has an assigned thread locaton:

- "K2_Init Servo.c" 
  - Initializes the servo controllers in the KFLOP and contains any tuning settings for this router. 
  - This runs when the *INIT* button is pressed in KMotionCNC
  - Correct settings for the soft limits must be programmed (X: 24.7" Y: 38.8" for the KG-3925 router)
  - Backlash settings based on recent tuning of the axes (X and Y axes are 10403.84 steps per inch)
    - X: 0.001" @ 50 msec rate = 10 steps / 208 steps/sec, 
    - Y: 0.003" @ 50 msec rate = 30 steps / 624 steps/sec
  - If axis needs to be plotted for PID tuning, see Bit 30 operation
  - Runs in thread 1

- "K2MC_driver.c" 
  - The main control program for running the router and must be executed for KMotionCNC to work.
  - Runs in thread 6
  - Note that thread 6 MUST be enabled to "Launch on Power Up" in the KMotion Config and Flash screen.

- "K2_SpindlwPWM.c" 
  - Controls the PWM to drive the speed of the spindle by changing the PWM (bit 33 on KFLOP)
  - Couldn't quite figure out how bit 33 gets to JP6 external connector pin SP
  - This does not control the spindle on/off relay - that is done in the KMotionCNC button configuration
  - Runs in thread 3

- "K2_Home.c" 
  - this is the homing program when the *Home* button is pressed in KMotionCNC
  - Runs in thread 7

	Note that SPI is Steps Per Inch

# Default threads (believed to be as delivered from K2 Machining)

Note that the number in parentheses is the thread number.

```
./KFLOP/K2_Init Servo(1).out
./KMotion434/C Programs/InitStepDir3Axis(2).out
./KFLOP/K2_SpindlePWM(3).out
./KMotion434/C Programs/SetStepPulseLength(4).out
./KMotion434/C Programs/CaptureXYZMotionToFile(5).out
./KFLOP/Driver/K2MC_driver(6).out
./KFLOP/K2_Home(7).out```

