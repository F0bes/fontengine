// This is a sample quickly written up to show how to use fontqueue and fontengine
// You don't have to use fontqueue, it just makes things easier compared to managing
// your GS drawing packets. But if that's your thing, feel free to use fontengine
// directly. The fontengine interface is a lot like drawlib functions
// ie: q = fontengine_print_string(). The function returns the pointer to your next
// qword.

// This sample is provided under the MIT license, accreditation is appreciated though
#include "../fontengine.h"
#include "../fontqueue.h"

#include <draw.h>
#include <graph.h>
#include <gs_psm.h>
#include <dma.h>

framebuffer_t g_fb;
zbuffer_t g_zb;

int main(void)
{
	// It's always good to reset the GIF
	// OSDSYS does something weird where it tells the GIF to expect data
	// and never sends data (oops!)
	(*(volatile u32*)0x10003000) = 1;

	g_fb.width = 640;
	g_fb.height = 448;
	g_fb.psm = GS_PSM_24;
	g_fb.address = graph_vram_allocate(g_fb.width, g_fb.height, g_fb.psm, GRAPH_ALIGN_PAGE);

	g_zb.address = graph_vram_allocate(g_fb.width, g_fb.height, g_fb.psm, GRAPH_ALIGN_PAGE);
	g_zb.enable = 1;
	g_zb.method = ZTEST_METHOD_ALLPASS; // Disable depth testing
	g_zb.mask = 0;
	g_zb.zsm = GS_ZBUF_24;

	// Initialise our privileged registers
	graph_initialize(g_fb.address, g_fb.width, g_fb.height, g_fb.psm, 0, 0);

	graph_wait_vsync(); // Waiting a vsync helps PCSX2 hardware mode

	// Set up our drawing settings
	qword_t setup_packet[30] __attribute__((aligned(64)));
	qword_t* q = setup_packet;

	q = draw_setup_environment(q, 0, &g_fb, &g_zb);
	q = draw_clear(q, 0, 0, 0, g_fb.width, g_fb.height, 0x33, 0x33, 0x00);
	q = draw_primitive_xyoffset(q, 0, 0, 0); // Set the primitive XYOFFSET to 0
	dma_channel_send_normal(DMA_CHANNEL_GIF, setup_packet, q - setup_packet, 0, 0);
	dma_channel_wait(DMA_CHANNEL_GIF, 0);

	// We have the option of storing our font data in the same address space
	// of the framebuffer and zbuffer, this is only possible when our buffers are 24 bit!!
	// If you wish to use a 32 bit or 16 bit framebuffer, pass 0 as the first arg
	// and the second argument is ignored!
	fontengine_init(1, g_fb.address);

	// Pick the amount of qword allocated. Pick too much, you run out of memory for yourself
	// pick too little, the queue will fill and you wont be able to write any more :(

	// You can use this to estimate
	// Each print call takes 3 base qwords
	// For each character, 4 qwords are used (excluding newlines)
	// So one call to print "Hello, World!" should be ~55 qwords ((13 * 4) + 3)
	fontqueue_init(4096);

	// Call this anytime to put the 'simple' cords back to the top of the screen
	fontqueue_reset_simple();
	// Each fontqueue_add_simple call will move the cursor down a line
	fontqueue_add_simple("This is using the 'simple' method of writing to screen\n");
	fontqueue_add_simple("Simple & full both support\nNewlines!");

	int x = 10, y = 225;
	// We pass a pointer to x and y, because fontengine will return the 'carrot' position
	fontqueue_add_full("This is using fontqueue_add_full", &x, &y, 10);

	x = 100;
	y = 250;
	fontqueue_add_full("We get to choose X and Y!", &x, &y, 10);

	while (1)
	{
		// Because we've used fontqueue, it stores all the font data for us and
		// provides a nifty function to kick it to the screen!
		fontqueue_kick();
		graph_wait_vsync();
	}

	// If you ever want to reset the font-queue, simply call this
	fontqueue_clear();
}
