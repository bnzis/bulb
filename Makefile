.DEFAULT_GOAL := all
SOURCE = $(wildcard src/modules/*c src/*c)

all: $(SOURCE)
	@ gcc $^ -o bulb 
	@ echo done.
