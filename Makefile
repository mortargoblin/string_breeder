CC    = gcc
OUT   = string_breeder
FLAGS = -Wall -Wextra

$(OUT): main.c
	$(CC) -o $(OUT) main.c $(FLAGS)
