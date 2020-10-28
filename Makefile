tx:
	gcc -o tx.out sender.c

rtx:
	./tx.out 127.0.0.1 1776
	
s:
	gcc -o server.out server.c

rs:
	./server.out 1776

rx:
	gcc -o rx.out receiver.c
rrx:
	./rx.out 1777

hd:
	gcc -o hd.out hd.c

fin:
	gcc -o fin.out fin.c
	./fin.out
	
all: tx s rx

clean:
	rm *.out
