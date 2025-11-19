/*
 * Starter code for ECE 266 Lab 7, Sound Player (PWM), fall 2024
 *
 * music_led.c: Set buzzer for a music note
 *
 * Created by Zhao Zhang
 */

#include "music.h"
#include "pwm_led.h"
#include "pwm_buzzer.h"

/*
 * PWM parameters for given pitch and volume levels.
 */

int music_freq[NUM_PITCH] =
{
 //C4: 261.63 Hz, Period =  50 MHz / 261.63 Hz = 191109.58
    191110,

    //D4: 293.66 Hz, Period =  50 MHz / 293.66 Hz = 170264.93
    170265,

    //E4: 329.63 Hz, Period = 50 MHz / 329.63 Hz = 151685.22
    151685,

    //F4: 349.23 Hz, Period = 50 MHz / 349.23 Hz = 143172.12
    143172,

    //G4: 392.00 Hz, Period = 50 MHz / 392.00 Hz = 127551.02
    127551,

    //A4: 440.00 Hz, Period = 50 MHz / 440.00 Hz = 113636.36
    113636,

    // B4: 493.88, Period = 50 MHz / 493.88 Hz = 101239.17
    101239
};

int music_loudness [NUM_VOLUME_LEVEL]=
{
    10, //low volume
    20, //medium
    40  //high
};

/*
 * Initialize the buzzer for music note playing
 */
void MusicInitBuzzer()
{
    PwmBuzzerInit();
}

/*
 * Set the buzzer for a given pitch and volume level.
 *
 * The function calculates the PWM parameters for buzzer
 * and then call BuzzerPwmSet() to set those parameters.
 */
void MusicSetBuzzer(int pitch, int volume)
{
   int cycle = music_loudness[volume];



      int pulse_width = (cycle * music_freq[pitch]) / 100;
      int period = music_freq[pitch];
      PwmBuzzerSet(period, pulse_width);
}

/*
 * Turn off the buzzer
 */
void MusicTurnOffBuzzer()
{
   PwmBuzzerSet(0, 0); //pulse width of 0 turns off buzzer
}
