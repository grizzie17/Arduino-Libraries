

#ifndef YogiDebug_h
#define YogiDebug_h

// #    define _DEBUG
// define _DEBUG at top of sketch

#if ! defined( _DEBUG ) && ! defined( NDEBUG )
#    define _DEBUG
#endif


#if defined( _DEBUG )

#    define DEBUG_STATEMENT( s ) \
        do                       \
        {                        \
            s;                   \
        } while ( 0 )
#    define DEBUG_PRINT( s, ... )   Serial.print( s, ##__VA_ARGS__ )
#    define DEBUG_PRINTLN( s, ... ) Serial.println( s, ##__VA_ARGS__ )
#    define DEBUG_VPRINT( s, v, ... )         \
        do                                    \
        {                                     \
            Serial.print( s );                \
            Serial.print( v, ##__VA_ARGS__ ); \
        } while ( 0 )
#    define DEBUG_VPRINTLN( s, v, ... )         \
        do                                      \
        {                                       \
            Serial.print( s );                  \
            Serial.println( v, ##__VA_ARGS__ ); \
        } while ( 0 )
#    define DEBUG_DELAY( x ) delay( x )

// actually just starting up the Serial port
inline void
debugOpen_( const char* sMessage = NULL )
{
    Serial.begin( 115200 );
    while ( ! Serial )
        ;  // wait for Serial
    delay( 600 );
    if ( sMessage )
        Serial.println( sMessage );
}


#else


#    define DEBUG_STATEMENT( s )        ( 0 )
#    define DEBUG_PRINT( s, ... )       ( 0 )
#    define DEBUG_PRINTLN( s, ... )     ( 0 )
#    define DEBUG_VPRINT( s, v, ... )   ( 0 )
#    define DEBUG_VPRINTLN( s, v, ... ) ( 0 )
#    define DEBUG_DELAY( x )            ( 0 )

inline void
debugOpen_( const char* sMessage = NULL )
{}


#endif  // _DEBUG


#define DEBUG_OPEN( ... ) debugOpen_( __VA_ARGS__ )


#endif
