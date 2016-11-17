  void compressR_LOLS(const char *fname, int segcount) {
    char ** execpass;
    *(execpass) = "
    pid_t pids[segcount];
    pid_t pid, parent;
    int status; 
    int seg_length = length / segcount;
    int slack = length % segcount;
    int i, position = 0;
    int length = check_file(fname, segcount);
        if (length == -1) return;  
    for (i; i<segcount; i++){
        pid = fork();
        if (pid != 0){
            pids[i] = pid;
            parent = waitpid(cpid, &status);}
        else { 
            execvp(