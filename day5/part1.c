#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "input.txt"
#define EXAMPLE_FILE "example.txt"
#define EXPECTED_EXAMPLE_VALUE 35
#define SKIP_EXAMPLE 0
#define SKIP_ACTUAL 0

typedef struct RangeMap {
    long dest;
    long origin;
    long length;
} RangeMap;

// Read in the list of seeds
void readSeeds(char *line, ssize_t read, long **seeds, long **seedsCopy, int *seedsLength)
{
    int i, j;
    j = 0;

    long currentNum = 0;
    int isOutOfBeginning = 0;

    for (i = 0; i < read; i++) {
        if ((line[i] == ' ' || line[i] == '\n') && currentNum != 0) {
            (*seeds)[j] = currentNum;
            (*seedsCopy)[j] = currentNum;
            currentNum = 0;
            j++;
        } else if (line[i] >= '0' && line[i] <= '9') {
            currentNum *= 10;
            currentNum += (line[i] - '0');
        }
    }

    *seedsLength = j;
    printf("Found %d seeds\n", *seedsLength);
}

// Read in and parse a range from a string
RangeMap readRange(char *line, ssize_t read)
{
    int i, j;
    long currentNum;
    RangeMap range;
    j = 0;
    currentNum = -1;

    for (i = 0; i < read; i++) {
        if (line[i] >= '0' && line[i] <= '9') {
            if (currentNum == -1)
                currentNum = 0;

            currentNum *= 10;
            currentNum += (line[i] - '0');
        } else if ((line[i] == ' ' || line[i] == '\n' || line[i] == '\0') && currentNum != -1) {
            switch(j) {
                case 0:
                    range.dest = currentNum;
                    break;
                case 1:
                    range.origin = currentNum;
                    break;
                case 2:
                    range.length = currentNum;
            }

            j++;
            currentNum = 0;
        }
    }

    return range;
}

long applyMapping(RangeMap *mappings, int rangeMapLength, long seed)
{
    // Work through the mappings and see if there's one that fits
    // Otherwise, return the seed as-is.
    int i;
    RangeMap currentMapping;
    long difference;

    for (i = 0; i < rangeMapLength; i++) {
        currentMapping = mappings[i];

        if (seed >= currentMapping.origin && seed <= (currentMapping.origin + currentMapping.length)) {
            difference = seed - currentMapping.origin;

            printf("Seed %ld -> %ld (%ld, %ld, %ld)\n", seed, currentMapping.dest + difference, currentMapping.dest, currentMapping.origin, currentMapping.length);

            return currentMapping.dest + difference;
        }
    }

    return seed;
}

int task(char *fileName)
{
    FILE *fp = fopen(fileName, "r");

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    long *seeds = (long*) malloc(sizeof(long) * 30);
    long *seedsCopy = (long*) malloc(sizeof(long) * 30);
    int i;
    int lineNo = 0;
    int seedsLength;

    for (i = 0; i < 30; i++) {
        seeds[i] = 0;
    }

    // Define a list of range maps
    // We assume that the flow of data is as follows: 1 -> 2 -> ... -> n - 1 -> n
    // RangeMap **rangeMaps = (RangeMap**) malloc(sizeof(RangeMap) * 10 * 50);
    RangeMap rangeMaps[10][50];
    int rangeMapLengths[10];

    for (i = 0; i < 10; i++) {
        rangeMapLengths[i] = 0;
    }

    int currentRangeNo = 0;
    int isInRange = 0;
    RangeMap currentRange;
    int currentRangeInnerNo = 0;

    // Read the file in
    while ((read = getline(&line, &len, fp)) != EOF) {
        if (lineNo == 0) {
            readSeeds(line, read, &seeds, &seedsCopy, &seedsLength);
        } else {
            if (strncmp(line, "\n", 1) == 0) { // If it's a blank line, then we must have exited a range (unless we weren't in one!)
                if (isInRange) {
                    rangeMapLengths[currentRangeNo] = currentRangeInnerNo;
                    currentRangeNo++;
                    currentRangeInnerNo = 0;
                }
                isInRange = 0;
            } else if (isInRange == 0 && strncmp(line, "\n", 1) != 0) {
                isInRange = 1;
            } else if (isInRange) {
                currentRange = readRange(line, read);
                printf("Current range no: %d, range inner no: %d\n", currentRangeNo, currentRangeInnerNo);
                rangeMaps[currentRangeNo][currentRangeInnerNo] = currentRange;
                currentRangeInnerNo++;
            }
        }

        lineNo++;
    }

    rangeMapLengths[currentRangeNo] = currentRangeInnerNo;

    int j;

    printf("Detected %d ranges\n", currentRangeNo + 1);
    for (i = 0; i <= currentRangeNo; i++) {
        printf("Range %d: %d mappings found\n", i + 1, rangeMapLengths[i]);
    }

    // Now, pass the seeds from earlier through each of the ranges and see what we end up with
    for (i = 0; i <= currentRangeNo; i++) {
        printf("--- Range %d ---\n", i);
        for (j = 0; j < seedsLength; j++) {
            // Apply the current mapping to this seed
            seedsCopy[j] = applyMapping(rangeMaps[i], rangeMapLengths[i], seedsCopy[j]);
        }
    }

    for (i = 0; i < seedsLength; i++) {
        printf("%ld -> %ld\n", seeds[i], seedsCopy[i]);
    }

    int finalValue = -1;

    for (i = 0; i < seedsLength; i++) {
        if (finalValue == -1 || seedsCopy[i] < finalValue) {
            finalValue = seedsCopy[i];
        }
    }

    free(seeds);
    free(seedsCopy);

    return finalValue;
}

int main()
{
    int taskVal;

    if (!SKIP_EXAMPLE) {
        taskVal = task(EXAMPLE_FILE);

        if (taskVal == EXPECTED_EXAMPLE_VALUE)
            printf("Example passed with value %d\n", taskVal);
        else
            printf("Example failed with value %d\n", taskVal);
    }

    if (!SKIP_ACTUAL) {
        taskVal = task(INPUT_FILE);
        printf("Actual value from task: %d\n", taskVal);
    }

    return 0;
}