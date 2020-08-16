
#ifndef YogiRelay_h
#define YogiRelay_h


// Control the macros below to enable which relay is used.
#if ! defined( RELAY_HFD2 ) && ! defined( RELAY_SONGLE )
#    define RELAY_SONGLE
#endif


//============== RELAY ===============


// This class controls either
// This class is for relays that use two pins.
// Each pin switches the output to one of two
// outputs.
class YogiRelay
{
public:
    // class lifecycle ----------------

    YogiRelay()
            : m_bSet( false )
            , m_pinSet( 0 )
#if defined( RELAY_HFD2 )
            , m_pinReset( 0 )
#endif
    {}
    YogiRelay( uint8_t on, uint8_t off )
            : m_bSet( false )
            , m_pinSet( on )
#if defined( RELAY_HFD2 )
            , m_pinReset( off )
#endif
    {}


public:
    // public functions ---------------

    void
    init( uint8_t set = 0, uint8_t reset = 0 )
    {
        if ( 0 < set )
            m_pinSet = set;
        if ( 0 < m_pinSet )
            pinMode( m_pinSet, OUTPUT );
#if defined( RELAY_HFD2 )
        if ( 0 < reset )
            m_pinReset = reset;
        if ( 0 < m_pinReset )
            pinMode( m_pinReset, OUTPUT );
#endif
        this->reset();
    }

    void
    set()
    {
        m_bSet = true;
#if defined( RELAY_HFD2 )
        trigger( m_pinSet );
#elif defined( RELAY_SONGLE )
        digitalWrite( m_pinSet, HIGH );
        delay( 400 );
#endif
    }

    void
    reset()
    {
        m_bSet = false;
#if defined( RELAY_HFD2 )
        trigger( m_pinReset );
#elif defined( RELAY_SONGLE )
        digitalWrite( m_pinSet, LOW );
        delay( 400 );
#endif
    }

    bool
    isSet()
    {
        return m_bSet;
    }

    inline bool
    isOn()
    {
        return isSet();
    }

protected:
    // protected functions ------------

#if defined( RELAY_HFD2 )
    void
    trigger( uint8_t pin )
    {
        if ( 0 < pin )
        {
            digitalWrite( pin, HIGH );
            delay( 5 );
            digitalWrite( pin, LOW );
        }
    }
#endif

    // protected data -----------------

    bool    m_bSet;
    uint8_t m_pinSet;
#if defined( RELAY_HFD2 )
    uint8_t m_pinReset;
#endif
};


#endif  // YogiRelay_h
