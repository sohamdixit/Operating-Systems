CC=gcc

all: kill ls shell wc ps rm rmv2 ReaderWriter ProdCon

kill: kill.c
	$(CC) kill.c -o kill

ls: ls.c
	$(CC) ls.c -o ls

shell: shell.c
	$(CC) shell.c -o shell -lreadline

wc: wc.c
	$(CC) wc.c -o wc

ps: ps.c
	$(CC) ps.c -o ps

rm: rm.c
	$(CC) rm.c -o rm

rmv2: rmv2.c
	$(CC) rmv2.c -o rmv2

ReaderWriter: ReaderWriter.c
	$(CC) ReaderWriter.c -o ReaderWriter -lpthread

ProdCon: ProdCon.c
	$(CC) ProdCon.c -o ProdCon -lpthread

clean:
	rm -f ls ps rm rmv2 ReaderWriter shell wc ProdCon kill
