#include "compress_LOLS.h"
#include "compressR_LOLS.h"



int main(int argc, char *argv[]){
	compress_segment(argv[3], argv[4], atoi(argv[1]), atoi(argv[2]));
	return 0;
}