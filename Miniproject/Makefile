CONTIKI_PROJECT = udp-client udp-server
all: $(CONTIKI_PROJECT)


TARGET_LIBFILES += -lm
LDLIBS += -lm
MAKE_NET = MAKE_NET_NULLNET
CONTIKI = ../../../..
include $(CONTIKI)/Makefile.include
