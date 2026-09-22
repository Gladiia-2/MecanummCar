#include "main.h"
#include "cmsis_os.h"
#include "usb_device.h"
#include "vofa_justfloat.h"
#include "pidmove.h"
#include "freertos.h"
#include "task.h"

#define SAMPLE_TIME        0.010f

#define PIE      3.14f
extern uint16_t g_last_cnt[4];        // ??? CNT

extern volatile int16_t g_delta_cnt[4];      // ? 10 ms ???????
extern volatile float g_speed_rpm[4];     // ?? rpm

extern uint16_t g_now_cnt[4];
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
extern float speed1;//set speed

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;


extern float vx, vy, vw, k;//centimeter


uint32_t last_tick = 0;
uint32_t now_tick,dt_tick;

void Encoder_Update(void)

{    
    now_tick=xTaskGetTickCount();
		dt_tick = now_tick - last_tick;
		last_tick = now_tick;
		
    /* 1. ???? CNT */
    g_now_cnt[0] = (uint16_t)__HAL_TIM_GET_COUNTER(&htim1);
    
    /* 2. ?? CNT - ?? CNT
     * ??? 10 ms ??????????
     * 
     * ?? 16 ?????,??????
     * 65535 -> 0 ? 0 -> 65535 ??????
     */
    g_delta_cnt[0] = (int16_t)(uint16_t)(g_now_cnt[0] - g_last_cnt[0]);

    /* 3. ???? CNT,???? */
    g_last_cnt[0] = g_now_cnt[0];

    /* 4. ??? -> ????? */
    g_delta_rev[0] = (float)g_delta_cnt[0] / (4.0f * 13.0f * 34.0f);

    /* 5. ? / 10 ms -> rpm */
    g_speed_rpm[0] = (g_delta_rev[0] / SAMPLE_TIME) * 60.0f /2.86f;

    /* 6. ???? */
    g_angle[0] += g_delta_rev[0] /2.86f * 360.0f;

    /* 7. ?????? 0~360? */
    while (g_angle[0] >= 360.0f)
    {
        g_angle[0] -= 360.0f;
    }

    while (g_angle[0] < 0.0f)
    {
        g_angle[0] += 360.0f;
    }
		//PID_calc_yaw_move(&angle1,g_angle[i],setangle);
		g_setspeed[0]= (vx- vy - k*vw)/(6.8f *PIE);//set rpm;per minute!!!
			
		PID_calc_move(&speed[0],g_speed_rpm[0],g_setspeed[0]);
		
		
		
		/* 1. ???? CNT */
    g_now_cnt[1] = (uint16_t)__HAL_TIM_GET_COUNTER(&htim2);
    
    /* 2. ?? CNT - ?? CNT
     * ??? 10 ms ??????????
     * 
     * ?? 16 ?????,??????
     * 65535 -> 0 ? 0 -> 65535 ??????
     */
    g_delta_cnt[1] = (int16_t)(uint16_t)(g_now_cnt[1] - g_last_cnt[1]);

    /* 3. ???? CNT,???? */
    g_last_cnt[1] = g_now_cnt[1];

    /* 4. ??? -> ????? */
    g_delta_rev[1] = (float)g_delta_cnt[1] / (4.0f * 13.0f * 34.0f);

    /* 5. ? / 10 ms -> rpm */
    g_speed_rpm[1] = g_delta_rev[1] / SAMPLE_TIME * 60.0f / 2.86f;

    /* 6. ???? */
    g_angle[1] += g_delta_rev[1]/2.86f * 360.0f;

    /* 7. ?????? 0~360? */
    while (g_angle[1] >= 360.0f)
    {
        g_angle[1] -= 360.0f;
    }

    while (g_angle[1] < 0.0f)
    {
        g_angle[1] += 360.0f;
    }
		//PID_calc_yaw_move(&angle1,g_angle[i],setangle);
	
			g_setspeed[1]= (vx+vy -k*vw)/(6.8f *PIE);
			
		PID_calc_move(&speed[1],g_speed_rpm[1],g_setspeed[1]);
		
		
		
		
		
		/* 1. ???? CNT */
    g_now_cnt[2] = (uint16_t)__HAL_TIM_GET_COUNTER(&htim3);
    
    /* 2. ?? CNT - ?? CNT
     * ??? 10 ms ??????????
     * 
     * ?? 16 ?????,??????
     * 65535 -> 0 ? 0 -> 65535 ??????
     */
    g_delta_cnt[2] = (int16_t)(uint16_t)(g_now_cnt[2] - g_last_cnt[2]);

    /* 3. ???? CNT,???? */
    g_last_cnt[2] = g_now_cnt[2];

    /* 4. ??? -> ????? */
    g_delta_rev[2] = (float)g_delta_cnt[2] / (4.0f * 13.0f * 34.0f);

    /* 5. ? / 10 ms -> rpm */
    g_speed_rpm[2] = g_delta_rev[2] / SAMPLE_TIME * 60.0f / 2.86f;

    /* 6. ???? */
    g_angle[2] += g_delta_rev[2]/2.86f * 360.0f;

    /* 7. ?????? 0~360? */
    while (g_angle[2] >= 360.0f)
    {
        g_angle[2] -= 360.0f;
    }

    while (g_angle[2] < 0.0f)
    {
        g_angle[2] += 360.0f;
    }
		//PID_calc_yaw_move(&angle1,g_angle[i],setangle);
		
			g_setspeed[2]= (vx-vy+k*vw)/(6.8f *PIE);
			
		PID_calc_move(&speed[2],g_speed_rpm[2],g_setspeed[2]);
		
		
		
		
		/* 1. ???? CNT */
    g_now_cnt[3] = (uint16_t)__HAL_TIM_GET_COUNTER(&htim4);
    
    /* 2. ?? CNT - ?? CNT
     * ??? 10 ms ??????????
     * 
     * ?? 16 ?????,??????
     * 65535 -> 0 ? 0 -> 65535 ??????
     */
    g_delta_cnt[3] = (int16_t)(uint16_t)(g_now_cnt[3] - g_last_cnt[3]);

    /* 3. ???? CNT,???? */
    g_last_cnt[3] = g_now_cnt[3];

    /* 4. ??? -> ????? */
    g_delta_rev[3] = (float)g_delta_cnt[3] / (4.0f * 13.0f * 34.0f);

    /* 5. ? / 10 ms -> rpm */
    g_speed_rpm[3] = g_delta_rev[3] / SAMPLE_TIME * 60.0f/ 2.86f;

    /* 6. ???? */
    g_angle[3] += g_delta_rev[3]/2.86f * 360.0f;

    /* 7. ?????? 0~360? */
    while (g_angle[3] >= 360.0f)
    {
        g_angle[3] -= 360.0f;
    }

    while (g_angle[3] < 0.0f)
    {
        g_angle[3] += 360.0f;
    }
		//PID_calc_yaw_move(&angle1,g_angle[i],setangle);
		
			g_setspeed[3]= (vx+vy+k*vw)/(6.8f *PIE);
		PID_calc_move(&speed[3],g_speed_rpm[3],g_setspeed[3]);
	
}



void Task_Ser(void const *argument)
{	
	while(1)
	{
		Encoder_Update();
	
		vTaskDelay(pdMS_TO_TICKS(10));
	}
	
}	
