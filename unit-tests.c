#include "compressT_LOLS.h"
#include "compressR_LOLS.h"

/*
 * -Test file not there
 * -compressed files already there
 * -more splits than bytes
 * -even number of splits
 * -odd number of splits
 * -non-alpha characters
 * -scaling with more splits
 * -scaling with file size
 * -scaling with varying mixtures of characters
 */

void time_test(char *tstname, int (*funct)()) {
	clock_t begin = clock();
	int bytes = funct();
	clock_t end = clock();
	double time_spent = (double)(end - begin) / (CLOCKS_PER_SEC / 1000);
	double kbytes_sec = bytes / time_spent;
	printf("'%50s' test took: %.05lf ms to run, at %.02lf kB/sec\n", tstname, time_spent, kbytes_sec);
}


char *test0str = "File DNE";
int test0() {
	compressT_LOLS("test0.txt", 1);
	return compressR_LOLS("test0.txt", 1);
}

char *test1str = "File already compressed";
int test1() {
	compressT_LOLS("test1.txt", 2);
	return compressR_LOLS("test1.txt", 2);
}

char *test2str = "Segments > length";
int test2() {
	return compressT_LOLS("test1.txt", 9);
}

char *test3str = "Segments = length";
int test3() {
	return compressT_LOLS("test1.txt", 8);
}

char *test4str = "Simple standard test";
int test4() {
	return compressT_LOLS("test1.txt", 2);
}

char *test5str = "Non-alphabetic character";
int test5() {
	int b = compressT_LOLS("test5.txt", 3);
	return b;
}

char *test6str = "Even splits";
int test6() {
	int b = compressT_LOLS("test6.txt", 12);
	system("rm test6_txt_LOLS*");
	return b;
}

char *test7str = "Odd splits";
int test7() {
	int b = compressT_LOLS("test6.txt", 11);
	system("rm test6_txt_LOLS*");
	return b;
}


void verify_pause() {
	printf("Verify output. Press enter to continue...");
	getchar();
}

int main() {
	// The following tests are testing features independant of threading or processing
	// these include testing split boundaries, whether the file is already compressed,
	// non-alphabetic characters, etc. and therefore these tests are only run using
	// compressT_LOLS, which was an arbitrary decision.
	time_test(test0str, test0);
	time_test(test1str, test1);
	system("rm test1_txt_LOLS*");
	time_test(test2str, test2);
	time_test(test3str, test3);
	verify_pause();
	system("rm test1_txt_LOLS*");
	time_test(test4str, test4);
	verify_pause();
	time_test(test5str, test5);
	verify_pause();
	system("rm test5_txt_LOLS*");
	time_test(test6str, test6);
	verify_pause();
	system("rm test6_txt_LOLS*");
	time_test(test7str, test7);
	verify_pause();
	system("rm test6_txt_LOLS*");
	// The following tests are specific to testing threads VS. processes
	// This includes scaling with threads, scaling with file sizes, and the
	// contents of the files
	
	return 0;
}