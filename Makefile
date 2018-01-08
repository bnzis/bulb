.DEFAULT_GOAL := all
all: src/*.c
	@ gcc $^ -o bulb -Iinclude/
	@ echo done.
