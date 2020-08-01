#ifndef ADXL345_setup_h
#define ADXL345_setup_h


#include <SparkFun_ADXL345.h>


const uint8_t ADXL_M_WATERMARK = ( 1 << ADXL345_INT_WATERMARK_BIT );
const uint8_t ADXL_M_DATA_READY = ( 1 << ADXL345_INT_DATA_READY_BIT );
const uint8_t ADXL_M_OVERRUNY = ( 1 << ADXL345_INT_OVERRUNY_BIT );
const uint8_t ADXL_M_ACTIVITY = ( 1 << ADXL345_INT_ACTIVITY_BIT );
const uint8_t ADXL_M_INACTIVITY = ( 1 << ADXL345_INT_INACTIVITY_BIT );
const uint8_t ADXL_M_SINGLE_TAP = ( 1 << ADXL345_INT_SINGLE_TAP_BIT );
const uint8_t ADXL_M_DOUBLE_TAP = ( 1 << ADXL345_INT_DOUBLE_TAP_BIT );
const uint8_t ADXL_M_FREE_FALL = ( 1 << ADXL345_INT_FREE_FALL_BIT );

const uint8_t k_maskActivity = ADXL_M_ACTIVITY | ADXL_M_SINGLE_TAP
        | ADXL_M_DOUBLE_TAP | ADXL_M_FREE_FALL;

const uint8_t k_maskAll = k_maskActivity | ADXL_M_INACTIVITY;

#ifndef ADXL_SENSITIVITY
#    define ADXL_SENSITIVITY 20
#endif
#ifndef ADXL_SLEEP_DELAY
#    define ADXL_SLEEP_DELAY 15
#endif


extern ADXL345                adxl;
extern volatile unsigned long g_uCountInterrupt;
extern long                   g_nActivity;


void
adxlStatus( const char* sMessage );

void
adxlSetup( int nSleepDelay = 0 );

uint8_t
adxlGetInterrupts();


void
adxlPrintXYZ();

void
adxlDataHandler( uint8_t mInterrupts );


void
adxlIntHandler();


#endif
