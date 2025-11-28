#ifndef TPS546E25_H_
#define TPS546E25_H_

#include <stdint.h>
#include <esp_err.h>
#include <stdbool.h>

#include "global_state.h"

#define TPS546E25_I2CADDR         0x11  // TPS546E25 i2c address
#define TPS546E25_I2CADDR_ALERT   0x0C  // TPS546E25 SMBus Alert address
#define TPS546E25_MANUFACTURER_ID 0xFE  // Manufacturer ID
#define TPS546E25_REVISION        0xFF  // Chip revision

/*-------------------------*/
/* These are the inital values for the voltage regulator configuration */
/* when the config revision stored in the TPS546E25 doesn't match, these values are used */


//#define TPS546E25_INIT_ON_OFF_CONFIG 0x18 /* use ON_OFF command to control power */
#define OPERATION_OFF 0x00
#define OPERATION_ON  0x80

#define TPS546E25_INIT_PHASE 0xFF  /* default phase register value from TPS546E25 datasheet */

#define TPS546E25_INIT_FREQUENCY 800  /* KHz */

typedef struct
{
  /* vin voltage */
  float TPS546E25_INIT_VIN_ON;  /* V */
  float TPS546E25_INIT_VIN_OFF; /* V */
  float TPS546E25_INIT_VIN_UV_WARN_LIMIT; /* V */
  float TPS546E25_INIT_VIN_OV_FAULT_LIMIT; /* V */
  /* vout voltage */
  float TPS546E25_INIT_SCALE_LOOP; /* Voltage Scale factor */
  float TPS546E25_INIT_VOUT_MIN; /* V */
  float TPS546E25_INIT_VOUT_MAX; /* V */
  float TPS546E25_INIT_VOUT_COMMAND;  /* V absolute value */
  /* iout current */
  float TPS546E25_INIT_IOUT_OC_WARN_LIMIT; /* A */
  float TPS546E25_INIT_IOUT_OC_FAULT_LIMIT; /* A */
} TPS546E25_CONFIG;

