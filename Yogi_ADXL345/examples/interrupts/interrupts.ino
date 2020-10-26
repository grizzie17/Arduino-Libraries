#define _DEBUG
#include <YogiDebug.h>

#include <YogiSleep.h>

#include <ADXL345_setup.h>


const uint8_t k_pinINT = 2;
const uint8_t k_pinLED = LED_BUILTIN;


unsigned long g_uTimeCurrent = 0;
unsigned long g_uTimePrevious = 0;
unsigned long g_uTimeInterrupt = 0;

const uint8_t       k_uAdxlDelaySleep = 45;
const unsigned long k_uDelaySleep = 600 * k_uAdxlDelaySleep;


//================ ADXL =================

void
adxlAttachInterrupt()
{
    pinMode( k_pinINT, INPUT );
    attachInterrupt(
            digitalPinToInterrupt( k_pinINT ), adxlIntHandler, RISING );
}

void
adxlDetachInterrupt()
{
    detachInterrupt( digitalPinToInterrupt( k_pinINT ) );
}


void
adxlDrowsy()
{
    g_uCountInterrupt = 0;
    adxl.setInterruptMask( 0 );
    adxl.getInterruptSource();  // clear mInterrupts

    adxl.setInterruptMask( k_maskActivity );
    adxl.setLowPower( true );
}


void
adxlSleep()
{
    adxlDetachInterrupt();
    g_uCountInterrupt = 0;
    adxl.getInterruptSource();  // clear mInterrupts

    adxl.setInterruptMask( 0 );
    adxl.setLowPower( true );
}


void
adxlWakeup()
{
    adxl.setLowPower( false );
    adxl.setInterruptMask( k_maskAll );
    g_uCountInterrupt = 0;
    adxl.getInterruptSource();
    adxlAttachInterrupt();
    adxlIntHandler();
    g_nActivity = 0;
}


//============== SLEEP ===============
YogiSleep g_tSleep;

void
enterSleep()
{
    DEBUG_PRINTLN( "Sleepy" );
    DEBUG_DELAY( 300 );

    // don't generate inactivity interrupts during sleep
    adxlDrowsy();

    g_tSleep.prepareSleep();
    adxlAttachInterrupt();
    g_tSleep.sleep();
    g_tSleep.postSleep();

    // we wakeup here
    adxlWakeup();
    DEBUG_PRINTLN( "Wake Up" );

    // ? do we need to reintialize any of the sensors ?
}


void
setup()
{
    Serial.begin( 115200 );
    while ( ! Serial )
        ;  // wait for serial bus

    delay( 500 );

    Serial.println( "Yogi_ADXL345 setup" );

    adxlSetup( k_uAdxlDelaySleep );
    adxlAttachInterrupt();

    pinMode( k_pinLED, OUTPUT );
    digitalWrite( k_pinLED, HIGH );


    g_uTimePrevious = 0;
    g_uTimeInterrupt = 0;

    Serial.println( "setup complete" );
}


void
loop()
{
    uint8_t mInterrupts = 0;
    g_uTimeCurrent = millis();
    if ( 0 < g_uCountInterrupt )
    {
        mInterrupts = adxlGetInterrupts();
        g_uTimeInterrupt = millis();
    }
    else
    {
        if ( k_uDelaySleep < g_uTimeCurrent - g_uTimeInterrupt )
        {
            g_uTimeInterrupt = g_uTimeCurrent;
            adxl.getInterruptSource();
            mInterrupts = ADXL_M_INACTIVITY;
            Serial.println( "Time Delay Sleep" );
        }
    }

    if ( 0 != mInterrupts )
    {
        adxlDataHandler( mInterrupts );
        if ( 0 != ( mInterrupts & ADXL_M_INACTIVITY ) )
        {
            digitalWrite( k_pinLED, LOW );
            enterSleep();

            // stuff to do when we wake up
            g_nActivity = 0;
            digitalWrite( k_pinLED, HIGH );
        }
        // else
        // {
        //     if ( 200 < g_uTimeCurrent - g_uTimePrevious )
        //     {
        //         g_uTimePrevious = g_uTimeCurrent;
        //     }
        // }
    }
}
