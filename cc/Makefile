CC=gcc
CFLAGS=
LDFLAGS=-O2 -g

LEX=flex

YACC=yacc

DISPAST_OBJS=dispast.o gram.o main.o scan.o 

all: c2bf-dispast

c2bf-dispast: $(DISPAST_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(DISPAST_OBJS) -o $@

.PREFIXES: .y .l .c .o

.y.c:
	$(YACC) -dv $<
	mv y.tab.c $@

.l.c:
	$(LEX) -t $< > $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(DISPAST_OBJS) gram.c y.tab.h y.output scan.c c2bf-dispast
