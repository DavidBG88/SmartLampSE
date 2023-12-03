#ifndef IAQ_CORE_H
#define IAQ_CORE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void APA102_init(uint8_t config_num_leds);

void APA102_set_color(uint8_t power, uint8_t r, uint8_t g, uint8_t b);

#ifdef __cplusplus
}
#endif

#endif  // IAQ_CORE_H
