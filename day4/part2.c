#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE "input.txt"

#define CARD_STATE 0
#define WINNING_STATE 1
#define ENTRIES_STATE 2

void readCard(char *line, ssize_t read, int lineNo, int **repeats)
{
    // Read the card in and decide how many winning numbers we have

    int winningNums[10];
    int i, j, k;

    for (i = 0; i < 10; i++)
        winningNums[i] = 0;

    int currentState = CARD_STATE;
    int currentNum = 0;
    j = k = 0;
    int total = 0;

    for (i = 0; i < read; i++) {
        if (currentState == CARD_STATE && line[i] == ':') {
            currentState = WINNING_STATE;
        } else if (currentState == WINNING_STATE && line[i] >= '0' && line[i] <= '9') {
            currentNum *= 10;
            currentNum += (line[i] - '0');
        } else if (currentState == WINNING_STATE && line[i] == ' ' && currentNum != 0) {
            winningNums[j] = currentNum;
            currentNum = 0;
            j++;
        } else if (currentState == WINNING_STATE && line[i] == '|') {
            currentState = ENTRIES_STATE;
        } else if (currentState == ENTRIES_STATE && line[i] >= '0' && line[i] <= '9') {
            currentNum *= 10;
            currentNum += (line[i] - '0');
        } else if (currentState == ENTRIES_STATE && (line[i] == ' ' || line[i] == '\n' || line[i] == '\0')) {
            for (k = 0; k < j; k++) {
                if (winningNums[k] == currentNum) {
                    total++;
                    break;
                }
            }
            currentNum = 0;
        }
    }

    // Now, add one to the following n repeats (where n = total)
    for (i = 0; i < total; i++) {
        (*repeats)[lineNo + i + 1]++;
    }
}

int main()
{
    FILE *fp = fopen(INPUT_FILE, "r");

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int lineNo = 0;
    int *repeats = malloc(sizeof(int) * 300);
    int i;

    for (i = 0; i < 300; i++) {
        repeats[i] = 1;
    }

    while((read = getline(&line, &len, fp)) != EOF) {
        for (i = 0; i < repeats[lineNo]; i++)
            readCard(line, read, lineNo, &repeats);

        lineNo++;
    }

    int total = 0;

    for (i = 0; i < lineNo; i++)
        total += repeats[i];

    printf("Final total: %d\n", total);

    free(repeats);

    return 0;
}