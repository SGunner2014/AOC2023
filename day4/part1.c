#include <stdio.h>

#define INPUT_FILE "input.txt"

#define CARD_STATE 0
#define WINNING_STATE 1
#define ENTRIES_STATE 2

int readCard(char *line, ssize_t read)
{
    int total = 0;
    int i, j, k;
    int currentState = CARD_STATE;
    int currentNum = 0;
    int winningNums[10];

    for (i = 0; i < 10; i++) {
        winningNums[i] = 0;
    }

    j = 0;

    for (i = 0; i < read; i++) {
        if (currentState == CARD_STATE && line[i] == ':') {
            currentState = WINNING_STATE;
        } else if ((currentState == WINNING_STATE || currentState == ENTRIES_STATE) && line[i] >= '0' && line[i] <= '9') {
            currentNum *= 10;
            currentNum += (line[i] - '0');
        } else if (currentState == WINNING_STATE && line[i] == ' ' && currentNum != 0) {
            winningNums[j] = currentNum;
            j++;
            currentNum = 0;
        } else if (currentState == WINNING_STATE && line[i] == '|') {
            currentState = ENTRIES_STATE;
        } else if (currentState == ENTRIES_STATE && (line[i] == ' ' || line[i] == '\n' || line[i] == '\0') && currentNum != 0) {
            // Check if the currentNum is a winning num and then increase the score
            for (k = 0; k < j; k++) {
                if (winningNums[k] == currentNum) {
                    if (total == 0)
                        total++;
                    else
                        total *= 2;
                    break;
                }
            }

            currentNum = 0;
        }
    }

    return total;
}

int main()
{
    FILE *fp = fopen(INPUT_FILE, "r");

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int total = 0;

    while((read = getline(&line, &len, fp)) != EOF) {
        total += readCard(line, read);
    }

    printf("Final total: %d\n", total);

    return 0;
}