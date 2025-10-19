#include <stdint.h>
#include <lvgl.h>

void Cosine(int16_t angle);
void Sine(int16_t angle);
int16_t CoordinateXRelocate(int16_t x);
int16_t CoordinateYRelocate(int16_t y);
lv_point_t CoordinateRelocate(int16_t radius, int16_t angle);
void UpdateClock(void);
void Refresh(void);