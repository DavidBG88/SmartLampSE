#ifndef IAQ_CORE_H
#define IAQ_CORE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct iAQCore_Reading {
    uint8_t status;
    uint16_t eco2_ppm;
    uint16_t etvoc_ppb;
    uint32_t resistance;
} iAQCore_Reading;

iAQCore_Reading iAQCore_read();

#ifdef __cplusplus
}
#endif

#endif  // IAQ_CORE_H
