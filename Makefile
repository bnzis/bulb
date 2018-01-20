.DEFAULT_GOAL := all
SOURCE = $(wildcard src/*c)

all: $(SOURCE)
	@ gcc $^ -o bulb -I./src/modules
	@ echo done.