/*****************************************************************************************************************************************************  */
/*                                              COMMAND     COMMAND NAME                R/W     NVM     DEFAULT         DEFAULT BEHAVIOR                   */
/*                                              CODE                                                    VALUE (Hex)                                         */
#define TPS546E25_CMD_OPERATION                 0x01     /* OPERATION                   R/W     NO      04h             Defines the operation of the device. */
#define TPS546E25_CMD_ON_OFF_CONFIG             0x02     /* ON_OFF_CONFIG               R/W     YES     16h             Turn ON/OFF by CNTL pin, Use TOFF_DELAY */
#define TPS546E25_CMD_CLEAR_FAULTS              0x03     /* CLEAR_FAULTS                W       NO      N/A             Clear all faults. */
#define TPS546E25_CMD_PHASE                     0x04     /* PHASE                       R       NO      N/A             STACK_POSITION set by pin-strap selection. */
#define TPS546E25_CMD_P2_PLUS_WRITE             0x09     /* P2_PLUS_WRITE               W       NO      N/A             Page Plus Write function to send a command to a specific page and phase or all phases. */
#define TPS546E25_CMD_P2_PLUS_READ              0x0A     /* P2_PLUS_READ                R       NO      N/A             Page Plus Read function to read data in a specific page and phase or all phases. */
#define TPS546E25_CMD_PASSKEY                   0x0E     /* PASSKEY                     R/W     YES     00h             Passkey to lock access to (DDh) EXT_WRITE_PROTECTION */
#define TPS546E25_CMD_WRITE_PROTECT             0x10     /* WRITE_PROTECT               R/W     YES     00h             All Commands are writable */
#define TPS546E25_CMD_STORE_USER_ALL            0x15     /* STORE_USER_ALL              W       NO      N/A             Stores all current storable register settings into NVM. */
#define TPS546E25_CMD_RESTORE_USER_ALL          0x16     /* RESTORE_USER_ALL            W       NO      N/A             Restores all storable register settings from NVM. */
#define TPS546E25_CMD_CAPABILITY                0x19     /* CAPABILITY                  R       NO      D0h             The device has an SMB_ALERT# pin. */
#define TPS546E25_CMD_SMBALERT_MASK             0x1B     /* SMBALERT_MASK               R/W     YES     N/A             Sets ability to mask events that trigger SMB_ALERT#. */
#define TPS546E25_CMD_VOUT_MODE                 0x20     /* VOUT_MODE                   R       NO      97h             Indicates the device is relative format with an exponent value of -9 for an equivalent LSB of 1.953mV. */
#define TPS546E25_CMD_VOUT_COMMAND              0x21     /* VOUT_COMMAND                R/W     NO      VSEL            Set the output voltage through PMBus. */
#define TPS546E25_CMD_VOUT_TRIM                 0x22     /* VOUT_TRIM                   R/W     YES     0000h           Apply a fixed offset voltage to the output voltage command value. */
#define TPS546E25_CMD_VOUT_MAX                  0x24     /* VOUT_MAX                    R/W     YES     VSEL            Maximum output voltage, initially set by pin-strap and settable by PMBus. */
#define TPS546E25_CMD_VOUT_MARGIN_HIGH          0x25     /* VOUT_MARGIN_HIGH            R/W     YES     0210h           Sets the margin high percentage when selected in OPERATION register. */
#define TPS546E25_CMD_VOUT_MARGIN_LOW           0x26     /* VOUT_MARGIN_LOW             R/W     YES     01F0h           Sets the margin low percentage when selected in OPERATION register. */
#define TPS546E25_CMD_VOUT_TRANSITION_RATE      0x27     /* VOUT_TRANSITION_RATE        R/W     YES     E850h           Sets the rate in mV/μs the output changes voltage. */
#define TPS546E25_CMD_VOUT_SCALE_LOOP           0x29     /* VOUT_SCALE_LOOP             R/W     YES     VSEL            Sets the feedback resistor ratio. */
#define TPS546E25_CMD_VOUT_SCALE_MONITOR        0x2A     /* VOUT_SCALE_MONITOR          R/W     YES     VSEL            Sets the feedback resistor ratio when external feedback divider is used for telemetry purposes. */
#define TPS546E25_CMD_VOUT_MIN                  0x2B     /* VOUT_MIN                    R/W     YES     VSEL            Minimum output voltage, initially set by pin-strap and settable by PMBus. */
#define TPS546E25_CMD_FREQUENCY_SWITCH          0x33     /* FREQUENCY_SWITCH            R/W     YES     MSEL2           Sets the switching frequency with default set by MSEL2 resistor */
#define TPS546E25_CMD_VIN_ON                    0x35     /* VIN_ON                      R/W     YES     0002h           PVIN ON threshold */
#define TPS546E25_CMD_VIN_OFF                   0x36     /* VIN_OFF                     R/W     YES     0002h           PVIN OFF threshold */
#define TPS546E25_CMD_IOUT_CAL_OFFSET           0x39     /* IOUT_CAL_OFFSET             R/W     YES     F000h           Used to add or subtract a fixed offset from READ_IOUT with default of 0A. */
#define TPS546E25_CMD_VOUT_OV_FAULT_LIMIT       0x40     /* VOUT_OV_FAULT_LIMIT         R/W     YES     024Dh           VOUT Tracking OV Fault threshold = +12% */
#define TPS546E25_CMD_VOUT_OV_FAULT_RESPONSE    0x41     /* VOUT_OV_FAULT_RESPONSE      R/W     YES     MSEL1           Fault Response from MSEL1 */
#define TPS546E25_CMD_VOUT_OV_WARN_LIMIT        0x42     /* VOUT_OV_WARN_LIMIT          R/W     YES     0229h           VOUT Tracking OV Warn threshold = +8% */
#define TPS546E25_CMD_VOUT_UV_WARN_LIMIT        0x43     /* VOUT_UV_WARN_LIMIT          R/W     YES     01D7h           VOUT Tracking UV Fault threshold = -8% */
#define TPS546E25_CMD_VOUT_UV_FAULT_LIMIT       0x44     /* VOUT_UV_FAULT_LIMIT         R/W     YES     0185Dh          VOUT Tracking UV Fault threshold = -24% */
#define TPS546E25_CMD_VOUT_UV_FAULT_RESPONSE    0x45     /* VOUT_UV_FAULT_RESPONSE      R/W     YES     MSEL1           Fault Response from MSEL1 */
#define TPS546E25_CMD_IOUT_OC_FAULT_LIMIT       0x46     /* IOUT_OC_FAULT_LIMIT         R/W     YES     MSEL1           Valley Current Limit set by MSEL1 */
#define TPS546E25_CMD_IOUT_OC_LV_FAULT_LIMIT    0x48     /* IOUT_OC_LV_FAULT_LIMIT      R       NO      VOUT_UV         Same as VOUT_UV_FAULT_LIMIT */
#define TPS546E25_CMD_IOUT_OC_LV_FAULT_RESPONSE 0x49     /* IOUT_OC_LV_FAULT_RESPONSE   R       NO      VOUT_UV         Fault Response from VOUT_UV_FAULT_LIMIT */
#define TPS546E25_CMD_IOUT_OC_WARN_LIMIT        0x4A     /* IOUT_OC_WARN_LIMIT          R/W     YES     0030h           Output Overcurrent Warning Level 48A */
#define TPS546E25_CMD_OT_FAULT_LIMIT            0x4F     /* OT_FAULT_LIMIT              R/W     YES     1024h           Programmable OT fault limit = 145° C */
#define TPS546E25_CMD_OT_FAULT_RESPONSE         0x50     /* OT_FAULT_RESPONSE           R/W     YES     MSEL1           Fault Response from MSEL1 */
#define TPS546E25_CMD_OT_WARN_LIMIT             0x51     /* OT_WARN_LIMIT               R/W     YES     101Fh           Programmable OT fault limit = 125° C */
#define TPS546E25_CMD_VIN_OV_FAULT_LIMIT        0x55     /* VIN_OV_FAULT_LIMIT          R/W     YES     0809h           PVIN OV Fault Threshold = 18.5V */
#define TPS546E25_CMD_TON_DELAY                 0x60     /* TON_DELAY                   R/W     YES     F800h           50μs turn-on delay */
#define TPS546E25_CMD_TON_RISE                  0x61     /* TON_RISE                    R/W     YES     MSEL1           Set by MSEL1 */
#define TPS546E25_CMD_TOFF_DELAY                0x64     /* TOFF_DELAY                  R/W     YES     F800h           0ms turn-off delay */
#define TPS546E25_CMD_TOFF_FALL                 0x65     /* TOFF_FALL                   R/W     YES     F800h           0.5ms from the end of Toff Delay */
#define TPS546E25_CMD_STATUS_BYTE               0x78     /* STATUS_BYTE                 R       NO      41h             Status is device is OFF, and OTH is 1b. */
#define TPS546E25_CMD_STATUS_WORD               0x79     /* STATUS_WORD                 R       NO      2841h           VIN is off and PGOOD_Z is 1b. */
#define TPS546E25_CMD_STATUS_VOUT               0x7A     /* STATUS_VOUT                 R/W     YES     00h             Current status */
#define TPS546E25_CMD_STATUS_IOUT               0x7B     /* STATUS_IOUT                 R/W     YES     00h             Current status */
#define TPS546E25_CMD_STATUS_INPUT              0x7C     /* STATUS_INPUT                R/W     YES     00h             Current status */
#define TPS546E25_CMD_STATUS_TEMPERATURE        0x7D     /* STATUS_TEMPERATURE          R/W     YES     00h             Current status */
#define TPS546E25_CMD_STATUS_CML                0x7E     /* STATUS_CML                  R/W     NO      00h             Current status */
#define TPS546E25_CMD_STATUS_OTHER              0x7F     /* STATUS_OTHER                R/W     NO      00h             Current status */
#define TPS546E25_CMD_STATUS_MFR_SPECIFIC       0x80     /* STATUS_MFR_SPECIFIC         R/W     YES     00h             Current status */
#define TPS546E25_CMD_READ_VIN                  0x88     /* READ_VIN                    R       NO      N/A             Measured input voltage. */
#define TPS546E25_CMD_READ_VOUT                 0x8B     /* READ_VOUT                   R       NO      N/A             Measured output voltage */
#define TPS546E25_CMD_READ_IOUT                 0x8C     /* READ_IOUT                   R       NO      N/A             Measured output current. */
#define TPS546E25_CMD_READ_TEMP_1               0x8D     /* READ_TEMP_1                 R       NO      N/A             Measured Controller die temperature */
#define TPS546E25_CMD_PMBUS_REVISION            0x98     /* PMBUS_REVISION              R       NO      55h             PMBus 1.5 */
#define TPS546E25_CMD_MFR_ID                    0x99     /* MFR_ID                      R       NO      4954h           ASCII for "TI" */
#define TPS546E25_CMD_MFR_MODEL                 0x9A     /* MFR_MODEL                   R       YES     0000h           Blank Manufacturer Model */
#define TPS546E25_CMD_MFR_REVISION              0x9B     /* MFR_REVISION                R/W     YES     00h             Device revision */
#define TPS546E25_CMD_IC_DEVICE_ID              0xAD     /* IC_DEVICE_ID                R       NO      5449546E2500h   IC part number */
#define TPS546E25_CMD_IC_DEVICE_REV             0xAE     /* IC_DEVICE_REV               R       NO      00h             IC revision */
#define TPS546E25_CMD_SYS_CFG_USER1             0xD1     /* SYS_CFG_USER1               R/W     YES     0000h           User Configuration Options */
#define TPS546E25_CMD_PMBUS_ADDR                0xD3     /* PMBUS_ADDR                  R/W     YES     PMBUS ADDR      PMBus Address set by PMBUS_ADDR pin */
#define TPS546E25_CMD_COMP                      0xD4     /* COMP                        R/W     YES     MSEL2           COMP set by MSEL2 pin detection */
#define TPS546E25_CMD_VBOOT_OFFSET_1            0xD5     /* VBOOT_OFFSET_1              R/W     YES     VSEL            VBOOT set by VSEL */
#define TPS546E25_CMD_STACK_CONFIG              0xD6     /* STACK_CONFIG                R       NO      N/A             Set by PMBUS_ADDR pin programming */
#define TPS546E25_CMD_PIN_DETECT_OVERRIDE       0xD8     /* PIN_DETECT_OVERRIDE         R/W     YES     8E7Dh           All Pin Detection Used */
#define TPS546E25_CMD_NVM_CHECKSUM              0xD9     /* NVM_CHECKSUM                R       NO      DE7Eh           NVM Checksum excluding Passkey */
#define TPS546E25_CMD_READ_TELEMETRY            0xDA     /* READ_TELEMETRY              R       NO      N/A             Read VOUT, IOUT, and TEMP with a block read. */
#define TPS546E25_CMD_STATUS_ALL                0xDB     /* STATUS_ALL                  R       NO      N/A             Read all STATUS with a block read. */
#define TPS546E25_CMD_EXT_WRITE_PROTECTION      0xDD     /* EXT_WRITE_PROTECTION        R/W     YES     0000h           All Pin Detection Used */
#define TPS546E25_CMD_IMON_CAL                  0xDE     /* IMON_CAL                    R/W     YES     07h             READ_IOUT calibration adjustment 0% */
#define TPS546E25_CMD_FUSION_ID0                0xFC     /* FUSION_ID0                  R       NO      02C0h           Device Identification used by FUSION */
#define TPS546E25_CMD_FUSION_ID1                0xFD     /* FUSION_ID1                  R       NO      4B434F4Ch       Device Identification used by FUSION */

