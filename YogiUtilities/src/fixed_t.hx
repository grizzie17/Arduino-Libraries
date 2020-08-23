/*------------------------------------------------------------------*//**
*
*	@file	fixed_t.h
*	@brief	class that operates as a fixed_point data type
*
*//*-------------------------------------------------------------------*/
/*---------------------------------------------------------------------+\
|
|	Revision History:					(most recent entries first)
|
	14-Dec-2009			J.Griswold		(Reviewed by: M.Rose)
		Add more overloaded operators to deal with 'long' as a
		left-hand operand.
    14-Dec-2009         R.Hosea        (Reviewed by: J.Griswold)
        Fixed gcc compile problem for omniscape
	07-Dec-2009			J.Griswold		(Reviewed by: M.Rose)
		Move the more common (and small) functions to be inline
		Also provide several predefined constants for the more
		common floating point constants
	05-Dec-2009			J.Griswold		(Reviewed by: M.Rose)
		Initial Revision
|
\+---------------------------------------------------------------------*/
#ifndef _H_fixed_t
#define _H_fixed_t
#pragma once

//#define FIXED_USE_DOUBLE 1


/*---------------------------------------------------------------------+\
|																		|
|	Include Files														|
|																		|
\+---------------------------------------------------------------------*/
#if FIXED_USE_DOUBLE
// #    include "UWarnings.h"
// BEGIN_IGNORE_WARNING( 4514 )  // unreferenced inline function
#    include <float.h>
#    include <math.h>
// END_IGNORE_WARNING
#endif
// #include "UDeclPlatform.h"
//#include "UMath.h"

//
/*---------------------------------------------------------------------+\
|																		|
|	Defines																|
|																		|
\+---------------------------------------------------------------------*/

//namespace Yogi { namespace Common {


#ifndef FIXED_USE_DOUBLE

// 16.16 format

// with this we get a max of 32767.999985 with resolution 1.52588e-5.

#    define FP_DEC_BITS 16          // # bits used to represent the decimal part
#    define FP_DEC_MASK 0x0000FFFF  // to get the decimal part

#    define FXD_F_MAX     32767.999985
#    define FXD_F_MIN     -FXD_F_MAX
#    define FXD_F_EPSILON 1.52588e-5

#    define FXD_MAX_INT   32767
#    define FXD_MAX_LOCAL 2147483647


#    define fp_max_value FXD_MAX_INT
#    define fp_min_value -FXD_MAX_INT

#    define FXD_0      fixed_t::k_0       //	0
#    define FXD_1      fixed_t::k_1       //	1
#    define FXD_NEG1   fixed_t::k_neg1    //	-1
#    define FXD_1_10   fixed_t::k_1_10    //	1/10
#    define FXD_1_2    fixed_t::k_1_2     //	1/2
#    define FXD_NEG1_2 fixed_t::k_neg1_2  //	-1/2
#    define FXD_2      fixed_t::k_2       //	2
#    define FXD_PI     fixed_t::k_PI      //	pi
#    define FXD_PI_2   fixed_t::k_PI_2    //	pi/2
#    define FXD_2PI    fixed_t::k_2PI     //	2*pi
#    define FXD_10     fixed_t::k_10      //	10
#    define FXD_e      fixed_t::k_e       //	e

#    define FXD_MAX     fixed_t::k_MAX
#    define FXD_MIN     fixed_t::k_MIN
#    define FXD_EPSILON fixed_t::k_EPSILON

//#define fp_1           65536    // 1
//#define fp_1_2         32768    // 1/2
//#define fp_2           131072   // 2
//#define fp_pi          205887   // pi
//#define fp_pi_2        102943   // pi/2
//#define fp_2_pi        411774   // 2pi
//#define fp_e           178145   // e
//#define fp_epsilon     65       // 0.001

// almost the same angular resolution/storage requirements as above
#    define fp_trig_shift 9  // every 512 angle values we have a table value

#    define pgm_read_fixed pgm_read_dword

#else  // FIXED_USE_DOUBLE

// although it's ultra slow, it may be useful to replace our
// computations with doubles or floats for accuracy comparision or
// overflow testing


#    define fp_max_value FLT_MAX
#    define fp_min_value -FLT_MAX

#    define fp_1       1.0             // 1
#    define fp_1_2     0.5             // 1/2
#    define fp_2       2.0             // 2
#    define fp_pi      M_PI            // pi
#    define fp_pi_2    M_PI_2          // pi/2
#    define fp_2_pi    ( 2.0 * M_PI )  // 2pi
#    define fp_e       M_E             // e
#    define fp_epsilon 1e-5            // 0.00001

