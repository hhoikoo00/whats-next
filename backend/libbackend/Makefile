TESTDIR = ../libtest
ERRDIR  = ../liberror
UTILDIR = ../libutils
CFLAGS  = -c -g -Wall -Wextra -pedantic -D_DEFAULT_SOURCE -std=c99 -I$(TESTDIR) -I$(ERRDIR) -I$(UTILDIR)
LDLIBS  = -L$(TESTDIR) -L$(ERRDIR) -L$(UTILDIR) -ltest -lerror -lutils
LIB     = libbackend.a
LIBOBJS = csv_reader.o guess.o
BUILD   = $(LIB)

all:	$(BUILD)

clean:
	/bin/rm -f $(BUILD) *.o $(LIB)

$(LIB): $(LIBOBJS)
	emar rcs $(LIB) $(LIBOBJS)

csv_reader.o:	csv_reader.h
guess.o:	guess.h
