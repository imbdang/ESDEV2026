#ifndef __ENCODER_H__
#define __ENCODER_H__

#include <stdint.h>

void Encoder_Init(void);

void Encoder_Update(void);

int32_t EncoderA_GetCount(void);
int32_t EncoderB_GetCount(void);

int32_t EncoderA_GetRPM(void);
int32_t EncoderB_GetRPM(void);

void EncoderA_ResetCount(void);
void EncoderB_ResetCount(void);

#endif /* __ENCODER_H__ */