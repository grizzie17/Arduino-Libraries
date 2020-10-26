#define _DEBUG

#include <YogiDebug.h>
#include <YogiMath.h>
#include <YogiParse.h>


#if ! defined( __SIZE_TYPE__ )
typedef unsigned long size_t;
#endif


bool
decodeDegrees( const char* s, size_t n, size_t* pParseCount,  //
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

        long dddmm = Yogi::ParseText::parseDecDigits( s, n, &nCount );
        if ( nCount < 1 )
            break;
        s += nCount;
        n -= nCount;
        nDegPart = dddmm / 100;             // truncate minutes
        nMinPart = dddmm - nDegPart * 100;  // remove degrees

        if ( '.' == *s )
        {
            ++s;
            long nTemp = Yogi::ParseText::parseDecDigits( s, n, &nCount );
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
parseDegrees( const char* s, size_t n, size_t* pParseCount,  //
        long* pFixedPoint, float* pNmeaDeg )
{
    float fDeg = 0.0;
    float fNmeaDeg = 0.0;
    long  nFixedDeg = 0;

    size_t      nParseCount = 0;
    const char* sStart = s;
    size_t      c;  // count

    long  nDegPart = 0;
    long  nMinPart = 0;
    long  nSecPart = 0;
    long  nSecFracPart = 0;
    float fFracPart = 0;

    if ( ',' == *s )
    {
        // empty value
        ++s;
        --n;
        if ( ',' != *s )
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
#if false
            // TODO: rework algorithm
            nFixedDeg = nDegPart * 10000000 + ( nMinPart * 10000000 ) / 60
                    + ( fFracPart * 10000000 ) / 60;
            fNmeaDeg = nDegPart * 100 + nMinPart + fFracPart;
            fDeg = nFixedDeg / 10000000.0;
#endif
            s += c;
            n -= c;

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


char sLatNmea[] = "4717.112671,N";
char g_sLat[] = "3448.35902,N";
char g_sLon[] = "08641.94026,W";


void
setup()
{
    DEBUG_OPEN( "Test Degrees Conversion" );

    float fNmeaLat = 4717.112671;
    float fNmeaLon = 00833.914843;
    float fDegLat = 47.28521118;
    float fDegLon = 8.56524738;

    size_t      nCount = 0;
    const char* s = sLatNmea;
    size_t      n = strlen( sLatNmea );

    long  nLatFixed = 0;
    float fLatNmea = 0.0;
    float fLat = parseDegrees( s, n, &nCount, &nLatFixed, &fLatNmea );

    Serial.print( "s=" );
    Serial.println( s );
    Serial.print( "nmea=" );
    Serial.println( fLatNmea, 6 );
    Serial.print( "deg=" );
    Serial.println( fLat, 8 );
    Serial.print( "fix=" );
    Serial.println( nLatFixed );
}


void
loop()
{}
