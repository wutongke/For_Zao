#include "OSAL.h"
#include "ZGlobals.h"
#include "OnBoard.h"

#include "hal_uart.h"

#include "app_driver.h"
#include <stdio.h>


static byte key_rspTaskID = 0;
static uint16 key_event = 0;
static uint16 key_time = 0;

void keyInit( byte task, uint16 event, uint16 time )
{
  key_rspTaskID = task;
  key_event = event;
  key_time = time;
  
  //P0中断优先级最高
  //  IP0 |= (1<<5);
  //  IP1 |= (1<<5);
  
  // P00
  P0SEL &= ~0x01;       // 0:GPIO
  P0DIR &= ~0x01;       // 0:input    1:output
  P0INP |= 0x01;        // 0:上下拉    1:三态
  P0IEN |= 0X01;        // P00 设置为中断方式
  PICTL |= 0x01;        // 下降沿触发  
  
  // P01
  P0SEL &= ~0x02;       // 0:GPIO
  P0DIR &= ~0x02;       // 0:input    1:output
  P0INP |= 0x02;        // 0:上下拉    1:三态
  P0IEN |= 0X02;        // P00 设置为中断方式
  PICTL |= 0x02;        // 下降沿触发  
  
  IEN1 |= 0X20;         // 允许P0口中断;
  P0IFG = 0x00;         // 初始化中断标志位
  EA = 1; 
}

uint8 readKeyPress(void)
{
  uint8 key = 0;
  
  if(P0_0==0)
  {
    key |= 0x01;
  }
  else
  {
    key &= ~0x01;
  }
  
  if(P0_1==0)
  {
    key |= 0x02;
  }
  else
  {
    key &= ~0x02;
  }
  
  return key;
}

 /**************************************************************************************************
* @fn      halKeyPort0Isr
*
* @brief   Port0 ISR
*
* @param
*
* @return
**************************************************************************************************/
HAL_ISR_FUNCTION( halKeyPort0Isr, P0INT_VECTOR )
{
  HAL_ENTER_ISR()
//  P0SEL |= 0x0C;        //串口IO处理

  if(key_event)osal_start_timerEx( key_rspTaskID, key_event,key_time);
  
  //清中断标志
  P0IFG = 0;             
  P0IF = 0;           
  HAL_EXIT_ISR();
}


void uartInit( uint8 buad, halUARTCBack_t serialCallBack )
{
  halUARTCfg_t uartConfig;

  // configure UART
  uartConfig.configured           = TRUE;
  uartConfig.baudRate             = buad;
  uartConfig.flowControl          = FALSE;
  uartConfig.flowControlThreshold = 64;
  uartConfig.rx.maxBufSize        = 255;
  uartConfig.tx.maxBufSize        = 255;
  uartConfig.idleTimeout          = 6;
  uartConfig.intEnable            = TRUE;
  uartConfig.callBackFunc         = serialCallBack;

  // start UART
  // Note: Assumes no issue opening UART port.
  (void)HalUARTOpen( 0, &uartConfig );

  return;
}


void printDebugString( char *str )
{
  HalUARTWrite(0,str,osal_strlen(str)); 
}

void printDebugStringNum( char *str, uint16 num )
{
  char buf[15];
  
  HalUARTWrite(0,str,osal_strlen(str)); 
  sprintf(buf,"%d - %x\n",num,num);
  HalUARTWrite(0,buf,osal_strlen(buf)); 
}

void printResString( char *str )
{
  HalUARTWrite(0,str,osal_strlen(str)); 
}



