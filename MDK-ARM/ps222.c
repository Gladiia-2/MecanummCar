#include "main.h"
#include "cmsis_os.h"
#include "usb_device.h"
#include "vofa_justfloat.h"
#include "pidmove.h"
#include "FreeRTOS.h"
#include "task.h"
#include "ps2.h"

//extern unsigned char buf[9];

extern float vx,vy,vw;


/**
  * @brief  PS2 手柄读取任务
  * @param  argument: 未使用
  * @retval None
  */
void PS2(void const *argument)
{
	/* Infinite loop */
	for(;;)
	{
		/* 读一帧：成功会刷新 ps2_data / ps2_btn_low / ps2_btn_high / ps2_lx ... */
		PS2_Poll();
		
		vx= (ps2_data[8]-127)*8;
		vy= (ps2_data[7]-128)*8;
		

		/* 20ms 一帧足够，同时把 CPU 让给别的任务 */
		osDelay(20);
	}
}
