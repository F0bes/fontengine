#pragma once

#include <draw.h>
#include <dma.h>

void fontengine_init(u32 shareFrameSpace, u32 fbptr);
qword_t* fontengine_print_string(qword_t* q, const char* str, int* x, int* y, int z);
void fontqueue_clear();

extern const u32 FE_WIDTH;
extern const u32 FE_HEIGHT;
