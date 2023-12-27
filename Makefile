#CC=clang
CC=gcc
CFLAGS=-g -Wall -lpthread
LFLAGS=-lpthread
OBJS=shared_memory.o
EXE=write_shmem.elf read_shmem.elf #destroy_shmem.elf

all: $(EXE)

%.elf: %.o $(OBJS) 
	$(CC) $(CFLAGS) $< $(OBJS) $(LFLAGS) -o $@

%.o: %.c %.h 
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.dSYM *.o $(EXE)