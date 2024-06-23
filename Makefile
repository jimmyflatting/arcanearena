CC = clang
STD = -std=c11
CFLAGS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
LDFLAGS = -Llib/raylib/src -lraylib -lcjson -ltmx -lxml2 -lz
HOMEBREW = -I/opt/homebrew/include -L/opt/homebrew/lib
EXTRAFLAGS = -Wl,-rpath,@executable_path

INCLUDES = -Ilib/raylib/src -Isrc
SRC = $(wildcard src/*.c) $(wildcard src/**/*.c) 

all: libs game

libs:
	git submodule update --init --recursive
	cd lib/raylib/src && make PLATFORM=PLATFORM_DESKTOP

game:
	$(CC) -o build/app $(SRC) $(STD) $(CFLAGS) $(LDFLAGS) $(HOMEBREW) $(INCLUDES) $(EXTRAFLAGS)
	./build/app

debug:
	$(CC) -o build/app $(SRC) $(STD) $(CFLAGS) $(LDFLAGS) $(HOMEBREW) $(INCLUDES) $(EXTRAFLAGS) -g
	lldb ./build/app

clean:
	rm -rf build/*