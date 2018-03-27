CC = g++
CFLAGS = -I. -std=c++11

#lexus: lexus.o lextoken.o
#	$(CC) $(CFLAGS) lexus.o lextoken.o -o lexus -g
#lexus.o: lexus.cc lextoken.h
#	$(CC) $(CFLAGS) lexus.cc -c -g
#lextoken.o: lextoken.cc lextoken.h
#	$(CC) lextoken.cc -c -g

lexus: lexus_full.cc
	$(CC) $(CFLAGS) lexus_full.cc -o lexus