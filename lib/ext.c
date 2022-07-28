#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h> // flock()
#include <unistd.h>   // getHostname()
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "ext.h"

#define DEBUG_ON 1

// uint16_t outgoing_tcp_port;
// char *timestamp;
// address *source_ip;
// uint32_t source_asn;
// address *destination_ip;
// uint32_t destination_asn;
// uint8_t path_id[SHA_DIGEST_LENGTH];
// uint8_t hop_count;
// hop *hops[HOP_MAX]; // maximum hop length is 35. any hops longer than that do not get included.

// static const char *TRACEROUTE_FORMAT_IN = "\n%[^,], %d, %d";
// static const char *TRACEROUTE_FORMAT_OUT = "%s, %d, %d\n";

static const char *TRACEROUTE_FORMAT_IN = "\n%d,%[^,], %[], %d, %[], %d, %[^,], %d, %[]";
static const char *TRACEROUTE_FORMAT_OUT = "%d, %s, %[], %d, %[], %d, %[^,], %d, %[]\n";

address *createAddress()
{
    address *a;
    if ((a = calloc(1, sizeof(address))) == NULL)
    {
        perror("Error");
        exit(1);
    }

    return a;
}

traceroute *createTraceroute()
{
    traceroute *t;
    if ((t = calloc(1, sizeof(traceroute))) == NULL)
    {
        perror("Error");
        exit(1);
    }

    return t;
}

hop *createHop()
{
    hop *h;
    if ((h = calloc(1, sizeof(hop))) == NULL)
    {
        perror("Error");
        exit(1);
    }

    return h;
}

void sPrintHash(uint8_t *digest, char *s)
{
    const int MAX = 41;
    int i, length = 0;
    for (i = 0; i < SHA_DIGEST_LENGTH; i++)
    {
        length += snprintf(s + length, MAX - length, "%02x", digest[i]);
    }
}

void printHash(uint8_t *digest)
{
    int i;
    for (i = 0; i < SHA_DIGEST_LENGTH; i++)
    {
        printf("%02x", digest[i]);
    }
    printf("\n");
}

uint8_t *hashPath(address arr[], int arraySize)
{
    // // The data to be hashed
    // SHA_CTX shactx;
    // // uint8_t digest[SHA_DIGEST_LENGTH];
    // uint8_t *digest = (uint8_t *)calloc(SHA_DIGEST_LENGTH, sizeof(uint8_t));
    // SHA1_Init(&shactx);
    // SHA1_Update(&shactx, l, sizeof(traceroute));
    // SHA1_Final(digest, &shactx); // digest now contains the 20-byte SHA-1 hash
    // return digest;

    unsigned char *obuf = malloc(sizeof(uint8_t) * 20);
    SHA_CTX shactx;

    SHA1_Init(&shactx);
    for (int i = 0; i < arraySize; i++)
    {
        SHA1_Update(&shactx, &arr[i], sizeof(address));
    }
    SHA1_Final(obuf, &shactx); // digest now contains the 20-byte SHA-1 hash

    return obuf;
}

int asnLookup(address *ipv6_address)
{
    int ASN;
    FILE *fp;
    char input_buffer[1024], open_string_buffer[1024];
    int num;
    int i = 1;

    sprintf(open_string_buffer, "python3 main.py %d", atoi(addressToString(ipv6_address)));

#if DEBUG_ON == 1
    printf("DEBUG:\tvalue of open_string_buffer:\t%s\n", open_string_buffer);
#endif

    fp = popen(open_string_buffer, "r");
    if (fp == NULL)
    {
        perror("Failed to create file pointer\n");
        fprintf(stderr, "Errno:\t%s\n", strerror(errno));
        exit(1);
    }

    while (fgets(input_buffer, sizeof(input_buffer), fp) != NULL)
    {
        printf("Read line:\t%d\n", i++);
        num = atoi(input_buffer);
        printf("Num = %d\n", num);
    }
    pclose(fp);
    return ASN;
}

