#include "routecompare.h" // also includes ext.h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int compareHops(hop *h1, hop *h2)
{
}

int compareAddresses(address *a1, address *a2)
{
    if (memcmp(a1, a2, sizeof(short) * 8) == 0)
        return 1;
    return 0;
}

int compareIndexedPaths(traceroute *t1, traceroute *t2)
{
}

int comparePaths(traceroute *t1, traceroute *t2)
{
    hop *h1 = t1->hops, *h2 = t2->hops;

    for (int i = 0; i < t1->hop_count; i++)
    {
        /* First check if source and destination IP is the same */
        if ((compareAddresses(t1->source_ip, t2->source_ip) == 0) && compareAddresses(t1->destination_ip, t2->destination_ip) == 0)
        {
            /* If yes, check if all the hops are the same */
            if (compareAddresses(h1[i].hop_address, h2[i].hop_address) == -1)
            {
                puts("The paths are not equal!");
                return 0;
            }
            else
            {
                puts("The paths are equal!");
                return 1;
            }
        }
    }
    fprintf(stderr, "Error: comparePaths() did not find any matching \
    source/destination address\n");
    return -1;
}

char **fCompareIndexedPaths(char *file1, char *file2)
{
    int arraySize = 16000;
    char *results[arraySize];
    int resultsIndex = 0;

    traceroute *arr1[arraySize];
    traceroute *arr2[arraySize];

    readTracerouteFile(file1, arr1, arraySize);
    readTracerouteFile(file2, arr1, arraySize);

    for (int i = 0; i < arraySize; i++)
    {
        for (int j = 0; j < arraySize; j++)
        {
            if (compareIndexedPaths(arr1[i], arr2[j]) == 0) // equal!
            {
                results[resultsIndex] = "Src %s Dst %s are equal";
                resultsIndex++;
            }
            else
            {
                results[resultsIndex] = "Src %s Dst %s are NOT equal, the paths diverged at index: %d";
                resultsIndex++;
            }
        }
    }

    return 0;
}

char **fComparePaths(char *file1, char *file2)
{
    int arraySize = 16000;
    char *results[arraySize];
    int resultsIndex = 0;

    traceroute *arr1[arraySize];
    traceroute *arr2[arraySize];

    readTracerouteFile(file1, arr1, arraySize);
    readTracerouteFile(file2, arr1, arraySize);

    for (int i = 0; i < arraySize; i++)
    {
        for (int j = 0; j < arraySize; j++)
        {
            if (comparePaths(arr1[i], arr2[j]) == 0) // equal!
            {
                results[resultsIndex] = "Src %s Dst %s are equal";
                resultsIndex++;
            }
            else
            {
                results[resultsIndex] = "Src %s Dst %s are NOT equal";
                resultsIndex++;
            }
        }
    }

    return 0;
}

int main(int argc, char **argv)
{
    return EXIT_SUCCESS;
}