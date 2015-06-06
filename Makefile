CFLAGS = -g -O2 -Wall -Iutils
LIB = utils/utils.a
OBJ = derp.o
INSTALL = bin

all: $(OBJ)
	$(CC) $(CFLAGS) -o derp $(OBJ) $(LIB)
	@echo [-] Building binaries into - $(INSTALL)
	@test -d $(INSTALL) || mkdir $(INSTALL)
	@install -m 0755 derp $(INSTALL)/
	@echo [-] Executable files builded in - $(INSTALL) folder

derp.o: stdinc.h

.PHONY: clean

clean:
	$(RM) derp $(OBJ)