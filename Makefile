BIN=box
OBJ=main.o box.o
CFLAGS=-O2

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN)

clean:
	rm *.o $(BIN)
