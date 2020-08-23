
#include "YogiParse.h"


namespace Yogi { namespace ParseText {


const char*
parseSymbol( const char* s, size_t n, size_t* pParseCount )
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
parseHexDigits( const char* s, size_t n, size_t* pParseCount )
{
    long        nValue = 0;
    const char* sEnd = s + n;
    const char* sStart = s;
    char        c;

    while ( s < sEnd )
    {
        nValue *= 16;
        c = static_cast<char>( ::toupper( *s ) );
        if ( '0' <= c && c <= '9' )
            nValue += c - '0';
        else if ( 'A' <= c && c <= 'F' )
            nValue += c - 'A' + 10;
        ++s;
    }

    if ( pParseCount )
        *pParseCount = ( size_t )( s - sStart );

    return nValue;
}


long
parseHexidecimal( const char* s, size_t n, size_t* pParseCount )
{
    long        nValue = 0;
    size_t      nCount;
    const char* sEnd = s + n;
    const char* sStart = s;

    if ( '0' == *s )
    {
        ++s;
        if ( 'x' == ::tolower( *s ) )
        {
            ++s;
            nValue = parseHexDigits( s, ( size_t )( sEnd - s ), &nCount );
            if ( 0 < nCount )
            {
                s += nCount;
            }
            else
            {
                s = sStart;
            }
        }
    }

    if ( pParseCount )
        *pParseCount = ( size_t )( s - sStart );

    return nValue;
}


long
parseDecDigits( const char* s, size_t n, size_t* pParseCount )
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
parseDecimal( const char* s, size_t n, size_t* pParseCount )
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

    nValue = parseDecDigits( s, ( size_t )( sEnd - s ), &nCount );
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


long
parseInteger( const char* s, size_t n, size_t* pParseCount )
{
    long   nValue = 0;
    size_t nCount;

    nValue = parseHexidecimal( s, n, &nCount );
    if ( nCount < 1 )
        nValue = parseDecimal( s, n, &nCount );

    if ( pParseCount )
        *pParseCount = nCount;

    return nValue;
}


float
parseSimpleFloat( const char* s, size_t n, size_t* pParseCount )
{
    float       fValue = 0.0;
    float       fSign = 1.0;
    size_t      nParseCount = 0;
    const char* sStart = s;
    size_t      c;  // count

    do
    {
        fValue = static_cast<float>( parseDecimal( s, n, &c ) );
        // if ( c < 1 )
        //     break;

        s += c;
        n -= c;
        if ( '.' != *s )
            break;

        ++s;
        --n;
        long nFrac = parseDecDigits( s, n, &c );

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


float
parseFloat( const char* s, size_t n, size_t* pParseCount )
{
    float       fValue = 0.0f;
    size_t      nParseCount = 0;
    const char* sStart = s;
    size_t      c;  // count

    do
    {
        fValue = parseSimpleFloat( s, n, &c );
        if ( c < 1 )
        {
            fValue = static_cast<float>( parseDecimal( s, n, &c ) );
            if ( c < 1 )
                break;
        }

        s += c;
        n -= c;
        int ce = tolower( *s );
        if ( 'e' == ce || 'f' == ce )
        {
            ++s;
            --n;
            long nExp = parseDecimal( s, n, &c );
            if ( c < 1 )
                break;

            float fSign = 1.0f;
            if ( fValue < 0.0f )
            {
                fSign = -1.0f;
                fValue *= -1.0f;
            }

            s += c;
            n -= c;
            fValue = powf( 10.0f, static_cast<float>( nExp ) ) * fValue;
            fValue *= fSign;
        }
        nParseCount = ( size_t )( s - sStart );
    } while ( 0 );

    if ( pParseCount )
        *pParseCount = nParseCount;

    return fValue;
}


}}  // namespace Yogi::ParseText
