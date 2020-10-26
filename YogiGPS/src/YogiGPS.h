#ifndef YogiGPS_h
#define YogiGPS_h

#include <Arduino.h>

#if ! defined( __SIZE_TYPE__ )
typedef unsigned long size_t;
#endif

//===============================================
// YogiGPS
//===============================================


template <class TSerial>
class YogiGpsSentence
{
    // class lifecycle --------------------------
public:
    YogiGpsSentence( TSerial& port )
            : m_serial( port )
            , m_started( false )
            , m_complete( false )
            , m_index( 0 )
    {
        m_sentence[0] = 0;
    }

public:
    // public types -----------------------------
    // public functions -------------------------

    // read chars from identified serial port
    // until we have a complete sentence.
    // This function may need to be called
    // multiple times until all is read.
    //
    // returns: zero if not complete, count of chars if full sentence
    int
    read()
    {
        int n = 0;

        int nAvail = m_serial.available();
        if ( 0 < nAvail )
        {
            char c = 0;

            if ( ! m_started )
            {
                while ( 0 < nAvail-- )
                {
                    c = m_serial.read();
                    if ( '$' == c )
                    {
                        m_index = 0;
                        m_sentence[m_index++] = c;
                        m_started = true;
                        break;
                    }
                }
            }
            if ( m_started )
            {
                while ( 0 < nAvail-- )
                {
                    c = m_serial.read();
                    if ( m_index + 1 <= sizeof( m_sentence ) )
                    {
                        if ( '\n' == c || '\r' == c || '$' == c )
                        {
                            m_sentence[m_index]
                                    = '\0';  // don't increment as null
                            m_complete = true;
                            m_started = false;
                            n = m_index;
                            break;
                        }
                        else
                        {
                            m_sentence[m_index++] = c;
                        }
                    }
                    else  // buffer overflow
                    {
                        n = -1;
                        m_started = false;
                        m_complete = false;
                    }
                }
            }
        }

        return n;
    }


    const char*
    sentence( int* pLength ) const
    {
        if ( m_complete )
        {
            if ( pLength )
                *pLength = m_index;
            return m_sentence;
        }
        else
            return nullptr;
    }

    void
    clearSentence()
    {
        m_index = 0;
        m_complete = false;
        m_started = false;
    }

    // public data ------------------------------

protected:
    // protected types --------------------------
    // protected functions ----------------------
    // protected data ---------------------------

    TSerial& m_serial;
    bool     m_started = false;   // have initial '$'
    bool     m_complete = false;  // sentence complete
    int      m_index;             // current index in sentence
    char     m_sentence[100];
};


//===============================================
// Parse NMEA Sentence
//===============================================


class YogiGpsParse
{
    // class lifecycle --------------------------
public:
    YogiGpsParse();

public:
    // public types -----------------------------
    // public constants -------------------------
    // public functions -------------------------

    void
    initData();


    bool
    parse( const char* sData, size_t nChars );

    // public data ------------------------------

    uint16_t year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  hour;
    uint8_t  minute;
    uint8_t  seconds;
    uint16_t milliseconds;

    float   latitude;
    float   longitude;
    float   altitude;
    int32_t latitude_fixed;
    int32_t longitude_fixed;
    char    lat = 0;
    char    lon = 0;
    char    mag = 0;
    char    status = 0;
    uint8_t quality = 0;
    uint8_t satellites = 0;
    int8_t  severity = -1;

protected:
    // protected types --------------------------

    //using ParseFunc = bool ( YogiGpsParse::* )();
    typedef bool ( YogiGpsParse::*ParseFunc )( const char* s, size_t n );

    // protected constants ----------------------

    const char* k_prefixes[6]  //
            = {                //
                  "GPGGA",     //
                  "GPGLL",     //
                  "GPGSA",     //
                  "GPGSV",     //
                  "GPRMC",     //
                  "GPVTG"
              };
    const size_t k_pfxSize = sizeof( k_prefixes ) / sizeof( k_prefixes[0] );

    ParseFunc k_funcVec[6]                    //
            = {                               //
                  &YogiGpsParse::parseGPGGA,  //
                  &YogiGpsParse::parseGPGLL,  //
                  &YogiGpsParse::parseGPGSA,  //
                  &YogiGpsParse::parseGPGSV,  //
                  &YogiGpsParse::parseGPRMC,  //
                  &YogiGpsParse::parseGPVTG
              };
    const size_t k_fvSize = sizeof( k_funcVec ) / sizeof( k_funcVec[0] );

    // protected functions ----------------------

    bool
    parseGPGGA( const char* s, size_t n );

    bool
    parseGPGLL( const char* s, size_t n );

    bool
    parseGPGSA( const char* s, size_t n );

    bool
    parseGPGSV( const char* s, size_t n );

    bool
    parseGPRMC( const char* s, size_t n );

    bool
    parseGPVTG( const char* s, size_t n );

    bool
    parseOther( const char* s, size_t n );

    // binary search
    int
    searchPrefixes( const char* sPrefix, size_t nPfxSize );


    bool
    parseTime( const char* s, size_t n, size_t* pParseCount, int* pHR, int* pMN,
            int* pSC, int* pMS );
    bool
    parseDate( const char* s, size_t n, size_t* pParseCount, int* pYr, int* pMn,
            int* pDy );


    // format is DDMM.mmm to decimal degrees
    //
    // @returns: decimal degrees
    float
    encodeDegrees( float fLat );


    bool
    decodeDegrees( const char* s, size_t n, size_t* pParseCount,  //
            long* pDegPart, long* pMinPart, long* pSecPart,
            long* pSecFracPart );


    float
    parseDegrees( const char* s, size_t n, size_t* pParseCount,
            long* pFixedPoint = 0, float* pNmeaDeg = 0 );


    // debug print NMEA data
    void
    debugPrintMsg( const char* sPfx, const char* s, size_t n );

    // protected data ---------------------------
};


#endif
