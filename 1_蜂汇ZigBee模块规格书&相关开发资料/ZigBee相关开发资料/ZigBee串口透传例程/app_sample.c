/**************************************************************************************************
  Filename:       zcl_samplesw.c
  Revised:        $Date: 2015-08-19 17:11:00 -0700 (Wed, 19 Aug 2015) $
  Revision:       $Revision: 44460 $

  Description:    Zigbee Cluster Library - sample device application.


  Copyright 2006-2013 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
**************************************************************************************************/


/*********************************************************************
 * INCLUDES
 */
#include "ZComDef.h"
#include "OSAL.h"
#include "AF.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "ZDProfile.h"
#include "MT_SYS.h"

#include "app_sample.h"
#include "onboard.h"

#include "OSAL_PwrMgr.h"
#include "OSAL_Nv.h"
#include "sapi.h"
#include "nwk_globals.h"
#include "AssocList.h"
#include "AddrMgr.h"

/* HAL */
#include "hal_uart.h"
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"

#include "app_driver.h"
#include "stdio.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */
/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
byte appSample_TaskID;

uint8 appSampleSeqNum;          // This is the unique message ID (counter)


/*********************************************************************
 * GLOBAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
devStates_t appSample_NwkState = DEV_INIT;

endPointDesc_t appSample_epDesc;


/*********************************************************************
 * LOCAL FUNCTIONS
 */
void appSample_MessageMSGCB( afIncomingMSGPacket_t *pkt );
void serialCallback( uint8 port, uint8 events );
void print_dev_mac(void);


/*********************************************************************
 * @fn          appSample_Init
 *
 * @brief       Initialization function for the zclGeneral layer.
 *
 * @param       none
 *
 * @return      none
 */
void appSample_Init( byte task_id )
{
  appSample_TaskID = task_id;

  keyInit(appSample_TaskID,SAMPLEAPP_KEY_EVT,10);
    
    
  if(zgDeviceLogicalType==ZG_DEVICETYPE_COORDINATOR)
  {
    printDebugString("COORDINATOR\n");
  }
  else if(zgDeviceLogicalType==ZG_DEVICETYPE_ROUTER)
  {
    printDebugString("ROUTER\n");
  }
  else if(zgDeviceLogicalType==ZG_DEVICETYPE_ENDDEVICE)
  {
    printDebugString("ENDDEVICE\n");
  }
  else 
  {
    printDebugString("DEV ERR\n");
  }
  
#ifdef HAL_PA_LNA
    printDebugString("PA\n");
#else
    printDebugString("NO PA\n");
#endif
  
  //´®¿Ú³õÊ¼»¯
  uartInit(HAL_UART_BR_115200,serialCallback);
  
  // Fill out the endpoint description.
  appSample_epDesc.endPoint = SAMPLE_ENDPOINT;
  appSample_epDesc.task_id = &appSample_TaskID;
  appSample_epDesc.simpleDesc
            = (SimpleDescriptionFormat_t *)&appSample_SimpleDesc;
  appSample_epDesc.latencyReq = noLatencyReqs;

  // Register the endpoint description with the AF
  afRegister( &appSample_epDesc );

}

/*********************************************************************
 * @fn          zclSample_event_loop
 *
 * @brief       Event Loop Processor for zclGeneral.
 *
 * @param       none
 *
 * @return      none
 */
