CFLAGS = -g -O2 -Wall -I../slog -I../sms -lcurl -lpthread -lcrypt
LIB = ../slog/libslog.a ../sms/libmagtisun.a
OBJ = derp.o proto.o send.o agent.o conf.o irc.o info.o
INSTALL = ../bin


all: $(OBJ)
	$(CC) $(CFLAGS) -o derp $(OBJ) $(LIB)
	$(CC) -o keygen keygen.c -lcrypt
	@echo [-] Building binaries into - $(INSTALL)
	@test -d $(INSTALL) || mkdir $(INSTALL)
	@install -m 0755 keygen $(INSTALL)/
	@install -m 0755 derp $(INSTALL)/
	@install -m 0664 conf.cfg $(INSTALL)/
	@echo [-] Executable files builded in - $(INSTALL) folder

derp.o: irc.o conf.h info.h stdinc.h
send.o: send.h stdinc.h
agent.o: agent.h stdinc.h
irc.o: irc.h proto.o stdinc.h
proto.o: proto.h stdinc.h
conf.o: conf.h stdinc.h
info.o: info.h stdinc.h

.PHONY: clean

clean:
	$(RM) derp $(OBJ)