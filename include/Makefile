TARGETS	= $(OBJDIR)VPN.h
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

$(OBJDIR)VPN.h: ../data/VPN.interface
	../data/appbroker.sh -P "$(PREFIX)" -- "$(OBJDIR)VPN.h"

clean:
	$(RM) -- $(VPN.h_OBJS)
	../data/appbroker.sh -c -P "$(PREFIX)" -- "VPN.h"

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)

uninstall:

.PHONY: all clean distclean install uninstall
