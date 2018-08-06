/**
  * @file    modbus.c
  * @brief   MODBUS protocol header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef MODBUS_H
#define MODBUS_H


#include <stdint.h>
#include <stdio.h>


#define MODBUS_RTU_FRAME_SIZE_MAX_OCT           256
#define MODBUS_ASCII_FRAME_SIZE_MAX_OCT         513


typedef enum    modbus_func_type_e
{
        MODBUS_FUNC_READ_COILS                          = 0x01,
        MODBUS_FUNC_READ_DISCRETE_INPUTS                = 0x02,
        MODBUS_FUNC_READ_HOLDING_REGISTERS              = 0x03,
        MODBUS_FUNC_READ_INPUT_REGISTERS                = 0x04,
        MODBUS_FUNC_WRITE_SINGLE_COIL                   = 0x05,
        MODBUS_FUNC_WRITE_SINGLE_REGISTER               = 0x06,
        MODBUS_FUNC_READ_EXCEPTION_STATUS               = 0x07,
        MODBUS_FUNC_DIAGNOSTICS                         = 0x08,
        MODBUS_FUNC_GET_COMM_EVENT_COUNTER              = 0x0B,
        MODBUS_FUNC_GET_COMM_EVENT_LOG                  = 0x0C,
        MODBUS_FUNC_WRITE_MULTIPLE_COILS                = 0x0F,
        MODBUS_FUNC_WRITE_MULTIPLE_REGISTERS            = 0x10,
        MODBUS_FUNC_REPORT_SERVER_ID                    = 0x11,
        MODBUS_FUNC_READ_FILE_RECORD                    = 0x14,
        MODBUS_FUNC_WRITE_FILE_RECORD                   = 0x15,
        MODBUS_FUNC_MASK_WRITE_REGISTER                 = 0x16,
        MODBUS_FUNC_READ_WRITE_MULTIPLE_REGISTERS       = 0x17,
        MODBUS_FUNC_READ_FIFO_QUEUE                     = 0x18,
        MODBUS_FUNC_ENCAPSULATED_INTERFACE_TRANSPORT    = 0x2B,
        MODBUS_FUNC_CANOPEN_GENERAL_REFERENCE           = 0x2B,
} modbus_func_type_t;


typedef enum    modbus_rslt_e
{
        MODBUS_RSLT_OK                                  = 0x00,
        MODBUS_RSLT_INVALID_PARAMETER,
        MODBUS_RSLT_INVALID_CRC,
        MODBUS_RSLT_INVALID_ADDR,
        MODBUS_RSLT_UNDEFINED_ERROR,
} modbus_rslt_t;

typedef enum    modbus_cfg_e
{
        MODBUS_CFG_DEV_ADDR,
        MODBUS_CFG_SER_BAUDRATE,
        MODBUS_CFG_SER_STOPBITS,
        MODBUS_CFG_SER_PARITY,
} modbus_cfg_t;


typedef struct  modbus_reg_s
{
        uint16_t                data;
} modbus_reg_t;


typedef struct  modbus_reg_map_s
{
        modbus_reg_t    device_status;
        modbus_reg_t    alarm_status;
        modbus_reg_t    device_year_month;
        modbus_reg_t    device_day_hour;
        modbus_reg_t    device_minute_second;
        modbus_reg_t    sensor_temperature;
        modbus_reg_t    board_temperature;
        modbus_reg_t    sensor_signal_real;
        modbus_reg_t    sensor_signal_exponent;
        modbus_reg_t    sensor_calc_zero_offset_real;
        modbus_reg_t    sensor_calc_zero_offset_exponent;
        modbus_reg_t    sensor_calc_sensitivity_coef_real;
        modbus_reg_t    sensor_calc_sensitivity_coef_exponent;
        modbus_reg_t    sensor_value;
        modbus_reg_t    sensor_value_high;
        modbus_reg_t    sensor_value_low;
        modbus_reg_t    current_loop_value;
        modbus_reg_t    sensor_id;
        modbus_reg_t    version;
        modbus_reg_t    sensor_scale_min_real;
        modbus_reg_t    sensor_scale_min_exponent;
        modbus_reg_t    sensor_scale_max_real;
        modbus_reg_t    sensor_scale_max_exponent;
        modbus_reg_t    sensor_scale_dimension;
        modbus_reg_t    sensor_threshold_warning_real;
        modbus_reg_t    sensor_threshold_warning_exponent;
        modbus_reg_t    sensor_threshold_alarm_real;
        modbus_reg_t    sensor_threshold_alarm_exponent;
} modbus_reg_map_t;

/*
typedef struct  modbus_tbl_s
{
        uint16_t        device_id;
        uint16_t        device_firmware_version;
        uint16_t        device_serial_7;
        uint16_t        device_serial_6;
        uint16_t        device_serial_5;
        uint16_t        device_serial_4;
        uint16_t        device_serial_3;
        uint16_t        device_serial_2;
        uint16_t        device_serial_1;
        uint16_t        device_serial_0;
        uint16_t        sensor_ch0_value_raw;
        uint16_t        sensor_ch1_value_raw;
        uint16_t        sensor_ch0_value_ppm; //q15_t
        uint16_t        sensor_ch1_value_ppm; //q15_t
        uint16_t        unix_time64_3;
        uint16_t        unix_time64_2;
        uint16_t        unix_time64_1;
        uint16_t        unix_time64_0;
} modbus_tbl_t;
*/

typedef struct  modbus_xfer_s
{
        uint8_t *               raw;
        size_t                  len;
} modbus_xfer_t;

typedef struct  modbus_tbl_s
{
        uint16_t                data;
        size_t                  size;
} modbus_tbl_t;

typedef struct  modbus_dev_s
{
        uint8_t                 addr;
        //uint16_t *              tbl0;
        //uint16_t *              tbl1;
        //uint16_t *              tbl2;
        //uint16_t *              tbl3;
        uint16_t *              tbl[4];
        //modbus_tbl_t            tbl[ 4 ];
        modbus_reg_map_t        reg;
        modbus_xfer_t           buf;
} modbus_dev_t;


void
modbus_init(                                    modbus_dev_t *  dev,
                                                uint8_t *       buf_raw,
                                                size_t          buf_len,
                                                uint16_t *      tbl0,
                                                uint16_t *      tbl1,
                                                uint16_t *      tbl2,
                                                uint16_t *      tbl3 );

uint16_t
modbus_crc(                             const   uint8_t *       data,
                                                size_t          size    );

modbus_rslt_t
modbus_cfg(                                     modbus_dev_t *  dev,
                                                modbus_cfg_t    cfg,
                                        const   void *          value );

modbus_rslt_t
modbus_rtu_rqst_recv(                           modbus_dev_t *  dev );

modbus_rslt_t
modbus_rtu_resp_xmit(                           modbus_dev_t *  dev );

modbus_rslt_t
modbus_reg_update(                              modbus_reg_t *  reg,
                                                uint16_t        data );

modbus_rslt_t
modbus_dev_read(                                uint16_t *      data,
                                                uint16_t        addr,
                                                size_t          cnt );


#endif  //MODBUS_H
