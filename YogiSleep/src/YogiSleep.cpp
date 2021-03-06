
#include <Arduino.h>

#include "YogiSleep.h"

#ifndef __asm__
#    ifdef _MSC_VER
#        define __asm__ __asm
#    endif
#endif

// xxx_B_xxx - bits to shift
// xxx_V_xxx - value of field
// xxx_M_xxx - mask

// Sleep Mode Control Register
#define SMCR_B_SM210               1  // number of bits to shift
#define SMCR_V_IDLE                0b000
#define SMCR_V_ADC_NOISE_REDUCTION 0b001
#define SMCR_V_POWER_DOWN          0b010
#define SMCR_V_POWER_SAVE          0b011
#define SMCR_V_STANDBY             0b110
#define SMCR_V_EXT_STANDBY         0b111
#define SMCR_V_ALLBITS             0b111

#define SMCR_M_SM2 0b1000
#define SMCR_M_SM1 0b0100
#define SMCR_M_SM0 0b0010
#define SMCR_M_SE  0b0001

// MCU Control Register
#define MCUCR_M_BODS  0b01000000
#define MCUCR_M_BODSE 0b00100000
#define MCUCR_M_PUD   0b00010000

// Power Reduction Register
#define PRR_M_PRTWI0   0b10000000
#define PRR_M_PRTIM2   0b01000000
#define PRR_M_PRTIM0   0b00100000
#define PRR_M_PRTIM1   0b00001000
#define PRR_M_PRSPI0   0b00000100
#define PRR_M_PRUSART0 0b00000010
#define PRR_M_PRADC    0b00000001

// ADC Control and Status Register A
#define ADCSRA_M_ADEN 0b10000000  // ADC enable


volatile bool    YogiSleep::m_bYogiInterrupt = false;
volatile uint8_t YogiSleep::m_uYogiIntValue = LOW;
uint8_t          YogiSleep::m_pinINT = 2;


// class lifecycle ====================

YogiSleep::YogiSleep()
{
    m_bYogiInterrupt = false;
    for ( uint8_t i = PIN_MIN; i <= PIN_MAX; ++i )
    {
        m_aModes[i].mode = PIN_UNKNOWN;
        m_aModes[i].value = 0;
    }
}

// public functions ===================


void
YogiSleep::setup( uint8_t pinInterrupt )
{
    m_bYogiInterrupt = false;
    m_uYogiIntValue = LOW;
    m_pinINT = pinInterrupt;
    attachInterrupt(
            digitalPinToInterrupt( m_pinINT ), YogiSleepInterrupt, RISING );
}


bool
YogiSleep::getInterrupt()
{
    bool bINT = m_bYogiInterrupt;
    m_bYogiInterrupt = false;
    return bINT;
}


uint8_t
YogiSleep::getIntValue()
{
    uint8_t v = m_uYogiIntValue;
    m_uYogiIntValue = LOW;
    return v;
}


void
YogiSleep::prepareSleep()
{
    savePinModes();
    setPinModesOutput();
}


void
YogiSleep::sleep()
{
    uint8_t saveADC = ADCSRA & ADCSRA_M_ADEN;
    uint8_t saveSMCR_PD = SMCR & ( SMCR_V_ALLBITS << SMCR_B_SM210 );
    uint8_t saveSMCR_SE = SMCR & SMCR_M_SE;
    uint8_t saveMCUCR_BOD = MCUCR & ( MCUCR_M_BODS | MCUCR_M_BODSE );

    // Disable ADC
    ADCSRA &= ~ADCSRA_M_ADEN;
    // Enable Sleep
    SMCR |= SMCR_V_POWER_DOWN << SMCR_B_SM210;
    SMCR |= SMCR_M_SE;  // enable Sleep

    // BOD (Brown-Out) Disable
    MCUCR |= MCUCR_M_BODS | MCUCR_M_BODSE;  // set S & SE
    MCUCR = ( MCUCR & ~MCUCR_M_BODSE ) | MCUCR_M_BODS;
    __asm__ __volatile__( "sleep" );

    // we wake up here

    // restore modified registers
    MCUCR = ( MCUCR & ~( MCUCR_M_BODS | MCUCR_M_BODSE ) ) | saveMCUCR_BOD;
    SMCR = ( SMCR & ~SMCR_M_SE ) | saveSMCR_SE;
    SMCR = ( SMCR & ~( SMCR_V_ALLBITS << SMCR_B_SM210 ) ) | saveSMCR_PD;
    ADCSRA = ( ADCSRA & ~ADCSRA_M_ADEN ) | saveADC;
}


void
YogiSleep::postSleep()
{
    restorePinModes();
}


void
YogiSleep::powerDown()
{
    prepareSleep();
    sleep();
    postSleep();
}


void
YogiSleep::savePinModes()
{
    for ( uint8_t i = PIN_MIN; i <= PIN_MAX; ++i )
    {
        uint8_t m = getPinMode( i );
        m_aModes[i].mode = m;
        m_aModes[i].value = m == OUTPUT ? digitalRead( i ) : 0;
    }
}


void
YogiSleep::restorePinModes()
{
    uint8_t uMode = PIN_UNKNOWN;
    uint8_t uValue = 0;
    for ( uint8_t i = PIN_MIN; i <= PIN_MAX; ++i )
    {
        uMode = m_aModes[i].mode;
        if ( PIN_UNKNOWN != uMode )
        {
            pinMode( i, uMode );
            if ( OUTPUT == uMode )
                digitalWrite( i, m_aModes[i].value );
        }
    }
}


void
YogiSleep::setPinModesOutput()
{
    for ( uint8_t i = PIN_MIN; i <= PIN_MAX; ++i )
    {
        pinMode( i, OUTPUT );
        digitalWrite( i, LOW );
    }
}


uint8_t
YogiSleep::getPinMode( uint8_t pin )
{
    uint8_t bit = digitalPinToBitMask( pin );
    uint8_t port = digitalPinToPort( pin );

    // I don't see an option for mega to return this, but whatever...
    if ( NOT_A_PIN == port )
        return PIN_UNKNOWN;

    // Is there a bit we can check?
    if ( 0 == bit )
        return PIN_UNKNOWN;

    // Is there only a single bit set?
    if ( bit & bit - 1 )
        return PIN_UNKNOWN;

    volatile uint8_t *reg, *out;
    reg = portModeRegister( port );
    out = portOutputRegister( port );

    if ( *reg & bit )
        return OUTPUT;
    else if ( *out & bit )
        return INPUT_PULLUP;
    else
        return INPUT;
}


void
YogiSleep::printPinModes()
{
    for ( uint8_t i = PIN_MIN; i <= PIN_MAX; ++i )
    {
        uint8_t uMode = m_aModes[i].mode;
        Serial.print( i );
        Serial.print( ": " );
        switch ( uMode )
        {
        case OUTPUT:
            Serial.print( "OUTPUT" );
            break;
        case INPUT:
            Serial.print( "INPUT" );
            break;
        case INPUT_PULLUP:
            Serial.print( "INPUT_PULLUP" );
            break;
        default:
            Serial.print( "unknown" );
            break;
        }
        Serial.println( "" );
    }
}

// protected functions ================


void
YogiSleep::YogiSleepInterrupt()
{
    m_bYogiInterrupt = true;
    m_uYogiIntValue = digitalRead( m_pinINT );
}
