TESTDIR = ../libtest
ERRDIR  = ../liberror
CFLAGS  = -g -Wall -Wextra -pedantic -D_DEFAULT_SOURCE -std=c99
LDLIBS  = -L./ -lutils
LIB     = libutils.a
LIBOBJS = file_iterator.o file_utils.o string_tokenizer.o
BUILD   = $(LIB)

all:	$(BUILD)

clean:
	/bin/rm -f $(BUILD) *.o

$(LIB):	$(LIBOBJS)
	emar rcs $(LIB) $(LIBOBJS)

file_iterator.o:	file_iterator.h
file_utils.o:	file_utils.h
string_tokenizer.o:	string_tokenizer.h
