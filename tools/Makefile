TARGETS	= $(OBJDIR)libVPN.so
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
LIBDIR	= $(PREFIX)/lib
CC	= cc
CPPFLAGSF=
CPPFLAGS= -I $(PREFIX)/include
CFLAGSF	= -W -fPIC `pkg-config --cflags libApp`
CFLAGS	= -Wall -g -O2 -pedantic
LDFLAGSF= `pkg-config --libs libApp`
LDFLAGS	= 
AR	= ar
RANLIB	= ranlib
CCSHARED= $(CC) -shared
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

libVPN_OBJS = $(OBJDIR)libvpn.o
libVPN_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
libVPN_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)libVPN.so: $(libVPN_OBJS)
	$(CCSHARED) -o $(OBJDIR)libVPN.so $(libVPN_OBJS) $(libVPN_LDFLAGS)

$(OBJDIR)libvpn.o: libvpn.c ../include/VPN.h ../src/common.c
	$(CC) $(libVPN_CFLAGS) -o $(OBJDIR)libvpn.o -c libvpn.c

clean:
	$(RM) -- $(libVPN_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	$(MKDIR) $(DESTDIR)$(LIBDIR)/AppWrapper
	$(INSTALL) -m 0755 $(OBJDIR)libVPN.so $(DESTDIR)$(LIBDIR)/AppWrapper/libVPN.so

uninstall:
	$(RM) -- $(DESTDIR)$(LIBDIR)/AppWrapper/libVPN.so

.PHONY: all clean distclean install uninstall
