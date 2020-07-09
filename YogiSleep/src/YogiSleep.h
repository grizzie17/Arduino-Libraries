

class YogiSleep
{
    // class lifecycle ----------------
public:
    YogiSleep();

public:
    // public types -------------------

    static const uint8_t PIN_UNKNOWN = ~0;
    static const uint8_t PIN_MAX = 20;

    // public functions ---------------

    /// put MCU into deep sleep
    void
    powerDown();


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
    // protected functions ------------
    // protected data -----------------

    uint8_t m_aModes[PIN_MAX];
};
