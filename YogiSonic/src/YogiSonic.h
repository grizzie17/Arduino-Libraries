//===============================================
/// @file: YogiSonic.h
//===============================================

#ifndef H_YOGISONIC
#define H_YOGISONIC


//-----------------------------------------------
// YogiSonic class
//-----------------------------------------------
class YogiSonic
{
public:
    YogiSonic();
    virtual ~YogiSonic();

public:
    void
    init( uint8_t nTrigger, uint8_t nEcho, unsigned long uTimeout = 20000UL );

    double
    getDistanceCm();

    double
    lastDistanceCm();

protected:
    double
    CmFromMicroseconds( unsigned long duration );

    uint8_t       m_nPinTrigger;
    uint8_t       m_nPinEcho;
    unsigned long m_uTimeout;
    double        m_dLastDistance;
    int           m_nZeroCount;

    const int US_ROUNDTRIP_CM = 57;
    const int DIVISOR_CM = 28;
    const int DIVISOR_IN = 71;
};


#endif  // H_YOGISONIC
