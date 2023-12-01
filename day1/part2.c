#include <stdio.h>

#define INPUT_FILE "example.txt"

int main()
{
    FILE *fp;

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int total = 0;
    int i;
    int current_line;

    while ((read = getline(&line, &len, fp)) != -1) {
        //
    }

    printf("Total: %d\n", total);

    return 0;
}
