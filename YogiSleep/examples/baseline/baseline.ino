
const uint8_t k_pinLED = LED_BUILTIN;

unsigned long g_uTimeCurrent = 0;
unsigned long g_uTimePrevious = 0;
unsigned long g_uTimeDelay = 1000;
uint8_t       g_vLedState = LOW;

void
setup()
{
    // Serial.begin( 115200 );
    // while ( ! Serial )
    //     ;


    for ( uint8_t i = 2; i <= 13 + 8; ++i )
    {
        pinMode( i, OUTPUT );
        // digitalWrite( i, LOW );
    }
    pinMode( k_pinLED, OUTPUT );
    g_uTimeCurrent = 0;
    g_uTimePrevious = 0;
    g_vLedState = LOW;
}


void
loop()
{
    g_uTimeCurrent = millis();
    if ( g_uTimeDelay < g_uTimeCurrent - g_uTimePrevious )
    {
        g_uTimePrevious = g_uTimeCurrent;
        if ( HIGH == g_vLedState )
        {
            g_uTimeDelay = 1000 * 5;
            g_vLedState = LOW;
        }
        else
        {
            g_uTimeDelay = 1000;
            g_vLedState = HIGH;
        }
        digitalWrite( k_pinLED, g_vLedState );
    }
    // digitalWrite( k_pinLED, HIGH );
    // delay( 1000 );
    // digitalWrite( k_pinLED, LOW );
    // delay( 1000 * 5 );
}
