
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
// the relay pin that is active (normally closed)
// to the 'confirm' pin.  You may need to
// put a resistor depending upon the output
// voltage.


class YogiRelay
{
public:
    // class lifecycle ----------------

    YogiRelay()
            : m_bSet( false )
            , m_pinSet( 0 )
            , m_pinReset( 0 )
            , m_pinConfirm( 0 )
    {}

    YogiRelay( uint8_t set,         //! < pin that controls "on"
            uint8_t    reset = 0,   //! < if using latching relay "off"
            uint8_t    confirm = 0  //! < extra pin to confirm "set"
            )
            : m_bSet( false )
            , m_pinSet( set )
            , m_pinReset( reset )
            , m_pinConfirm( confirm )
    {}


public:
    // public functions ---------------

    void
    init( uint8_t set = 0, uint8_t reset = 0, uint8_t confirm = 0 )
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
            m_pinConfirm = confirm;
        // latching with confirm circuit
        if ( 0 < m_pinConfirm )
            pinMode( m_pinConfirm, INPUT );
        this->reset();
    }

    //! set the relay such that the 'normally open'
    //! pin is now 'closed'.
    void
    set()
    {
        m_bSet = true;
        // are we dealing with a latching relay
        if ( 0 < m_pinReset )
        {
            trigger( m_pinSet, LOW );
        }
        else
        {
            digitalWrite( m_pinSet, HIGH );
            delay( 400 );
        }
    }

    //! reset the relay to its 'normally open'
    //! and 'normally closed' state.
    void
    reset()
    {
        m_bSet = false;
        if ( 0 < m_pinReset )
        {
            trigger( m_pinReset, HIGH );
        }
        else
        {
            digitalWrite( m_pinSet, LOW );
            delay( 400 );
        }
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

    // trigger the indicated 'pin'
    // confirm that 'reset' state is either HIGH (reset) or LOW (set)
    void
    trigger( uint8_t pin, uint8_t state )
    {
        if ( 0 < pin )
        {
            if ( 0 < m_pinConfirm )
            {
                digitalWrite( pin, HIGH );
                for ( int i = 1; i < 20; ++i )
                {
                    delay( 5 );
                    if ( state == digitalRead( m_pinConfirm ) )
                        break;
                }
                digitalWrite( pin, LOW );
            }
            else
            {
                digitalWrite( m_pinSet, HIGH );
                delay( 5 );
                digitalWrite( m_pinSet, LOW );
            }
        }
    }

    // protected data -----------------

    bool    m_bSet;
    uint8_t m_pinSet;
    uint8_t m_pinReset;
    uint8_t m_pinConfirm;
};


#endif  // YogiRelay_h
