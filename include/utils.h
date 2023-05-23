#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t Days2Month(uint8_t days, uint8_t leap);
uint8_t Days2Date(uint8_t days, uint8_t leap);

#ifdef __cplusplus
}
#endif

#endif