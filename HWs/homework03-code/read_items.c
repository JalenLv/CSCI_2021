#include <stdio.h>
#include <string.h>

typedef struct {
    char name[32];
    int count;
} item_t;

int main(int argc, char *argv[]) {
    // Expect name of file to read as command line argument
    if (argc < 2) {
        printf("Usage: %s <file_name>\n", argv[0]);
        return 1;
    }

    char *file_name = argv[1];
    FILE *f = fopen(file_name, "r");    // Open binary file for reading
    if (f == NULL) {
        printf("Failed to open binary file\n");
        return 1;
    }

    item_t current;
    int name_len;
    while (1) {
        fread(&name_len, sizeof(int), 1, f);    // Read in length of item name
        if (feof(f)) {                          // If we hit end of the file, exit loop
            break;
        }
        fread(current.name, sizeof(char), name_len, f);    // Read item name
        current.name[name_len] = '\0';                     // Properly null terminate string
        fread(&current.count, sizeof(int), 1, f);          // Read item count

        printf("%d of Item '%s'\n", current.count, current.name);
    }

    fclose(f);
    return 0;
}
