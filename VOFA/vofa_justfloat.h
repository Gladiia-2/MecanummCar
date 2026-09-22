#ifndef __VOFA_JUSTFLOAT_H__
#define __VOFA_JUSTFLOAT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Max number of float channels supported in one frame */
#define VOFA_JUSTFLOAT_MAX_CHANNELS  32U

/**
  * @brief  Pack `channels` float values into a VOFA+ JustFloat frame
  *         (little-endian float data + 4-byte tail 00 00 80 7F) and
  *         send it over USB CDC.
  * @param  data:     pointer to the float array to send
  * @param  channels: number of floats in data (1..VOFA_JUSTFLOAT_MAX_CHANNELS)
  * @retval 0: OK, -1: invalid channel count, other: CDC_Transmit_FS status
  */
int8_t VOFA_JustFloat_Send(const float *data, uint16_t channels);

#ifdef __cplusplus
}
#endif

#endif /* __VOFA_JUSTFLOAT_H__ */
