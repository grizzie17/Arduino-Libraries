//=====================================================================//
//
//!	@file	TCharDescriptor.h
//!	@brief	Implement a character descriptor
//!
//!	@warning
//!		There is NO memory management included in any of the
//!		methods for a Character Descriptor
//!
//!	@note
//!		A Character descriptor contains a pointer to a character
//!		and a length (number of characters, not a byte count).
//!		The length does not include any terminating null (if present).
//
//=====================================================================//
/*---------------------------------------------------------------------+\
|
|	Revision History:					(most recent entries first)
|
	03-Dec-2015			J.Griswold
		Move to 'Root' namespace.
	12-Jul-2010			S.Snider		(Reviewed by: R.Hosea)
		Remove return in CompareFiltered - unreachable code.
	21-Jan-2010			J.Griswold		(Reviewed by: M.Rose)
		Add new ParseFloat, ParseInt, ParseDec, ParseDecDigits
		ParseHex, and ParseHexDigits functions
	18-Dec-2009			J.Griswold		(Reviewed by: M.Rose)
		change equals operator to be const
	08-Dec-2009			J.Griswold		(Reviewed by: M.Rose)
		minor fix for the "Compare" function
	06-Dec-2009			J.Griswold		(Reviewed by: M.Rose)
		Remove from Gauge namespace
	05-Oct-2009			J.Griswold		(Reviewed by: M.Rose)
		File the Compare function to properly operate it was
		operating in a similar manner to strncmp.
	03-Oct-2009			J.Griswold		(Reviewed by: M.Rose)
		Compare function did not correctly handle null pointers
	15-Sep-2009			M.Rose			(Reviewed by: J.Griswold)
		Manipulate code to satisfy linux gcc 4.2.1 compiler
	09-Sep-2009			J.Griswold		(Reviewed by: M.Rose)
		Add ConcatenateTo function
	08-Sep-2009			J.Griswold		(Reviewed by: M.Rose)
		Add CopyTo function
		Also change some of the compare functions to inline
	31-Aug-2009			J.Griswold		(Reviewed by: R.Hosea)
		Add CompareFiltered
	28-Aug-2009			J.Griswold		(Reviewed by: M.Rose)
		Initial Revision
|
\+---------------------------------------------------------------------*/
#ifndef _H_TCharDescriptor
#define _H_TCharDescriptor
#pragma once
/*---------------------------------------------------------------------+\
|																		|
|	Include Files														|
|																		|
\+---------------------------------------------------------------------*/
//#include "UFloat.h"
//#include "LogFile.h"
//#include "UPlatform.h"
//#include "UTypes.h"

#include <Arduino.h>

#include <ctype.h>
#include <math.h>
#include <stdexcept>

//namespace Yogi { namespace Core {
/*---------------------------------------------------------------------+\
|																		|
|	Defines																|
|																		|
\+---------------------------------------------------------------------*/
#define GFLOAT float
/*---------------------------------------------------------------------+\
|																		|
|	Type Definitions													|
|																		|
\+---------------------------------------------------------------------*/
typedef intptr_t index_t;
/*---------------------------------------------------------------------+\
|																		|
|	Class Definitions													|
|																		|
\+---------------------------------------------------------------------*/

//! @class TCharDescriptor
//!
//!	template class that identifies a descriptor.
//!
//!	A 'descriptor' is logically a struct containing a pointer and
//!	a size.  The data to which the descriptor is pointing does not
//!	need to be null terminated.  Character descriptors are really
//!	handy for parsing text.
//!
//!	@warning
//!		There is NO memory management included in any of the
//!		methods for a Character Descriptor
//!
//!	@note
//!		A Character descriptor contains a pointer to a character
//!		and a length (number of characters, not a byte count).
//!		The length does not include any terminating null (if present).
//!
template <class T>
class TCharDescriptor
{
    //	class lifecycle  ----------------------------------------------------
public:
    TCharDescriptor();
    TCharDescriptor( const TCharDescriptor<T>& r );  // copy constructor
    TCharDescriptor( T* s );  // search for null and set Length
    TCharDescriptor( T* s, size_t n );
    virtual ~TCharDescriptor();

public:
    //	public types  -------------------------------------------------------

    //	public functions  ---------------------------------------------------

