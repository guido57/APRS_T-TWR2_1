# ESP32APRS T-TWR Version 2.1

## OVERVIEW

- The original code https://github.com/nakhonthai/ESP32APRS_T-TWR version 0.5 was written for T-TWR Version 2.0 among others.
- This is a simple adaptation to work with LILYGO T-TWR Version 2.1
- I never tried the original version 0.5 on a T-TWR 2.0 but on my T-TWR 2.1 it didn't work because never turned on the SA866S TX.

For all the details please refer to https://github.com/nakhonthai/ESP32APRS_T-TWR
except for the following modifications.

## MODIFICATIONS

- The main modification I did was to add     hw_afsk_dac_isr = true;  
  inside the function void DAC_TimerEnable(bool sts) At line 467 on AFSK.cpp

  ```
  void DAC_TimerEnable(bool sts)
    {
      if (timer_dac == NULL)
        return;
      // portENTER_CRITICAL_ISR(&timerMux);
      if (sts == true)
      {
        timerStart(timer_dac);
        hw_afsk_dac_isr = true; //GG
      }
      else
      {
        timerStop(timer_dac);
      }
      // portEXIT_CRITICAL_ISR(&timerMux);
      dacEn = 0;
    }
  ```
  
  in this way the DAC timer function can correctly work. See

  void IRAM_ATTR sample_dac_isr()

  at line 939 of AFSK.cpp

  In my opinion, without this modification the software https://github.com/nakhonthai/ESP32APRS_T-TWR version 0.5 cannot work on T-TWR Version 2.0 either, but I'm not sure because I don't own that hardware
😃

- taskAPRS in main.cpp called pkgListUpdate skipping the last character of the body.
  Similarly, both event_lastHeard in webservice.cpp and dispWindow in gui_lcd.cpp called parse_aprs without the last character.

  No problem if that character was the last of the comment, but if it was the last character of the location parse_aprs failed. Corrected at lines:

  6687 main.cpp
  
  6785 gui_lcd.cpp

  874 webservice.cpp

- In function RF_MODULE_CHECK in main.cpp, PULLDOWN_PIN must be set as OUTPUT before setting it to LOW.

  ```
  2939   pinMode(PULLDOWN_PIN, OUTPUT);
  ```
  # INSTALLATION

  Use the following method https://github.com/nakhonthai/ESP32APRS_T-TWR?tab=readme-ov-file#platformio-quick-start
 


  # SETTINGS

  Use the following settings:

 ![images/MOD.png](images/MOD.png)

 ![images/Radio.png](images/Radio.png)
 
 ![images/Radio.png](images/TRACKER.png)

 ![images/DashBoard.png](images/DashBoard.png)
 
      
