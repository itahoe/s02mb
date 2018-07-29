/**
  * @file    modbus.c
  * @brief   MODBUS protocol header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef MODBUS_H
#define MODBUS_H


#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


#define MODBUS_RTU_FRAME_SIZE_MAX_OCT           256
#define MODBUS_ASCII_FRAME_SIZE_MAX_OCT         513


typedef struct  modbus_reg_s
{
        uint16_t                addr;
        size_t                  size;
} modbus_reg_t;


typedef enum    modbus_func_e
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
} modbus_func_t;


typedef struct  modbus_rtu_rqst_s
{
                uint8_t *               data;
                size_t                  len;
                modbus_func_t           func;
        const   modbus_reg_t *          reg;
} modbus_rtu_rqst_t;


typedef struct  modbus_rtu_resp_s
{
                uint8_t *               data;
                size_t                  len;
                size_t                  offset;
                modbus_func_t           func;
        const   modbus_reg_t *          reg;
} modbus_rtu_resp_t;


typedef struct  modbus_rtu_s
{
                uint16_t                dev_addr;
                modbus_rtu_rqst_t       rqst;
                modbus_rtu_resp_t       resp;
} modbus_rtu_t;


uint16_t modbus_crc(                    const   uint8_t *       data,
                                                size_t          size    );

void    modbus_rtu_init(                        modbus_rtu_t *  p,
                                                uint16_t        dev_addr,
                                                uint8_t *       data_xmit,
                                                uint8_t *       data_recv );

size_t  modbus_rtu_rqst(                        modbus_rtu_t *  p );

int     modbus_rtu_resp(                        modbus_rtu_t *  p );


#endif  //MODBUS_H
