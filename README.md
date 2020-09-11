# Control Board     {#mainpage}

Someday i will create control board for transceiver with LCD+encoder+...

## Compile prerequisites:

On Debian (and may be on Ubuntu):

```
apt-get update
apt-get install make gcc-arm-none-eabi libnewlib-dev doxygen cppcheck vera++ stlink-tools
```

## Compilation and related:

  * `make clean` -- clean up sources from compile-time artifacts
  * `make` -- simply make `main.elf` binary
  * `make test` -- run tests on some functions (not all)
  * `make check` -- run `cppcheck` and `vera++` on `*.c` and `*.h` with some configs
  * `make bin` -- make `main.bin` firmware
  * `make main.o` -- make `main.o` object file from `main.c` sources, if you need it separately. You may make `*.o` from any `*.c`.
  * `make flash` -- run `st-flash` to program microcontroller via st-link

  * `mk/cppcheck.includes` -- config for `cppcheck` in `make check`
  * `mk/vera++.excl` -- config for `vera++` in `make check`

## Ready:

### 1. Base hardware interfaces:

  * serial interface (shell) -- some commands may be added on demand

## ToDo:

### 1. Base hardware interfaces:

  * display (ST7789 or PCD8544) on SPI
  * buttons (one pin, one button or multiplexing?)
  * encoder with button (interrupt or timer?)
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
