#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "rax.h"

// int asnLookup(address *a)
//{
// int asn = 0;

// return asn;
//}

int readRouteviews(char *filename, rax *rt)
{
    FILE *f;
    char *line = NULL;
    size_t len = 0;
    size_t bufferSize = 100;

    char *delimBuffer[100];

    uint8_t prefixBuffer[bufferSize];
    uint8_t prefixlengthBuffer[2];
    uint8_t asnBuffer[bufferSize];

    if ((f = fopen(filename, "r")) == NULL)
    {
        perror("Error ");
        return -1;
    }

    // while ((nread = getline(prefixBuffer, &len, stream)) != -1)
    //{
    // sprintf(prefixBuffer, "")
    //}

    for (int i = 0; i < 9; i++)
    {
        getdelim(&line, &bufferSize, 9, f);
        printf("%s\n", line);
    }
    // while ((getdelim(delimBuffer, &bufferSize, 32, f)) != -1)
    //{
    // printf("Retrieved line of length %zu:\n", nread);
    // printf("Delim:\t%s\n", delimBuffer);
    //// raxInsert(rt, (unsigned char *)delimBuffer, bufferSize, (void *)asnBuffer, NULL);
    //}

    return 0;
}

int main(void)
{
    rax *rt = raxNew();
    char *filename = "routeviews-rv6-20220411-1200.pfx2as.txt";
    readRouteviews(filename, rt);

    return EXIT_SUCCESS;
}
