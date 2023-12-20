#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "input.txt"
#define EXAMPLE_FILE "example.txt"
#define EXPECTED_EXAMPLE_VALUE 46
#define SKIP_EXAMPLE 0
#define SKIP_ACTUAL 1

typedef struct RangeMap {
    long dest;
    long origin;
    long length;
} RangeMap;

typedef struct Seed {
    long start;
    long end;
} Seed;

// Read in the list of seeds
void readSeeds(char *line, ssize_t read, long **seeds, long **seedsCopy, int *seedsLength)
{
    int i, j;
    j = 0;

    long currentNum = 0;

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

void readSeedsNew(char *line, ssize_t read, Seed **seeds, int *seedsLength)
{
    int i, j;
    j = 0;

    long currentNum = 0;

    for (i = 0; i < read; i++) {
        if ((line[i] == ' ' || line[i] == '\n') && currentNum != 0) {
            if (j % 2 == 0) {
                (*seeds)[j / 2].start = currentNum;
            } else {
                (*seeds)[j / 2].end = (*seeds)[j / 2].start + currentNum - 1;
            }

            currentNum = 0;
            j++;
        } else if (line[i] >= '0' && line[i] <= '9') {
            currentNum *= 10;
            currentNum += (line[i] - '0');
        }
    }

    *seedsLength = j / 2;
    printf("Found %d seeds\n", *seedsLength);
    for (i = 0; i < *seedsLength; i++) {
        printf("%ld -> %ld\n", (*seeds)[i].start, (*seeds)[i].end);
    }
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

void applyMappingsNew(RangeMap *mappings, int rangeMapLength, Seed **seeds, int *seedsLength)
{
    // Work through the mapping and see if there's one that fits
    // If we need to split a seed range, we'll realloc and append a new range
    // Otherwise, make no changes and return.
    int i, j;
    long seed_low, seed_high, map_low, map_high, over_low, over_high, map_diff;
    int beginSeedsLength = *seedsLength;
    RangeMap currentMapping;
    
    // Iterate through rangemaps
    for (i = 0; i < rangeMapLength; i++) {
        map_low = mappings[i].origin;
        map_high = mappings[i].origin + mappings[i].length - 1;
        map_diff = mappings[i].dest - mappings[i].origin;

        printf("Map diff: %ld (%ld -> %ld)\n", map_diff, map_low, map_high);

        // Iterate through seed ranges
        for (j = 0; j < beginSeedsLength; j++) {
            // For each seed range:
            // Find if the seed range overlaps with the mapping at all
            // If it does, generate a new seed range for the mapped areas and
            // update the seedsLength
            seed_low = (*seeds)[j].start;
            seed_high = (*seeds)[j].end;

            // Find an overlap if one exists
            if (seed_high < map_low || map_high < seed_low) {
                continue;
            }

            over_low = (seed_low > map_low) ? seed_low : map_low;
            over_high = (seed_high < map_high) ? seed_high : map_high;

            // printf("Detected overlap (%ld, %ld) from (%ld, %ld) and (%ld, %ld)\n", over_low, over_high, seed_low, seed_high, map_low, map_high);

            // Using this overlap, we can now generate seed ranges as required
            // Case 1 - overlap is exactly the same as the original range
            if (over_low == seed_low && over_high == seed_high) {
                (*seeds)[j].start += map_diff;
                (*seeds)[j].end += map_diff;
            // Case 2 - overlap is at the start of the original range
            } else if (over_low == seed_low && over_high < seed_high) {
                (*seeds)[j].start += map_diff;
                (*seeds)[j].end = (over_high + map_diff);

                // Add another seed range
                *seeds = realloc(*seeds, sizeof(Seed) * (*seedsLength) + 1);
                (*seedsLength) += 1;

                (*seeds)[*seedsLength - 1].start = over_high + 1;
                (*seeds)[*seedsLength - 1].end = seed_high;
            // Case 3 - overlap is at the end of the original range
            } else if (over_high == seed_high && over_low > seed_low) {
                (*seeds)[j].start = (over_low + map_diff);
                (*seeds)[j].end += map_diff;

                // Add another seed range
                *seeds = realloc(*seeds, sizeof(Seed) * (*seedsLength) + 1);
                (*seedsLength) += 1;

                (*seeds)[*seedsLength - 1].start = seed_low;
                (*seeds)[*seedsLength - 1].end = over_low - 1;
            // Case 4 - overlap is somewhere in the middle of the original range
            } else if (over_high < seed_high && over_low > seed_low) {
                printf("--- here ---\n");

                (*seeds)[j].start = (over_low + map_diff);
                (*seeds)[j].end = (over_high + map_diff);

                // Add 2 more seed ranges
                *seeds = realloc(*seeds, sizeof(Seed) * (*seedsLength) + 2);
                (*seedsLength) += 2;

                (*seeds)[*seedsLength - 2].start = seed_low;
                (*seeds)[*seedsLength - 2].end = over_low - 1;

                (*seeds)[*seedsLength - 1].start = over_high + 1;
                (*seeds)[*seedsLength - 1].end = seed_high;
            }
        }
    }
}

long task(char *fileName)
{
    FILE *fp = fopen(fileName, "r");

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    Seed *seeds = (Seed*) malloc(sizeof(Seed) * 20);
    int i;
    int lineNo = 0;
    int seedsLength;

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
            // readSeeds(line, read, &seeds, &seedsCopy, &seedsLength);
            readSeedsNew(line, read, &seeds, &seedsLength);
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
                // printf("Current range no: %d, range inner no: %d\n", currentRangeNo, currentRangeInnerNo);
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

    printf("Seed ranges: %d\n", seedsLength);
    // Now, pass the seeds from earlier through each of the ranges and see what we end up with
    for (i = 0; i <= currentRangeNo; i++) {
        printf("--- Range %d ---\n", i);
        // for (j = 0; j < seedsLength; j++) {
        //     // Apply the current mapping to this seed


        //     // applyMapping(rangeMaps[i], rangeMapLengths[i], seedsCopy[j]);
        // }
        applyMappingsNew(rangeMaps[i], rangeMapLengths[i], &seeds, &seedsLength);
        printf("Seed ranges: %d\n", seedsLength);
    }

    long finalValue = -1;

    for (i = 0; i < seedsLength; i++) {
        printf("(%ld -> %ld)\n", seeds[i].start, seeds[i].end);
        if (finalValue == -1 || seeds[i].start < finalValue) {
            finalValue = seeds[i].start;
            printf("Setting final value: %ld (%d)\n", seeds[i].start, i);
        }
    }

    free(seeds);

    return finalValue;
}

int main()
{
    long taskVal;

    if (!SKIP_EXAMPLE) {
        taskVal = task(EXAMPLE_FILE);

        if (taskVal == EXPECTED_EXAMPLE_VALUE)
            printf("Example passed with value %ld\n", taskVal);
        else
            printf("Example failed with value %ld\n", taskVal);
    }

    if (!SKIP_ACTUAL) {
        taskVal = task(INPUT_FILE);
        printf("Actual value from task: %ld\n", taskVal);
    }

    return 0;
}