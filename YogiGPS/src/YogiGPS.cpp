
#include <Arduino.h>
//#include <math.h>

#include "YogiGPS.h"


// step over comma
#define MATCH_COMMA( s, n ) \
    if ( ',' != *s )        \
        break;              \
    ++s;                    \
    --n

//===============================================
// Character Descriptor
//===============================================

const char*
ParseSymbolT( const char* s, size_t n, size_t* pParseCount )
{
    const char* pValue = 0;
    size_t      nCount;
    const char* sEnd = s + n;
    const char* sStart = s;
    char        c;

    while ( s < sEnd )
    {
        c = tolower( *s );
        if ( c < 'a' || 'z' < c )
            break;
        ++s;
    }
    nCount = ( size_t )( s - sStart );
    if ( 0 < nCount )
        pValue = sStart;
    if ( pParseCount )
        *pParseCount = nCount;

    return pValue;
}

long
ParseDecDigitsT( const char* s, size_t n, size_t* pParseCount )
{
    long        nValue = 0;
    const char* sEnd = s + n;
    const char* sStart = s;

    while ( s < sEnd )
    {
        if ( *s < '0' || '9' < *s )
            break;
        else
            nValue = nValue * 10 + ( *s - '0' );
        ++s;
    }

    if ( pParseCount )
        *pParseCount = ( size_t )( s - sStart );

    return nValue;
}

long
ParseDecT( const char* s, size_t n, size_t* pParseCount )
{
    long        nValue = 0;
    size_t      nCount;
    const char* sEnd = s + n;
    const char* sStart = s;
    long        nSign = 1;

    if ( s < sEnd )
    {
        if ( '-' == *s )
        {
            nSign = -1;
            ++s;
        }
    }

    nValue = ParseDecDigitsT( s, ( size_t )( sEnd - s ), &nCount );
    if ( 0 < nCount )
    {
        s += nCount;
        nValue *= nSign;
    }
    else
    {
        s = sStart;
        nValue = 0;
    }
    if ( pParseCount )
        *pParseCount = ( size_t )( s - sStart );

    return nValue;
}


float
ParseSimpleFloatT( const char* s, size_t n, size_t* pParseCount )
{
    float       fValue = 0.0;
    float       fSign = 1.0;
    size_t      nParseCount = 0;
    const char* sStart = s;
    size_t      c;  // count

    do
    {
        fValue = static_cast<float>( ParseDecT( s, n, &c ) );
        // if ( c < 1 )
        //     break;

        s += c;
        n -= c;
        if ( '.' != *s )
            break;

        ++s;
        --n;
        long nFrac = ParseDecDigitsT( s, n, &c );

        if ( c < 1 )
            break;

        s += c;
        n -= c;

        if ( fValue < 0.0 )
        {
            fSign = -1.0;
            fValue *= -1.0;
        }

        nParseCount = ( size_t )( s - sStart );

        fValue += powf( 10.0, -static_cast<float>( c ) )
                * static_cast<float>( nFrac );
        fValue *= fSign;
    } while ( 0 );

    if ( pParseCount )
        *pParseCount = nParseCount;

    return fValue;
}


//===============================================
// Parse NMEA Sentence
//===============================================


// class lifecycle --------------------------
YogiGpsParse::YogiGpsParse()
{
    initData();
}

// public types -----------------------------
// public constants -------------------------
// public functions -------------------------

bool
YogiGpsParse::parse( const char* sData, size_t nChars )
{
    bool bResult = false;

    if ( '$' == *sData )
    {
        const char* sHeader;
        size_t      nHdrSize;

        ++sData;
        --nChars;

        sHeader = ParseSymbolT( sData, nChars, &nHdrSize );
        if ( 0 < nHdrSize )
        {
            if ( ',' == *( sHeader + nHdrSize ) )
            {
                int indexHeader = searchPrefixes( sHeader, nHdrSize );
                if ( 0 <= indexHeader )
                {
                    sData += nHdrSize + 1;  // step past header and comma
                    nChars -= nHdrSize + 1;
                    ParseFunc f = k_funcVec[indexHeader];
                    bResult = ( this->*f )( sData, nChars );
                }
                else
                {
                    bResult = parseOther( sData, nChars );
                }
            }
        }
    }

    return bResult;
}

// public data ------------------------------

// uint16_t year;
// uint8_t  month;
// uint8_t  day;
// uint8_t  hour;
// uint8_t  minute;
// uint8_t  seconds;
// uint16_t milliseconds;

