#include "patricia-test.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    struct in6_addr my_addr;
    unsigned short my_mask;
    int my_asn;
    init(false);

    insert(AF_INET6, (inx_addr)my_addr, my_mask, my_asn);
    int lookup_result = lookup_addr(AF_INET6, (inx_addr)my_addr);

    printf("Lookup result (returned ASN):\t%d\n", lookup_result);
    return 0;
}