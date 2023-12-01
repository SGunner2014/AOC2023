#include <stdio.h>

#define INPUT_FILE "input.txt"

int is_num(char chr)
{
    return chr >= '0' && chr <= '9';
}

int main()
{
    FILE *fp;

    fp = fopen(INPUT_FILE, "r");

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int total = 0;
    int i;
    int current_line;

    while ((read = getline(&line, &len, fp)) != -1) {
        current_line = 0;

        // Get the number from the start of the line
        for (i = 0; i < read; i++) {
            if (is_num(line[i])) {
                current_line = line[i] - '0';
                break;
            }
        }

        for (i = read - 1; i >= 0; i--) {
            if (is_num(line[i])) {
                current_line = current_line * 10;
                current_line += line[i] - '0';
                break;
            }
        }

        total += current_line;
    }

    printf("Total: %d\n", total);

    return 0;
}