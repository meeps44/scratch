#include <stdio.h>
#include <stdlib.h>
#include "ext.h"

int first_run = 1;

int main(void)
{
    char *filename = "test.txt";
    traceroute *t;

    if(first_run)
    {
        t = calloc(1, sizeof(traceroute));
        t->timestamp;
        t->source_ip;
        t->source_asn;
        t->destination_ip;
        t->source_asn;
        first_run = 0;
    } 

    // Create 36 hops
    for (int i = 0; i < 36; i++)
    {
        hop *h = createHop();
        h->hopnumber = i;
        h->hop_address;
        h->returned_flowlabel;
        if (appendHop(h, t) == -1)
        {
            fprintf(stderr, "Failed to append hop: Hop array is full\n");
        }
    }

    fWriteTraceroute(t, filename);

    return EXIT_SUCCESS;
}