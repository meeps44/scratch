#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "hashmap.h"
#include "ext.h"

typedef struct
{
    char *prefix;
} address_prefix;

int asnLookup(address_prefix *p)
{
}

int createHashmap(char *filepath)
{
}

address_prefix *getLongestMatchingPrefix(address *a)
{
}

int main(void)
{
    // Step 1: Create hashmap based on routeviews-data
    // Step 1.5: Fill hashmap with key = network prefix, value = ASN
    // Step 2: Get the longest matching prefix for a given IPv6-address
    // Step 3: Match the resolved network prefix to a key in the hashmap and
    // get the resulting ASN.
    return EXIT_SUCCESS;
}