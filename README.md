# ESP32APRS T-TWR Version 2.1

## OVERVIEW

- The original code https://github.com/nakhonthai/ESP32APRS_T-TWR version 0.5 was written for T-TWR Version 2.0 among others.
- This is a simple adaptation to work with LILYGO T-TWR Version 2.1
- I never tried the original version 0.5 on a T-TWR 2.0 but on my T-TWR 2.1 it didn't work because never turned on the SA866S TX.

For all the details please refer to https://github.com/nakhonthai/ESP32APRS_T-TWR
except for the following modifications.

## MODIFICATIONS

- The main modification I did was to add

  ```
     hw_afsk_dac_isr = true; 
  ```
  inside the function

  void DAC_TimerEnable(bool sts)

at line 467 on AFSK.cpp
