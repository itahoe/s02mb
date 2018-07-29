/**
  * @file    modbus.c
  * @brief   MODBUS protocol
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "modbus.h"


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


void    modbus_rtu_init(                        modbus_rtu_t *  p,
                                                uint16_t        dev_addr,
                                                uint8_t *       data_xmit,
                                                uint8_t *       data_recv )
{
        modbus_rtu_rqst_t *     rqst    = &( p->rqst );
        modbus_rtu_resp_t *     resp    = &( p->resp );


        p->dev_addr     =   dev_addr;
        rqst->data      =   data_xmit;
        rqst->len       =   0;
        resp->data      =   data_recv;
        resp->len       =   0;
        resp->offset    =   0;
}


size_t  modbus_rtu_rqst(                        modbus_rtu_t *  p )
{
        uint16_t                crc;
        size_t                  len     = 0;
        uint16_t                reg_addr;
        modbus_rtu_rqst_t *     rqst    = &( p->rqst );
        uint8_t *               data    = rqst->data;


        *(data + 0)     =   (uint8_t) (p->dev_addr);
        *(data + 1)     =   rqst->func;
        reg_addr        =   modbus_addr_translate( rqst->reg->addr );
        *(data + 2)     =   reg_addr >> 8;
        *(data + 3)     =   reg_addr & 0xFF;
        *(data + 4)     =   rqst->reg->size >> 8;
        *(data + 5)     =   rqst->reg->size & 0xFF;
        crc             =   modbus_crc( data, (len = 6) );
        *(data + 6)     =   crc >> 8;
        *(data + 7)     =   crc & 0xFF;
        len                     =   8;

        return( len );
}


int     modbus_rtu_resp(                        modbus_rtu_t *  p )
{
        int                     err     = 0;
        uint16_t                crc;
        modbus_rtu_resp_t *     resp    = &( p->resp );
        uint8_t *               raw     = p->resp.data + p->resp.offset;
        size_t                  len;


        resp->func      =   (modbus_func_t) *(raw + 1);
        len             =   *(raw + 2);
        crc             =   *(raw + 2 + len + 1) << 8;
        crc             |=  *(raw + 2 + len + 2) & 0xFF;

        if( crc != modbus_crc( raw, 3 + len ) )
        {
                err     =  -1;
        }

        return( err );
}

