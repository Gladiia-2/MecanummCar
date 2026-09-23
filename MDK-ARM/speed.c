#include "main.h"
#include "cmsis_os.h"
#include "usb_device.h"
#include "vofa_justfloat.h"
#include "pidmove.h"
#include "freertos.h"
#include "task.h"


#define PIE      3.14f
extern uint16_t g_pwm[4];
extern float g_delta_rev[4];
extern float kp1,ki1,kd1;
extern float kp2,ki2,kd2;
extern float kp3,ki3,kd3;
extern float kp4,ki4,kd4;
extern float setrpm;
extern pid_type_def angle1[4],speed[4];
extern float g_angle[4];
extern float send[2];
extern float g_setspeed[4];
uint8_t i;
extern TIM_HandleTypeDef htim5;
extern float g_speed1[4];//set speed
extern volatile float g_speed_rpm[4];

extern float vx,vy,vw,k;



void Task_Control(void const *argument)  
{
	
	while(1)
	{	
		
			//PID 输出 >=0 时正转
			if(speed[0].out>= 0)
			{
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
				g_pwm[0]=speed[0].out;
			}
			else
			{
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
				g_pwm[0]=-speed[0].out;
			}
			
			
			if(speed[1].out>= 0)
			{
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);
		    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET);
				g_pwm[1]=speed[1].out;
			}
			else
			{
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_RESET);
				g_pwm[1]=-speed[1].out;
			}
			
		
			if(speed[2].out>= 0)
			{
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);
				g_pwm[2]=speed[2].out;
			}
			else
			{
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);
				g_pwm[2]=-speed[2].out;
			}
			
		
			if(speed[3].out>= 0)
			{
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
				g_pwm[3]=speed[3].out;
			}
			else
			{
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
				g_pwm[3]=-speed[3].out;
			}
		
		 
			
		
		
		
			//PWM 值写进 TIM5通道
			__HAL_TIM_SetCompare(&htim5,TIM_CHANNEL_1,g_pwm[0]);
			__HAL_TIM_SetCompare(&htim5,TIM_CHANNEL_2,g_pwm[1]);
			__HAL_TIM_SetCompare(&htim5,TIM_CHANNEL_3,g_pwm[2]);
			__HAL_TIM_SetCompare(&htim5,TIM_CHANNEL_4,g_pwm[3]);
			vTaskDelay(pdMS_TO_TICKS(2));
	
  }
	
}
		
