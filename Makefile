CC = g++
INCL = ./include
SRC = ./src

CFLAGS  = -std=c++11 -I $(INCL)

default: TheGame2048

TheGame2048:  Field.o Game.o GameView.o main.o
	$(CC) $(CFLAGS) -o TheGame2048 Field.o Game.o GameView.o main.o

Field.o:  $(SRC)/Field.cpp 
	$(CC) $(CFLAGS) -c $(SRC)/Field.cpp

Game.o:  $(SRC)/Game.cpp 
	$(CC) $(CFLAGS) -c $(SRC)/Game.cpp

GameView.o:  $(SRC)/GameView.cpp 
	$(CC) $(CFLAGS) -c $(SRC)/GameView.cpp

main.o:  main.cpp
	$(CC) $(CFLAGS) -c main.cpp

clean: 
	$(RM) TheGame2048 *.o *~

