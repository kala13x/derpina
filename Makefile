CFLAGS = -g -O2 -Wall -Islog
LIB = slog/slog.a
OBJ = derp.o conf.o irc.o info.o
INSTALL = bin

all: $(OBJ)
	$(CC) $(CFLAGS) -o derp $(OBJ) $(LIB)
	@echo [-] Building binaries into - $(INSTALL)
	@test -d $(INSTALL) || mkdir $(INSTALL)
	@install -m 0755 derp $(INSTALL)/
	@echo [-] Executable files builded in - $(INSTALL) folder

derp.o: irc.o conf.h info.o stdinc.h
conf.o: conf.h stdinc.h
info.o: info.h stdinc.h
irc.o: irc.h stdinc.h

.PHONY: clean

clean:
	$(RM) derp $(OBJ)