    //! assignment operators
    //!
    //!	note: this is not a copy operation but initializes the
    //!		descriptor so that it points to the characters
    //!		and identifies the length
    //!
    TCharDescriptor<T>&
    operator=( const TCharDescriptor<T>& r );
    TCharDescriptor<T>&
    operator=( const T* s );


    T AtIndex            //!@return the character identified by the index
            ( index_t n  //!< [in] requested index (zero based)
            ) const;


    const T* Pointer         //!@return const pointer to character at index
            ( index_t n = 0  //!< [in] requested index
            ) const;


    size_t Length  //!@return number of characters identified by descriptor
            ( void ) const;

    //! CopyTo
    //!
    //!	@note copies the data described by the current descriptor
    //!			to the destination buffer.  If their is enough
    //!			room it is null terminated.  If the target buffer
    //!			is too small then false is returned and no data
    //!			is copied.
    //!
    bool CopyTo                //!@return result of copy operation
            ( T*           s,  //!< [in] address of target buffer to fill
                    size_t n   //!< [in] size of target buffer
            ) const;

    //!	ConcatenateTo
    //!
    //!	@see CopyTo
    //!
    bool ConcatenateTo         //!@return result of concatenate operation
            ( T*           s,  //!< [in] address of target buffer to append
                    size_t n   //!< [in] size of target buffer
            ) const;

    //!	Compare
    //!
    //!	compare the contents of this descriptor with the arguments passed in.
    //!	Operates similarly to strcmp function
    //!
    //!	@return 0 if equal, positive if this greater, negative if arg greater
    //!
    int
    Compare( const TCharDescriptor<T>& r ) const;
    int
    Compare( const T* s, size_t n ) const;
    int
    Compare( const T* s ) const;

    // create a TCharDescriptorCompareFilter function of the same
    //	template type.  This is most frequently used to do compare
    //	while ignoring case.
    int
    CompareFiltered( const TCharDescriptor<T>& r ) const;
    int
    CompareFiltered( const T* s, size_t n ) const;
    int
    CompareFiltered( const T* s ) const;

    //!	parse the descriptor
    long
    ParseInt( size_t* pParseCount ) const;
    GFLOAT
    ParseFloat( size_t* pParseCount ) const;

    uintmax_t
    Hash( void ) const;


protected:
    //	protected types  ----------------------------------------------------

    //	protected functions  ------------------------------------------------

    //! @return number of characters before null; -1 if no null found
    index_t
    FindPossibleNull( size_t nMaxChars );

    //! @return index of character or -1 if not found
    index_t
    FindCharacter( const T* s, T ch, size_t nMaxChars ) const;

    int
    DiffCharacters( T t1, T t2 ) const;

    //	protected data  -----------------------------------------------------

    T*     m_s;  //!< pointer to data
    size_t m_n;  //!< data count (without null)

private:
    //	private functions  --------------------------------------------------

    //	private data  -------------------------------------------------------
};


template <typename T>
T
TCharDescriptorCompareFilter( T c )
{
    return c;
}


/*---------------------------------------------------------------------+\
|																		|
|	External Variables													|
|																		|
\+---------------------------------------------------------------------*/
/*=====================================================================+\
||																		|
||	Function Prototypes													|
||																		|
\+=====================================================================*/

template <typename T>
long
ParseIntT( const T* s, size_t n, size_t* pParseCount = 0 );

template <typename T>
long
ParseDecT( const T* s, size_t n, size_t* pParseCount = 0 );

template <typename T>
long
ParseDecDigitsT( const T* s, size_t n, size_t* pParseCount = 0 );

template <typename T>
long
ParseHexT( const T* s, size_t n, size_t* pParseCount = 0 );

template <typename T>
long
ParseHexDigitsT( const T* s, size_t n, size_t* pParseCount = 0 );

template <typename T>
GFLOAT
ParseFloatT( const T* s, size_t n, size_t* pParseCount = 0 );

template <typename T>
GFLOAT
ParseSimpleFloatT( const T* s, size_t n, size_t* pParseCount = 0 );


/*=====================================================================+\
||																		|
||	Template Functions													|
||																		|
\+=====================================================================*/

/*=====================================================================+\
||	Class Lifecycle Functions											|
\+=====================================================================*/


