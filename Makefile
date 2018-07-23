# This file is part of the Bulb Interpreter and is released under the terms
# of the MIT/Expat License - see LICENSE.

include config.mk

SRC = src/core.c \
      src/gc.c \
      src/parser.c \
      src/xxhash.c \
      src/hashmap.c \
      src/environment.c \
      src/evaluator.c \
      src/std.c \

OBJ = $(SRC:.c=.o)

all: bulb libbulb.a

.c.o:
	@echo CC $< 
	@${CC} -c ${CFLAGS} ${INCS} $<
	@mv *.o src/
	
clean: 
	@echo cleaning
	@rm -f bulb ${OBJ} src/bulb.o libbulb.a bulb-${VERSION}${OS}.tar.gz

dist: clean
	@echo creating dist tarball
	@mkdir -p bulb-${VERSION}
	@cp -R include/ LICENSE Makefile README.md ${SRC} bulb-${VERSION}
	@tar -cf bulb-${VERSION}.tar bulb-${VERSION}
	@gzip bulb-${VERSION}.tar
	@rm -rf bulb-${VERSION}

bulb: src/bulb.o ${OBJ} 
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} src/bulb.o ${LDFLAGS}

libbulb.a: ${OBJ}
	@echo ar rcs $@
	@ar rcs $@ ${OBJ}

install: all
	@echo installing executable file to ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f bulb ${DESTDIR}${PREFIX}/bin/bulb

uninstall: 
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/bulb
	
.PHONY: all clean dist install uninstall