/********************************************************************************************************* */
/* vin voltage */
// #define TPS546E25_INIT_VIN_ON  11.0  /* V */
// #define TPS546E25_INIT_VIN_OFF 10.5  /* V */
// #define TPS546E25_INIT_VIN_UV_WARN_LIMIT 14.0 /* V */
// #define TPS546E25_INIT_VIN_OV_FAULT_LIMIT 15.0 /* V */

//VIN_OV_FAULT_RESPONSE pg98
//0xB7 -> 1011 0111
//10 -> Immediate Shutdown. Shut down and restart according to VIN_OV_RETRY.
//110 -> After shutting down, wait one HICCUP period, and attempt to restart up to 6 times. After 6 failed restart attempts, do not attempt to restart (latch off).
//111 -> Shutdown delay of seven PWM_CLK, HICCUP equal to 7 times TON_RISE
#define TPS546E25_INIT_VIN_OV_FAULT_RESPONSE 0xB7

  /* vout voltage */
//#define TPS546E25_INIT_SCALE_LOOP 0.25  /* Voltage Scale factor */
//#define TPS546E25_INIT_VOUT_MAX 3 /* V */
#define TPS546E25_INIT_VOUT_OV_FAULT_LIMIT 1.25 /* %/100 above VOUT_COMMAND */
#define TPS546E25_INIT_VOUT_OV_WARN_LIMIT  1.16 /* %/100 above VOUT_COMMAND */
#define TPS546E25_INIT_VOUT_MARGIN_HIGH 1.1 /* %/100 above VOUT */
//#define TPS546E25_INIT_VOUT_COMMAND 1.2  /* V absolute value */
#define TPS546E25_INIT_VOUT_MARGIN_LOW 0.90 /* %/100 below VOUT */
#define TPS546E25_INIT_VOUT_UV_WARN_LIMIT 0.90  /* %/100 below VOUT_COMMAND */
#define TPS546E25_INIT_VOUT_UV_FAULT_LIMIT 0.75 /* %/100 below VOUT_COMMAND */
//#define TPS546E25_INIT_VOUT_MIN 1 /* v */

