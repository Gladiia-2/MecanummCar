#include "ps2.h"

extern SPI_HandleTypeDef hspi3;
//extern unsigned char buf[9];
volatile uint8_t ps2_data[PS2_DATA_LEN] = {0};
volatile uint8_t ps2_ok = 0;
volatile uint8_t ps2_btn_low  = 0xFF;    /* 0xFF = 按键全部松开 */
volatile uint8_t ps2_btn_high = 0xFF;
volatile uint8_t ps2_rx = 0x80;
volatile uint8_t ps2_ry = 0x80;
volatile uint8_t ps2_lx = 0x80;
volatile uint8_t ps2_ly = 0x80;

static void PS2_ATT_Low(void)
{
    HAL_GPIO_WritePin(PS2_ATT_GPIO_Port, PS2_ATT_PIN, GPIO_PIN_RESET);
}
static void PS2_ATT_High(void)
{
    HAL_GPIO_WritePin(PS2_ATT_GPIO_Port, PS2_ATT_PIN, GPIO_PIN_SET);
}
void PS2_Init(void)
{
    PS2_ATT_High();
    /* 先空跑一个字节，让 SCK 停在空闲电平(CPOL=1 时为高)，
       否则第一帧会整体错开一个 bit。此时 ATT 为高，手柄会忽略它。 */
    (void)PS2_TransferByte(0xFF);
}
uint8_t PS2_TransferByte(uint8_t tx)                           //传输数据并返回接受数据
{
    uint8_t rx = 0;
    HAL_SPI_TransmitReceive(&hspi3,&tx,&rx,1,2);
    return rx;
}
uint8_t PS2_ReadData(uint8_t *buf)
{
    if (buf == 0) return 0;                                   //接受数据失败失败，返回0
    PS2_ATT_High();                                           //准备接受数据，拉高ATT再拉低
    PS2_ATT_Low();
    buf[0]=PS2_TransferByte(0x01);
    buf[1]=PS2_TransferByte(0x42);
    buf[2]=PS2_TransferByte(0x00);
    buf[3]=PS2_TransferByte(0x00);
    buf[4]=PS2_TransferByte(0x00);
    buf[5]=PS2_TransferByte(0x00);
    buf[6]=PS2_TransferByte(0x00);
    buf[7]=PS2_TransferByte(0x00);
    buf[8]=PS2_TransferByte(0x00);
    PS2_ATT_High();
    if (buf[2] == 0x5a) return 1;                           //成功接受到数据，返回状态1
    else return 0;
}

/* 读一帧并刷新 ps2_data / ps2_btn_low / ps2_lx 等全局量，成功返回 1 */
uint8_t PS2_Poll(void)
{
    uint8_t buf[PS2_DATA_LEN];
    uint8_t i;
    uint8_t ok = PS2_ReadData(buf);

    if (ok)
    {
        for (i = 0; i < PS2_DATA_LEN; i++)
        {
            ps2_data[i] = buf[i];
        }
        ps2_btn_low  = buf[3];
        ps2_btn_high = buf[4];
        ps2_rx = buf[5];
        ps2_ry = buf[6];
        ps2_lx = buf[7];
        ps2_ly = buf[8];
    }
    ps2_ok = ok;
    return ok;
}