#    define fp_trig_shift \
        9  // not actually used with doubles, here for compilation
#    define pgm_read_fixed pgm_read_dword  // also not used


#endif


/*---------------------------------------------------------------------+\
|																		|
|	Type Definitions													|
|																		|
\+---------------------------------------------------------------------*/


#ifndef FIXED_USE_DOUBLE

// 16.16 format

// with this we get a max of 32767.999985 with resolution 1.52588e-5.


typedef long local_fixed_t;  // used as local data
//typedef	long 	long_fixed_t;	// used internally for mul/div
typedef long long long_fixed_t;  // used internally for mul/div

// forward declaration
class fixed_t;  // public class


#else

typedef float  local_fixed_t;
typedef double long_fixed_t;

typedef float fixed_t;


#endif


/*---------------------------------------------------------------------+\
|																		|
|	Class Definitions													|
|																		|
\+---------------------------------------------------------------------*/

#ifndef FIXED_USE_DOUBLE


class fixed_t
{
    //	class lifecycle  ----------------------------------------------------
public:
    fixed_t( void );
    fixed_t( const fixed_t& r );  // copy constructor
    fixed_t( const fixed_t* p );
    fixed_t( const long n );
    fixed_t( const int n );
    fixed_t( const short n );
    fixed_t( const double d );
    fixed_t( const float f );

public:
    //	public types  -------------------------------------------------------

    static fixed_t k_0;       //	0
    static fixed_t k_1;       //	1
    static fixed_t k_neg1;    //	-1
    static fixed_t k_1_10;    //	1/10	0.1
    static fixed_t k_1_2;     //	1/2
    static fixed_t k_neg1_2;  //	-1/2
    static fixed_t k_2;       //	2
    static fixed_t k_PI;      //	pi
    static fixed_t k_PI_2;    //	pi/2
    static fixed_t k_2PI;     //	2*pi
    static fixed_t k_10;      //	10
    static fixed_t k_180;     //	180
    static fixed_t k_360;     //	360
    static fixed_t k_e;       //	e

    static fixed_t k_MAX;
    static fixed_t k_MIN;
    static fixed_t k_EPSILON;  //

    //	public functions  ---------------------------------------------------

    fixed_t&
    operator=( const fixed_t& r );  // assignment
    fixed_t&
    operator=( const float f );
    fixed_t&
    operator=( const double d );
    fixed_t&
    operator=( const int n );
    fixed_t&
    operator=( const unsigned int n );
    fixed_t&
    operator=( const long n );
    fixed_t&
    operator=( const unsigned long n );
    fixed_t&
    operator=( const short n );
    fixed_t&
    operator=( const unsigned short n );
    fixed_t&
    operator=( const char n );
    fixed_t&
    operator=( const unsigned char n );

    operator float( void );
    operator double( void );
    operator int( void );
    operator unsigned int( void );
    operator short( void );
    operator unsigned short( void );
    operator long( void );
    operator unsigned long( void );

    bool
    Equals( const fixed_t& f ) const;
    bool
    operator==( const fixed_t& f ) const;
    bool
    operator==( const float f ) const;
    bool
    operator==( const double d ) const;
    bool
    operator==( const int n ) const;
    bool
    operator==( const long n ) const;
    bool
    operator==( const short n ) const;

    bool
    NotEqual( const fixed_t& f ) const;
    bool
    operator!=( const fixed_t& f ) const;
    bool
    operator!=( const float f ) const;
    bool
    operator!=( const double d ) const;
    bool
    operator!=( const int n ) const;
    bool
    operator!=( const long n ) const;
    bool
    operator!=( const short n ) const;

    bool
    LessThan( const fixed_t& f );
    bool
    operator<( const fixed_t& f );
    bool
    operator<( const float f );
    bool
    operator<( const double d );
    bool
    operator<( const int n );
    bool
    operator<( const long n );
    bool
    operator<( const short n );

    bool
    LessThanEqual( const fixed_t& f );
    bool
    operator<=( const fixed_t& f );
    bool
    operator<=( const float f );
    bool
    operator<=( const double d );
    bool
    operator<=( const int n );
    bool
    operator<=( const long n );
    bool
    operator<=( const short n );

