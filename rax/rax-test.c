#include <netinet/in.h>
#include <arpa/inet.h>
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

void *getValue(rax *rt, unsigned char *key, size_t keyLength)
{
    printf("Search key: %s\n", key);
    void *data = raxFind(rt, key, keyLength);
    if (data == raxNotFound)
    {
        // fprintf(stderr, "DEBUG: Key lookup error: Key not found\n");
        return raxNotFound;
    }
    printf("DEBUG: Lookup successful!\nKey:\t%.*s\nValue:\t%s\n", (int)keyLength, key, (char *)data);
    // printf("DEBUG: Data pointer is %p\n", data);
    // printf("DEBUG: Data value at key %s is %s\n", key, (char *) data);

    return data;

    // data = raxFind(rt,buffer2,bufferLength);
    // if (data == raxNotFound) return 1;
    // printf("Key value is %p\n", data);
    // printf("Key value is %s\n", (char *) data);

    // data = raxFind(rt,buffer3,bufferLength);
    // if (data == raxNotFound) return 1;
    // printf("Key value is %p\n", data);
    // printf("Key value is %s\n", (char *) data);
}

// int search(rax *rt, unsigned char *element, size_t elementLength)
//{
//// Initialize iterator
// raxIterator iter;
// raxStart(&iter, rt);

//// raxSeek(&iter,">=",(unsigned char*)"cut",3);
// raxSeek(&iter, "<=", element, elementLength); // <= seek the element equal, or immediately smaller than the given one.

// while (raxNext(&iter))
//{
// printf("Key: %.*s\n", (int)iter.key_len, (char *)iter.key);
// if (raxCompare(&iter, ">", (unsigned char *)"cutiie", 6))
// break;
//}

// raxStop(&iter);

// return 0;
//}

void *search(rax *rt, unsigned char *element, size_t elementLength)
{
    raxIterator iter;
    raxStart(&iter, rt); // Note that 'rt' is the radix tree pointer.

    fprintf(stderr, "DEBUG: Starting raxSeek for element %s\n", element);
    // raxSeek(&iter, "<=", element, elementLength);
    raxSeek(&iter, "==", element, elementLength);
    // fprintf(stderr, "DEBUG: Key: %.*s\n", (int)iter.key_len, (char *)iter.key);
    fprintf(stderr, "DEBUG: Iter key: %s\n", (char *)iter.key);

    fprintf(stderr, "DEBUG: Starting raxFind\n");
    void *data = raxFind(rt, (char *)iter.key, iter.key_len);
    fprintf(stderr, "DEBUG: raxFind done. Result data:\t%s\n", (char *)data);
    // void *data = raxFind(rt, element, elementLength);

    // if (data == raxNotFound)
    //{
    // fprintf(stderr, "DEBUG: Element not found. Retrying...\n");
    // raxPrev(&iter);
    // data = raxFind(rt, iter.key, iter.key_len);
    //}

    // while (data == raxNotFound)
    //{
    // raxPrev(&iter);
    // printf("Key: %.*s\n", (int)iter.key_len, (char *)iter.key);
    // data = raxFind(rt, iter.key, iter.key_len);
    //}
    printf("DEBUG: Lookup successful!\nKey:\t%.*s\nValue:\t%s\n", (int)elementLength, element, (char *)data);

    return data;
}

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

    struct in6_addr a;

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
            // strcpy(prefixBuffer, line);
            memcpy(prefixBuffer, line, strlen(line));
            // prefixBuffer[strlen(line)] = '\0';
            // printf("Prefixbuffer:\t%s\n", prefixBuffer);
            break;
        case 1:
            // strcpy(prefixLenBuffer, line);
            memcpy(prefixLenBuffer, line, strlen(line));
            prefixLenBuffer[strlen(line)] = '\0';
            break;
        case 2:
            // strcpy(asnBuffer, line);
            memcpy(asnBuffer, line, strlen(line));
            asnBuffer[strlen(line)] = '\0';
            inet_pton(AF_INET6, prefixBuffer, &(a));
            char *result = prefixedIntToBinary(a.__in6_u.__u6_addr32, atoi(prefixLenBuffer), atoi(asnBuffer), rt);
            // raxInsert(rt, (unsigned char *)prefixBuffer, bufferSize, (void *)asnBuffer, NULL);
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

    raxShow(rt);

    return 0;
}

