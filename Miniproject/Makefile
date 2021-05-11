PROJECT_SOURCEFILES += udp-client.c
CONTIKI_PROJECT = Client
all: $(CONTIKI_PROJECT)

TARGET_LIBFILES += -lm
LDLIBS += -lm
MAKE_NET = MAKE_NET_NULLNET
CONTIKI = ../../../..
include $(CONTIKI)/Makefile.include
