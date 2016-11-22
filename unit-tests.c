#include "compress_LOLS.h"
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
	printf("\nTesting case '%s'\n", tstname);
	clock_t begin = clock();
	int bytes = funct();
	clock_t end = clock();
	double time_spent = (double)(end - begin) / (CLOCKS_PER_SEC / 1000);
	double kbytes_sec = bytes / time_spent;
	printf("Test took: %.05lf ms to run, at %.02lf kB/sec\n", tstname, time_spent, kbytes_sec);
}


char *test0str = "File DNE";
int test0() {
	return compressT_LOLS("test0.txt", 1);
}

char *test00str = "File is empty";
int test00(){
	return compressT_LOLS("test.txt", 1);
}

char *test1str = "File already compressed";
int test1() {
	return compressT_LOLS("test1.txt", 2);
}

char *test2str = "Segments > length";
int test2() {
	return compressT_LOLS("test2.txt", 9);
}

char *test3str = "Segments = length";
int test3() {
	return compressT_LOLS("test3.txt", 8);
}

char *test4str = "Simple standard test";
int test4() {
	return compressT_LOLS("test4.txt", 2);
}

char *test5str = "Non-alphabetic character and whitespace";
int test5() {
	return compressT_LOLS("test5.txt", 3);
}

char *test6str = "Even splits";
int test6() {
	return compressT_LOLS("test6.txt", 12);
}

char *test7str = "Odd splits";
int test7() {
	return compressT_LOLS("test7.txt", 11);
}

char *test8str = "Thread time consistency";
int test8() {
	return compressT_LOLS("test8.txt", 4);
}


char *test9str = "Process time consistency";
int test9() {
	return compressR_LOLS("test8.txt", 4);
}

char * test10str = "Scaling test: One process";
int test10() { 
	return compressR_LOLS("test6.txt", 1);
	}

char * test11str = "Scaling test: One thread";
int test11() { 
	return compressT_LOLS("test6.txt", 1);
	}

char * test12str = "Scaling test: 5 processes";
int test12() { 
	return compressR_LOLS("test6.txt", 5);
}
char * test13str = "Scaling test: 5 threads";
int test13() { 
	return compressT_LOLS("test6.txt", 5);
}
char * test14str = "Scaling test: 25 processes";
int test14() { 
	return compressR_LOLS("test6.txt", 25);
}	
char * test15str = "Scaling test: 25 threads";
int test15() { 
	return compressT_LOLS("test6.txt", 25);
}	
char * test16str = "Scaling test: One process";
int test16() { 
	return compressR_LOLS("test9.txt", 1);
}	
char * test17str = "Scaling test: One thread";
int test17() { 
	return compressT_LOLS("test9.txt", 1);
}
char * test18str = "Scaling test: 5 processes";
int test18() { 
	return compressR_LOLS("test9.txt", 5);
}
char * test19str = "Scaling test: 5 threads";
int test19() { 
	return compressT_LOLS("test9.txt", 5);
}
char * test20str = "Scaling test: 25 processes";
int test20() { 
	return compressR_LOLS("test9.txt", 25);
}	
char * test21str = "Scaling test: 25 threads";
int test21() { 
	return compressT_LOLS("test9.txt", 25);
}
char * test22str = "Scaling test processes: small file";
int test22() { 
	return compressR_LOLS("test11.txt", 5);
}	
char * test23str = "Scaling test threads: small file";
int test23() { 
	return compressT_LOLS("test11.txt", 5);
}	
char * test24str = "Scaling test processes: medium file";
int test24() { 
	return compressR_LOLS("test8.txt", 5);
}	
char * test25str = "Scaling test threads: medium file";
int test25() { 
	return compressT_LOLS("test8.txt", 5);
}
char * test26str = "Scaling test processes: large file";
int test26() { 
	return compressR_LOLS("test10.txt", 5);
}	
char * test27str = "Scaling test threads: large file";
int test27() { 
	return compressT_LOLS("test10.txt", 5);
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
	time_test(test00str, test00);
	time_test(test1str, test1);
	time_test(test2str, test2);
	time_test(test3str, test3);
	verify_pause();
	system("rm test3_txt_LOLS*");
	time_test(test4str, test4);
	verify_pause();
	system("rm test4_txt_LOLS*");
	time_test(test5str, test5);
	verify_pause();
	system("rm test5_txt_LOLS*");
	time_test(test6str, test6);
	verify_pause();
	system("rm test6_txt_LOLS*");
	time_test(test7str, test7);
	verify_pause();
	system("rm test7_txt_LOLS*");
	time_test(test8str, test8);
	system("rm test8_txt_LOLS*");
	time_test(test8str, test8);
	system("rm test8_txt_LOLS*");
	time_test(test8str, test8);
	system("rm test8_txt_LOLS*");
	time_test(test8str, test8);
	system("rm test8_txt_LOLS*");
	time_test(test8str, test8);
	system("rm test8_txt_LOLS*");
	time_test(test9str, test9);
	system("rm test8_txt_LOLS*");
	time_test(test9str, test9);
	system("rm test8_txt_LOLS*");
	time_test(test9str, test9);
	system("rm test8_txt_LOLS*");
	time_test(test9str, test9);
	system("rm test8_txt_LOLS*");
	time_test(test9str, test9);
	system("rm test8_txt_LOLS*");
	verify_pause();

	
	// The following tests are specific to testing threads VS. processes
	// This includes scaling with threads, scaling with file sizes, and the
	// contents of the files
	printf("The following tests will check compression speeds on fully compressible file\n");
	time_test(test10str, test10);
	system("rm test6_txt_LOLS*");
	time_test(test11str, test11);
	system("rm test6_txt_LOLS*");
	time_test(test12str, test12);
	system("rm test6_txt_LOLS*");	
	time_test(test13str, test13);
	system("rm test6_txt_LOLS*");	
	time_test(test14str, test14);
	system("rm test6_txt_LOLS*");	
	time_test(test15str, test15);
	system("rm test6_txt_LOLS*");
	verify_pause();
	
	printf("The following tests will check compression speeds on non compressible files\n");
	time_test(test16str, test16);
	system("rm test9_txt_LOLS*");
	time_test(test17str, test17);
	system("rm test9_txt_LOLS*");	
	time_test(test18str, test18);
	system("rm test9_txt_LOLS*");	
	time_test(test19str, test19);
	system("rm test9_txt_LOLS*");	
	time_test(test20str, test20);
	system("rm test9_txt_LOLS*");	
	verify_pause();

	printf("The following test will check process vs thread compression speed for small, medium, and large input files\n");
	time_test(test22str, test22);
	system("rm test11_txt_LOLS*");
	time_test(test23str, test23);
	system("rm test11_txt_LOLS*");
	time_test(test24str, test24);
	system("rm test8_txt_LOLS*");
	time_test(test25str, test25);
	system("rm test8_txt_LOLS*");
	time_test(test26str, test26);
	system("rm test10_txt_LOLS*");
	time_test(test27str, test27);
	system("rm test10_txt_LOLS*");
	verify_pause();

	return 0;
}



