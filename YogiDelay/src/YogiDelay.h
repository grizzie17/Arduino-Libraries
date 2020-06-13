
#ifndef H_YOGIDELAY
#define H_YOGIDELAY


class YogiDelay
{
public:
    YogiDelay();
    YogiDelay( unsigned long nMilliSeconds );

public:
    void
    init( unsigned long nMilliSeconds );

    void
    newDelay( unsigned long nMilliSeconds );

    void
    reset();

    bool
    timesUp( unsigned long nCurrentTime = 0 );

    // does not reset previous-time
    bool
    peekTime( unsigned long uCurrentTime = 0 );

protected:
    unsigned long        m_nDelay;
    unsigned long        m_nPreviousTime;
    static unsigned long m_nCurrentTime;
};


#endif  // H_YOGIDELAY
