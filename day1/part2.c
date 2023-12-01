#include <stdio.h>
#include <string.h>

#define INPUT_FILE "input.txt"

/**
 * Gets the current number, if it is one
 * Otherwise, returns 0 as this will never be used.
*/
int get_num(char *line, ssize_t read, int i)
{
    // Check for easiest possibility - it's a single digit
    char chr = line[i];
    if (chr >= '0' && chr <= '9') {
        return chr - '0';
    }

    // Check for next possibility - it's written as a word
    // Unfortunately I can't think of a nicer way to write this :(
    char *part;

    // Check we've enough chars
    if (read >= 3 && i >= 2) {
        part = &line[i - 2];
        if (strncmp(part, "one", 3) == 0) {
            return 1;
        } else if (strncmp(part, "two", 3) == 0) {
            return 2;
        } else if (strncmp(part, "six", 3) == 0) {
            return 6;
        }
    }

    // Check we've enough chars
    if (read >= 4 && i >= 3) {
        part = &line[i - 3];
        if (strncmp(part, "four", 4) == 0) {
            return 4;
        } else if (strncmp(part, "five", 4) == 0) {
            return 5;
        } else if (strncmp(part, "nine", 4) == 0) {
            return 9;
        }
    }

    // Check we've enough chars
    if (read >= 5 && i >= 4) {
        part = &line[i - 4];
        if (strncmp(part, "three", 5) == 0) {
            return 3;
        } else if (strncmp(part, "seven", 5) == 0) {
            return 7;
        } else if (strncmp(part, "eight", 5) == 0) {
            return 8;
        }
    }

    return 0;
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
    int current_num;

    while ((read = getline(&line, &len, fp)) != -1) {
        current_line = 0;

        // Get the number from the start of the line, this could also be
        // as a word
        for (i = 0; i < read; i++) {
            if ((current_num = get_num(line, read, i))) {
                current_line = current_num;
                break;
            }
        }

        for (i = read - 1; i >= 0; i--) {
            if ((current_num = get_num(line, read, i))) {
                current_line *= 10;
                current_line += current_num;
                break;
            }
        }

        total += current_line;
    }

    printf("Total: %d\n", total);

    return 0;
}
