ECHO=@echo

CC_SOURCES = \
	utility/bmp.c \
	utility/draw.c \
	extract.c

CC_OBJS=$(patsubst %.c,%.o,$(CC_SOURCES))

CCOPT = -std=c++11
CCOPT  = -fPIC -Wall -O
CCOPT += \
	-I . \
	-ggdb

all: extract
	$(ECHO) "Done building."

%.o: %.cpp Makefile
	$(CC) $(CCOPT) -Wall $< -c -o $@

extract: $(CC_OBJS)
	$(CC) $(CCOPT) $^ -o $@

run: extract
	@./extract

clean: 
	@rm -f $(CC_OBJS)
	@rm -f extract
	@rm -f *.bmp
	@rm -f *.ppm
	@rm -f *.ycbcr
