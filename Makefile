C_SOURCES := $(wildcard src/sources/*.c)
H_SOURCES := $(wildcard src/headers/*.h)
MAIN_SOURCE := $(wildcard src/main.c)

PRAGMA = ./doom
CC_FLAGS = -lglut -lGLU -lGL -lm -Wall -Wextra -pedantic 

run: compile exe

build: compile

compile:$(MAIN_SOURCE) $(C_SOURCES) $(H_SOURCES)
	$(CC) -o $(PRAGMA) $^ $(CC_FLAGS)

exe: 
	$(PRAGMA)
