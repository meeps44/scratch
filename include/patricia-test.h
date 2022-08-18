#include "patricia.h"
#include <stdbool.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <errno.h>

typedef union _inx_addr
{
    struct in_addr sin;
    struct in6_addr sin6;
} inx_addr;

inline static prefix_t* make_prefix();

inline static bool set_prefix(prefix_t* subnet, int family, inx_addr* addr, unsigned int width);

inline static bool parse_cidr(const char *cidr, int *family, inx_addr *subnet, unsigned short *mask);

void init(bool arg_binary_lookup_mode);

int insert(int family, inx_addr subnet, unsigned short mask, int data);

int lookup_cidr(const char *cidr, int size);

int lookup_addr(int family, inx_addr addr);

bool get_binary_lookup_mode();

void set_binary_lookup_mode(bool arg_binary_lookup_mode);
