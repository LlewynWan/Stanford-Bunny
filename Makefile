IDIR = include
CC = g++
CFLAGS = -I$(IDIR)

ODIR = obj
LDIR = lib
SDIR = src

LIBS = -lGL -lGLU -lGLEW -lSDL2 -lSDL2_image

_DEPS = camera.h display.h shader.h
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))

_OBJ = bunny.o camera.o display.o shader.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

bunny: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ bunny $(INCDIR)/*~
