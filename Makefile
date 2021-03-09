ECHO=@echo

CC_SOURCES_ext = \
	utility/bmp.c \
	utility/draw.c \
	extract.c

CC_SOURCES_conv = \
	utility/bmp.c \
	utility/draw.c \
	convert.c

CC_OBJS_ext=$(patsubst %.c,%.o,$(CC_SOURCES_ext))
CC_OBJS_conv=$(patsubst %.c,%.o,$(CC_SOURCES_conv))

CCOPT = -std=c++11
CCOPT  = -fPIC -Wall -O
CCOPT += \
	-I . \
	-ggdb

all: extract convert
	$(ECHO) "Done building."

%.o: %.cpp Makefile
	$(CC) $(CCOPT) -Wall $< -c -o $@

extract: $(CC_OBJS_ext)
	$(CC) $(CCOPT) $^ -o $@

convert: $(CC_OBJS_conv)
	$(CC) $(CCOPT) $^ -o $@

run_ext: extract
	@./extract
	
run_conv: convert
	@./convert
	@mv e_*.raw data/

clean: 
	@rm -f $(CC_OBJS_ext)
	@rm -f $(CC_OBJS_conv)
	@rm -f extract
	@rm -f convert
	@rm -f *.bmp
	@rm -f dnn/*.raw
	@rm -f data/*.raw
	@rm -f *.ppm
	@rm -f *.ycbcr
