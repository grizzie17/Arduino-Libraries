#ifndef YogiSleep_h
#define YogiSleep_h


class YogiSleep
{
    // class lifecycle ----------------
public:
    YogiSleep();

public:
    // public types -------------------

    static const uint8_t PIN_UNKNOWN = ~0;
    static const uint8_t PIN_MIN = 2;
    static const uint8_t PIN_MAX = 13 + 8;  // D2-13 and A0-7 ports

    // public functions ---------------

    void
    setup( uint8_t pinInterrupt = 2 );

    /// prepare for sleep
    ///
    /// put interrupt setup between this function and powerDown
    void
    prepareSleep();


    void
    sleep();


    void
    postSleep();


    /// put MCU into deep sleep
    void
    powerDown();


    bool
    getInterrupt();


    uint8_t
    getIntValue();


    /// save current 'pinMode' settings for each pin
    void
    savePinModes();

    /// restore 'pinMode' settings
    void
    restorePinModes();

    void
    setPinModesOutput();

    uint8_t
    getPinMode( uint8_t pin );

    void
    printPinModes();

    // public data --------------------

protected:
    // protected types ----------------

    typedef struct pinstate_t
    {
        uint8_t mode;
        uint8_t value;
    } pinstate_t;

    // protected functions ------------

    static void
    YogiSleepInterrupt();

    // protected data -----------------

    static volatile bool    m_bYogiInterrupt;
    static volatile uint8_t m_uYogiIntValue;
    static uint8_t          m_pinINT;
    pinstate_t              m_aModes[PIN_MAX + 1];
};


#endif
