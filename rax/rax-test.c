#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "rax.h"

/* NOTE: Remember to remove all newlines and other separaters from input file
    The only separator should be a whitespace (ascii 32):
    tr '\t' ' ' < routeviews-rv6-20220411-1200.pfx2as.data > nn-routeviews-rv6-20220411-1200.pfx2as.data
    tr '\n' ' ' < routeviews-rv6-20220411-1200.pfx2as.data > nn-routeviews-rv6-20220411-1200.pfx2as.data
*/

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
    size_t bufferSize = 60;

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
            // printf("%s\n%s\n%s\n", prefixBuffer, prefixLenBuffer, asnBuffer);
            i = 0;
        }
        switch (i)
        {
        case 0:
            // printf("Case:%d\tLine:%s\n", i, line);
            strcpy(prefixBuffer, line);
            prefixBuffer[strlen(line)] = '\0';
            printf("Prefixbuffer:\t%s\n", prefixBuffer);
            break;
        case 1:
            strcpy(prefixLenBuffer, line);
            prefixBuffer[strlen(line)] = '\0';
            break;
        case 2:
            strcpy(asnBuffer, line);
            prefixBuffer[strlen(line)] = '\0';
            // asnBuffer[strcspn(asnBuffer, "\n")] = 0; // Strip newline
            raxInsert(rt, (unsigned char *)prefixBuffer, bufferSize, (void *)asnBuffer, NULL);
            break;
        default:
            fprintf(stderr, "Switch error\n");
            return -1;
        }
        // printf("%s\n", line);
        i++;
        // raxInsert(rt, (unsigned char *)delimBuffer, bufferSize, (void *)asnBuffer, NULL);
    }

    fclose(f);

    return 0;
}

int search(rax *rt, char *search_string)
{
    raxIterator iter;
    raxStart(&iter, rt); // Note that 'rt' is the radix tree pointer.
    int result = raxSeek(&iter, ">=", (unsigned char *)search_string, strlen(search_string));
    return result;
}

int main(void)
{
    rax *rt = raxNew();
    // char *filename = "routeviews-rv6-20220411-1200.pfx2as.data";
    char *filename = "output.data";
    readRouteviews(filename, rt);
    char *search_string = "2001:200:c000::";
    printf("Search result:\t%d\n", search(rt, search_string));

    return EXIT_SUCCESS;
}