uint16 appSample_event_loop( uint8 task_id, uint16 events )
{
  afIncomingMSGPacket_t *MSGpkt;
  afDataConfirm_t *afDataConfirm;

  // Data Confirmation message fields
  byte sentEP;
  ZStatus_t sentStatus;
  byte sentTransID;       // This should match the value sen
  (void)task_id;          // Intentionally unreferenced parameter

  if ( events & SYS_EVENT_MSG )
  {
    while ( (MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( appSample_TaskID )) )
    {
      switch ( MSGpkt->hdr.event )
      {
        case ZDO_STATE_CHANGE:
          appSample_NwkState = (devStates_t)(MSGpkt->hdr.status);
            
          // now on the network
          if ( (appSample_NwkState == DEV_ZB_COORD) ||
               (appSample_NwkState == DEV_ROUTER)   ||
               (appSample_NwkState == DEV_END_DEVICE) )
          {
            HalLedSet( HAL_LED_1, HAL_LED_MODE_ON );
            
            if(appSample_NwkState==DEV_ZB_COORD)
            {
              printDebugString("DEV_ZB_COORD\n");
            }
            else if(appSample_NwkState==DEV_ROUTER)
            {
              printDebugString("DEV_ROUTER\n");
            }
            else if(appSample_NwkState==DEV_END_DEVICE)
            {
              printDebugString("DEV_END_DEVICE\n");
            }
          }
          else
          {
            HalLedSet( HAL_LED_1, HAL_LED_MODE_OFF );
          }
          break;
          
        case AF_DATA_CONFIRM_CMD:
          // This message is received as a confirmation of a data packet sent.
          // The status is of ZStatus_t type [defined in ZComDef.h]
          // The message fields are defined in AF.h
          afDataConfirm = (afDataConfirm_t *)MSGpkt;

          sentEP = afDataConfirm->endpoint;
          (void)sentEP;  // This info not used now
          sentTransID = afDataConfirm->transID;
          (void)sentTransID;  // This info not used now

          sentStatus = afDataConfirm->hdr.status;
          // Action taken when confirmation is received.
          if ( sentStatus != ZSuccess )
          {
            // The data wasn't delivered -- Do something
            printDebugString("\nmsg send rsp err\n"); 
          }
          else
          {
//            printDebugString("msg send rsp success\n"); 
          }
          break;

        // Received when a messages is received (OTA) for this endpoint
        case AF_INCOMING_MSG_CMD:
          appSample_MessageMSGCB( MSGpkt );
          break;

        default:
          break;
      }

      // Release the memory
      osal_msg_deallocate( (uint8 *)MSGpkt );
    }

    // return unprocessed events
    return (events ^ SYS_EVENT_MSG);
  }
  
  if ( events & SAMPLEAPP_TIMEOUT_EVT )
  {
    // Send the periodic message
    SampleApp_SendPeriodicMessage("hello",5);

    // Setup to send message again in normal period (+ a little jitter)
    osal_start_timerEx( appSample_TaskID, SAMPLEAPP_TIMEOUT_EVT,
        (5000 + (osal_rand() & 0x00FF)) );

    // return unprocessed events
    return (events ^ SAMPLEAPP_TIMEOUT_EVT);
  }
  
  if ( events & SAMPLEAPP_KEY_EVT )
  {
    printDebugStringNum("key press : ",readKeyPress());
    
    // return unprocessed events
    return (events ^ SAMPLEAPP_KEY_EVT);
  }


  // Discard unknown events
  return 0;
}

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * @fn      appSample_MessageMSGCB
 *
 * @brief   Data message processor callback.  This function processes
 *          any incoming data - probably from other devices.  So, based
 *          on cluster ID, perform the intended action.
 *
 * @param   none
 *
 * @return  none
 */
void appSample_MessageMSGCB( afIncomingMSGPacket_t *pkt )
{
  //´®¿ÚÊä³ö½ÓÊÕµ½µÄÊý¾Ý
  HalUARTWrite(0, pkt->cmd.Data, pkt->cmd.DataLength);
  
  switch ( pkt->clusterId )
  {
    case APP_PERIODIC_CLUSTERID:
      
      break;
      
    case APP_UNICAST_CLUSTERID:
      break;
      
    case APP_MULTICAST_CLUSTERID:
      break;
  }
}


/*********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      SampleApp_SendPeriodicMessage
 *
 * @brief   Send the periodic message.
 *
 * @param   none
 *
 * @return  none
 */