/*---------------------------------------------------------------------+\

 * TCharDescriptor - constructor

\+--------------------------------------------------------------------**/
template <class T>
TCharDescriptor<T>::TCharDescriptor( void )
        : m_s( 0 )
        , m_n( 0 )
{}


/*---------------------------------------------------------------------+\

 * TCharDescriptor - constructor

\+---------------------------------------------------------------------*/
template <class T>
TCharDescriptor<T>::TCharDescriptor( const TCharDescriptor<T>& r )
        : m_s( r.m_s )
        , m_n( r.m_n )
{}


/*---------------------------------------------------------------------+\

 * TCharDescriptor - constructor

\+---------------------------------------------------------------------*/
template <class T>
TCharDescriptor<T>::TCharDescriptor( T* s )
        : m_s( s )
        , m_n( 0 )
{
    index_t n = FindPossibleNull( 32000 );  // TODO: change to #define
    if ( n < 0 )
        m_n = 0;
    else
        m_n = (size_t)n;
}


/*---------------------------------------------------------------------+\

 * TCharDescriptor - constructor

\+---------------------------------------------------------------------*/
template <class T>
TCharDescriptor<T>::TCharDescriptor( T* s, size_t n )
        : m_s( s )
        , m_n( n )
{}


/*---------------------------------------------------------------------+\

 * ~TCharDescriptor - destructor

\+---------------------------------------------------------------------*/
template <class T>
TCharDescriptor<T>::~TCharDescriptor( void )
{}


/*=====================================================================+\
||	Public Functions													|
\+=====================================================================*/


/*---------------------------------------------------------------------+\

 * operator = - assignment operator

\+---------------------------------------------------------------------*/
template <class T>
TCharDescriptor<T>&
TCharDescriptor<T>::operator=( const TCharDescriptor<T>& r )
{
    m_s = r.m_s;
    m_n = r.m_n;
    return *this;
}


/*---------------------------------------------------------------------+\

 * operator = - assignment operator

\+---------------------------------------------------------------------*/
template <class T>
TCharDescriptor<T>&
TCharDescriptor<T>::operator=( const T* s )
{
    m_s = s;

    index_t n = FindPossibleNull( 32000 );
    if ( n < 0 )
        m_n = 0;
    else
        m_n = n;
    return *this;
}


/*---------------------------------------------------------------------+\

 * operator== - Equality

\+---------------------------------------------------------------------*/
template <class T>
inline bool
operator==( const TCharDescriptor<T>& lhs, const TCharDescriptor<T>& rhs )
{
    return 0 == lhs.Compare( rhs );
}


template <class T>
inline bool
operator==( const TCharDescriptor<T>& lhs, const T* rhs )
{
    return 0 == lhs.Compare( rhs );
}

template <class T>
inline bool
operator==( const T* lhs, const TCharDescriptor<T>& rhs )
{
    return 0 == rhs.Compare( lhs );
}


/*---------------------------------------------------------------------+\

 * operator!= - Non-Equality

\+---------------------------------------------------------------------*/
template <class T>
inline bool
operator!=( const TCharDescriptor<T>& lhs, const TCharDescriptor<T>& rhs )
{
    return 0 != lhs.Compare( rhs );
}

template <class T>
inline bool
operator!=( const TCharDescriptor<T>& lhs, const char* rhs )
{
    return 0 != lhs.Compare( rhs );
}

template <class T>
inline bool
operator!=( const char* lhs, const TCharDescriptor<T>& rhs )
{
    return 0 != rhs.Compare( lhs );
}


/*---------------------------------------------------------------------+\

 * operator &lt; - Less-Than

\+---------------------------------------------------------------------*/
template <class T>
inline bool
operator<( const TCharDescriptor<T>& lhs, const TCharDescriptor<T>& rhs )
{
    return 0 > lhs.Compare( rhs );
}


template <class T>
inline bool
operator<( const TCharDescriptor<T>& a, const T* b )
{
    return 0 > a.Compare( b );
}


template <class T>
inline bool
operator<( const T* a, const TCharDescriptor<T>& b )
{
    return 0 < b.Compare( a );
}


/*---------------------------------------------------------------------+\

 * operator &le; - Less-Than-Or-Equal

\+---------------------------------------------------------------------*/
template <class T>
inline bool
operator<=( const TCharDescriptor<T>& lhs, const TCharDescriptor<T>& rhs )
{
    return ! ( lhs > rhs );
}