#define TPS546E25_VOUT_EXPONENT 1.953

  /* iout current */
// #define TPS546E25_INIT_IOUT_OC_WARN_LIMIT  50.00 /* A */
// #define TPS546E25_INIT_IOUT_OC_FAULT_LIMIT 55.00 /* A */

//IOUT_OC_FAULT_RESPONSE - pg91
//0xC0 -> 1100 0000
//11 -> Shutdown Immediately
//000 -> Do not attempt to restart (latch off).
//000 -> Shutdown delay of one PWM_CLK, HICCUP equal to TON_RISE
#define TPS546E25_INIT_IOUT_OC_FAULT_RESPONSE 0xC0  /* shut down, no retries */

  /* temperature */
// It is better to set the temperature warn limit for TPS546E25 more higher than Ultra 
#define TPS546E25_INIT_OT_WARN_LIMIT  105 /* degrees C */
#define TPS546E25_INIT_OT_FAULT_LIMIT 145 /* degrees C */

//OT_FAULT_RESPONSE - pg94
//0xFF -> 1111 1111
//11 -> Shutdown until Temperature is below OT_WARN_LIMIT, then restart according to OT_RETRY*.
//111 -> After shutting down, wait one HICCUP period, and attempt to restart indefinitely, until commanded OFF or a successful start-up occurs.
//111 -> Shutdown delay of 7 ms, HICCUP equal to 4 times TON_RISE
#define TPS546E25_INIT_OT_FAULT_RESPONSE 0xBF /* wait for cooling, and retry */