    bool
    GreaterThan( const fixed_t& f );
    bool
    operator>( const fixed_t& f );
    bool
    operator>( const float f );
    bool
    operator>( const double d );
    bool
    operator>( const int n );
    bool
    operator>( const long n );
    bool
    operator>( const short n );

    bool
    GreaterThanEqual( const fixed_t& f );
    bool
    operator>=( const fixed_t& f );
    bool
    operator>=( const float f );
    bool
    operator>=( const double d );
    bool
    operator>=( const int n );
    bool
    operator>=( const long n );
    bool
    operator>=( const short n );

    fixed_t&
    operator++( void );
    fixed_t&
    operator--( void );
    fixed_t
    operator-( void );  // negate

    fixed_t
    Add( const fixed_t& f );
    fixed_t
    operator+( const fixed_t& f );
    fixed_t
    operator+( const float f );
    fixed_t
    operator+( const double d );
    fixed_t
    operator+( const int n );
    fixed_t
    operator+( const long n );
    fixed_t
    operator+( const short n );

    fixed_t
    Subtract( const fixed_t& f );
    fixed_t
    operator-( const fixed_t& f );
    fixed_t
    operator-( const float f );
    fixed_t
    operator-( const double d );
    fixed_t
    operator-( const int n );
    fixed_t
    operator-( const long n );
    fixed_t
    operator-( const short n );

    fixed_t
    Multiply( const fixed_t& f );
    fixed_t
    operator*( const fixed_t& f );
    fixed_t
    operator*( const float f );
    fixed_t
    operator*( const double d );
    fixed_t
    operator*( const int n );
    fixed_t
    operator*( const long n );
    fixed_t
    operator*( const short n );

    fixed_t
    Divide( const fixed_t& f );
    fixed_t
    operator/( const fixed_t& f );
    fixed_t
    operator/( const float f );
    fixed_t
    operator/( const double d );
    fixed_t
    operator/( const int n );
    fixed_t
    operator/( const long n );
    fixed_t
    operator/( const short n );

    fixed_t
    Modulo( const fixed_t& f );
    fixed_t
    operator%( const fixed_t& f );
    fixed_t
    operator%( const float f );
    fixed_t
    operator%( const double d );
    fixed_t
    operator%( const int n );
    fixed_t
    operator%( const long n );
    fixed_t
    operator%( const short n );

    fixed_t&
    operator+=( const fixed_t& f );
    fixed_t&
    operator+=( const float f );
    fixed_t&
    operator+=( const double d );
    fixed_t&
    operator+=( const int n );
    fixed_t&
    operator+=( const long n );
    fixed_t&
    operator+=( const short n );

    fixed_t&
    operator-=( const fixed_t& f );
    fixed_t&
    operator-=( const float f );
    fixed_t&
    operator-=( const double d );
    fixed_t&
    operator-=( const int n );
    fixed_t&
    operator-=( const long n );
    fixed_t&
    operator-=( const short n );

    fixed_t&
    MultiplyIn( const fixed_t& f );
    fixed_t&
    operator*=( const fixed_t& f );
    fixed_t&
    operator*=( const float f );
    fixed_t&
    operator*=( const double d );
    fixed_t&
    operator*=( const int n );
    fixed_t&
    operator*=( const long n );
    fixed_t&
    operator*=( const short n );

    fixed_t&
    DivideIn( const fixed_t& f );
    fixed_t&
    operator/=( const fixed_t& f );
    fixed_t&
    operator/=( const float f );
    fixed_t&
    operator/=( const double d );
    fixed_t&
    operator/=( const int n );
    fixed_t&
    operator/=( const long n );
    fixed_t&
    operator/=( const short n );

    fixed_t&
    abs( void );
    fixed_t&
    floor( void );
    fixed_t&
    ceil( void );

    fixed_t&
    cos( void );
    fixed_t&
    sin( void );
    fixed_t&
    tan( void );
    fixed_t&
    atan( void );
    fixed_t&
    sqrt( void );
    fixed_t&
    square( void );


    ///friend CFixedData	CFixedData_cast( fixed_t f );
    friend local_fixed_t
    fixed_t_data( const fixed_t& r );
    friend fixed_t
    ToFixed_t( const local_fixed_t f );

protected:
    //	protected types  ----------------------------------------------------

    //	protected functions  ------------------------------------------------

    // local constructor that loads 'n' directly into the value
    fixed_t( bool bPrivate, local_fixed_t n );


