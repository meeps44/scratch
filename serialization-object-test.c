#include "serialization-object-test.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//static const char *HOP_FORMAT_IN = "\n%d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, \
//%d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, \
//%d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, \
//%d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d, %d, %d, %d:%d";
// static const char *TR_TEST_FORMAT_OUT = "%d, %s, %x%x:%x%x:%x%x:%x%x, %d, %x%x:%x%x:%x%x:%x%x, %d, %[^,], %d, ";
// static const char *ADDRESS_FORMAT_OUT = "%d:%d";
static const char *HOP_FORMAT_IN = " %d, %d, %d:%d";
static const char *HOP_FORMAT_OUT = "%d, %d, %d:%d ";
static const char *TR_TEST_FORMAT_IN = "%d, %[^,], %d:%d, %d, %d:%d, %d, %[^,]";
static const char *TR_TEST_FORMAT_OUT = "%d, %s, %d:%d, %d, %d:%d, %d, %s, ";

int printTraceroute(traceroute *t1)
{
    printf(TR_TEST_FORMAT_OUT, t1->outgoing_tcp_port, t1->timestamp, t1->source_ip.high_order_bits, t1->source_ip.low_order_bits,
           t1->source_asn, t1->destination_ip.high_order_bits, t1->destination_ip.low_order_bits, t1->destination_asn,
           t1->path_id);
    for (int i = 0; i < 35; i++)
    {
        printf(HOP_FORMAT_OUT, t1->hops[i].returned_flowlabel, t1->hops[i].hopnumber, t1->hops[i].hop_address.high_order_bits, t1->hops[i].hop_address.low_order_bits);
    }
}

int serialize(char *filePath, traceroute *t)
{
    FILE *file;
    if ((file = fopen("waffles.dat", "w+")) == 0)
    {
        return 1;
    }

    /* Write to file */
    fprintf(file, TR_TEST_FORMAT_OUT, t->outgoing_tcp_port, t->timestamp, t->source_ip.high_order_bits, t->source_ip.low_order_bits,
            t->source_asn, t->destination_ip.high_order_bits, t->destination_ip.low_order_bits, t->destination_asn,
            t->path_id);
    for (int i = 0; i < 35; i++)
    {
        fprintf(file, HOP_FORMAT_OUT, t->hops[i].returned_flowlabel, t->hops[i].hopnumber, t->hops[i].hop_address.high_order_bits, t->hops[i].hop_address.low_order_bits);
    }
    return 0;
}

int deserialize(char *filePath, traceroute *t)
{
    FILE *file;
    if ((file = fopen("waffles.dat", "w+")) == 0)
    {
        return 1;
    }

    /* Read from file */
    rewind(file);
    fscanf(file, TR_TEST_FORMAT_IN, &t->outgoing_tcp_port, t->timestamp, &t->source_ip.high_order_bits, &t->source_ip.low_order_bits,
           &t->source_asn, &t->destination_ip.high_order_bits, &t->destination_ip.low_order_bits, &t->destination_asn,
           t->path_id);
    for (int i = 0; i < 35; i++)
    {
        fscanf(file, HOP_FORMAT_IN, &t->hops[i].returned_flowlabel, &t->hops[i].hopnumber, &t->hops[i].hop_address.high_order_bits, &t->hops[i].hop_address.low_order_bits);
    }
    puts("Done reading from file!");
    return 0;
}

