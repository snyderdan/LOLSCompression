#include "compress_LOLS.h"
#include "compressR_LOLS.h"

 
int compressR_LOLS(char *fname, int segcount) {
	int length = check_file(fname, segcount);
    if (length == -1) return 0;  
    //make array to pass in execvp
    char * execpass[6];
    char passLength[10], passStart[10], exec[22] = "compressR_worker_LOLS\0";
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
    int cur_len, seg_length = length / segcount;
    int slack = length % segcount;
    int i, position = 0;
    char * out_file;
    // copy file name, append _LOLS to the end and add %d for segment number
	char *outfmt = malloc(strlen(fname) + 10);
	strcpy(outfmt, fname);
   	*(outfmt + (strlen(outfmt) - 4)) = '_';
    strcat(outfmt, "_LOLS%d\0");
    //make processes and make children run worker file
    for (i=0; i<segcount; i++){
		cur_len = seg_length + ((slack > 0) ? 1 : 0);
        sprintf(passStart, "%d", position);
        sprintf(passLength, "%d", cur_len);
        position += cur_len;
        slack--;
        out_file = malloc(strlen(fname) + 15);
        sprintf(out_file, outfmt, i);
        execpass[4] = out_file;
        if (segcount == 1) {
        	// if we are only processing 1 segment, then we remove the "0" at the end of the file name
        	out_file[strlen(out_file) - 1] = '\0';
		}
        pid = fork();
        if (pid != 0){
			//if the parent then put the PID in array and wait
            pids[i] = pid;
        } else {
        	//if child then get metadata and pass to execvp
			printf("%d\n", execv(execpass[0], execpass));
		}
		free(out_file);
	}
	wait(NULL);
	free(outfmt);
	return length;
}
