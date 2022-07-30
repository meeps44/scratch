#include "tpl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ipv6_address
{
    uint16_t address_short[8];
} address;

typedef struct hop
{
    uint32_t returned_flowlabel;
    uint8_t hopnumber;
    address hop_address; // Could be a list of address pointers
} hop;

typedef struct traceroute
{
    /*
        "outgoing_tcp_port": "443",
        "flow_label": "1048575",
        "timestamp": "2022-05-07 15:50:47.559550",
        "source": "2a03:b0c0:1:d0::b45:6001",
        "source_asn": "14061",
        "destination": "2600:9000:20a5:f800:10:15f0:8cc0:93a1",
        "destination_asn": "16509",
        "path_id": "c0f8ed8a7c1f3d725bd89de7ed7eced0b9dcc67b",
    */
    uint16_t outgoing_tcp_port;
    char *timestamp;
    address source_ip;
    uint32_t source_asn;
    address destination_ip;
    uint32_t destination_asn;
    char *path_id;
    uint32_t hop_count;

    hop hops[35]; // maximum hop length is 35. any hops longer than that do not get included.

} traceroute;

typedef struct person
{
    char *name;
    int age;
} person;

int serialize(person *p[], int arrayLength)
{
    tpl_node *tn;
    tn = tpl_map("S(ci)#", p, arrayLength);   /* fixed-length array of 100 structures */
    tpl_pack(tn, 0);                          // pack the elements
    tpl_dump(tn, TPL_FILE, "traceroute.tpl"); // write to file
    tpl_free(tn);                             // free memory
}

int deserialize(person *p[], int arrayLength)
{
    tpl_node *tn;
    tn = tpl_map("S(ci)#", p, arrayLength); /* fixed-length array of 100 structures */

    tpl_load(tn, TPL_FILE, "traceroute.tpl"); // load from file
    int i = 0;
    while (tpl_unpack(tn, 1) > 0)
    {
        /* got another element */
        printf("i is %s %d\n", p[i]->name, p[i]->age); /* unpack index 1 */
        i++;
    }
    tpl_free(tn); // free memory
}

void initPerson(person *p, char *name, int age)
{
    p->name = name;
    p->age = age;
}

int main(void)
{
    int length = 20000;
    person arr[length];
    // person p1 = {
    // .name = "Jerry",
    // .age = 50,
    // };
    // person p2 = {
    // .name = "Anne",
    // .age = 32,
    // };
    // arr[0] = p1;
    // arr[1] = p2;

    person *p;
    for (int i = 0; i < length; i++)
    {
        p = malloc(sizeof(person));
        initPerson(p, "Steve", i);
        arr[i] = *p;
    }

    // for (int i = 0; i < length; i++)
    //{
    // arr[i] = malloc(sizeof(person));
    //}
    // initPerson(arr[0], "John", 22);
    // initPerson(arr[1], "Steve", 54);

    // serialize(arr, length);
    // deserialize(arr, length);

    /* Serialize */
    tpl_node *tn;
    tn = tpl_map("S(si)#", arr, length); /* pass structure address */
    tpl_pack(tn, 0);
    tpl_dump(tn, TPL_FILE, "/home/erlend/dev/c/projects/test/tpl-test/struct.tpl");
    tpl_free(tn);
    puts("Done serializing!");

    /* Deserialize */
    person arr2[length];
    tn = tpl_map("S(si)#", arr2, length); /* fixed-length array of 100 structures */
    tpl_load(tn, TPL_FILE,
             "/home/erlend/dev/c/projects/test/tpl-test/struct.tpl"); // load from file
    tpl_unpack(tn, 0);                                                /* unpack all elements at once */
    // for (int i = 0; i < length; i++)
    //{
    // printf("%s, %d\n", arr2[i].name, arr2[i].age);
    //}

    // int i = 0;
    // while (tpl_unpack(tn, 1) > 0)
    // {
    // /* got another element */
    // printf("i is %s %d\n", arr[i].name, arr[i].age);
    // i++;
    // }
    tpl_free(tn); // free memory

    // BEGIN NESTED STRUCT TEST //
    /* Serialize */
    traceroute TR_ARR_OUT[length];
    tpl_node *tn;
    // S(vs$(v)#u$(v)#usu$(uu$(j)#)#
    tn = tpl_map("S(vs$(v)#u$(v)#usu$(uu$(j)#)#", TR_ARR_OUT, length); /* pass structure address */
    // tn = tpl_map("S(c$(c))#", TR_ARR_OUT, length);                     /* pass structure address */
    tpl_pack(tn, 0);
    tpl_dump(tn, TPL_FILE,
             "/home/erlend/dev/c/projects/test/tpl-test/struct.tpl");
    tpl_free(tn);
    puts("Done serializing!");

    /* Deserialize */
    traceroute TR_ARR_IN[length];
    tn = tpl_map("S(c$(c))#", TR_ARR_IN, length); /* fixed-length array of 100 structures */
    tpl_load(tn, TPL_FILE,
             "/home/erlend/dev/c/projects/test/tpl-test/struct.tpl"); // load from file
    tpl_unpack(tn, 0);                                                /* unpack all elements at once */
    tpl_free(tn);                                                     // free memory
    // END NESTED STRUCT TEST //

    return EXIT_SUCCESS;
}