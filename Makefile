default: all

all: compressT_LOLS.o compressR_LOLS.o compressR_worker_LOLS unit_tests.c
	gcc -o unit_tests unit_tests.c compressT_LOLS.o compressR_LOLS.o
	
compressT_LOLS.o: compress_LOLS.o compressT_LOLS.c compressT_LOLS.h
	gcc -o compressT_LOLS.o -c compressT_LOLS.c compress_LOLS.o
	
compressR_LOLS.o: compress_LOLS.o compressR_LOLS.c compressR_LOLS.h compressR_worker_LOLS
	gcc -o compressR_LOLS.o -c compressR_LOLS.c compress_LOLS.o

compressR_worker_LOLS: compressR_worker_LOLS.c compress_LOLS.o
	gcc -o compressR_worker_LOLS compressR_worker_LOLS.c compress_LOLS.o
	
compress_LOLS.o: compress_LOLS.c compress_LOLS.h
	gcc -o compress_LOLS.o -c compress_LOLS.c
	
clean:
	rm compress_LOLS.o compressR_LOLS.o compressT_LOLS.o compressR_worker_LOLS unit_tests
