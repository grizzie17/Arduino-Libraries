
#ifndef YogiRelay_h
#define YogiRelay_h


//============== RELAY ===============

// This class controls two types of relays.
// - Simple relay where you set the control
//   pin to HIGH to enable.
// - Latching relay where there are two pins:
//   one to 'set' and another to 'reset'
//
// The constructor (or the init function)
// controls the various modes of operation.
//
// To use the 'confirm' functionality hook
// the relay pin that is active (normally open)
// to the 'confirm' pin.  The output voltage
// must be reduced to 5V or 3.3V.


class YogiRelay
{
public:
    // class lifecycle ----------------

    YogiRelay()
            : m_bSet( false )
            , m_pinSet( 0 )
            , m_pinReset( 0 )
            , m_pinSense( 0 )
            , m_pinGate( 0 )
    {}

    YogiRelay(                  //
            uint8_t set,        //! < pin that controls "on"
            uint8_t reset = 0,  //! < if using latching relay "off"
            uint8_t sense = 0,  //! < extra pin to confirm "set"
            uint8_t gate = 0    //! < turn on to allow sensing "set"
            )
            : m_bSet( false )
            , m_pinSet( set )
            , m_pinReset( reset )
            , m_pinSense( sense )
            , m_pinGate( gate )
    {}


public:
    // public functions ---------------

    void
    init( uint8_t set = 0, uint8_t reset = 0, uint8_t confirm = 0,
            uint8_t gate = 0 )
    {
        if ( 0 < set )
            m_pinSet = set;
        if ( 0 < m_pinSet )
            pinMode( m_pinSet, OUTPUT );
        if ( 0 < reset )
            m_pinReset = reset;

        // latching relay
        if ( 0 < m_pinReset )
            pinMode( m_pinReset, OUTPUT );
        if ( 0 < confirm )
            m_pinSense = confirm;
        if ( 0 < gate )
            m_pinGate = gate;
        // latching with confirm circuit
        if ( 0 < m_pinSense )
            pinMode( m_pinSense, INPUT );
        if ( 0 < m_pinGate )
        {
            pinMode( m_pinGate, OUTPUT );
            digitalWrite( m_pinGate, LOW );
        }
        this->reset();
    }

    //! set the relay such that the 'normally open'
    //! pin is now 'closed'.
    void
    set()
    {
        m_bSet = true;
        // are we dealing with a latching relay
        if ( 0 < m_pinReset && 0 < m_pinGate )
        {
            trigger( m_pinSet, HIGH );
        }
        else
        {
            digitalWrite( m_pinSet, HIGH );
            delay( 200 );
        }
    }

    //! reset the relay to its 'normally open'
    //! and 'normally closed' state.
    void
    reset()
    {
        m_bSet = false;
        if ( 0 < m_pinReset && 0 < m_pinGate )
        {
            trigger( m_pinReset, LOW );
        }
        else
        {
            digitalWrite( m_pinSet, LOW );
            delay( 200 );
        }
    }

    bool
    isSet()
    {
        bool bResult = m_bSet;
        if ( 0 < m_pinSense && 0 < m_pinGate )
        {
            digitalWrite( m_pinGate, HIGH );
            delay( 10 );
            bResult = HIGH == digitalRead( m_pinSense );
            digitalWrite( m_pinGate, LOW );
        }
        else
        {
            return m_bSet;
        }
        return m_bSet;
    }

    inline bool
    isOn()
    {
        return isSet();
    }

protected:
    // protected functions ------------

    // trigger the indicated 'pin'
    // confirm that 'reset' state is either HIGH (reset) or LOW (set)
    void
    trigger( uint8_t pin, uint8_t state )
    {
        if ( 0 < pin )
        {
            if ( 0 < m_pinSense && 0 < m_pinGate )
            {
                digitalWrite( m_pinGate, HIGH );
                digitalWrite( pin, HIGH );
                for ( int i = 1; i < 20; ++i )
                {
                    delay( 5 );
                    if ( state == digitalRead( m_pinSense ) )
                        break;
                }
                digitalWrite( pin, LOW );
                digitalWrite( m_pinGate, LOW );
            }
            else
            {
                digitalWrite( pin, HIGH );
                delay( 8 );
                digitalWrite( pin, LOW );
            }
            delay( 25 );
        }
    }

    // protected data -----------------

    bool    m_bSet;
    uint8_t m_pinSet;
    uint8_t m_pinReset;
    uint8_t m_pinSense;
    uint8_t m_pinGate;
};


#endif  // YogiRelay_h
