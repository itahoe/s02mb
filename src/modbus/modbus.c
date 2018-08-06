/**
  * @file    modbus.c
  * @brief   MODBUS protocol
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "modbus.h"

/*
static
uint16_t modbus_addr_translate(         uint16_t                addr )
{
        if(             (addr >=     1) && (addr <=  9999)      )
        {
                addr    -=  1;
        }
        else if(        (addr >= 10001) && (addr <= 19999)      )
        {
                addr    -=  10001;
        }
        else if(        (addr >= 30001) && (addr <= 39999)      )
        {
                addr    -= 30001;
        }
        else if(        (addr >= 40001) && (addr <= 49999)      )
        {
                addr    -= 40001;
        }
        else
        {
                return( 0 );
        }

        return( addr );
}
*/


void
modbus_rtu_init(                                modbus_dev_t *  dev,
                                                //uint8_t *       data_xmit,
                                                uint8_t *       data_recv,
                                                size_t          cnt,
                                                uint16_t *      tbl0,
                                                uint16_t *      tbl1,
                                                uint16_t *      tbl2,
                                                uint16_t *      tbl3 )
{
        modbus_xfer_t *         recv            = &( dev->raw );
        //modbus_xfer_t *         xmit            = &( dev->xmit );


        //xmit->buf       =   data_xmit;
        //xmit->cnt       =   cnt;
        recv->buf       =   data_recv;
        recv->cnt       =   cnt;
        dev->tbl0       =   tbl0;
        dev->tbl1       =   tbl1;
        dev->tbl2       =   tbl2;
        dev->tbl3       =   tbl3;
}


void
modbus_reg_update(                              modbus_reg_t *  reg,
                                                uint16_t        data )
{
        reg->data         =   data;
}


modbus_rslt_t
modbus_cfg(                                     modbus_dev_t *  dev,
                                                modbus_cfg_t    cfg,
                                        const   void *          value )
{
        modbus_rslt_t   rslt    = MODBUS_RSLT_OK;


        switch( cfg )
        {
                case MODBUS_CFG_DEV_ADDR:
                        dev->addr               =   *( (uint8_t *) value );
                        break;

                default:
                        rslt                    =   MODBUS_RSLT_INVALID_PARAMETER;
                        break;
        }

        return( rslt );
}
