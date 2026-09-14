SOURCE_DIR=src
OBJECT_DIR=obj
INCLUDE_DIR=includes
BINARY_DIR=bin

VERSION := $(shell (git tag --points-at HEAD | grep '[[:digit:]]\+\.[[:digit:]]\+\.[[:digit:]]\+' 2>/dev/null || git rev-parse --short HEAD))
OS := $(shell uname -s | tr '[:upper:]' '[:lower:]')
ARCH := $(shell uname -m)
BIN_NAME := ${BINARY_DIR}/parrot-${OS}-${ARCH}-${VERSION}

OBJECTS=${OBJECT_DIR}/version.o ${OBJECT_DIR}/config.o ${OBJECT_DIR}/main.o

.PHONY: all
all: ${BIN_NAME}

${INCLUDE_DIR}/version_string.h:
	@echo "Generate version_string.h"
	@printf "#ifndef __PARROT_VERSION_STRING_H__\n#define __PARROT_VERSION_STRING_H__\n#define PARROT_VERSION_STRING \"${VERSION}\"\n#endif // __PARROT_VERSION_STRING_H__\n" > $@

${OBJECT_DIR}/%.o: ${SOURCE_DIR}/%.c ${INCLUDE_DIR}/version_string.h
	${CC} ${CFLAGS} -I${INCLUDE_DIR} -c $< -o $@
 
${BIN_NAME}:${OBJECTS}
	${CC} ${CFLAGS} -o $@ $^

.PHONY: clean
clean:
	rm -rf ${OBJECT_DIR}/* ${INCLUDE_DIR}/version_string.h ${BIN_NAME}
