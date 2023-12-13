#include <stdio.h>

#define INPUT_FILE "input.txt"
#define EXAMPLE_FILE "example.txt"
#define EXPECTED_EXAMPLE_VALUE 35
#define SKIP_EXAMPLE 0

int task(char *fileName)
{
    return 0;
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

    taskVal = task(INPUT_FILE);
    printf("Actual value from task: %d\n", taskVal);

    return 0;
}