template <class T>
inline bool
operator<=( const TCharDescriptor<T>& a, const T* b )
{
    return ! ( a > b );
}


template <class T>
inline bool
operator<=( const T* a, const TCharDescriptor<T>& b )
{
    return ! ( a > b );
}


/*---------------------------------------------------------------------+\

 * operator &gt; - Greater-Than

\+---------------------------------------------------------------------*/
template <class T>
inline bool
operator>( const TCharDescriptor<T>& lhs, const TCharDescriptor<T>& rhs )
{
    return 0 < lhs.Compare( rhs );
}


template <class T>
inline bool
operator>( const TCharDescriptor<T>& a, const T* b )
{
    return 0 < a.Compare( b );
}


template <class T>
inline bool
operator>( const T* a, const TCharDescriptor<T>& b )
{
    return 0 > b.Compare( a );
}

/*---------------------------------------------------------------------+\

 * operator &ge; - Greater-Than-Or-Equal

\+---------------------------------------------------------------------*/
template <class T>
inline bool
operator>=( const TCharDescriptor<T>& lhs, const TCharDescriptor<T>& rhs )
{
    return ! ( lhs < rhs );
}


template <class T>
inline bool
operator>=( const TCharDescriptor<T>& a, const T* b )
{
    return ! ( a < b );
}


template <class T>
inline bool
operator>=( const T* a, const TCharDescriptor<T>& b )
{
    return ! ( a < b );
}


/*---------------------------------------------------------------------+\

 * AtIndex -

\+---------------------------------------------------------------------*/
template <class T>
T
TCharDescriptor<T>::AtIndex( index_t n ) const
{
    if ( n < index_t( m_n ) )
        return *( m_s + n );
    else
        return 0;
}


/*---------------------------------------------------------------------+\

 * Pointer -

\+---------------------------------------------------------------------*/
template <class T>
inline const T*
TCharDescriptor<T>::Pointer( index_t n ) const
{
    if ( n < index_t( m_n ) )
        return m_s + n;
    else
        return 0;
}


/*---------------------------------------------------------------------+\

 * CopyTo - copy data from descriptor to target buffer

\+---------------------------------------------------------------------*/
template <class T>
bool
TCharDescriptor<T>::CopyTo( T* t, size_t tc ) const
{
    bool bResult = true;

    if ( tc < m_n + 1 )  // add one for null
    {
        bResult = false;
    }
    else
    {
        const T* s = m_s;
        const T* sEnd = s + m_n;
        T*       tEnd = t + tc;

        while ( t < tEnd )
        {
            if ( s < sEnd )
            {
                if ( *s )
                {
                    *t++ = *s++;
                }
                else
                {
                    *t = *s;
                    break;
                }
            }
            else
            {
                // we null terminate if there is room
                *t = 0;
                break;
            }
        }
    }

    return bResult;
}


/*---------------------------------------------------------------------+\

 * ConcatenateTo - copy data from descriptor to target buffer

\+---------------------------------------------------------------------*/
template <class T>
bool
TCharDescriptor<T>::ConcatenateTo( T* t, size_t tc ) const
{
    bool    bResult = false;
    index_t tn = FindCharacter( t, 0, tc );
    if ( -1 < tn )
    {
        if ( tn < index_t( tc ) )
        {
            t += tn;
            tc -= tn;
            if ( m_n <= tc )
                bResult = CopyTo( t, tc );
        }
    }

    return bResult;
}


/*---------------------------------------------------------------------+\

 * Compare -

\+---------------------------------------------------------------------*/
template <class T>
inline int
TCharDescriptor<T>::Compare( const TCharDescriptor<T>& r ) const
{
    return Compare( r.m_s, r.m_n );
}


