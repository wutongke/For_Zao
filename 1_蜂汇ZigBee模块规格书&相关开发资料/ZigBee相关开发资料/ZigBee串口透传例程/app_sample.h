/**************************************************************************************************
  Filename:       zcl_samplesw.h
  Revised:        $Date: 2015-08-19 17:11:00 -0700 (Wed, 19 Aug 2015) $
  Revision:       $Revision: 44460 $


  Description:    This file contains the Zigbee Cluster Library Home
                  Automation Sample Application.


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
  PROVIDED ìAS ISî WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
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

#ifndef ZCL_SAMPLESW_H
#define ZCL_SAMPLESW_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */


/*********************************************************************
 * CONSTANTS
 */
#define SAMPLE_ENDPOINT             36
#define SAMPLE_PROFILE_ID           0x1234
#define SAMPLE_DEVICE_ID            0x0001
#define SAMPLE_DEVICE_VERSION       0
#define SAMPLE_FLAGS                0

#define APP_MAX_INCLUSTERS              4
#define APP_PERIODIC_CLUSTERID          1
#define APP_UNICAST_CLUSTERID           2
#define APP_MULTICAST_CLUSTERID         3  
#define APP_SERIALA_CLUSTERID           4
   
// Events for the sample app
#define SAMPLEAPP_TIMEOUT_EVT           0x0001
#define SAMPLEAPP_KEY_EVT               0x0002

/*********************************************************************
 * MACROS
 */
   
// ±‡“Î∞Ê±æ& ±º‰
#define PRINTER_COMPLIE_TIME __DATE__ " " __TIME__
#define PRINTER_COMPLIE_VER  "V3.0"
   
   
/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * VARIABLES
 */
extern SimpleDescriptionFormat_t appSample_SimpleDesc;

/*********************************************************************
 * FUNCTIONS
 */

 /*
  * Initialization for the task
  */
extern void appSample_Init( byte task_id );

/*
 *  Event Process for the task
 */
extern UINT16 appSample_event_loop( byte task_id, UINT16 events );

   
afStatus_t SampleApp_SendPeriodicMessage( uint8 *data, uint16 datalen );
afStatus_t SampleApp_SendUnicastMessage( uint16 addr, uint8 *data, uint16 datalen );
afStatus_t SampleApp_SendMulticastMessage( uint16 group, uint8 *data, uint16 datalen );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* ZCL_SAMPLEAPP_H */