// address *parseIPv6(packet_t packet);

// void printParsedPacket(parsed_packet *p);

// int getFlowLabel(parsed_packet *p);

int printHop(hop *h)
{
    printf("Returned flow label:\t%u\n", h->returned_flowlabel);
    printf("Hop number:\t%d\n", h->hopnumber);
    printAddress(h->hop_address);
    return 0;
}

int printAddress(address *a)
{
    char *address_string = addressToString(a);
    printf("Address:\t%s\n", address_string);
    return 0;
}

int printTraceroute(traceroute *t)
{
    /*
    uint16_t outgoing_tcp_port;
    char *timestamp;
    address source_ip;
    uint32_t source_asn;
    address destination_ip;
    uint32_t destination_asn;
    uint8_t path_id[SHA_DIGEST_LENGTH];
    hop *hops[35]; // maximum hop length is 35. any hops longer than that do not get included.
    */
    printf("Outgoing tcp port:\t%d\n", t->outgoing_tcp_port);
    printf("Timestamp:\t%d\n", t->outgoing_tcp_port);
    printAddress(t->source_ip);
    printf("Source ASN:\t%d\n", t->source_asn);
    printAddress(t->destination_ip);
    printf("Destination ASN:\t%d\n", t->destination_asn);
    printf("Path ID:\t%x\n", t->path_id);
    for (int i = 0; i < t->hop_count; i++)
    {
        printHop(t->hops[i]);
    }

    return 0;
}

char *tracerouteToJSON(traceroute *t)
{
    return 0;
}

char *createFileName(struct tm *now) // (Might not be needed)
{
    // TODO: Implement malloc guards (check malloc return value for errors)
    char *fileName = malloc(sizeof(char) * 100);
    char *hostname = malloc(sizeof(char) * 30);
    char *timestamp = malloc(sizeof(char) * 50);
    gethostname(hostname, 30);

    // Output timestamp in format "YYYY-MM-DD-hh_mm_ss : "
    sprintf(timestamp, "-%04d-%02d-%02d-%02d_%02d_%02d",
            now->tm_year + 1900, now->tm_mon + 1, now->tm_mday,
            now->tm_hour, now->tm_min, now->tm_sec);

    strcat(hostname, timestamp);

    return fileName;
}

struct tm *getCurrentTime()
{
    time_t t = time(NULL);
    struct tm *now = gmtime(&t);

    return now;
}

int appendAddress(address *a, traceroute *t, uint8_t hopnumber, uint32_t returned_flowlabel)
{
    traceroute *tmp = t;
    int i;

    for (i = 0; i < 35; i++)
    {
        if (tmp->hops[i] == NULL)
        {
            printf("appendAddress:\tAvailable spot found at index:\t%d\n", i);
            hop *h = malloc(sizeof(hop));
            h->hop_address = a;
            h->hopnumber = hopnumber;
            h->returned_flowlabel = returned_flowlabel;
            tmp->hops[i] = h;
            return 0;
        }
    }

    return -1;
}

int appendHop(hop *h, traceroute *t)
{
    int i;

    for (i = 0; i < 35; i++)
    {
        if (t->hops[i] == NULL)
        {
            printf("appendHop:\tAvailable spot found at index:\t%d\n", i);
            t->hops[i] = h;
            return 0;
        }
    }

    return -1;
}

char *addressToString(address *a)
{
    char *address_string = malloc(sizeof(char) * 17); // 1 more extra char for \0-terminator
    for (int i = 0, k = 0; i < 8; i++, k += 2)
    {

        address_string[k] = (char)a->address_short[i];
        address_string[k + 1] = (char)a->address_short[i] >> 8;
    }

    address_string[17] = '\0';

    return address_string;
}