/*---------------------------------------------------------------------+\

 * Compare -

\+---------------------------------------------------------------------*/
template <class T>
int
TCharDescriptor<T>::Compare( const T* s, size_t n ) const
{
    int      nResult = 0;  // assume equal
    const T* p1 = m_s;
    const T* p1End = p1 + m_n;
    T        t1;
    const T* p2 = s;
    const T* p2End = p2 + n;
    T        t2;

    if ( p1 && p2 )  // check for null
    {
        while ( p1 < p1End )
        {
            if ( p2 < p2End )
            {
                try
                {
                    t1 = *p1++;
                    t2 = *p2++;
                    nResult = DiffCharacters( t1, t2 );
                    if ( nResult )
                        return nResult;
                }
                catch ( std::exception const& e )
                {
                    //DbgPrint( "***** Memory addressing error in Compare ***** \n");
                    //DbgPrint( "%s", e.what() );
                    std::exception ex = e;
                    ex = ex;
                    nResult = -1;
                    return nResult;
                }
                BEGIN_IGNORE_WARNING( 4571 )
                catch ( ... )
                {
                    //DbgPrint( "***** Memory addressing error in Compare ***** \n");
                    throw;
                }
                END_IGNORE_WARNING
            }
            else
            {
                return 1;
            }
        }
        if ( p2 < p2End )
            return -1;
        else
            return 0;
    }
    else if ( p2 )
    {
        return -1;
    }
    else if ( p1 )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/*---------------------------------------------------------------------+\

 * Compare -

\+---------------------------------------------------------------------*/
template <class T>
int
TCharDescriptor<T>::Compare( const T* s ) const
{
    int      nResult = 0;  // assume equal
    const T* p1 = m_s;
    const T* p1End = p1 + m_n;
    T        t1;
    const T* p2 = s;
    T        t2;

    if ( p1 && p2 )
    {
        while ( p1 < p1End )
        {
            t2 = *p2++;
            if ( t2 )
            {
                t1 = *p1++;
                nResult = DiffCharacters( t1, t2 );
                if ( nResult )
                    return nResult;
            }
            else
            {
                return 1;
            }
        }
        if ( *p2 )
            return -1;
        else
            return 0;
    }
    else if ( p2 )
    {
        return -1;
    }
    else if ( p1 )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/*---------------------------------------------------------------------+\

 * CompareFiltered -

\+---------------------------------------------------------------------*/
template <class T>
inline int
TCharDescriptor<T>::CompareFiltered( const TCharDescriptor<T>& r ) const
{
    return CompareFiltered( r.m_s, r.m_n );
}


/*---------------------------------------------------------------------+\

 * CompareFiltered -

\+---------------------------------------------------------------------*/
template <class T>
int
TCharDescriptor<T>::CompareFiltered( const T* s, size_t n ) const
{
    int      nResult = 0;  // assume equal
    const T* p1 = m_s;
    const T* p1End = p1 + m_n;
    T        t1;
    const T* p2 = s;
    const T* p2End = p2 + n;
    T        t2;

    if ( p1 && p2 )  // check for null
    {
        while ( p1 < p1End )
        {
            if ( p2 < p2End )
            {
                t1 = TCharDescriptorCompareFilter( *p1++ );
                t2 = TCharDescriptorCompareFilter( *p2++ );
                nResult = DiffCharacters( t1, t2 );
                if ( nResult )
                    return nResult;
            }
            else
            {
                return 1;
            }
        }
        if ( p2 < p2End )
            return -1;
        else
            return 0;
    }
    else if ( p2 )
    {
        return -1;
    }
    else if ( p1 )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/*---------------------------------------------------------------------+\

 * CompareFiltered -

\+---------------------------------------------------------------------*/
template <class T>
int
TCharDescriptor<T>::CompareFiltered( const T* s ) const
{
    int      nResult = 0;  // assume equal
    const T* p1 = m_s;
    const T* p1End = p1 + m_n;
    T        t1;
    const T* p2 = s;
    T        t2;

    if ( p1 && p2 )
    {
        while ( p1 < p1End )
        {
            t2 = TCharDescriptorCompareFilter( *p2++ );
            if ( t2 )
            {
                t1 = TCharDescriptorCompareFilter( *p1++ );
                nResult = DiffCharacters( t1, t2 );
                if ( nResult )
                    return nResult;
            }
            else
            {
                return 1;
            }
        }
        if ( *p2 )
            return -1;
        else
            return 0;
    }
    else if ( p2 )
    {
        return -1;
    }
    else if ( p1 )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/*---------------------------------------------------------------------+\

 * ParseInt -

\+---------------------------------------------------------------------*/
template <class T>
long
TCharDescriptor<T>::ParseInt( size_t* pParseCount ) const
{
    return ParseIntT<T>( m_s, m_n, pParseCount );
}


/*---------------------------------------------------------------------+\

 * ParseInt -

\+---------------------------------------------------------------------*/
template <class T>
long
ParseIntT( const T* s, size_t n, size_t* pParseCount )
{
    long   nValue = 0;
    size_t nCount;

    nValue = ParseHexT<T>( s, n, &nCount );
    if ( nCount < 1 )
        nValue = ParseDecT<T>( s, n, &nCount );

    if ( pParseCount )
        *pParseCount = nCount;

    return nValue;
}


/*---------------------------------------------------------------------+\

 * ParseDec -

\+---------------------------------------------------------------------*/
template <typename T>
long
ParseDecT( const T* s, size_t n, size_t* pParseCount )
{
    long     nValue = 0;
    size_t   nCount;
    const T* sEnd = s + n;
    const T* sStart = s;
    long     nSign = 1;

    if ( s < sEnd )
    {
        if ( '-' == *s )
        {
            nSign = -1;
            ++s;
        }
    }

    nValue = ParseDecDigitsT<T>( s, ( size_t )( sEnd - s ), &nCount );
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


/*---------------------------------------------------------------------+\

 * ParseDecDigits -

\+---------------------------------------------------------------------*/
template <typename T>
long
ParseDecDigitsT( const T* s, size_t n, size_t* pParseCount )
{
    long     nValue = 0;
    const T* sEnd = s + n;
    const T* sStart = s;

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


/*---------------------------------------------------------------------+\

 * ParseHex -

\+---------------------------------------------------------------------*/
template <typename T>
long
ParseHexT( const T* s, size_t n, size_t* pParseCount )
{
    long     nValue = 0;
    size_t   nCount;
    const T* sEnd = s + n;
    const T* sStart = s;

    if ( '0' == *s )
    {
        ++s;
        if ( 'x' == ::tolower( *s ) )
        {
            ++s;
            nValue = ParseHexDigitsT<T>( s, ( size_t )( sEnd - s ), &nCount );
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


/*---------------------------------------------------------------------+\

 * ParseHexDigits -

\+---------------------------------------------------------------------*/
template <typename T>
long
ParseHexDigitsT( const T* s, size_t n, size_t* pParseCount )
{
    long     nValue = 0;
    const T* sEnd = s + n;
    const T* sStart = s;
    char     c;

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


/*---------------------------------------------------------------------+\

 * ParseFloat -

\+---------------------------------------------------------------------*/
template <class T>
GFLOAT
TCharDescriptor<T>::ParseFloat( size_t* pParseCount ) const
{
    return ParseFloatT<T>( m_s, m_n, pParseCount );
}


template <typename T>
GFLOAT
ParseSimpleFloatT( const T* s, size_t n, size_t* pParseCount )
{
    GFLOAT   fValue = GFLOAT_0;
    GFLOAT   fSign = GFLOAT_1;
    size_t   nParseCount = 0;
    const T* sStart = s;
    size_t   c;  // count

    do_sequence
    {
        fValue = static_cast<GFLOAT>( ParseDecT<T>( s, n, &c ) );
        if ( c < 1 )
            break;

        s += c;
        n -= c;
        if ( '.' != *s )
            break;

        ++s;
        --n;
        long nFrac = ParseDecDigitsT<T>( s, n, &c );

        if ( c < 1 )
            break;

        s += c;
        n -= c;

        if ( fValue < GFLOAT_0 )
        {
            fSign = GFLOAT_NEG1;
            fValue *= GFLOAT_NEG1;
        }

        nParseCount = ( size_t )( s - sStart );

        fValue += powf( GFLOAT_10, -static_cast<GFLOAT>( c ) )
                * static_cast<GFLOAT>( nFrac );
        fValue *= fSign;
    }

    if ( pParseCount )
        *pParseCount = nParseCount;

    return fValue;
}


template <typename T>
GFLOAT
ParseFloatT( const T* s, size_t n, size_t* pParseCount )
{
    GFLOAT   fValue = GFLOAT_0;
    size_t   nParseCount = 0;
    const T* sStart = s;
    size_t   c;  // count

    do_sequence
    {
        fValue = ParseSimpleFloatT<T>( s, n, &c );
        if ( c < 1 )
        {
            fValue = static_cast<GFLOAT>( ParseDecT<T>( s, n, &c ) );
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
            long nExp = ParseDecT<T>( s, n, &c );
            if ( c < 1 )
                break;

            GFLOAT fSign = GFLOAT_1;
            if ( fValue < GFLOAT_0 )
            {
                fSign = GFLOAT_NEG1;
                fValue *= GFLOAT_NEG1;
            }

            s += c;
            n -= c;
            fValue = powf( GFLOAT_10, static_cast<GFLOAT>( nExp ) ) * fValue;
            fValue *= fSign;
        }
        nParseCount = ( size_t )( s - sStart );
    }

    if ( pParseCount )
        *pParseCount = nParseCount;

    return fValue;
}


/*---------------------------------------------------------------------+\

 * Length -

\+---------------------------------------------------------------------*/
template <class T>
inline size_t
TCharDescriptor<T>::Length( void ) const
{
    return m_n;
}


/*---------------------------------------------------------------------+\

 * Hash -

\+---------------------------------------------------------------------*/
template <class T>
uintmax_t
TCharDescriptor<T>::Hash( void ) const
{
    uintmax_t hash = 5381;

    T*     arKey = m_s;
    size_t nKeyLength = m_n;

    /* variant with the hash unrolled eight times */
    for ( ; 8 <= nKeyLength; nKeyLength -= 8 )
    {
        hash = ( ( hash << 5 ) + hash ) + *arKey++;
        hash = ( ( hash << 5 ) + hash ) + *arKey++;
        hash = ( ( hash << 5 ) + hash ) + *arKey++;
        hash = ( ( hash << 5 ) + hash ) + *arKey++;
        hash = ( ( hash << 5 ) + hash ) + *arKey++;
        hash = ( ( hash << 5 ) + hash ) + *arKey++;
        hash = ( ( hash << 5 ) + hash ) + *arKey++;
        hash = ( ( hash << 5 ) + hash ) + *arKey++;
    }
    switch ( nKeyLength )
    {
    case 7:
        hash = ( ( hash << 5 ) + hash ) + *arKey++; /* fallthrough... */
    case 6:
        hash = ( ( hash << 5 ) + hash ) + *arKey++; /* fallthrough... */
    case 5:
        hash = ( ( hash << 5 ) + hash ) + *arKey++; /* fallthrough... */
    case 4:
        hash = ( ( hash << 5 ) + hash ) + *arKey++; /* fallthrough... */
    case 3:
        hash = ( ( hash << 5 ) + hash ) + *arKey++; /* fallthrough... */
    case 2:
        hash = ( ( hash << 5 ) + hash ) + *arKey++; /* fallthrough... */
    case 1:
        hash = ( ( hash << 5 ) + hash ) + *arKey++;
        break;
    case 0:
        break;
    default:
        break;
    }
    return hash;
}


/*=====================================================================+\
||	Protected Functions													|
\+=====================================================================*/


/*---------------------------------------------------------------------+\

 * FindPossibleNull -

\+---------------------------------------------------------------------*/
template <class T>
inline index_t
TCharDescriptor<T>::FindPossibleNull( size_t nMaxChars )
{
    return FindCharacter( m_s, 0, nMaxChars );
}


/*---------------------------------------------------------------------+\

 * FindCharacter -

\+---------------------------------------------------------------------*/
template <class T>
index_t
TCharDescriptor<T>::FindCharacter(
        const T* sString, T cSearch, size_t nMaxChars ) const
{
    if ( sString )
    {
        const T* pBegin = sString;
        const T* p = pBegin;
        const T* pEnd = p + nMaxChars;

        while ( p < pEnd )
        {
            if ( *p == cSearch )
                break;
            ++p;
        }
        if ( p < pEnd )
            return long( p - pBegin );
        else
            return -1;
    }
    else
    {
        return -1;
    }
}


/*---------------------------------------------------------------------+\

* FindCharacter -

\+---------------------------------------------------------------------*/
template <class T>
inline int
TCharDescriptor<T>::DiffCharacters( T t1, T t2 ) const
{
    const int mask = ~0 >> 1;
    return ( (int)t1 & mask ) - ( (int)t2 & mask );
}


//}}  // namespace Yogi::Core


#endif /* _H_TCharDescriptor */
