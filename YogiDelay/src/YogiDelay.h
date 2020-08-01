
#ifndef YogiDelay_h
#define YogiDelay_h


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

    unsigned long
    getDelay();

    void
    reset();

    bool
    timesUp( unsigned long nCurrentTime = 0 );

    // does not reset previous-time
    bool
    peekTime( unsigned long uCurrentTime = 0 );

    static unsigned long
    getCurrentTime();

protected:
    unsigned long        m_nDelay;
    unsigned long        m_nPreviousTime;
    static unsigned long m_nCurrentTime;
};


#endif