    local_fixed_t
    fx( const double f ) const;
    local_fixed_t
    fx( const float f ) const;
    local_fixed_t
    fx( const int f ) const;
    local_fixed_t
    fx( const unsigned int f ) const;
    local_fixed_t
    fx( const long f ) const;
    local_fixed_t
    fx( const unsigned long f ) const;
    local_fixed_t
    fx( const short f ) const;
    local_fixed_t
    fx( const unsigned short f ) const;


    //	protected data  -----------------------------------------------------

    local_fixed_t m_n;

private:
    //	private functions  --------------------------------------------------

    //	private data  -------------------------------------------------------
};


#endif

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


/*=====================================================================+\
||																		|
||	Inline Functions													|
||																		|
\+=====================================================================*/
#ifndef FIXED_USE_DOUBLE

// conversion tools

/*---------------------------------------------------------------------+\

 * someFunction -

\+---------------------------------------------------------------------*/
inline local_fixed_t
fixed_t_data( const fixed_t& r )
{
    return r.m_n;
}


/*---------------------------------------------------------------------+\

 * someFunction -

\+---------------------------------------------------------------------*/
inline fixed_t
ToFixed_t( const local_fixed_t f )
{
    return fixed_t( true, f );
}


// constructors

/*---------------------------------------------------------------------+\

 * fixed_t - constructors

\+---------------------------------------------------------------------*/
inline fixed_t::fixed_t( void )
        : m_n( 0 )
{}

inline fixed_t::fixed_t( const fixed_t& r )
        : m_n( r.m_n )
{}

inline fixed_t::fixed_t( const fixed_t* p )
        : m_n( p->m_n )
{}


inline fixed_t::fixed_t( const float f )
{
    m_n = fx( f );
}


// assignment operators

/*---------------------------------------------------------------------+\

 * operator = - assignment

\+---------------------------------------------------------------------*/
inline fixed_t&
fixed_t::operator=( const fixed_t& r )
{
    m_n = r.m_n;
    return *this;
}

inline fixed_t&
fixed_t::operator=( const float f )
{
    m_n = fx( f );
    return *this;
}


// comparison operators

/*---------------------------------------------------------------------+\

 * operator == - Equality

\+---------------------------------------------------------------------*/
inline bool
fixed_t::Equals( const fixed_t& f ) const
{
    return m_n == f.m_n;
}

inline bool
fixed_t::operator==( const fixed_t& f ) const
{
    return m_n == f.m_n;
}

inline bool
fixed_t::operator==( const float f ) const
{
    return m_n == fx( f );
}

inline bool
operator==( const float a, const fixed_t& b )
{
    fixed_t x( a );
    return x == b;
}


inline bool
operator==( const long a, const fixed_t& b )
{
    fixed_t x( a );
    return x == b;
}


/*---------------------------------------------------------------------+\

 * operator != - not equal

\+---------------------------------------------------------------------*/
inline bool
fixed_t::NotEqual( const fixed_t& f ) const
{
    return m_n != f.m_n;
}

inline bool
fixed_t::operator!=( const fixed_t& f ) const
{
    return m_n != f.m_n;
}

inline bool
fixed_t::operator!=( const float f ) const
{
    return m_n != fx( f );
}

inline bool
operator!=( const double a, const fixed_t& b )
{
    fixed_t x( a );
    return x != b;
}


inline bool
operator!=( const float a, const fixed_t& b )
{
    fixed_t x( a );
    return x != b;
}


inline bool
operator!=( const long a, const fixed_t& b )
{
    fixed_t x( a );
    return x != b;
}


/*---------------------------------------------------------------------+\

 * operator &lt; - less than

\+---------------------------------------------------------------------*/
inline bool
fixed_t::LessThan( const fixed_t& f )
{
    return m_n < f.m_n;
}

inline bool
fixed_t::operator<( const fixed_t& f )
{
    return m_n < f.m_n;
}

inline bool
fixed_t::operator<( const float f )
{
    return m_n < fx( f );
}


inline bool
operator<( const long a, const fixed_t& b )
{
    fixed_t x( a );
    return x < b;
}

inline bool
operator<( const float a, const fixed_t& b )
{
    fixed_t x( a );
    return x < b;
}

inline bool
operator<( const double a, const fixed_t& b )
{
    fixed_t x( a );
    return x < b;
}

/*---------------------------------------------------------------------+\

 * operator [le] - less than or equal

\+---------------------------------------------------------------------*/
inline bool
fixed_t::LessThanEqual( const fixed_t& f )
{
    return m_n <= f.m_n;
}

