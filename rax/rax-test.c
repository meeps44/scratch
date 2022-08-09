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
    uint8_t prefixLenBuffer[bufferSize];
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

    // for (int i = 0; i < 9; i++)
    //{
    // getdelim(&line, &bufferSize, 9, f);
    // printf("%s\n", line);
    //}

    int i = 0;
    while ((getdelim(&line, &bufferSize, 32, f)) != -1)
    {
        if (i % 3 == 0)
        {
            printf("%s\n%s\n%s\n", prefixBuffer, prefixLenBuffer, asnBuffer);
            i = 0;
        }
        switch (i)
        {
        case 0:
            strcpy(prefixBuffer, line);
            break;
        case 1:
            strcpy(prefixLenBuffer, line);
            break;
        case 2:
            strcpy(asnBuffer, line);
            asnBuffer[strcspn(asnBuffer, "\n")] = 0;
            raxInsert(rt, (unsigned char *)delimBuffer, bufferSize, (void *)asnBuffer, NULL);
            break;
        default:
            fprintf(stderr, "Switch error\n");
            return -1;
        }
        // printf("%s\n", line);
        i++;
        // raxInsert(rt, (unsigned char *)delimBuffer, bufferSize, (void *)asnBuffer, NULL);
    }

    return 0;
}

int main(void)
{
    rax *rt = raxNew();
    char *filename = "routeviews-rv6-20220411-1200.pfx2as.txt";
    readRouteviews(filename, rt);

    return EXIT_SUCCESS;
}