// int search(rax *rt, char *search_string)
//{
// raxIterator iter;
// raxStart(&iter, rt); // Note that 'rt' is the radix tree pointer.
// int result;
// int searchlen = strlen(search_string);
// while (result != 1)
//{
// if (searchlen < 1)
//{
// fprintf(stderr, "Search string not found in radix tree\n");
// break;
//}
// result = raxSeek(&iter, ">=", (unsigned char *)search_string, searchlen);
// searchlen--;
//}
// return result;
//}

char *prefixedIntToBinary(int a[4], int networkPrefix, int asn, rax *rt)
{
    char *binaryRepresentation = malloc(sizeof(char) * networkPrefix);
    char *tmp = malloc(sizeof(char) * 128);
    int k = 0;
    unsigned i;
    for (int j = 0; j < 4; j++)
    {
        for (i = 1 << 31; i > 0; i = i / 2)
        {
            if (a[j] & i)
            {
                tmp[k] = '1';
                printf("1");
            }
            else
            {
                tmp[k] = '0';
                printf("0");
            }
            k++;
        }
        puts("");
    }

    memcpy(binaryRepresentation, tmp, networkPrefix);

    // Insert binary representation of address into radix Trie
    // raxInsert(rt, (unsigned char *)binaryRepresentation, networkPrefix, (void *)&asn, NULL);
    return binaryRepresentation;
}

char *intToBinary(int a[4])
{
    char *binaryRepresentation = malloc(sizeof(char) * 128);
    int k = 0;
    unsigned i;
    for (int j = 0; j < 4; j++)
    {
        for (i = 1 << 31; i > 0; i = i / 2)
        {
            if (a[j] & i)
            {
                binaryRepresentation[k] = '1';
                printf("1");
            }
            else
            {
                binaryRepresentation[k] = '0';
                printf("0");
            }
            k++;
        }
        puts("");
    }

    return binaryRepresentation;
}

int main(void)
{
    rax *rt = raxNew();
    // char *filename = "routeviews-rv6-20220411-1200.pfx2as.data";
    // char *filename = "output.data";
    // readRouteviews(filename, rt);
    //// unsigned char *search_string = "2001:200:c000::";
    // unsigned char search_string[15];
    // search_string[0] = '2';
    // search_string[1] = '0';
    // search_string[2] = '0';
    // search_string[3] = '1';
    // search_string[4] = ':';
    // search_string[5] = '2';
    // search_string[6] = '0';
    // search_string[7] = '0';
    // search_string[8] = ':';
    // search_string[9] = 'c';
    // search_string[10] = '0';
    // search_string[11] = '0';
    // search_string[12] = '0';
    // search_string[13] = ':';
    // search_string[14] = ':';
    // int r = *(int *)search(rt, search_string, 15);
    // printf("Search result:\t%d\n", r);

    unsigned char *example_address = "2001:200:c000::";
    unsigned char *example_network = "2001:200:e000::";
    int example_prefix = 35;
    struct in6_addr a;

    // Here I had to convert from NSString to c string
    inet_pton(AF_INET6, example_address, &(a));

    // This line gets the integer value
    int c1 = a.__in6_u.__u6_addr32[0];
    int c2 = a.__in6_u.__u6_addr32[1];
    int c3 = a.__in6_u.__u6_addr32[2];
    int c4 = a.__in6_u.__u6_addr32[3];
    printf("Integer rep of IPv6-address:\n%d\n%d\n%d\n%d\n", c1, c2, c3, c4);

    char *char_array = intToBinary(a.__in6_u.__u6_addr32);
    for (int i = 0; i < 128; i++)
    {
        if (i % 32 == 0)
            puts("");
        printf("%c", char_array[i]);
    }
    puts("");

    // raxInsert(rt, (unsigned char *)prefixBuffer, bufferSize, (void *)asnBuffer, NULL);
    return EXIT_SUCCESS;
}
