#include <ctype.h>
#include "compress_LOLS.h"


/**
 * checks to ensure that the file both exists and was not previously compressed.
 * 
 * Returns -1 if the file cannot be compressed (already compressed, bad size to segment ratio
 */
int check_file(char * infile, int segments) {
    if (segments <= 0) {
        printf("Invalid number of segments - cannot be processed\n");
        return -1;
	}
    int fsize = 0;
    FILE * fp;
    FILE * fp2;
    FILE * fp3;
    char * cName = (char *)malloc(strlen(infile) + 20);
    fp = fopen(infile, "rb");
    if (fp != NULL) {
		fseek(fp, 0, SEEK_END);
        fsize = ftell(fp); 
        fclose(fp);
        if (fsize == 0) {
            printf("Empty input file\n");
            free(cName);
            return -1;
        } else if (fsize < segments) {
            printf("Fewer characters than segments - cannot be processed\n");
            free(cName);
            return -1; 
		}
    } else {
        printf("File opening error: %s\n", strerror(errno));
        free(cName);
        return -1;
    }
    
    strcpy(cName, infile);
    *(cName + (strlen(cName) - 4)) = '_';
    strcat(cName, "_LOLS");
    fp2 = fopen(cName, "rb");
    if (fp2 != NULL){
        printf("compressed file already exists\n");
        fclose(fp2);
        free(cName);
        return -1;
   
    }
    int temp = strlen(cName);
    *(cName + temp) = '0';
    *(cName + temp + 1) = '\0';
    fp3 = fopen(cName, "rb");
    if (fp3 != NULL) {
        printf("compressed file already exists\n");
        fclose(fp3);
        free(cName);
        return -1;
    }
    
    free(cName);
    return fsize;    
}

/**
 *
 * Core compression method. If the string is not long enough to be compressed then null is returned.
 * If the string is long enough, then a new malloc()ed string is returned with the compressed data.
 * Returning a new string does not ensure that the string was compressed at all, only that it was long
 * enough to compress. 
 *
 * This compression method shortens runs of the same character. If there is either 1 or 2 of the same
 * character, then no change is made. If more than 2 are found, then it is replaced with the count followed
 * by the character to replace. 
 *
 */
char *_compress_LOLS(char *section, int length) {
    
    // if no modifications are made, return null
    if (length <= 2) return NULL;
    // declare output array, the current 'run' character, count of character, input index and output index
    char *output = malloc(sizeof(char) * length * 1.1);
    char running = section[0], current;
    int count, inI, outI;
    
    // write_output() writes the compressed information to the output string
    void write_output() {
        if (count == 2) {
            // if count == 2, we write the running char once. It will be written again later, effectively no change.
            output[outI++] = running;
        } else if (count > 2) {
            // if we have more than two, we write the count, and set out index to the end of the string
            sprintf(output+outI, "%d\0", count);
            outI = strlen(output);
        }
        // finally, write the running char no matter what, and set the new running char
        output[outI++] = running;
        running = current;
        count = 1;
    }
    // running is set to the first char (or zero-th) so set count to 1 and in index to 1
    for (count=1, inI=1, outI=0; inI<length; inI++) {
        current = section[inI];
        
        if (!isalpha(current)) {
			printf("***Got unexpected character '%c'. Ignoring***\n", current);
			continue;
		}
		
        if (current == running) {
            count++;
        } else {
            // if there was a change in running char, we write the output
            write_output();
        }
    }
    // write remaining output and terminate string
    write_output();
    output[outI++] = '\0';
    return output;
}

/**
 * compress_segment takes a file name, an output file name, a starting pos, and length.
 * 
 * This method compresses a specific segment of a file and writes it to the 
 * specified output file. This is the method called by both thread and process versions
 * of the LOLS compression project.
 */
void compress_segment(char *fname, char *outname, int start, int length) {
    FILE *file = fopen(fname, "rb");
    char *section = malloc(sizeof(char) * length);
    
    if (fseek(file, start, SEEK_SET) == 0) {
        // we successfully moved to our starting position
        fread(section, 1, length, file);
        fclose(file);
        // compress our specific section
        char *new = _compress_LOLS(section, length);
        file = fopen(outname, "wb");
       
        if (new == NULL) {
            // if no changes were made, write the old section
            fwrite(section, 1, length, file);
        } else {
            // otherwise write the new section
            fwrite(new, 1, strlen(new), file);
        }
        fclose(file);
        free(section);
        free(new);
    }
}
