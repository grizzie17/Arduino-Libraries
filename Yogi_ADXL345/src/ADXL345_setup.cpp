#include <ADXL345_setup.h>
#include <YogiDebug.h>


ADXL345                adxl;
volatile unsigned long g_uCountInterrupt = 0;
long                   g_nActivity = 0;


// Interrupt handler (ISR) for ADXL345 Accelerometer
void
adxlIntHandler()
{
    ++g_uCountInterrupt;
}


void
adxlStatus( const char* sMessage )
{
    bool bSts = adxl.status;
    byte eCode = adxl.error_code;

    DEBUG_PRINT( sMessage );
    if ( ADXL345_ERROR == bSts )
    {
        DEBUG_VPRINT( ": error-code=", eCode, HEX );
    }
    else
    {
        DEBUG_PRINT( ": Success" );
    }
    DEBUG_PRINTLN( "" );
}


void
adxlSetup( int nSleepDelay )
{
    DEBUG_PRINTLN( "adxlSetup" );

    if ( 0 == nSleepDelay )
        nSleepDelay = ADXL_SLEEP_DELAY;

    adxl.powerOn();  // Power on the ADXL345
    adxlStatus( "adxl.powerOn" );


    adxl.setRangeSetting( 2 );
    adxlStatus( "adxl.setRangeSetting" );
    // Give the range settings
    // Accepted values are 2g, 4g, 8g or 16g
    // Higher Values = Wider Measurement Range
    // Lower Values = Greater Sensitivity


    // adxl.setRate( 100 );

    adxl.setInterruptLevelBit( 0 );
    // set interrupts to High

    // adxl.setSpiBit( 0 );
    // Configure the device to be in 4 wire SPI mode when set to '0' or 3 wire SPI mode when set to 1
    // Default: Set to 1
    // SPI pins on the ATMega328: 11, 12 and 13 as reference in SPI Library

    adxl.setActivityXYZ( 1, 1, 1 );
    adxlStatus( "adxl.setActivityXYZ" );
    // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
    adxl.setActivityThreshold( ADXL_SENSITIVITY );
    adxlStatus( "adxl.setActivityThreshold" );
    // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)

    adxl.setInactivityXYZ( 1, 1, 1 );
    adxlStatus( "adxl.setInactivityXYZ" );
    // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
    adxl.setInactivityThreshold( ADXL_SENSITIVITY );
    adxlStatus( "adxl.setInactivityThreshold" );
    // 62.5mg per increment
    // Set inactivity
    // Inactivity thresholds (0-255)
    adxl.setTimeInactivity( nSleepDelay );
    adxlStatus( "adxl.setTimeInactivity" );
    // How many seconds of no activity is inactive?

    adxl.setTapDetectionOnXYZ( 1, 1, 1 );
    // Detect taps in the directions turned ON "adxl.setTapDetectionOnX(X, Y, Z);" (1 == ON, 0 == OFF)

    // Set values for what is considered a TAP and what is a DOUBLE TAP (0-255)
    adxl.setTapThreshold( ADXL_SENSITIVITY );  // 62.5 mg per increment
    adxl.setTapDuration( 15 );                 // 625 μs per increment
    adxl.setDoubleTapLatency( 80 );            // 1.25 ms per increment
    adxl.setDoubleTapWindow( 200 );            // 1.25 ms per increment

    // Set values for what is considered FREE FALL (0-255)
    adxl.setFreeFallThreshold( 7 );
    // (5 - 9) recommended - 62.5mg per increment
    adxl.setFreeFallDuration( 30 );
    // (20 - 70) recommended - 5ms per increment

    // Setting all interupts to take place on INT1 pin
    adxl.setImportantInterruptMapping( 1, 1, 1, 1, 1 );
    // Sets "adxl.setEveryInterruptMapping(single tap, double tap, free fall, activity, inactivity);"
    // Accepts only 1 or 2 values for pins INT1 and INT2. This chooses the pin on the ADXL345 to use for Interrupts.
    // Default to INT1 pin.

    // Turn on Interrupts for each mode (1 == ON, 0 == OFF)
    // adxl.InactivityINT( 1 );
    // adxl.ActivityINT( 1 );
    // adxl.FreeFallINT( 1 );
    // adxl.doubleTapINT( 1 );
    // adxl.singleTapINT( 1 );
    adxl.setInterruptMask( k_maskAll );


    // adxl.printAllRegister();
    // adxl.setSelfTestBit( true );

    g_nActivity = 0;
    g_uCountInterrupt = 0;

    adxlStatus( "adxl setup complete" );
}


uint8_t
adxlGetInterrupts()
{
    unsigned int n = 0;
    // noInterrupts();
    n = g_uCountInterrupt;
    if ( 0 != n )
    {
        adxl.setInterruptMask( 0 );

        // noInterrupts();
        g_uCountInterrupt = 0;
        // interrupts();

        uint8_t mInterrupts = 0;
        while ( 0 < n-- )
        {
            mInterrupts |= adxl.getInterruptSource();
            // adxl.readAccel( &x, &y, &z );
        }

        uint8_t m = ( 0 != ( mInterrupts & k_maskActivity ) )
                ? ADXL_M_INACTIVITY
                : 0;
        adxl.setInterruptMask( k_maskActivity | m );
        adxl.getInterruptSource();

        return mInterrupts & (uint8_t)k_maskAll;
    }
    else
    {
        // interrupts();
        return 0;
    }
}


void
adxlPrintXYZ()
{
#if defined( _DEBUG )
    int x, y, z;
    adxl.readAccel( &x, &y, &z );
    Serial.print( x );
    Serial.print( "," );
    Serial.print( y );
    Serial.print( "," );
    Serial.println( z );
#endif
}


static void
adxlPrintActivity( uint8_t mInterrupts, uint8_t mask, const char* message )
{
    if ( adxl.triggered( mInterrupts, mask ) )
    {
        DEBUG_VPRINTLN( message, g_nActivity );
    }
}


void
adxlDataHandler( uint8_t mInterrupts )
{
    // adxlPrintXYZ();

    ++g_nActivity;
    // Free Fall Detection
    adxlPrintActivity( mInterrupts, ADXL345_FREE_FALL, "*** FREE FALL *** " );

    // Inactivity
    adxlPrintActivity( mInterrupts, ADXL345_INACTIVITY, "*** INACTIVITY *** " );

    // Activity
    adxlPrintActivity( mInterrupts, ADXL345_ACTIVITY, "*** ACTIVITY *** " );

    // Double Tap Detection
    adxlPrintActivity( mInterrupts, ADXL345_DOUBLE_TAP, "*** DOUBLE TAP *** " );

    // Tap Detection
    adxlPrintActivity( mInterrupts, ADXL345_SINGLE_TAP, "*** SINGLE TAP *** " );
}