// float   latitude;
// float   longitude;
// float   altitude;
// int32_t latitude_fixed;
// int32_t longitude_fixed;
// char    lat = 'X';
// char    lon = 'X';
// char    mag = 'X';

// protected types --------------------------

//using ParseFunc = bool ( YogiGpsParse::* )();
// typedef bool ( YogiGpsParse::*ParseFunc )( const char* s, size_t n );

// protected constants ----------------------

// const char*  k_prefixes[2] = { "GPGGA", "GPGLL" };
// const size_t k_pfxSize = sizeof( k_prefixes ) / sizeof( k_prefixes[0] );

// ParseFunc k_funcVec[2]
//         = { &YogiGpsParse::parseGPGGA, &YogiGpsParse::parseGPGLL };
// const size_t k_fvSize = sizeof( k_funcVec ) / sizeof( k_funcVec[0] );

// protected functions ----------------------

void
YogiGpsParse::initData()
{
    year = 0;
    month = 0;
    day = 0;
    hour = 0;
    minute = 0;
    seconds = 0;
    milliseconds = 0;

    latitude = 0.0;
    longitude = 0.0;
    altitude = 0.0;
    latitude_fixed = 0;
    longitude_fixed = 0;
    lat = 0;
    lon = 0;
    mag = 0;
    status = 0;
    quality = 0;
    satellites = 0;
    severity = -1;
}

// Fix information
bool
YogiGpsParse::parseGPGGA( const char* s, size_t n )
{
    // time,lat,X,lon,X,fix,sats,hdop,alt,M,hgt,M,,station*chksum
    bool bResult = false;

    debugPrintMsg( "$GPGGA==", s, n );

    do
    {
        size_t nCount;

        // time
        int hr;
        int mn;
        int sc;
        int ms;  // milliseconds
        if ( ! parseTime( s, n, &nCount, &hr, &mn, &sc, &ms ) )
            break;
        // long nValue = ParseDecDigitsT( s, n, &nCount );
        // if ( 0 == nCount )
        //     break;
        s += nCount;
        n -= nCount;


        MATCH_COMMA( s, n );

        // latitude
        long  nLatFixed = 0;
        float fLatNmea = 0.0;
        float fLat = parseDegrees( s, n, &nCount, &nLatFixed, &fLatNmea );
        bool  bLat = 4 < nCount;
        // if ( 0 == nCount )
        //     break;
        s += nCount;
        n -= nCount;

        MATCH_COMMA( s, n );

        // longitude
        long  nLonFixed = 0;
        float fLonNmea = 0.0;
        float fLon = parseDegrees( s, n, &nCount, &nLonFixed, &fLonNmea );
        bool  bLon = 4 < nCount;
        // if ( 0 == nCount )
        //     break;
        s += nCount;
        n -= nCount;

        MATCH_COMMA( s, n );

        // fix quality
        int  nFix = ParseDecDigitsT( s, n, &nCount );
        bool bFix = 0 < nCount;
        // if ( 0 == nCount )
        //     break;
        s += nCount;
        n -= nCount;

        MATCH_COMMA( s, n );

        int  nSats = ParseDecDigitsT( s, n, &nCount );
        bool bSats = 0 < nCount;
        // if ( 0 == nCount )
        //     break;
        s += nCount;
        n -= nCount;

        MATCH_COMMA( s, n );

        float fHDOP = ParseSimpleFloatT( s, n, &nCount );
        // if ( 0 == nCount )
        //     break;
        s += nCount;
        n -= nCount;


        // we got this far, load the data

        // year = 0;
        // month = 0;
        // day = 0;

        hour = hr;
        minute = mn;
        seconds = sc;
        milliseconds = ms;

        if ( bLat )
        {
            latitude = abs( fLat );
            lat = fLat < 0 ? 'S' : 'N';
            latitude_fixed = nLatFixed;
        }
        if ( bLon )
        {
            longitude = abs( fLon );
            lon = fLon < 0 ? 'W' : 'E';
            longitude_fixed = nLonFixed;
        }
        if ( bFix )
            quality = nFix;
        //altitude = 0.0;
        // if ( bSats )
        //     satellites = nSats;

        bResult = true;
    } while ( 0 );


    return bResult;
}