inline bool
fixed_t::operator<=( const fixed_t& f )
{
    return m_n <= f.m_n;
}

inline bool
fixed_t::operator<=( const float f )
{
    return m_n <= fx( f );
}

inline bool
operator<=( const float a, const fixed_t b )
{
    fixed_t x( a );
    return x <= b;
}

inline bool
operator<=( const long a, const fixed_t b )
{
    fixed_t x( a );
    return x <= b;
}


/*---------------------------------------------------------------------+\

 * operator &gt; - greater than

\+---------------------------------------------------------------------*/
inline bool
fixed_t::GreaterThan( const fixed_t& f )
{
    return m_n > f.m_n;
}

inline bool
fixed_t::operator>( const fixed_t& f )
{
    return m_n > f.m_n;
}

inline bool
fixed_t::operator>( const float f )
{
    return m_n > fx( f );
}

inline bool
operator>( const float a, const fixed_t b )
{
    fixed_t x( a );
    return x > b;
}

inline bool
operator>( const long a, const fixed_t b )
{
    fixed_t x( a );
    return x > b;
}


/*---------------------------------------------------------------------+\

 * operator [ge] - greater than or equal

\+---------------------------------------------------------------------*/
inline bool
fixed_t::GreaterThanEqual( const fixed_t& f )
{
    return m_n >= f.m_n;
}

inline bool
fixed_t::operator>=( const fixed_t& f )
{
    return m_n >= f.m_n;
}

inline bool
fixed_t::operator>=( const float f )
{
    return m_n >= fx( f );
}

inline bool
operator>=( const float a, const fixed_t b )
{
    fixed_t x( a );
    return x >= b;
}

inline bool
operator>=( const long a, const fixed_t b )
{
    fixed_t x( a );
    return x >= b;
}


/*---------------------------------------------------------------------+\

 * operator + - addition

\+---------------------------------------------------------------------*/
inline fixed_t
fixed_t::Add( const fixed_t& f )
{
    return fixed_t( true, m_n + f.m_n );
}

inline fixed_t
fixed_t::operator+( const fixed_t& f )
{
    return fixed_t( true, m_n + f.m_n );
}

inline fixed_t
fixed_t::operator+( const float f )
{
    return fixed_t( true, m_n + fx( f ) );
}

inline fixed_t
operator+( const float a, const fixed_t& b )
{
    fixed_t x( a );
    return x + b;
}

inline fixed_t
operator+( const long a, const fixed_t& b )
{
    fixed_t x( a );
    return x + b;
}


/*---------------------------------------------------------------------+\

 * operator - - subtract

\+---------------------------------------------------------------------*/
inline fixed_t
fixed_t::Subtract( const fixed_t& f )
{
    return fixed_t( true, m_n - f.m_n );
}

inline fixed_t
fixed_t::operator-( const fixed_t& f )
{
    return fixed_t( true, m_n - f.m_n );
}

inline fixed_t
fixed_t::operator-( const float f )
{
    return fixed_t( true, m_n - fx( f ) );
}

inline fixed_t
operator-( const float a, const fixed_t& b )
{
    fixed_t x( a );
    return x - b;
}

inline fixed_t
operator-( const long a, const fixed_t& b )
{
    fixed_t x( a );
    return x - b;
}


/*---------------------------------------------------------------------+\

 * operator * - multiply

\+---------------------------------------------------------------------*/
inline fixed_t
fixed_t::operator*( const fixed_t& f )
{
    return Multiply( f );
}

inline fixed_t
fixed_t::operator*( const float f )
{
    return Multiply( fixed_t( f ) );
}

inline fixed_t
operator*( const float a, const fixed_t& b )
{
    fixed_t x( a );
    return x * b;
}

inline fixed_t
operator*( const long a, const fixed_t& b )
{
    fixed_t x( a );
    return x * b;
}


/*---------------------------------------------------------------------+\

 * operator / - divide

\+---------------------------------------------------------------------*/
inline fixed_t
fixed_t::operator/( const fixed_t& f )
{
    return Divide( f );
}

inline fixed_t
fixed_t::operator/( const float f )
{
    return Divide( fixed_t( f ) );
}

inline fixed_t
operator/( const float a, const fixed_t& b )
{
    fixed_t x( a );
    return x / b;
}

inline fixed_t
operator/( const long a, const fixed_t& b )
{
    fixed_t x( a );
    return x / b;
}


