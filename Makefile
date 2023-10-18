C_SOURCES := $(wildcard src/sources/*.c)
H_SOURCES := $(wildcard src/headers/*.h)
MAIN_SOURCE := $(wildcard src/main.c)

PRAGMA = ./doom
CLANG = clang
CLANG_FLAGS = -lglut -lGLU -lGL -lm

run: compile exe

compile:$(MAIN_SOURCE) $(C_SOURCES) $(H_SOURCES)
	$(CLANG) -o $(PRAGMA) $^ $(CLANG_FLAGS) 	

exe: 
	$(PRAGMA)
