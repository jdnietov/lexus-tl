CC = g++
CFLAGS = -I. -std=c++11

#lexus: lexus_full.cc
#	$(CC) $(CFLAGS) lexus_full.cc -o lexus
synth: synth_full.cc
	$(CC) $(CFLAGS) synth_full.cc -o synth