/********************************************************************************************************* */
/* timing */
#define TPS546E25_INIT_TON_DELAY 0
#define TPS546E25_INIT_TON_RISE 3
#define TPS546E25_INIT_TON_MAX_FAULT_LIMIT 0
#define TPS546E25_INIT_TON_MAX_FAULT_RESPONSE 0x3B
#define TPS546E25_INIT_TOFF_DELAY 0
#define TPS546E25_INIT_TOFF_FALL 0

#define INIT_STACK_CONFIG 0x0001 //One-Slave, 2-phase
#define INIT_SYNC_CONFIG 0x00D0 //Enable Auto Detect SYNC
#define INIT_PIN_DETECT_OVERRIDE 0xFFFF //use pin values

/*-------------------------*/

/********************************************************************************************************* */
/* PMBUS_ON_OFF_CONFIG initialization values */
#define ON_OFF_CONFIG_PU        0x10 // Act on CONTROL. (01h) OPERATION command to start/stop power conversion, or both.
#define ON_OFF_CONFIG_CMD       0x08 // Act on (01h) OPERATION Command (and CONTROL pin if configured by CP) to start/stop power conversion.
#define ON_OFF_CONFIG_CP        0x04 // Act on CONTROL pin (and (01h) OPERATION Command if configured by bit [3]) to start/stop power conversion.
#define ON_OFF_CONFIG_POLARITY  0x02 // CONTROL pin has active high polarity.
#define ON_OFF_CONFIG_DELAY     0x01 // When power conversion is commanded OFF by the CONTROL pin (must be configured to respect the CONTROL pin as above), stop power conversion immediately.



