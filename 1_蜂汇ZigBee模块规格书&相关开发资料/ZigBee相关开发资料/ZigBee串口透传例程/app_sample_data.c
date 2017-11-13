/**************************************************************************************************
  Filename:       zcl_samplesw_data.c
  Revised:        $Date: 2014-07-30 12:57:37 -0700 (Wed, 30 Jul 2014) $
  Revision:       $Revision: 39591 $


  Description:    Zigbee Cluster Library - sample device application.


  Copyright 2006-2014 Texas Instruments Incorporated. All rights reserved.

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
  PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
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
#include "ZDConfig.h"

#include "app_sample.h"

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */


/*********************************************************************
 * SIMPLE DESCRIPTOR
 */
// This is the Cluster ID List and should be filled with Application
// specific cluster IDs.
//#define APP_SAMPLE_MAX_INCLUSTERS       2
const cId_t appSample_InClusterList[APP_MAX_INCLUSTERS] =
{
  APP_PERIODIC_CLUSTERID,
  APP_UNICAST_CLUSTERID,
  APP_MULTICAST_CLUSTERID,
  APP_SERIALA_CLUSTERID,
};

const cId_t appSample_OutClusterList[] =
{
  APP_PERIODIC_CLUSTERID,
  APP_UNICAST_CLUSTERID,
  APP_MULTICAST_CLUSTERID,
  APP_SERIALA_CLUSTERID,
};

#define APP_MAX_OUTCLUSTERS    ( ( sizeof( appSample_OutClusterList ) / sizeof( appSample_OutClusterList[0] ) ) - 1 )


SimpleDescriptionFormat_t appSample_SimpleDesc =
{
  SAMPLE_ENDPOINT,                    //  int Endpoint;
  SAMPLE_PROFILE_ID,                  //  uint16 AppProfId[2];
  SAMPLE_DEVICE_ID,                   //  uint16 AppDeviceId[2];
  SAMPLE_DEVICE_VERSION,              //  int   AppDevVer:4;
  SAMPLE_FLAGS,                       //  int   AppFlags:4;
  APP_MAX_INCLUSTERS,          //  byte  AppNumInClusters;
  (cId_t *)appSample_InClusterList, //  byte *pAppInClusterList;
  APP_MAX_OUTCLUSTERS,       //  byte  AppNumInClusters;
  (cId_t *)appSample_OutClusterList //  byte *pAppInClusterList;
};

/*********************************************************************
 * GLOBAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/****************************************************************************
****************************************************************************/