int main(void)
{
    hop hop_arr1[35];
    hop hop_arr2[35];

    address a1_2 = {
        .high_order_bits = 77,
        .low_order_bits = 88,
    };
    address a1_3 = {
        .high_order_bits = 99,
        .low_order_bits = 11,
    };
    address a1 = {
        .high_order_bits = 55,
        .low_order_bits = 44,
    };

    hop h1_2 = {
        .returned_flowlabel = 34,
        .hopnumber = 52,
        .hop_address = a1,
    };

    hop h1 = {
        .returned_flowlabel = 34,
        .hopnumber = 52,
        .hop_address = a1,
    };

    traceroute t1 = {
        .outgoing_tcp_port = 443,
        .timestamp = "hello t1",
        .source_ip = a1_2,
        .source_asn = 4,
        .destination_ip = a1_3,
        .destination_asn = 59,
        .path_id = "path_id",
    };

    hop_arr1[0] = h1;
    hop_arr1[1] = h1_2;

    for (int i = 2; i < 35; i++)
    {
        address *a;
        a = malloc(sizeof(address));
        a->high_order_bits = 55;
        a->low_order_bits = 44;
        hop *h;
        h = malloc(sizeof(hop));
        h->returned_flowlabel = 30;
        h->hopnumber = 6;
        h->hop_address = *a;
        hop_arr1[i] = *h;
    }
    traceroute *t2 = malloc(sizeof(traceroute));

    // FILE *file;
    // if ((file = fopen("waffles.dat", "w+")) == 0)
    //{
    // return 1;
    //}

#ifdef TR_TEST

    /* Write to file */
    fprintf(file, TR_TEST_FORMAT_OUT, t1.outgoing_tcp_port, t1.timestamp, t1.source_ip.high_order_bits, t1.source_ip.low_order_bits,
            t1.source_asn, t1.destination_ip.high_order_bits, t1.destination_ip.low_order_bits, t1.destination_asn,
            t1.path_id);

    /* Read from file */
    rewind(file);
    fscanf(file, TR_TEST_FORMAT_IN, &t2->outgoing_tcp_port, t2->timestamp, &t2->source_ip.high_order_bits, &t2->source_ip.low_order_bits,
           &t2->source_asn, &t2->destination_ip.high_order_bits, &t2->destination_ip.low_order_bits, &t2->destination_asn,
           t2->path_id);

    printf(TR_TEST_FORMAT_OUT, t2->outgoing_tcp_port, t2->timestamp, t2->source_ip.high_order_bits, t2->source_ip.low_order_bits,
           t2->source_asn, t2->destination_ip.high_order_bits, t2->destination_ip.low_order_bits, t2->destination_asn,
           t2->path_id);

    fclose(file);

#endif

#ifdef HOP_TEST

    if ((file = fopen("hops.dat", "w+")) == 0)
    {
        return 1;
    }

    /* Write to file */
    for (int i = 0; i < 35; i++)
    {
        fprintf(file, HOP_FORMAT_OUT, hop_arr1[i].returned_flowlabel, hop_arr1[i].hopnumber, hop_arr1->hop_address.high_order_bits, hop_arr1->hop_address.low_order_bits);
    }
    // fprintf(file, "\n");
    puts("Done writing to file!");

    /* Read from file */
    rewind(file);
    // fscanf(file, HOP_FORMAT_IN, &hop_arr2[0].returned_flowlabel, &hop_arr2[0].hopnumber, &hop_arr2[0].hop_address.high_order_bits, &hop_arr2[0].hop_address.low_order_bits);
    // printf(HOP_FORMAT_OUT, hop_arr2[0].returned_flowlabel, hop_arr2[0].hopnumber, hop_arr2[0].hop_address.high_order_bits, hop_arr2[0].hop_address.low_order_bits);
    for (int i = 0; i < 35; i++)
    {
        fscanf(file, HOP_FORMAT_IN, &hop_arr2[i].returned_flowlabel, &hop_arr2[i].hopnumber, &hop_arr2[i].hop_address.high_order_bits, &hop_arr2[i].hop_address.low_order_bits);
    }
    puts("Done reading from file!");

    for (int i = 0; i < 35; i++)
    {
        printf(HOP_FORMAT_OUT, hop_arr2[i].returned_flowlabel, hop_arr2[i].hopnumber, hop_arr2[i].hop_address.high_order_bits, hop_arr2[i].hop_address.low_order_bits);
    }
#endif

#ifdef COMBINED_TEST

    /* Write to file */
    fprintf(file, TR_TEST_FORMAT_OUT, t1.outgoing_tcp_port, t1.timestamp, t1.source_ip.high_order_bits, t1.source_ip.low_order_bits,
            t1.source_asn, t1.destination_ip.high_order_bits, t1.destination_ip.low_order_bits, t1.destination_asn,
            t1.path_id);
    for (int i = 0; i < 35; i++)
    {
        fprintf(file, HOP_FORMAT_OUT, t1.hops[i].returned_flowlabel, t1.hops[i].hopnumber, t1.hops[i].hop_address.high_order_bits, t1.hops[i].hop_address.low_order_bits);
    }
    // fprintf(file, "\n");

    /* Read from file */
    rewind(file);
    fscanf(file, TR_TEST_FORMAT_IN, &t2->outgoing_tcp_port, t2->timestamp, &t2->source_ip.high_order_bits, &t2->source_ip.low_order_bits,
           &t2->source_asn, &t2->destination_ip.high_order_bits, &t2->destination_ip.low_order_bits, &t2->destination_asn,
           t2->path_id);
    for (int i = 0; i < 35; i++)
    {
        fscanf(file, HOP_FORMAT_IN, &t1.hops[i].returned_flowlabel, &t1.hops[i].hopnumber, &t1.hops[i].hop_address.high_order_bits, &t1.hops[i].hop_address.low_order_bits);
    }
    puts("Done reading from file!");

    printf(TR_TEST_FORMAT_OUT, t2->outgoing_tcp_port, t2->timestamp, t2->source_ip.high_order_bits, t2->source_ip.low_order_bits,
           t2->source_asn, t2->destination_ip.high_order_bits, t2->destination_ip.low_order_bits, t2->destination_asn,
           t2->path_id);
    for (int i = 0; i < 35; i++)
    {
        printf(HOP_FORMAT_OUT, t1.hops[i].returned_flowlabel, t1.hops[i].hopnumber, t1.hops[i].hop_address.high_order_bits, t1.hops[i].hop_address.low_order_bits);
    }

    fclose(file);
#endif
    char *filename = "waffles.dat";
    // traceroute *t = malloc(sizeof(traceroute));
    serialize(filename, &t1);
    deserialize(filename, &t1);
    printTraceroute(&t1);

    return 0;
}