/********************************************************************************************************* */
/* STATUS_WORD Bitmasks */
#define TPS546E25_STATUS_VFW     0x8000 //bit 15    Output Voltage Fault or Warning. 
#define TPS546E25_STATUS_OCFW    0x4000 //bit 14    Output Current Fault or Warning.
#define TPS546E25_STATUS_INPUT   0x2000 //bit 13    NPUT fault or warning in (7Ch) STATUS_INPUT is present. 
#define TPS546E25_STATUS_MFR     0x1000 //bit 12    Manufacturer specific fault/warning condition. 
#define TPS546E25_STATUS_PGOOD   0x0800 //bit 11    Power Good Inverted. 
#define TPS546E25_STATUS_OTHER   0x0200 //bit 9     STATUS_OTHER fault/warning condition. 

//#define TPS546E25_STATUS_BUSY       0x0080    //Not supported and always set to 0
#define TPS546E25_STATUS_OFF        0x0040  //
#define TPS546E25_STATUS_VOUT_OVFW  0x0020  //
#define TPS546E25_STATUS_IOUT_OCF   0x0010  //
//#define TPS546E25_STATUS_VIN_UV     0x0008    //Not supported and always set to 0
#define TPS546E25_STATUS_OTFW       0x0004  //
#define TPS546E25_STATUS_CML        0x0002  //
#define TPS546E25_STATUS_OTH        0x0001  //

    /********************************************************************************************************* */
    /* STATUS_VOUT OFFSETS */
    #define TPS546E25_STATUS_VOUT_OVF           0x80 //bit 7 - Latched flag indicating a VOUT OV fault has occurred.
    #define TPS546E25_STATUS_VOUT_OVW           0x40 //bit 6 - Latched flag indicating a VOUT OV warn has occurred.
    #define TPS546E25_STATUS_VOUT_UVW           0x20 //bit 5 - Latched flag indicating a VOUT UV warn has occurred.
    #define TPS546E25_STATUS_VOUT_UVF           0x10 //bit 4 - Latched flag indicating a VOUT UV fault has occurred.
    #define TPS546E25_STATUS_VOUT_MIN_MAX       0x08 //bit 3 - Latched flag indicating a VOUT_MIN_MAX has occurred.

    /********************************************************************************************************* */
    /* STATUS_IOUT OFFSETS */
    #define TPS546E25_STATUS_IOUT_OCFB           0x80 //bit 7 - Latched flag indicating IOUT OC fault has occurred.
    #define TPS546E25_STATUS_IOUT_OCUV          0x40 //bit 6 - Latched flag indicating IOUT_OC_LV_FAULT has occurred.
    #define TPS546E25_STATUS_IOUT_OCW           0x20 //bit 5 - Latched flag indicating IOUT OC warn has occurred.
    #define TPS546E25_STATUS_IOUT_UCF           0x10 //bit 4 - Latched flag indicating an output undercurrent fault has occured.

    /********************************************************************************************************* */
    /* STATUS_INPUT OFFSETS */
    #define TPS546E25_STATUS_VIN_OVF            0x80 //bit 7 - Latched flag indicating PVIN OV fault has occurred.
    #define TPS546E25_STATUS_VIN_LOW_VIN        0x08 //bit 3 - LIVE (unlatched) status bit. PVIN is OFF.

    /********************************************************************************************************* */
    /* STATUS_TEMPERATURE OFFSETS */
    #define TPS546E25_STATUS_TEMP_OTF           0x80 //bit 7 - Latched flag indicating OT fault has occurred.
    #define TPS546E25_STATUS_TEMP_OTW           0x40 //bit 6 - Latched flag indicating OT warn has occurred

    /********************************************************************************************************* */
    /* STATUS_CML OFFSETS */
    #define TPS546E25_STATUS_CML_IVC            0x80 //bit 7 - Latched flag indicating an invalid or unsupported command was received.
    #define TPS546E25_STATUS_CML_IVD            0x40 //bit 6 - Latched flag indicating an invalid or unsupported data was received.
    #define TPS546E25_STATUS_CML_PEC            0x20 //bit 5 - Latched flag indicating a packet error check has failed.
    #define TPS546E25_STATUS_CML_MEM            0x10 //bit 4 - Latched flag indicating a memory error was detected.
    #define TPS546E25_STATUS_CML_COMM           0x02 //bit 1 - Latched flag indicating communication error detected.

    /********************************************************************************************************* */
    /* STATUS_OTHER */
    #define TPS546E25_STATUS_OTHER_FIRST        0x01 //bit 0 - Latched flag indicating that this device was the first to assert SMBALERT.

    /********************************************************************************************************* */
    /* STATUS_MFR */
    #define TPS546E25_STATUS_MFR_DCM            0x80 //bit 7 - The device is operating in DCM. LIVE (unlatched) status bit. 
    #define TPS546E25_STATUS_MFR_OTF_BG         0x40 //bit 6 - Latched flag indicating the controller fixed thermal shutdown has occurred.
    #define TPS546E25_STATUS_MFR_PS_FLT         0x20 //bit 5 - Latched flag indicating a power-stage fault has occured.
    #define TPS546E25_STATUS_MFR_PS_COMM_WRN    0x10 //bit 4 - Latched flag indicating a power-stage communication error has occured.
    #define TPS546E25_STATUS_MFR_PS_OT          0x02 //bit 1 - Latched flag indicating the power-stage fixed thermal shutdown has occurred
    #define TPS546E25_STATUS_MFR_PS_UV          0x01 //bit 0 - Live flag indicating a power-stage undervoltage fault has occurred


/* public functions */
esp_err_t TPS546E25_init(TPS546E25_CONFIG config);

void TPS546E25_read_mfr_info(uint8_t *);
void TPS546E25_write_entire_config(void);
int TPS546E25_get_frequency(void);
void TPS546E25_set_frequency(int);
int TPS546E25_get_temperature(void);
float TPS546E25_get_vin(void);
float TPS546E25_get_iout(void);
float TPS546E25_get_vout(void);
esp_err_t TPS546E25_set_vout(float volts);
void TPS546E25_show_voltage_settings(void);
void TPS546E25_print_status(void);

esp_err_t TPS546E25_check_status(GlobalState * GLOBAL_STATE);
esp_err_t TPS546E25_clear_faults(void);

const char* TPS546E25_get_error_message(void); //Get the current TPS error message

#endif /* TPS546E25_H_ */