// Lat/Lon data
bool
YogiGpsParse::parseGPGLL( const char* s, size_t n )
{
    bool bResult = false;

    debugPrintMsg( "$GPGLL==", s, n );

    do
    {
        size_t nCount;

        // latitude
        long  nLatFixed = 0;
        float fLatNmea = 0.0;
        float fLat = parseDegrees( s, n, &nCount, &nLatFixed, &fLatNmea );
        bool  bLat = 4 < nCount;
        // if ( 0 == nCount )
        //     break;
        s += nCount;
        n -= nCount;

        MATCH_COMMA( s, n );

        // longitude
        long  nLonFixed = 0;
        float fLonNmea = 0.0;
        float fLon = parseDegrees( s, n, &nCount, &nLonFixed, &fLonNmea );
        bool  bLon = 4 < nCount;
        // if ( 0 == nCount )
        //     break;
        s += nCount;
        n -= nCount;

        MATCH_COMMA( s, n );


        // time
        int  hr;
        int  mn;
        int  sc;
        int  ms;  // milliseconds
        bool bTm = parseTime( s, n, &nCount, &hr, &mn, &sc, &ms );
        s += nCount;
        n -= nCount;

        MATCH_COMMA( s, n );

        char        sts = 'V';
        const char* sT = ParseSymbolT( s, n, &nCount );
        if ( 1 == nCount )
            sts = *sT;


        if ( bTm )
        {
            hour = hr;
            minute = mn;
            seconds = sc;
            milliseconds = ms;
        }
        if ( bLat )
        {
            latitude = abs( fLat );
            lat = fLat < 0 ? 'S' : 'N';
            latitude_fixed = nLatFixed;
        }
        if ( bLon )
        {
            longitude = abs( fLon );
            lon = fLon < 0 ? 'W' : 'E';
            longitude_fixed = nLonFixed;
        }

        status = sts;

        bResult = true;
    } while ( 0 );


    return bResult;
}

bool
YogiGpsParse::parseGPGSA( const char* s, size_t n )
{
    bool bResult = false;

    //debugPrintMsg( "$GPGSA==", s, n );

    return bResult;
}

// detailed satellite data
bool
YogiGpsParse::parseGPGSV( const char* s, size_t n )
{
    bool bResult = false;

    //debugPrintMsg( "$GPGSV==", s, n );

    do
    {
        size_t nCount = 0;

        // number of sentences for full data: ignored
        ParseDecDigitsT( s, n, &nCount );
        s += nCount;
        n -= nCount;

        MATCH_COMMA( s, n );

        // sentence number: ignored
        ParseDecDigitsT( s, n, &nCount );
        s += nCount;
        n -= nCount;

        MATCH_COMMA( s, n );

        // number of satellites in view
        long nSats = ParseDecDigitsT( s, n, &nCount );
        bool bSats = 0 < nCount;
        s += nCount;
        n -= nCount;

        // Satellite PRN number
        // Elevation, degrees
        // Azimuth, degrees
        // SNR - higher is better (up to 4 satellites)

        if ( bSats )
            satellites = nSats;

        bResult = true;

    } while ( 0 );


    return bResult;
}

// Recommended Minimum data for gps
//
//         1         2 3       4 5        6 7   8   9    10  11 12
//  $GPRMC,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,xxxx,x.x,a*hh
//
//   1) Time (UTC)
//   2) Status, V = Navigation receiver warning
//   3) Latitude
//   4) N or S
//   5) Longitude
//   6) E or W
//   7) Speed over ground, knots
//   8) Track made good, degrees true
//   9) Date, ddmmyy
//  10) Magnetic Variation, degrees
//  11) E or W
//  12) Checksum
//
bool
YogiGpsParse::parseGPRMC( const char* s, size_t n )
{
    bool bResult = false;

    debugPrintMsg( "$GPRMC==", s, n );

    do
    {
        size_t nCount;
        //time,V,lat,X,lon,X,spd,trk,050820,,,N*73
        //172858.00,V,lat,X,lon,x,spd,trk,050820,,,N*73
        // time
        int hr;
        int mn;
        int sc;
        int ms;
        if ( ! parseTime( s, n, &nCount, &hr, &mn, &sc, &ms ) )
            break;
        s += nCount;
        n -= nCount;

        MATCH_COMMA( s, n );

        // Status
        char        sts = 'V';
        const char* st = ParseSymbolT( s, n, &nCount );
        if ( 1 == nCount )
            sts = *st;
        s += nCount;
        n -= nCount;

        MATCH_COMMA( s, n );

        // latitude
        long  nLatFixed = 0;
        float fLatNmea = 0.0;
        float fLat = parseDegrees( s, n, &nCount, &nLatFixed, &fLatNmea );
        bool  bLat = 4 < nCount;
        // if ( 0 == nCount )
        //     break;
        s += nCount;
        n -= nCount;

        MATCH_COMMA( s, n );

        // longitude
        long  nLonFixed = 0;
        float fLonNmea = 0.0;
        float fLon = parseDegrees( s, n, &nCount, &nLonFixed, &fLonNmea );
        bool  bLon = 4 < nCount;
        // if ( 0 == nCount )
        //     break;
        s += nCount;
        n -= nCount;

        MATCH_COMMA( s, n );

        // speed
        float fKnots = ParseSimpleFloatT( s, n, &nCount );
        s += nCount;
        n -= nCount;

        MATCH_COMMA( s, n );

        // course
        float fTrack = ParseSimpleFloatT( s, n, &nCount );
        s += nCount;
        n -= nCount;

        MATCH_COMMA( s, n );

        // date
        int  yr = 0;
        int  mo = 0;
        int  dy = 0;
        bool bDt = parseDate( s, n, &nCount, &yr, &mo, &dy );

        // TODO: add Magnetic Variation (ddd.dd,W)
        // TODO: mode

        if ( bDt )
        {
            year = yr;
            month = mo;
            day = dy;
        }
        if ( bLat )
        {
            latitude = abs( fLat );
            lat = fLat < 0 ? 'S' : 'N';
            latitude_fixed = nLatFixed;
        }
        if ( bLon )
        {
            longitude = abs( fLon );
            lon = fLon < 0 ? 'W' : 'E';
            longitude_fixed = nLonFixed;
        }

        hour = hr;
        minute = mn;
        seconds = sc;
        milliseconds = ms;

        status = sts;

        bResult = true;

    } while ( 0 );


    return bResult;
}

