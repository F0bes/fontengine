EE_BIN = fontengine_sample.elf
EE_OBJS = fontengine_sample.o font/font_pallete_tex.o font/font_tex.o

# Link to fontengine / fontqueue however you want, I'm gonna cheat
EE_OBJS += ../fontengine.o ../fontqueue.o
EE_LIBS = -ldma -ldraw -lgraph

all: $(EE_BIN)

clean:
	rm -f $(EE_OBJS)

run: $(EE_BIN)
	ps2client execee host:$(EE_BIN)

wsl: $(EE_BIN)
	$(PCSX2) --elf="$(shell wslpath -w $(shell pwd))/$(EE_BIN)"

emu: $(EE_BIN)
	$(PCSX2) --elf="$(shell pwd)/$(EE_BIN)"

font/font_tex.c: font/font_tex.raw
	bin2c $< font/font_tex.c font_tex

font/font_pallete_tex.c: font/font_pallete_tex.raw
	bin2c $< font/font_pallete_tex.c font_pallete_tex

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