int ptFileToTraceroute(char *filename)
{
    FILE *f;
    traceroute *t = calloc(1, sizeof(traceroute));

    if ((f = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "Error opening file:\t%s\nErrno:\t%s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* Seek to the beginning of the file */
    fseek(f, 0, SEEK_SET);

    /* Read and display data */
    fread(t, sizeof(traceroute), 1, f);
    fclose(f);

    return t;
    return 0;
}

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
    return 0;
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

int readTracerouteFromFile(char *filename, traceroute *t, long offset)
{
    FILE *f;

    if ((f = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "Error opening file:\t%s\nErrno:\t%s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* Seek to offset within file.
    The offset should always be a multiple of sizeof(traceroute)*/
    assert(offset % sizeof(traceroute) == 0); // erlend - remember to remove in prod!
    fseek(f, offset, SEEK_SET);

    /* Read and display data */
    // fread(t, sizeof(traceroute), 1, f);
    fscanf(f, TRACEROUTE_FORMAT_IN, t->timestamp, &t->hop_count, &t->destination_asn);
    fclose(f);

    return 0;
}

int readTracerouteArrayFromFile(char *filename, traceroute *tr_arr[], int arraySize)
{
    FILE *file;
    if ((file = fopen(filename, "r")) == NULL)
    {
        perror("Error:");
        return -1;
    }

    rewind(file);
    traceroute *t;
    int i = 0;
    while (1)
    {
        t = calloc(1, sizeof(traceroute));
        fscanf(file, TRACEROUTE_FORMAT_IN, t->timestamp, &t->hop_count, &t->destination_asn);
        tr_arr[i] = t;
        if (feof(file) || (i >= (arraySize - 1)))
        {
            break;
        }
        i++;
    }

    return 0;
}

int writeTracerouteToFile(traceroute *t, char *filename)
{

    // uint16_t outgoing_tcp_port;
    // char *timestamp;
    // address *source_ip;
    // uint32_t source_asn;
    // address *destination_ip;
    // uint32_t destination_asn;
    // uint8_t path_id[SHA_DIGEST_LENGTH];
    // uint8_t hop_count;
    // hop *hops[HOP_MAX]; // maximum hop length is 35. any hops longer than that do not get included.

    FILE *f;
    size_t numb_elements = 1;
    int my_int = 42;

    if ((f = fopen(filename, "a+")) == NULL) // Open file for reading and appending
    {
        fprintf(stderr, "Error opening file:\t%s\nErrno:\t%s\n", filename, strerror(errno));
        return -1;
    }

    if (flock(fileno(f), LOCK_EX) == -1) // exclusive lock - only 1 file may operate on it at a time
    {
        fprintf(stderr, "Error locking file:\t%s\nErrno:\t%s\n", filename, strerror(errno));
        return -1;
    }

    // fwrite(&t, sizeof(traceroute), 1, f);
    /* Write to file */
    fprintf(f, TRACEROUTE_FORMAT_OUT, t->timestamp, t->hop_count, t->destination_asn);

    flock(fileno(f), LOCK_UN); // unlock file
    fclose(f);
}

int writeTracerouteArrayToFile(char *filename, traceroute *tr_arr[], int arraySize)
{
    FILE *f;
    size_t numb_elements = 1;
    int my_int = 42;

    if ((f = fopen(filename, "a+")) == NULL) // Open file for reading and appending
    {
        fprintf(stderr, "Error opening file:\t%s\nErrno:\t%s\n", filename, strerror(errno));
        return -1;
    }

    if (flock(fileno(f), LOCK_EX) == -1) // exclusive lock - only 1 file may operate on it at a time
    {
        fprintf(stderr, "Error locking file:\t%s\nErrno:\t%s\n", filename, strerror(errno));
        return -1;
    }

    // fwrite(&t, sizeof(traceroute), 1, f);
    /* Write to file */
    for (int i = 0; i < arraySize; i++)
    {
        fprintf(filename, TRACEROUTE_FORMAT_OUT, tr_arr[i]->timestamp, tr_arr[i]->hop_count, tr_arr[i]->destination_asn);
    }

    flock(fileno(f), LOCK_UN); // unlock file
    fclose(f);
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
