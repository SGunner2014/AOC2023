#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "example.txt"

typedef struct Pos {
    int x;
    int y;
} Pos;

int detect_symbols(char *data, const Pos **positions, const int max_x, const int max_y)
{
    int count = 0;
    int i;

    while (data[i] != '\0') {
        if ()
    }

    return count;
}

int main()
{
    FILE *fp = fopen(INPUT_FILE, "r");

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    char *data = (char*) malloc(sizeof(char) * 142 * 140);

    int max_x, max_y;
    max_x = max_y = 0;
    
    // Read the entire file at once ?
    while ((read = getline(&line, &len, fp)) != -1) {
        if (max_x == 0)
            max_x = read;
        ++max_y;

        strncat(data, line, read); // Get rid of the '\n' at the end
    }

    // First, make a pass over the data to detect all the symbol locations
    Pos *symbols = (Pos*) malloc(sizeof(Pos) * 200);
    int symbolCount = detect_symbols(data, &symbols, max_x, max_y);

    free(symbols);
    free(data);

    return 0;
}