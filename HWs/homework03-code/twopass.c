// twopass.c: demonstrates two-pass input strategy to read entire
// contents of a file of numbers

#include <stdio.h>     // for printf() / fscanf() / etc.
#include <stdlib.h>    // for malloc() / free()

// Reads all doubles in file named 'filename'; sets 'set_len' to the
// number of doubles and returns an array of doubles read from the
// file.  Uses a two-pass input strategy.
double *read_all_doubles(const char *filename, int *set_len) {
    FILE *fin = fopen(filename, "r");    // open the file
    if (fin == NULL) {                   // check file opened successfully
        return NULL;
    }

    double tmp;                                // temporary space to read one double
    int count = 0;                             // count number of doubles in file
    while (1) {                                // first input pass: infinite loop to count doubles
        int ret = fscanf(fin, "%lf", &tmp);    // try to read a double
        if (ret == EOF) {    // check if End Of File reached, break from loop if so
            break;
        }
        count++;    // successfully read a double, increment count
    }

    double *data = malloc(count * sizeof(double));    // allocate space for the array after counting

    rewind(fin);                         // rewind to beginning of file
    for (int i = 0; i < count; i++) {    // second input pass: count of doubles known
        fscanf(fin, "%lf", &data[i]);    // read doubles into malloc()'d array
    }

    fclose(fin);         // close the file as all reading is done now
    *set_len = count;    // deref set length so caller knows size of array
    return data;         // return array of doubles
}

// Main function to demonstrate two-pass input function
int main(int argc, char *argv[]) {
    if (argc < 2) {    // require a command line argument: file name
        // First argument on command line is name of program
        printf("Usage: %s <data-file>\n", argv[0]);
        return 1;
    }

    const char *fname = argv[1];    // Second argument on command line is file name
    printf("Reading double values from file '%s'\n", fname);

    int len = -1;    // will be set by read_all_doubles()

    // call two-pass input function to read contents, set length
    double *nums = read_all_doubles(fname, &len);
    if (nums == NULL) {
        printf("ERROR: couldn't open file '%s'\n", fname);
    }
    for (int i = 0; i < len; i++) {
        printf("[%d] : %8.2f == %10.4e\n", i, nums[i], nums[i]);
    }

    free(nums);
    return 0;
}
