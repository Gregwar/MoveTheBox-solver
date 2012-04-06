BIN=box
OBJ=main.o box.o
CFLAGS=-g

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN)

clean:
	rm *.o $(BIN)