bool
YogiGpsParse::parseGPVTG( const char* s, size_t n )
{
    bool bResult = false;

    //debugPrintMsg( "$GPVTG==", s, n );

    return bResult;
}

bool
YogiGpsParse::parseOther( const char* s, size_t n )
{
    bool bResult = false;

    //debugPrintMsg( "---", s, n );

    return bResult;
}

// binary search
int
YogiGpsParse::searchPrefixes( const char* sPrefix, size_t nPfxSize )
{
    int mid;
    int low = 0;
    int high = k_pfxSize - 1;
    int cmp;

    while ( low <= high )
    {
        mid = ( low + high ) / 2;
        cmp = strncmp( sPrefix, k_prefixes[mid], nPfxSize );
        if ( cmp < 0 )
            high = mid - 1;
        else if ( 0 < cmp )
            low = mid + 1;
        else
            return mid;  // found
    }
    return -1;  // not found
}


bool
YogiGpsParse::parseTime( const char* s, size_t n, size_t* pParseCount, int* pHR,
        int* pMN, int* pSC, int* pMS )
{
    bool        bResult = false;
    const char* sStart = s;
    size_t      nCount;

    long nValue = ParseDecDigitsT( s, n, &nCount );
    if ( 0 < nCount )
    {
        s += nCount;
        n -= nCount;

        long hr = nValue / 10000;
        nValue -= hr * 10000;
        long mn = nValue / 100;
        nValue -= mn * 100;
        long sc = nValue;
        long ms = 0;

        if ( '.' == *s )
        {
            ++s;
            --n;
            size_t c = 0;
            nValue = ParseDecDigitsT( s, n, &c );
            if ( 0 < c )
            {
                s += c;
                //nCount += c + 1;  // +1 for decimal '.'
                if ( 1 == c )
                    nValue *= 100;
                else if ( 2 == c )
                    nValue *= 10;
                if ( 999 < nValue )
                    nValue = 999;
                ms = nValue;
            }
        }

        if ( pHR )
            *pHR = hr;
        if ( pMN )
            *pMN = mn;
        if ( pSC )
            *pSC = sc;
        if ( pMS )
            *pMS = ms;

        bResult = true;
    }

    if ( pParseCount )
        *pParseCount = s - sStart;

    return bResult;
}


bool
YogiGpsParse::parseDate( const char* s, size_t n, size_t* pParseCount, int* pYY,
        int* pMM, int* pDD )
{
    // format: DDMMYY
    bool   bResult = false;
    size_t nCount;

    long dy = 0;
    long mo = 0;
    long yr = 0;


    long nValue = ParseDecDigitsT( s, n, &nCount );
    if ( 0 < nCount )
    {
        dy = nValue / 10000;
        nValue -= dy * 10000;
        mo = nValue / 100;
        nValue -= mo * 100;
        yr = nValue;

        if ( 0 < yr )
            yr += 2000;

        bResult = true;
    }

    if ( pYY )
        *pYY = yr;
    if ( pMM )
        *pMM = mo;
    if ( pDD )
        *pDD = dy;

    if ( pParseCount )
        *pParseCount = nCount;

    return bResult;
}