/*---------------------------------------------------------------------+\

 * operator % - modulo

\+---------------------------------------------------------------------*/
inline fixed_t
fixed_t::operator%( const fixed_t& f )
{
    return Modulo( f );
}

inline fixed_t
fixed_t::operator%( const float f )
{
    return Modulo( fixed_t( f ) );
}

inline fixed_t
operator%( const float a, const fixed_t& b )
{
    fixed_t x( a );
    return x % b;
}

inline fixed_t
operator%( const long a, const fixed_t& b )
{
    fixed_t x( a );
    return x % b;
}


/*---------------------------------------------------------------------+\

 * operator += - add into

\+---------------------------------------------------------------------*/
inline fixed_t&
fixed_t::operator+=( const fixed_t& f )
{
    m_n += f.m_n;
    return *this;
}

inline fixed_t&
fixed_t::operator+=( const float f )
{
    m_n += fx( f );
    return *this;
}

/*---------------------------------------------------------------------+\

 * operator -= - subtract from

\+---------------------------------------------------------------------*/
inline fixed_t&
fixed_t::operator-=( const fixed_t& f )
{
    m_n -= f.m_n;
    return *this;
}

inline fixed_t&
fixed_t::operator-=( const float f )
{
    m_n -= fx( f );
    return *this;
}


/*---------------------------------------------------------------------+\

 * operator *= - multiply into

\+---------------------------------------------------------------------*/
inline fixed_t&
fixed_t::operator*=( const fixed_t& f )
{
    return MultiplyIn( f );
}

inline fixed_t&
fixed_t::operator*=( const float f )
{
    return MultiplyIn( fixed_t( f ) );
}


/*---------------------------------------------------------------------+\

 * operator /= - divide into

\+---------------------------------------------------------------------*/
inline fixed_t&
fixed_t::operator/=( const fixed_t& f )
{
    return DivideIn( f );
}

inline fixed_t&
fixed_t::operator/=( const float f )
{
    return DivideIn( fixed_t( f ) );
}


// math functions


/*---------------------------------------------------------------------+\

 * fabsf - absolute (float)

\+---------------------------------------------------------------------*/
inline fixed_t
fabsf( const fixed_t& r )
{
    fixed_t f( r );
    return f.abs();
}


/*---------------------------------------------------------------------+\

 * floorf - floor (float)

\+---------------------------------------------------------------------*/
inline fixed_t
floorf( const fixed_t& r )
{
    fixed_t f( r );
    return f.floor();
}

/*---------------------------------------------------------------------+\

 * ceilf - ceiling (float)

\+---------------------------------------------------------------------*/
inline fixed_t
ceilf( const fixed_t& r )
{
    fixed_t f( r );
    return f.ceil();
}


/*---------------------------------------------------------------------+\

 * sinf - sine (float)

\+---------------------------------------------------------------------*/
inline fixed_t
sinf( fixed_t& a )
{
    fixed_t x( a );
    return x.sin();
}


/*---------------------------------------------------------------------+\

 * cosf - cosine (float)

\+---------------------------------------------------------------------*/
inline fixed_t
cosf( fixed_t& a )
{
    fixed_t x( a );
    return x.cos();
}


/*---------------------------------------------------------------------+\

 * atanf - arc-tangent (float)

\+---------------------------------------------------------------------*/
inline fixed_t
atanf( fixed_t& a )
{
    fixed_t x( a );
    return x.atan();
}


/*---------------------------------------------------------------------+\

 * fmodf - modulo (float)

\+---------------------------------------------------------------------*/
inline fixed_t
fmodf( const fixed_t& a, const fixed_t& b )
{
    fixed_t x( a );
    return x % b;
}

inline fixed_t
fmodf( const fixed_t& a, const float b )
{
    fixed_t x( a );
    return x % b;
}

inline fixed_t
fmodf( const float a, const fixed_t& b )
{
    return a % b;
}


/*---------------------------------------------------------------------+\

 * fx - import float

\+---------------------------------------------------------------------*/
inline local_fixed_t
fixed_t::fx( const float f ) const
{
    if ( FXD_F_MAX < f )
        return FXD_MAX_LOCAL;
    else if ( f < -FXD_F_MAX )
        return FXD_MAX_LOCAL;
    else
        return ( local_fixed_t )( f * ( ( (long_fixed_t)1 ) << FP_DEC_BITS ) );
}


//}}


#endif  // FIXED_USE_DOUBLE


#endif /* _H_fixed_t */
