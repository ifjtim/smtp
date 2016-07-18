CC=g++
CFLAGS=-std=c++11 -static-libstdc++

all:	smtpklient

smtpklient.o: smtpklient.cpp
	$(CC) $(CFLAGS) -c smtpklient.cpp -o smtpklient.o



client: smtpklient.o			
	$(CC) $(CFLAGS) smtpklient.o -o smtpklient

	
clean:
	rm -f *.o  *.zip 
pack:
	zip xkonde03.tar.gz *.cpp *.h Makefile *pdf
