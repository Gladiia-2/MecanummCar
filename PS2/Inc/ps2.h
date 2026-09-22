#ifndef FINAL3_PS2_H
#define FINAL3_PS2_H

#include "main.h"

#define PS2_ATT_GPIO_Port   GPIOA
#define PS2_ATT_PIN         GPIO_PIN_4

#define PS2_DATA_LEN        9

/* 最近一次成功读到的一帧原始数据（读取失败时保持不变） */
extern volatile uint8_t ps2_data[PS2_DATA_LEN];
/* 最近一次通信结果：1 = 收到 0x5A 应答，0 = 没读到 */
extern volatile uint8_t ps2_ok;

/* 从 ps2_data 里解析出来的按键与摇杆，方便控制代码直接取用 */
extern volatile uint8_t ps2_btn_low;    /* ps2_data[3] */
extern volatile uint8_t ps2_btn_high;   /* ps2_data[4] */
extern volatile uint8_t ps2_rx;         /* ps2_data[5] 右摇杆 X */
extern volatile uint8_t ps2_ry;         /* ps2_data[6] 右摇杆 Y */
extern volatile uint8_t ps2_lx;         /* ps2_data[7] 左摇杆 X */
extern volatile uint8_t ps2_ly;         /* ps2_data[8] 左摇杆 Y */

/* 按键位掩码：对应 ps2_btn_low */
#define PS2_KEY_SELECT   0x01
#define PS2_KEY_L3       0x02
#define PS2_KEY_R3       0x04
#define PS2_KEY_START    0x08
#define PS2_KEY_UP       0x10
#define PS2_KEY_RIGHT    0x20
#define PS2_KEY_DOWN     0x40
#define PS2_KEY_LEFT     0x80

/* 按键位掩码：对应 ps2_btn_high */
#define PS2_KEY_L2       0x01
#define PS2_KEY_R2       0x02
#define PS2_KEY_L1       0x04
#define PS2_KEY_R1       0x08
#define PS2_KEY_TRIANGLE 0x10
#define PS2_KEY_CIRCLE   0x20
#define PS2_KEY_CROSS    0x40
#define PS2_KEY_SQUARE   0x80

/* PS2 按键为低电平有效：对应位为 0 表示按下 */
#define PS2_KEY_PRESSED(reg, key)  (((reg) & (key)) == 0)

void PS2_Init(void);
uint8_t PS2_TransferByte(uint8_t tx);
uint8_t PS2_ReadData(uint8_t *buf);
uint8_t PS2_Poll(void);

#endif //FINAL3_PS2_H
