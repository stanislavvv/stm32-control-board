# Control Board

This is old copy of [stm32-base](https://github.com/stanislavvv/stm32-control-board) at this moment.

Someday i will create control board with LCD+encoder+...

## ToDo:

### 1:

  * display (and choose it)
  * buttons (choose input method)
  * encoder with button (interrupt or timer?)
  * serial interface (shell)

may be testing adc + pwm

### 2:

  * si5351 (quadrature output + may be modulate third output)
  * quadrature ADC input from mixer and AM,NFM,SSB demodulation
  * output to audio codec (may be PWM for testing)

### 3:

  * audio mono input
  * AM,NFM,SSB digital modulation
  * quadrature output to mixer
  * modulated si5351 third output

### 4:

  * digital filtering of modulating and demodulated audio
