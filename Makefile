.DEFAULT_GOAL := all
SOURCE = $(wildcard src/modules/*c src/*c)

all: $(SOURCE)
	@ gcc $^ -lreadline -o bulb 
	@ echo done.
