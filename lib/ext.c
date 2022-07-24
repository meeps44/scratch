#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h> // flock()
#include <unistd.h> // getHostname()
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "ext.h"

#define DEBUG_ON 1


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

uint8_t *hashPath(address *l[])
{
    // The data to be hashed
    SHA_CTX shactx;
    // uint8_t digest[SHA_DIGEST_LENGTH];
    uint8_t *digest = (uint8_t *) calloc(SHA_DIGEST_LENGTH, sizeof(uint8_t));

    SHA1_Init(&shactx);
    SHA1_Update(&shactx, l, sizeof(traceroute));
    SHA1_Final(digest, &shactx); // digest now contains the 20-byte SHA-1 hash
    
    return digest;
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

traceroute *fReadTraceroute(char *filename, long offset)
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
}

int fWriteTraceroute(traceroute *t, char *filename)
{
    FILE *f;
    // char *filename = "/home/erlend/C-programming/library-test/write_test.txt";
    size_t numb_elements = 1;
    int my_int = 42;
    // address my_struct = {
        // .a = 52,
        // .b = 1,
        // .c = 96,
        // .d = 8765};
    // char my_str[50] = "Hello from process 1!\n";

    // opens a file for reading and appending
    if ((f = fopen(filename, "a+")) == NULL)
    {
        fprintf(stderr, "Error opening file:\t%s\nErrno:\t%s\n", filename, strerror(errno));
        return -1;
    }

    if (flock(fileno(f), LOCK_EX) == -1) // exclusive lock - only 1 file may operate on it at a time
    {
        fprintf(stderr, "Error locking file:\t%s\nErrno:\t%s\n", filename, strerror(errno));
        return -1;
    }

    fwrite(&t, sizeof(traceroute), 1, f);
    //fwrite(&my_struct, sizeof(int), sizeof(my_struct) / sizeof(int), f);
    // fwrite(my_str, sizeof(char), sizeof(my_str), f);

    flock(fileno(f), LOCK_UN); // unlock file
    fclose(f);
}

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
    char *address_string = malloc(sizeof(char)*17); // 1 more extra char for \0-terminator
    for (int i = 0, k = 0; i < 8; i++, k+=2)
    {

        address_string[k] = (char) a->address_short[i];
        address_string[k+1] = (char) a->address_short[i] >> 8;
    }

    address_string[17] = '\0';

    return address_string;
}