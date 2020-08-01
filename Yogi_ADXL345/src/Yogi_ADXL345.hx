
#ifndef Yogi_ADXL345_h
#define Yogi_ADXL345_h

#include <Arduino.h>

// Register MAP
#define ADXL345_DEVID        0x00  // Device ID
#define ADXL345_RESERVED1    0x01  // Reserved. Do Not Access.
#define ADXL345_THRESH_TAP   0x1D  // Tap Threshold.
#define ADXL345_OFSX         0x1E  // X-Axis Offset.
#define ADXL345_OFSY         0x1F  // Y-Axis Offset.
#define ADXL345_OFSZ         0x20  // Z- Axis Offset.
#define ADXL345_DUR          0x21  // Tap Duration.
#define ADXL345_LATENT       0x22  // Tap Latency.
#define ADXL345_WINDOW       0x23  // Tap Window.
#define ADXL345_THRESH_ACT   0x24  // Activity Threshold
#define ADXL345_THRESH_INACT 0x25  // Inactivity Threshold
#define ADXL345_TIME_INACT   0x26  // Inactivity Time
#define ADXL345_ACT_INACT_CTL \
    0x27  // Axis Enable Control for Activity and Inactivity Detection
#define ADXL345_THRESH_FF      0x28  // Free-Fall Threshold.
#define ADXL345_TIME_FF        0x29  // Free-Fall Time.
#define ADXL345_TAP_AXES       0x2A  // Axis Control for Tap/Double Tap.
#define ADXL345_ACT_TAP_STATUS 0x2B  // Source of Tap/Double Tap
#define ADXL345_BW_RATE        0x2C  // Data Rate and Power mode Control
#define ADXL345_POWER_CTL      0x2D  // Power-Saving Features Control
#define ADXL345_INT_ENABLE     0x2E  // Interrupt Enable Control
#define ADXL345_INT_MAP        0x2F  // Interrupt Mapping Control
#define ADXL345_INT_SOURCE     0x30  // Source of Interrupts
#define ADXL345_DATA_FORMAT    0x31  // Data Format Control
#define ADXL345_DATAX0         0x32  // X-Axis Data 0
#define ADXL345_DATAX1         0x33  // X-Axis Data 1
#define ADXL345_DATAY0         0x34  // Y-Axis Data 0
#define ADXL345_DATAY1         0x35  // Y-Axis Data 1
#define ADXL345_DATAZ0         0x36  // Z-Axis Data 0
#define ADXL345_DATAZ1         0x37  // Z-Axis Data 1
#define ADXL345_FIFO_CTL       0x38  // FIFO Control
#define ADXL345_FIFO_STATUS    0x39  // FIFO Status


#define POWER_CTL_WAKEUP     0b00000011
#define POWER_CTL_WAKEUP_8HZ 0b00000000
#define POWER_CTL_WAKEUP_4HZ 0b00000001
#define POWER_CTL_WAKEUP_2HZ 0b00000010
#define POWER_CTL_WAKEUP_1HZ 0b00000011
#define POWER_CTL_SLEEP      0b00000100
#define POWER_CTL_MEASURE    0b00001000
#define POWER_CTL_AUTO_SLEEP 0b00010000
#define POWER_CTL_LINK       0b00100000

// used for INT_ENABLE, INT_MAP, INT_SOURCE
#define INTERRUPT_DATA_READY 0b10000000
#define INTERRUPT_SINGLE_TAP 0b01000000
#define INTERRUPT_DOUBLE_TAP 0b00100000
#define INTERRUPT_ACTIVITY   0b00010000
#define INTERRUPT_INACTIVITY 0b00001000
#define INTERRUPT_FREE_FALL  0b00000100
#define INTERRUPT_WATERMARK  0b00000010
#define INTERRUPT_OVERRUN    0b00000001

#define FIFO_MODE_BITS    0b11000000
#define FIFO_MODE_BYPASS  0b00000000
#define FIFO_MODE_FIFO    0b01000000
#define FIFO_MODE_STREAM  0b10000000
#define FIFO_MODE_TRIGGER 0b11000000

