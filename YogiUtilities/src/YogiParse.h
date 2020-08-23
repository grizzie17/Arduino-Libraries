#ifndef YogiParse_h
#define YogiParse_h


#include <Arduino.h>

#if ! defined( __SIZE_TYPE__ )
typedef unsigned long size_t;
#endif


namespace Yogi { namespace ParseText {

const char*
parseSymbol( const char* s, size_t n, size_t* pParseCount );


long
parseInteger( const char* s, size_t n, size_t* pParseCount );


long
parseHexDigits( const char* s, size_t n, size_t* pParseCount );


long
parseHexidecimal( const char* s, size_t n, size_t* pParseCount );


long
parseDecDigits( const char* s, size_t n, size_t* pParseCount );


long
parseDecimal( const char* s, size_t n, size_t* pParseCount );


float
parseSimpleFloat( const char* s, size_t n, size_t* pParseCount );


float
parseFloat( const char* s, size_t n, size_t* pParseCount );


}}  // namespace Yogi::ParseText


#endif  // YogiParse_h
