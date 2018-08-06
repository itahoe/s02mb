/**
  * @file    modbus_rtu.c
  * @brief   MODBUS RTU routines
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "modbus.h"
#include "bsp.h"
#include "app_trace.h"


/******************************************************************************/
/* SLAVE RELATED ROUTINES                                                     */
/******************************************************************************/
modbus_rslt_t
modbus_rtu_rqst_recv(                           modbus_dev_t *  dev )
{
        //modbus_xfer_t *         recv            = &( dev->recv );
        //modbus_xfer_t *         xmit            = &( dev->xmit );
        //uint8_t *               raw             = recv->buf;
        uint8_t *               raw             = dev->raw.buf;
        uint8_t                 cnt             = dev->raw.cnt;
        //modbus_func_type_t      func_code       = (modbus_func_type_t) raw[ 1];
        uint8_t *               dev_addr        = &raw[ 0];
        uint8_t *               func_code       = &raw[ 1];
        //uint16_t                reg_addr        = (raw[2] << 8) | (raw[3] & 0xFF);
        //uint16_t                reg_cnt         = (raw[4] << 8) | (raw[5] & 0xFF);


        if( *dev_addr != dev->addr )
        {
                return( MODBUS_RSLT_INVALID_ADDR );
        }

        if( modbus_crc( raw, cnt ) != 0 )
        {
                return( MODBUS_RSLT_INVALID_CRC );
        }

        //xmit->buf[0]    =   dev->addr;
        //xmit->buf[1]    =   func_code;

        switch( (modbus_func_type_t) *func_code )
        {
                case MODBUS_FUNC_READ_HOLDING_REGISTERS:
                        //modbus_reg_read( &xmit->buf[3], dev->tbl0, offset, cnt );
                        break;

                case MODBUS_FUNC_READ_INPUT_REGISTERS:
                        break;


                case MODBUS_FUNC_READ_COILS:
                case MODBUS_FUNC_READ_DISCRETE_INPUTS:

                case MODBUS_FUNC_WRITE_SINGLE_COIL:
                case MODBUS_FUNC_WRITE_SINGLE_REGISTER:
                case MODBUS_FUNC_READ_EXCEPTION_STATUS:
                case MODBUS_FUNC_DIAGNOSTICS:
                case MODBUS_FUNC_GET_COMM_EVENT_COUNTER:
                case MODBUS_FUNC_GET_COMM_EVENT_LOG:
                case MODBUS_FUNC_WRITE_MULTIPLE_COILS:
                case MODBUS_FUNC_WRITE_MULTIPLE_REGISTERS:
                case MODBUS_FUNC_REPORT_SERVER_ID:
                case MODBUS_FUNC_READ_FILE_RECORD:
                case MODBUS_FUNC_WRITE_FILE_RECORD:
                case MODBUS_FUNC_MASK_WRITE_REGISTER:
                case MODBUS_FUNC_READ_WRITE_MULTIPLE_REGISTERS:
                case MODBUS_FUNC_READ_FIFO_QUEUE:
                case MODBUS_FUNC_ENCAPSULATED_INTERFACE_TRANSPORT:
                default:
                        *func_code      |=  0x80;
                        break;
        }

        return( MODBUS_RSLT_OK );
}

/*
typedef union
{
    float               f;
    uint8_t             u[ 4 ];
} conc_t;
*/
/*

static
void    next_data(                              float *         f )
{
        if( ++(*f) > 100 )
        {
                *f       =   0;
        }
}
*/

modbus_rslt_t
modbus_rtu_resp_xmit(                           modbus_dev_t *  dev )
{
        uint8_t *       raw             = &( dev->raw.buf[0] );
        size_t          len;
        uint8_t *       byte_count      = &raw[ 2];
        uint16_t        reg_count       = ( raw[ 4] << 8) | (raw[ 5] & 0xFF );
        uint16_t        crc;
        uint8_t *       tbl             = (uint8_t *) &dev->reg.sensor_value_high.data;


        bsp_ser1_recv_stop();

        if( reg_count < 128 )
        {
                *byte_count     =  reg_count * 2;
        }
        else
        {
                //error
        }

        len             =   3 + *byte_count;

        for( int i = 0; i < *byte_count; i++ )
        {
                raw[ i + 3 ]    =   tbl[ i ];
        }

        crc             =   modbus_crc( raw, (3 + *byte_count) );
        raw[ len++]     =   crc >> 8;
        raw[ len++]     =   crc & 0xFF;
        bsp_ser1_xmit( raw, len );

        return( MODBUS_RSLT_OK );
}


/******************************************************************************/
/* MASTER RELATED ROUTINES                                                    */
/******************************************************************************/
