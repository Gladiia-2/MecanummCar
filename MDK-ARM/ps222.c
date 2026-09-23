#include "main.h"
#include "cmsis_os.h"
#include "usb_device.h"
#include "vofa_justfloat.h"
#include "pidmove.h"
#include "FreeRTOS.h"
#include "task.h"
#include "ps2.h"



extern float vx,vy,vw;
 
void PS2(void const *argument)   // PS2 手柄任务
{
	
	while(1)
	{
		PS2_Poll();	
		osDelay(20);
	}
}