afStatus_t SampleApp_SendPeriodicMessage( uint8 *data, uint16 datalen )
{
  afStatus_t state;
  afAddrType_t SampleApp_DstAddr;

  SampleApp_DstAddr.addrMode = (afAddrMode_t)AddrBroadcast;
  SampleApp_DstAddr.endPoint = SAMPLE_ENDPOINT;
  SampleApp_DstAddr.addr.shortAddr = 0xFFFF;
  
  state = AF_DataRequest( &SampleApp_DstAddr, &appSample_epDesc,
                         APP_PERIODIC_CLUSTERID,
                         datalen,
                         data,
                         &appSampleSeqNum,
                         AF_DISCV_ROUTE,
                         AF_DEFAULT_RADIUS );
  if ( state == afStatus_SUCCESS )
  {
//    printDebugString("msg send success\n"); 
  }
  else
  {
    // Error occurred in request to send.
    printDebugString("msg send error\n"); 
  }
  
  return state;
}

/*********************************************************************
 * @fn      SampleApp_SendUnicastMessage
 *
 * @brief   Send the unicast message.
 *
 * @param   none
 *
 * @return  none
 */
afStatus_t SampleApp_SendUnicastMessage( uint16 addr, uint8 *data, uint16 datalen )
{
  afStatus_t state;
  afAddrType_t SampleApp_DstAddr;

  SampleApp_DstAddr.addrMode = (afAddrMode_t)Addr16Bit;
  SampleApp_DstAddr.endPoint = SAMPLE_ENDPOINT;
  SampleApp_DstAddr.addr.shortAddr = addr;
  
  state = AF_DataRequest( &SampleApp_DstAddr, &appSample_epDesc,
                         APP_UNICAST_CLUSTERID,
                         datalen,
                         data,
                         &appSampleSeqNum,
                         AF_DISCV_ROUTE,
                         AF_DEFAULT_RADIUS );
  if ( state == afStatus_SUCCESS )
  {
//    printDebugString("msg send success\n"); 
  }
  else
  {
    // Error occurred in request to send.
    printDebugString("msg send error\n"); 
  }

  return state;
}

/*********************************************************************
 * @fn      SampleApp_SendMulticastMessage
 *
 * @brief   Send the multicast message.
 *
 * @param   none
 *
 * @return  none
 */
afStatus_t SampleApp_SendMulticastMessage( uint16 group, uint8 *data, uint16 datalen )
{
  afStatus_t state;
  afAddrType_t SampleApp_DstAddr;

  SampleApp_DstAddr.addrMode = (afAddrMode_t)afAddrGroup;
  SampleApp_DstAddr.endPoint = SAMPLE_ENDPOINT;
  SampleApp_DstAddr.addr.shortAddr = group;
  
  state = AF_DataRequest( &SampleApp_DstAddr, &appSample_epDesc,
                         APP_MULTICAST_CLUSTERID,
                         datalen,
                         data,
                         &appSampleSeqNum,
                         AF_DISCV_ROUTE,
                         AF_DEFAULT_RADIUS );
  if ( state == afStatus_SUCCESS )
  {
//    printDebugString("msg send success\n"); 
  }
  else
  {
    // Error occurred in request to send.
    printDebugString("msg send error\n"); 
  }

  return state;
}


void serialCallback( uint8 port, uint8 events )
{
  (void)port;
  uint8 SampleApp_TxLen;
  uint8 SampleApp_TxBuf[85];
  
  if (events & ( HAL_UART_RX_FULL | HAL_UART_RX_ABOUT_FULL | HAL_UART_RX_TIMEOUT))
  {
    SampleApp_TxLen = HalUARTRead(0,SampleApp_TxBuf,80); 
    //Í¸´«Êý¾Ý·¢ËÍ
    if(appSample_NwkState==DEV_ZB_COORD)        //Â·ÓÉÆ÷-¹ã²¥
    {
      SampleApp_SendPeriodicMessage(SampleApp_TxBuf,SampleApp_TxLen);
    }
    else        //ÆäËû-µã²¥µ½Ð­µ÷Æ÷
    {
      SampleApp_SendUnicastMessage( 0x0000, SampleApp_TxBuf, SampleApp_TxLen );
    }

  }
}

/****************************************************************************
****************************************************************************/


