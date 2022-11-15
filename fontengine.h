#pragma once

#include <draw.h>
#include <dma.h>

#ifdef __cplusplus
extern "C"
{
#endif
void fontengine_init(u32 shareFrameSpace, u32 fbptr);
qword_t* fontengine_print_string(qword_t* q, const char* str, int x, int y, int z, u64 color);
qword_t* fontengine_print_string_ptr(qword_t* q, const char* str, int* x, int* y, int z, u64 color);
void fontqueue_clear();
#ifdef __cplusplus
}
#endif
extern const u32 FE_WIDTH;
extern const u32 FE_HEIGHT;
