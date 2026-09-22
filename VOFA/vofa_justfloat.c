#include "vofa_justfloat.h"
#include "usbd_cdc_if.h"
#include <string.h>

/* JustFloat frame tail: bytes 00 00 80 7F (little-endian, marks end of frame) */
static const uint8_t VOFA_JUSTFLOAT_TAIL[4] = {0x00, 0x00, 0x80, 0x7F};

int8_t VOFA_JustFloat_Send(const float *data, uint16_t channels)
{
  uint8_t buf[VOFA_JUSTFLOAT_MAX_CHANNELS * sizeof(float) + sizeof(VOFA_JUSTFLOAT_TAIL)];
  uint16_t data_len;

  if ((data == NULL) || (channels == 0U) || (channels > VOFA_JUSTFLOAT_MAX_CHANNELS))
  {
    return -1;
  }

  data_len = channels * (uint16_t)sizeof(float);

  /* STM32 is little-endian, so a plain copy matches JustFloat's byte order */
  memcpy(buf, data, data_len);
  memcpy(&buf[data_len], VOFA_JUSTFLOAT_TAIL, sizeof(VOFA_JUSTFLOAT_TAIL));

  return (int8_t)CDC_Transmit_FS(buf, data_len + (uint16_t)sizeof(VOFA_JUSTFLOAT_TAIL));
}
