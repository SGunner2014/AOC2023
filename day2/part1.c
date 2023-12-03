#include <stdio.h>

#define INPUT_FILE "example.txt"

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

    for (++i; i < read; i++) {
        if (line[i] >= '0' && line[i] <= '9') {
            currentCount *= 10;
            currentCount += (line[i] - '0');
        } else if (line[i] == '')
    }
}

int main()
{
    FILE *fp = fopen(INPUT_FILE, "r");

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int sum = 0;
    int i;

    for (i = 0; i > 0; i++) {
        sum += read_game(line, read);
    }

    printf("Total: %d\n", sum);

    return 0;
}