// format is DDMM.mmm
float
YogiGpsParse::encodeDegrees( float fLat )
{
    long  nValue = static_cast<long>( fLat );
    long  nDeg = nValue / 100;
    float dValue = fLat - static_cast<float>( nDeg * 100 );
    return static_cast<float>( nDeg ) + dValue / 60.0;
}

bool
YogiGpsParse::decodeDegrees( const char* s, size_t n, size_t* pParseCount,
        long* pDegPart, long* pMinPart, long* pSecPart, long* pSecFracPart )
{
    bool        bResult = false;
    const char* sStart = s;

    do
    {
        size_t nCount = 0;
        long   nDegPart = 0;
        long   nMinPart = 0;
        long   nSecPart = 0;
        long   nSecFracPart = 0;

        long dddmm = ParseDecDigitsT( s, n, &nCount );
        if ( nCount < 1 )
            break;
        s += nCount;
        n -= nCount;
        nDegPart = dddmm / 100;             // truncate minutes
        nMinPart = dddmm - nDegPart * 100;  // remove degrees
        nSecPart = 0;                       // calculate sec-part
        nSecFracPart = 0;                   // calculate sec-frac-part

        if ( '.' == *s )
        {
            fFracPart = ParseSimpleFloatT( s, n, &nCount );
            if ( nCount < 3 )
                break;
            s += nCount;
            n -= nCount;
        }

        if ( pDegPart )
            *pDegPart = nDegPart;
        if ( pMinPart )
            *pMinPart = nMinPart;
        if ( pSecPart )
            *pSecPart = nSecPart;
        if ( pSecFracPart )
            *pSecFracPart = nSecFracPart;

        bResult = true;
    } while ( 0 );

    if ( pParseCount )
        *pParseCount = s - sStart;


    return bResult;
}

float
YogiGpsParse::parseDegrees( const char* s, size_t n, size_t* pParseCount,
        long* pFixedPoint, float* pNmeaDeg )
{
    float fDeg = 0.0;
    float fNmeaDeg = 0.0;
    long  nFixedDeg = 0;

    size_t      nParseCount = 0;
    const char* sStart = s;
    size_t      c;  // count

    long nDegPart = 0;
    long nMinPart = 0;
    long nSecPart = 0;
    long nSecFracPart = 0;

    if ( ',' == *s )
    {
        // empty value
        ++s;
        --n;
        if ( ',' != *s && '*' != *s )
        {
            s = sStart;
            Serial.println( "parseDegrees: failure on empty" );
        }
    }
    else
    {
        if ( decodeDegrees( s, n, &c,  //
                     &nDegPart, &nMinPart, &nSecPart, &nSecFracPart ) )
        {
            // NEEDS_WORK: rework for new sec-part and sec-frac-part
            nFixedDeg = nDegPart * 10000000 + ( nMinPart * 10000000 ) / 60
                    + ( fFracPart * 10000000 ) / 60;
            fNmeaDeg = nDegPart * 100 + nMinPart + fFracPart;
            fDeg = nFixedDeg / 10000000.0;

            if ( ',' == *s )
            {
                ++s;
                --n;

                switch ( *s )
                {
                case 'N':
                    ++s;
                    break;
                case 'S':
                    ++s;
                    fDeg *= -1.0;
                    fNmeaDeg *= -1.0;
                    nFixedDeg *= -1;
                    break;
                case 'E':
                    ++s;
                    break;
                case 'W':
                    ++s;
                    fDeg *= -1.0;
                    fNmeaDeg *= -1.0;
                    nFixedDeg *= -1;
                    break;
                case ',':
                default:
                    fDeg = 0.0;
                    fNmeaDeg = 0.0;
                    nFixedDeg = 0;
                    s = sStart;
                    break;
                }
            }
        }
    }

    if ( pParseCount )
        *pParseCount = s - sStart;

    if ( pFixedPoint )
        *pFixedPoint = nFixedDeg;
    if ( pNmeaDeg )
        *pNmeaDeg = fNmeaDeg;

    return fDeg;
}


void
YogiGpsParse::debugPrintMsg( const char* sPrefix, const char* s, size_t n )
{
    Serial.print( sPrefix );
    const char* sEnd = s + n;
    while ( s < sEnd )
    {
        Serial.print( *s );
        ++s;
    }
    Serial.println( "" );
}
