#include <stdio.h>
#include <string.h>

#define INPUT_FILE "input.txt"

#define MIN_RED 12
#define MIN_GREEN 13
#define MIN_BLUE 14

#define RED 0
#define GREEN 1
#define BLUE 2

int read_game(char *line, ssize_t read)
{
    int gameId = 0;
    int i;
    int ballColour, currentCount;

    // Read the game ID
    for (i = 5; i < read; i++) {
        if (line[i] >= '0' && line[i] <= '9') {
            gameId *= 10;
            gameId += (line[i] - '0');
        } else {
            break;
        }
    }

    currentCount = 0;
    int isValid = 1;
    ++i;

    for (; i < read; i++) {
        // Iterate through the remaining characters.
        if (line[i] >= '0' && line[i] <= '9') {
            currentCount *= 10;
            currentCount += (line[i] - '0');
        } else if (line[i] == ',' || line[i] == ';' || line[i] == '\0' || line[i] == '\n') {
            // Check what the last 3/4/5 chars are
            if (strncmp(&line[i - 3], "red", 3) == 0) {
                if (currentCount > MIN_RED) {
                    isValid = 0;
                    break;
                }
            } else if (strncmp(&line[i - 4], "blue", 4) == 0) {
                if (currentCount > MIN_BLUE) {
                    isValid = 0;
                    break;
                }
            } else if (strncmp(&line[i - 5], "green", 5) == 0) {
                if (currentCount > MIN_GREEN) {
                    isValid = 0;
                    break;
                }
            }
            currentCount = 0;
        }
    }

    return isValid ? gameId : 0;
}

int main()
{
    FILE *fp = fopen(INPUT_FILE, "r");

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int sum = 0;
    
    while ((read = getline(&line, &len, fp)) != -1) {
        sum += read_game(line, read);
    }

    printf("Total: %d\n", sum);

    return 0;
}