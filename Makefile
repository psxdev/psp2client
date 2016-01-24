

  CC = gcc 



  ifeq "x$(MSYSTEM)" "x"
   LIBS = -lpthread
  else
   LIBS = -lwsock32 -lpthreadGC2
   FIX  = -mno-ms-bitfields 

  endif
  
  CFLAGS = -std=gnu99 -Wall -pedantic -I/usr/include -I/usr/local/include $(FIX)
  
  ifeq "x$(PREFIX)" "x"
   PREFIX = $(VITASDK)
  endif

  all: bin/psp2client

  clean:
	rm -f obj/*.o bin/*client*

  install: bin/psp2client
	strip bin/*client*
	cp bin/*client* $(PREFIX)/bin

 ####################
 ## CLIENT MODULES ##
 ####################

  OFILES += obj/network.o
  obj/network.o: src/network.c src/network.h
	@mkdir -p obj
	$(CC) $(CFLAGS) -c src/network.c -o obj/network.o

  OFILES += obj/psp2link.o
  obj/psp2link.o: src/psp2link.c src/psp2link.h
	@mkdir -p obj
	$(CC) $(CFLAGS) -c src/psp2link.c -o obj/psp2link.o

  OFILES += obj/utility.o
  obj/utility.o: src/utility.c src/utility.h
	@mkdir -p obj
	$(CC) $(CFLAGS) -c src/utility.c -o obj/utility.o

 #####################
 ## CLIENT PROGRAMS ##
 #####################


  bin/psp2client: $(OFILES) src/psp2client.c
	@mkdir -p bin
	$(CC) $(CFLAGS) $(OFILES) src/psp2client.c -o bin/psp2client $(LIBS)
