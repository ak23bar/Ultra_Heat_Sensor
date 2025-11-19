/*
 * Lab Project: ULTRA Heat Sensor
 *
 * For Zhao Zhang's
 * ECE 266
 * Last update: Fall 2024
 * by: AKBAR AMAN, David Mendoza, James Patel
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include "launchpad.h"
#include "seg7.h"
#include "ranger.h"
#include "buzzer_h.h"
#include "temp_sensor.h"
#include "music.h"

// Initial state of 7-segment display showing as "0000" with colon on
Seg7Display seg7 = { { 10, 10, 10, 10 }, false /* colon on */};

/*
 * Global variables
 */

// System state
typedef struct {
    enum {Heat, Inch} display_mode;// display distance or heat mode
    bool buzzer_on;
    bool activated;
    bool alerted;
    bool heat;
} SysState;

// The events
Event trigger_ranger_reading_event;
Event push_button_event;
Event clock_update_event;
Event buzzer_play_event;
Event ts_trigger_event;
Event ts_data_event;

SysState sys = {Inch, false, false, false, false};

/*******************************************
* Task: Switch between Distance and Heat!
 ******************************************/

// Trigger the ranger reading every 1/2 second
void TriggerRangerReading(Event *event)
{

    RangerTriggerReading();
    uint32_t pulseWidth = RangerGetData();
    uint32_t distance;


    uprintf("Ranger reading: %u \r\n", (uint32_t)pulseWidth);

    if (sys.display_mode == Inch){
        seg7.colon_on = true;
        //Calculate the distance in inches based on the pulse width
        distance = (pulseWidth * 12) / 89657;
        //Calculate feet and inches
        uint32_t feet = distance / 12;
        uint32_t inches = distance % 12;

        // Sets to feet
        if (feet >= 10) {
            seg7.digit[3] = feet / 10;
            seg7.digit[2] = feet % 10;
        }

        else if (feet > 0) {
            seg7.digit[3] = 11;
            seg7.digit[2] = feet;
        }

        else if (feet == 0) {
            seg7.digit[3] = 11;
            seg7.digit[2] = 11;
        }

        // Set to inches
        if (inches < 10 && feet != 0) {
            seg7.digit[1] = 0;
        }

        else{
            seg7.digit[1] = inches / 10;
        }

        if ((inches < 10) && (feet == 0)){
            seg7.digit[1] = 11;
        }
        seg7.digit[0] = inches % 10;
        // Check the distance in feet and set LED and Buzzer accordingly
        if (feet >= 5) { // Green case
                   LedTurnOnOff(false /* red */, false /* blue */, true /* green */);
                   sys.buzzer_on = false;
                   sys.alerted = false;
                   sys.activated = false;

                   BuzzerOff();
               } else if (feet >= 2 && feet < 5) { // Yellow case
                   LedTurnOnOff(true /* red */, false /* blue */, true /* green */);
                   sys.buzzer_on = true;
                   sys.alerted = true;
                   sys.activated = false;

               } else { // Red case
                   LedTurnOnOff(true /* red */, false /* blue */, false /* green */);
                   sys.buzzer_on = true;
                   sys.alerted = false;
                   sys.activated = true;

               }
           }

    // Update the seven-segment display
    Seg7Update(&seg7);



    EventSchedule(event, event->time + 500);
}
void TriggerTemperatureReading(Event *event)
{
    // Trigger ADC sampling for the temperature sensor
    TsTriggerReading();

    // Schedule the next callback in 4000 milliseconds
    EventSchedule(event, event->time + 4000);
}

/* * Process temperature reading data*/

void ProcessTemperatureReading(Event *event)
        {
            if (sys.display_mode == Heat){

            float temp_F = TsDataRead();
            int temp10 = temp_F * 10;

            // Print on terminal
            printf("Temperature Reading Sensor: %u.%u \r\n", temp10 / 10, temp10 % 10);

            //Print on Seg7
            int tempInt = (int) temp_F;

            seg7.digit[0] = 10; // this digit is not used
            seg7.digit[1] = temp10 % 10;
            seg7.digit[2] = (tempInt % 10);
            seg7.digit[3] = ((tempInt / 10) % 10);
            seg7.colon_on = true;   //Turn on colon
                if(tempInt >= 90){

                   LedTurnOnOff(true /* red */, false /* blue */, false /* green */);
                   sys.heat = true;
               }
               else{
                   LedTurnOnOff(false /* red */, false /* blue */, true /* green */);
                   BuzzerOff();
                   sys.heat = false;
               }

            Seg7Update(&seg7);
            }
        }


void CheckPushButton(Event *event)
{
     int input = PushButtonRead();
     switch (input)
     {
     //Pushbutton SW1 to display Inch (distance detection) Mode
     case 1:

        sys.display_mode = Inch;
        break;

     //Pushbutton SW2 to display Heat Sensing Mode
     case 2:

         sys.display_mode = Heat;
         break;
         }
}


/*******************************************
 * Task: Toggle Buzzer On/Off
 *******************************************/
void ToggleBuzzer(Event *event) {
    static bool buzzer_state = false;

    if (sys.display_mode == Inch) {
        if (sys.activated) { // Red case: constant buzzer at full volume
            MusicSetBuzzer(4 /* pitch G4 */, 2 /* high volume */); // loud G4
            buzzer_state = true;
        } else if (sys.alerted) { // Yellow case: lower volume, toggling
            if (buzzer_state) {
                MusicTurnOffBuzzer();
            } else {
                MusicSetBuzzer(4 /* pitch G4 */, 0 /* low volume */); // soft G4
            }
            buzzer_state = !buzzer_state;
            EventSchedule(event, event->time + 500); // Schedule next toggle in 500 ms
            return; // Exit early to avoid turning off the buzzer below
        } else { // Green case or no alert
            MusicTurnOffBuzzer();
            buzzer_state = false;
        }
    } else if (sys.display_mode == Heat) {
        // Heat mode: buzzer stays on or off based on sys.heat
        if (sys.heat) {
            MusicSetBuzzer(4 /* pitch G4 */, 2 /* high volume */); // Example: loud G4
        } else {
            MusicTurnOffBuzzer();
        }
    }

    // Schedule the next event to periodically re-check state
    EventSchedule(event, event->time + 500);
}

/*******************************************
 * The main function
 ******************************************/
void main(void)
{

    LaunchPadInit();
    RangerInit();
    Seg7Init();
    BuzzerInit();
    TsInit();
    MusicInitBuzzer();


    // Initialize the events
    EventInit(&trigger_ranger_reading_event, TriggerRangerReading);
    EventInit(&buzzer_play_event, ToggleBuzzer); // Initialize ToggleBuzzer event
    EventInit(&push_button_event, CheckPushButton);
    // Initialize and schedule timing events
    EventInit(&ts_trigger_event, TriggerTemperatureReading);
    EventSchedule(&ts_trigger_event, 100);

    // Initialize and register ISR events
    EventInit(&ts_data_event, ProcessTemperatureReading);
    TsEventRegister(&ts_data_event);


    //call the ranger event
    RangerEventRegister(&trigger_ranger_reading_event);
    //Register push button event
    PushButtonEventRegister(&push_button_event);



    // Schedule time event
    EventSchedule(&trigger_ranger_reading_event, 100);
    EventSchedule(&buzzer_play_event, 100); // Start toggling buzzer





    // Loop forever
    while (true)
    {
        // Wait for interrupt
        asm("   wfi");

        // Execute scheduled callbacks
        EventExecute();
    }
}
