CC	= gcc
CFLAGS	= -Wall -O2
LIBS	=
OBJS	= league.o filein.o sort.o list.o


all: league

league: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

.c.o:


clean:
	rm -f $(OBJS)
