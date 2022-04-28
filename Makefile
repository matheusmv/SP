BIN = bin/out

export VALGRINDCMD = valgrind -s\
					--leak-check=full\
					--show-leak-kinds=all\
					--track-origins=yes\
					--log-file=/tmp/valgrind-%p.log

export CC       = gcc
export CFLAGS   = -pedantic -Wall -Werror -Wextra -std=c11
export LDFLAGS  = -L
export LDLIBS   = -l
export CMDFLAGS = -D

export SOURCES = $(wildcard src/**/*.c src/*.c)

export TEST_SOURCES = $(wildcard tests/*_tests.c)
export TESTS        = $(patsubst %.c,%,$(TEST_SOURCES))


$(BIN) : build
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(SOURCES)

$(TESTS) : $(TEST_SOURCES) $(SOURCES)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^


all: release


build:
	@mkdir -p bin


release: CFLAGS +=-O2
release: $(BIN)


debug: CFLAGS +=-O0 -ggdb3
debug: $(BIN)


.PHONY: tests
tests: CFLAGS +=-O0 -ggdb3
tests:
	./tests/runtests.sh


valgrind: debug
valgrind:
	$(VALGRINDCMD) $(BIN)


clean:
	rm -rf $(BIN) $(TESTS)
	rm -rf tests/tests.log


BADFUNCS = '[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
check:
	@echo Files with potentially dangerous functions
	@egrep $(BADFUNCS) $(SOURCES) || true
