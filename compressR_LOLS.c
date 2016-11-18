#include "compress_LOLS.h"
#include "compressR_LOLS.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 
void compressR_LOLS(const char *fname, int segcount) {
    //make array to pass in execvp
    char * execpass[6];
    char passLength[10], passStart[10], exec[20];
    exec = "compressR_worker_LOLS\0";
    execpass[0] = exec;
    execpass[1] = passStart;
    execpass[2] = passLength;
    execpass[3] = fname;
    execpass[5] = NULL;
    //make array of PIDs to wait on
    pid_t pids[segcount];
    pid_t pid, parent;
    // variables for each process
    int status; 
    int seg_length = length / segcount;
    int slack = length % segcount;
    int i, position = 0;
    char * out_file;
    execpass[4] = out_file;
    // copy file name, append _LOLS to the end and add %d for segment number
	char *outfmt = malloc(fname_len + 10);
	int fname_len = strlen(fname);
	strcpy(outfmt, fname);
   	*(outfmt + (strlen(outfmt) - 4)) = '_';
    strcat(outfmt, "_LOLS%d\0");
    int length = check_file(fname, segcount);
        if (length == -1) return;  
    //make processes and make children run worker file
    for (i; i<segcount; i++){
        passStart = itoa(position);
        passLength = itoa(length + (slack > 0) ? 1 : 0);
        slack--;
        out_file = malloc(fname_len + 15);
        sprintf(out_file, outfmt, i);
        if (segcount == 1) {
        	// if we are only processing 1 segment, then we remove the "0" at the end of the file name
        	out_file[strlen(out_file) - 1] = '\0';}
    //if the parent then put the PID in array and wait
        pid = fork();
        if (pid != 0){
            pids[i] = pid;
            parent = waitpid(pid, &status);}
        else { 
        	//if child then get metadata and pass to execvp
			execvp("compressR_worker_LOLS", execpass);
			}	
		free(out_file);
		} free(out_fmt);
}