#define DATA_FORMAT_SELF_TEST  0b10000000
#define DATA_FORMAT_SPI        0b01000000
#define DATA_FORMAT_INT_INVERT 0b00100000
#define DATA_FORMAT_D4         0b00010000
#define DATA_FORMAT_FULL_RES   0b00001000
#define DATA_FORMAT_JUSTIFY    0b00000100
#define DATA_FORMAT_RANGE      0b00000011

// DATA_FORMAT_RANGE
#define DATA_RANGE_2G  0b00000000
#define DATA_RANGE_4G  0b00000001
#define DATA_RANGE_8G  0b00000010
#define DATA_RANGE_16G 0b00000011

// ACT_INACT_CTL
#define ACT_CTL_AC   0b10000000
#define ACT_CTL_X    0b01000000
#define ACT_CTL_Y    0b00100000
#define ACT_CTL_Z    0b00010000
#define INACT_CTL_AC 0b00001000
#define INACT_CTL_X  0b00000100
#define INACT_CTL_Y  0b00000010
#define INACT_CTL_Z  0b00000001


//! all appropriate functions return a status
//! value.  A value of less than zero is
//! considered an error.  All other values
//! provide informational


class ADXL345
{
    // class lifecycle --------------------
public:
    ADXL345();

public:
    // public types -----------------------
    // public constants -------------------

    //---- status values
    const int STATUS_OK = 0;
    const int STATUS_NO_ERROR = STATUS_OK;
    const int STATUS_ADDR_SEND = 1;
    const int STATUS_VALUE_SEND = 2;

    const int STATUS_ERROR = -1;
    const int STATUS_TOO_LONG = -2;
    const int STATUS_TWI_ERROR = -3;
    const int STATUS_READ_ERROR = -4;

    // public functions -------------------

    //! data Access
    int
    readAccel( int* xyz );
    int
    readAccel( int* x, int* y, int* z );

    //! begin configuration
    int
    configureI2C( byte nDevID = 0x53 );
    int
    configureSPI( byte uCS = 10 );

    //! should be last call of configure
    int
    endConfigure();

    //---- configuration commands

    int
    setRange( int nRange );

    int
    setInterrupts( byte mask );

    int
    setInterruptMap( byte mask );

    byte
    getInterruptSource();

    int
    setActivityX( bool state );
    int
    setActivityY( bool state );
    int
    setActivityZ( bool state );
    int
    setActivityXYZ( bool xState, bool yState, bool zState );
    int
    setActivityAc( bool state );
    int
    setActivityThreshold( int nThreshold );

    int
    setInactivityX( bool state );
    int
    setInactivityY( bool state );
    int
    setInactivityZ( bool state );
    int
    setInactivityXYZ( bool xState, bool yState, bool zState );
    int
    setInactivityAc( bool state );
    int
    setInactivityThreshold( int nThreshold );


    void
    printAllRegisters();


    // public data ------------------------

    double m_aGains[3];

protected:
    // protected types --------------------
    // protected functions ----------------

    void
    loadGains();

    //! write byte to address specified
    int
    writeTo( byte address, byte value );
    int
    readFrom( byte address, int num, byte buffer[] );

    //---- I2C
    int
    statusFromI2C( byte value );
    int
    writeToI2C( byte address, byte value );
    int
    readFromI2C( byte address, int num, byte buffer[] );

    //---- SPI
    int
    writeToSPI( byte address, byte value );
    int
    readFromSPI( byte address, int num, byte buffer[] );


    //----
    int
    intFromByteBuffer( byte* buff );

    int
    setRegisterBit( byte regAddress, int bitPos, bool state );

    bool
    getRegisterBit( byte regAddress, int bitPos );

    int
    setRegisterMask( byte regAddress, byte mask, byte value );


    void
    printByte( byte val );


    // protected data ---------------------

    byte          m_nDevID = 0x53;
    byte          m_buff[6];
    byte          m_CS = 10;
    bool          m_bI2C = true;
    unsigned long m_uSPIfreq = 5000000;
};

#endif  // Yogi_ADXL345_h
