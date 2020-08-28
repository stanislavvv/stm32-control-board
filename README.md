# Control Board     {#mainpage}

This was a copy of [stm32-base](https://github.com/stanislavvv/stm32-base).

Someday i will create control board for transceiver with LCD+encoder+...

## ToDo:

### 1. Base hardware interfaces:

  * display (ST7789 at now) -- WiP
  * buttons (one pin, one button or multiplexing?)
  * encoder with button (interrupt or timer?)
  * serial interface (shell) -- **done**, some commands may be added later
  * testing adc + pwm with basic filtering

### 2. Beginning radio:

  * si5351 (quadrature output + may be modulate third output)
  * quadrature ADC input from mixer and AM,NFM,SSB demodulation
  * output to audio codec (may be PWM for testing)

### 3. Radio goes transmitting:

  * audio mono input
  * AM,NFM,SSB digital modulation
  * quadrature output to mixer
  * modulated si5351 third output

### 4. Better radio:

  * digital filtering of modulating and demodulated audio
  * standard CAT interface
