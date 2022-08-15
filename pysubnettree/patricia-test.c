#include "patricia-test.h"
#include <stdio.h>

// typedef union _inx_addr
//{
// struct in_addr sin;
// struct in6_addr sin6;
//} inx_addr;

patricia_tree_t *tree;
bool binary_lookup_mode;

const uint8_t v4_mapped_prefix[12] = {0, 0, 0, 0,
                                      0, 0, 0, 0,
                                      0, 0, 0xff, 0xff};

inline static prefix_t *make_prefix()
{
    prefix_t *rval = (prefix_t *)malloc(sizeof(prefix_t));
    rval->ref_count = 1;
    return rval;
}

inline static bool set_prefix(prefix_t *subnet, int family, inx_addr *addr, unsigned int width)
{
    if (!(family == AF_INET || family == AF_INET6))
        return false;

    if (family == AF_INET && width > 32)
        return false;

    if (family == AF_INET6 && width > 128)
        return false;

    if (family == AF_INET)
    {
        memcpy(&subnet->add.sin6, v4_mapped_prefix, sizeof(v4_mapped_prefix));
        memcpy(&subnet->add.sin6.s6_addr[12], addr, sizeof(struct in_addr));
    }

    else if (family == AF_INET6)
        memcpy(&subnet->add.sin6, addr, sizeof(subnet->add.sin6));

    subnet->family = AF_INET6;
    subnet->bitlen = (family == AF_INET ? width + 96 : width);

    return true;
}

inline static bool parse_cidr(const char *cidr, int *family, inx_addr *subnet, unsigned short *mask)
{
    char buffer[40];
    const char *addr_str = 0;
    const char *mask_str = 0;
    char *endptr;

    if (!cidr)
        return false;

    const char *slash = strchr(cidr, '/');

    if (slash)
    {
        int len = slash - cidr < 40 ? slash - cidr : 39;
        memcpy(buffer, cidr, len);
        buffer[len] = '\0';
        addr_str = buffer;
        mask_str = slash + 1;
    }
    else
    {
        addr_str = cidr;
        mask_str = 0;
    }

    *family = AF_INET;

    if (inet_pton(*family, addr_str, subnet) != 1)
    {
        *family = AF_INET6;

        if (inet_pton(*family, addr_str, subnet) != 1)
            return false;
    }

    if (mask_str)
    {
        errno = 0;
        *mask = strtol(mask_str, &endptr, 10);

        if (endptr == mask_str || errno != 0)
            return false;

        if (*family == AF_INET && *mask > 32)
            return false;
        else if (*mask > 128)
            return false;
    }
    else
    {
        if (*family == AF_INET)
            *mask = 32;
        else
            *mask = 128;
    }

    return true;
}

void init(bool arg_binary_lookup_mode)
{
    tree = New_Patricia(128);
    binary_lookup_mode = arg_binary_lookup_mode;
}

int insert(int family, inx_addr subnet, unsigned short mask, int data)
{
    prefix_t *sn = make_prefix();

    if (!sn)
    {
        perror("Memory allocation failed\n");
        return 0;
    }

    bool res = set_prefix(sn, family, &subnet, mask);

    if (!res)
    {
        Deref_Prefix(sn);
        perror("invalid subnet/prefix\n");
        return 0;
    }

    patricia_node_t *node = patricia_lookup(tree, sn);
    Deref_Prefix(sn);

    if (!node)
    {
        perror("patricia_lookup failed.\n");
        return 0;
    }

    if (!data)
        data = 0;

    node->data = (void *)&data;

    return 1;
}

int lookup_addr(int family, inx_addr addr)
{
    prefix_t *subnet = make_prefix();

    if (!subnet)
    {
        perror("invalid subnet/prefix");
        return 0;
    }

    int mask = family == AF_INET ? 32 : 128;
    bool res = set_prefix(subnet, family, &addr, mask);

    if (!res)
    {
        Deref_Prefix(subnet);
        perror("out of memory");
        return 0;
    }

    patricia_node_t *node = patricia_search_best(tree, subnet);
    Deref_Prefix(subnet);

    if (!node)
        return 0;

    int data = *(int *)node->data;

    return data;
}

bool get_binary_lookup_mode()
{
    return binary_lookup_mode;
}

void set_binary_lookup_mode(bool arg_binary_lookup_mode)
{
    binary_lookup_mode = arg_binary_lookup_mode;
}

int main(void)
{
    init(false);

    struct in6_addr *my_addr1 = calloc(1, sizeof(struct in6_addr));
    unsigned char *example_address = "2001:200:c000::";
    inet_pton(AF_INET6, example_address, my_addr1);
    unsigned short my_mask = 35;
    int my_asn = 23634;
    insert(AF_INET6, (inx_addr)*my_addr1, my_mask, my_asn);

    struct in6_addr *my_addr2 = calloc(1, sizeof(struct in6_addr));
    example_address = "1900:5:2:2::5170";
    inet_pton(AF_INET6, example_address, my_addr2);
    my_mask = 126;
    my_asn = 3356;
    insert(AF_INET6, (inx_addr)*my_addr2, my_mask, my_asn);

    struct in6_addr *my_addr3 = calloc(1, sizeof(struct in6_addr));
    example_address = "1900:2100::2a2c";
    inet_pton(AF_INET6, example_address, my_addr3);
    my_mask = 126;
    my_asn = 3357;
    insert(AF_INET6, (inx_addr)*my_addr3, my_mask, my_asn);

    struct in6_addr *my_addr4 = calloc(1, sizeof(struct in6_addr));
    example_address = "2001::";
    inet_pton(AF_INET6, example_address, my_addr4);
    my_mask = 32;
    my_asn = 6939;
    insert(AF_INET6, (inx_addr)*my_addr4, my_mask, my_asn);

    // struct in6_addr *my_addr5 = calloc(1, sizeof(struct in6_addr));
    // example_address = "2001:200:e00::";
    // inet_pton(AF_INET6, example_address, my_addr5);
    // my_mask = 40;
    // my_asn = 4690;
    // insert(AF_INET6, (inx_addr)*my_addr5, my_mask, my_asn);

    struct in6_addr bar;
    unsigned char *example_address2 = "1900:2100::2a2c";
    inet_pton(AF_INET6, example_address2, &bar);
    int lookup_result = lookup_addr(AF_INET6, (inx_addr)bar);
    printf("Lookup result (returned ASN):\t%d\n", lookup_result);

    return 0;
}