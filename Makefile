LIBRARY  = libshmem.a
HEADERS  = shmem.h 
SOURCES  = shmem.c 
OBJECTS  = $(SOURCES:.c=.o)
TESTS    = test_start.c test_etext.c test_sheap.c
TESTS   += barrier_performance.c put_performance.c get_performance.c
BINARIES = $(TESTS:.c=.x)
MACCRAP  = $(BINARIES:.x=.x.dSYM)

CC      = mpicc
CFLAGS  = -g -O3 -std=c99 -Wall -I. #-DSHMEM_DEBUG=9
#CFLAGS  = -g -O0 -std=c99 -Wall -I. -DSHMEM_DEBUG=9
LDFLAGS = $(CFLAGS) $(LIBRARY)

all: $(LIBRARY) $(BINARIES)

$(LIBRARY): $(OBJECTS) 
	$(AR) $(ARFLAGS) $(LIBRARY) $(OBJECTS)

# Makefile dependency ensures code is recompiled when flags change
%.x: %.c $(LIBRARY) shmem.h Makefile
	$(CC) $(CFLAGS)    $< $(LDFLAGS) -o $@

# Makefile dependency ensures code is recompiled when flags change
%.o: %.c %.h Makefile
	$(CC) $(CFLAGS) -c $<            -o $@

clean:
	-rm -f  $(OBJECTS)
	-rm -f  $(BINARIES)
	-rm -fr $(MACCRAP)

realclean: clean
	-rm -f  $(LIBRARY)

