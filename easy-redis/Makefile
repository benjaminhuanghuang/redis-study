CFLAGS?= -std=c99 -pedantic -O2 -Wall -W
CCOPT= $(CFLAGS)

OBJ = adlist.o ae.o anet.o dict.o redis.o sds.o zmalloc.o lzf_c.o lzf_d.o pqsort.o
CLIOBJ = anet.o sds.o adlist.o redis-cli.o zmalloc.o

SERVER = server.exe
CLIENT = cli.exe

all: server.exe cli.exe

# Deps (use make dep to generate this)
adlist.o: adlist.c adlist.h zmalloc.h
ae.o: ae.c ae.h zmalloc.h
anet.o: anet.c fmacros.h anet.h
dict.o: dict.c fmacros.h dict.h zmalloc.h
lzf_c.o: lzf_c.c lzfP.h
lzf_d.o: lzf_d.c lzfP.h
pqsort.o: pqsort.c
redis-cli.o: redis-cli.c fmacros.h anet.h sds.h adlist.h zmalloc.h
redis.o: redis.c fmacros.h ae.h sds.h anet.h dict.h adlist.h zmalloc.h lzf.h pqsort.h config.h
sds.o: sds.c sds.h zmalloc.h
zmalloc.o: zmalloc.c config.h

$(SERVER): $(OBJ)
	$(CC) -o $(SERVER) $(CCOPT) $(DEBUG) $(OBJ)

$(CLIENT): $(CLIOBJ)
	$(CC) -o $(CLIENT) $(CCOPT) $(DEBUG) $(CLIOBJ)

clean:
	rm -rf $(SERVER) $(CLIENT)  *.o *.exe
