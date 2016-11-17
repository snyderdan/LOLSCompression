#include "compressT_LOLS.h"

/**
 * method used when spawning each thread
 */
void *compress_segment_thread(void *segment) {
    Segment *s = (Segment *) segment;
    compress_segment(s->in_file, s->out_file, s->start, s->length);
    return NULL;
}

void compressT_LOLS(char *fname, int segcount) {
    int length = check_file(fname, segcount);
    // if check_file returns -1, the file either doesn't exist, length is too short, or file is already compressed
    if (length == -1) return;
    
    // typical segment length
    int seg_length = length / segcount;
    // slack variable for when segments are not all equal length
    int slack = length % segcount;
    int fname_len = strlen(fname);
    int i, tcount, position = 0;
    
    // temp object for current segment
    Segment seg;
    // arrays for threads and their respective segment
    Segment   *segments = malloc(sizeof(Segment) * segcount);
    pthread_t *threads = malloc(sizeof(pthread_t) * segcount);
    // copy file name, append _LOLS to the end and add %d for segment number
    char *outfmt = malloc(fname_len + 10);
    strcpy(outfmt, fname);
    *(outfmt + (strlen(outfmt) - 4)) = '_';
    strcat(outfmt, "_LOLS%d\0");
    
    
    for (i=0; i<segcount; i++) {
        // get segment for this thread
        seg = segments[i];
        // set file name, starting and ending position
        seg.start = position;
        seg.length = seg_length + (slack > 0) ? 1 : 0;
        seg.in_file = fname;
        // allocate output file name, and insert segment number
        seg.out_file = malloc(fname_len + 15);
        sprintf(seg.out_file, outfmt, i);
        if (segcount == 1) {
            // if we are only processing 1 segment, then we remove the "0" at the end of the file name
            seg.out_file[strlen(seg.out_file) - 1] = '\0';
        }
        
        // attempt to create a new thread
        if (pthread_create(&threads[i], NULL, &compress_segment_thread, &segments[i])) {
            // if it fails, print error, kill all running threads, and return
            printf("Error creating threads for compression, output is not valid.\n");
            for (i=0; i<tcount; i++) {
                pthread_cancel(threads[i]);
            }
            break;
        } else {
            // otherwise, we move our position, increment thread count, and decrease slack
            position += seg.length;
            tcount++;
            slack--;
        }
    }
    
    if (tcount == segcount) {
        for (i=0; i<tcount; i++) {
            pthread_join(threads[i], NULL);
        }
    }
    
    for (i=0; i<tcount; i++) {
        free(segments[i].out_file);
    }
    
    free(segments);
    free(threads);
    free(outfmt);
    
}
