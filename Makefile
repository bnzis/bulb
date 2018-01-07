.DEFAULT_GOAL := all
all: bulb.c
	@ gcc $^ -o bulb
	@ echo done.
