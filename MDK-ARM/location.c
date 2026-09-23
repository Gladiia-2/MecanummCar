#include "main.h"
#include "cmsis_os.h"
#include "usb_device.h"
#include "vofa_justfloat.h"
#include "pidmove.h"
#include "FreeRTOS.h"
#include "task.h"
#include "ps2.h"

extern float vx,vy;
extern float g_speed_rpm[4];
extern float send[2];
extern float setx,sety;
extern pid_type_def location[2],speed[4];

float x1,y1,x,y;    

void Task_Location(void const *argument)
{
	while(1)
	{
		x1= (g_speed_rpm[0]*0.01f + g_speed_rpm[1]*0.01f + g_speed_rpm[2]*0.01f + g_speed_rpm[3]*0.01f) / 4 ;
		y1=  (-g_speed_rpm[0]*0.01f + g_speed_rpm[1]*0.01f - g_speed_rpm[2]*0.01f + g_speed_rpm[3]*0.01f) / 4 ;
		x+=x1;
		y+=y1;
		
		PID_calc_move(&location[0],x,setx);
		PID_calc_move(&location[1],y,sety);		
		
		send[0]= setx;
		send[1]= x;
		VOFA_JustFloat_Send(send,2);
	
		osDelay(10);
	}
}
