#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <errno.h>
#include "hashmap.h"
#include "ext.h"

typedef struct routeviews_prefix
{
    char prefix[50];
} routeviews_prefix;

typedef struct prefix_pair
{
    int asn;
    routeviews_prefix ap;
} prefix_pair;

char *addressToString(struct in6_addr ipv6addr)
{
    // struct sockaddr_storage addr;
    // socklen_t addr_len = sizeof(addr);
    // struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)&addr;
    // struct in6_addr ipv6addr = addr6->sin6_addr;

    // For both IPv4 and IPv6 the address passed in must be in network byte order (most significant byte first).
    char buffer[INET6_ADDRSTRLEN];
    const char *result = inet_ntop(AF_INET6, &ipv6addr, buffer, sizeof(buffer));
    if (result == 0)
    {
        printf(stderr, "failed to convert address to string (errno=%d)", errno);
        return NULL;
    }
    return buffer;
}

/**
 * @brief
 *
 * @param p
 * @param h
 * @return int
 */
int getASN(char *longestMatchingPrefix, struct hashmap *h)
{
    int asn;

    return asn;
}

/**
 * @brief Get the Longest Matching Network Prefix
 *
 * @param a
 * @return char*
 */
char *getLongestMatchingPrefix(address *a)
{
    char *lmp = malloc(sizeof(char) * 50);

    return lmp;
}

/**
 * @brief Fills a given hashmap with key = routeviews IPv6 prefix,
 * value = ASN.
 *
 * @param filepath
 * @param h
 * @return int
 */
int fillHashmap(char *filepath, struct hashmap *h)
{
    FILE *file;
    char *line;
    size_t len = 0;
    ssize_t read;

    file = fopen(filepath, "r");
    while ((read = getline(&line, &len, file)) != -1)
    {
        printf("Retrieved line of length %zu:\n", read);
        printf("%s", line);
    }

    // Create address prefix:ASN-pair from line

    // Add newly created address prefix to hashmap
}

int user_compare(const void *a, const void *b, void *udata)
{
    const struct user *ua = a;
    const struct user *ub = b;
    return strcmp(ua->name, ub->name);
}

bool user_iter(const void *item, void *udata)
{
    const struct user *user = item;
    printf("%s (age=%d)\n", user->name, user->age);
    return true;
}

uint64_t user_hash(const void *item, uint64_t seed0, uint64_t seed1)
{
    const struct user *user = item;
    return hashmap_sip(user->name, strlen(user->name), seed0, seed1);
}

int createHashmap(char *filepath)
{
    // create a new hash map where each item is a `struct user`. The second
    // argument is the initial capacity. The third and fourth arguments are
    // optional seeds that are passed to the following hash function.
    struct hashmap *map = hashmap_new(sizeof(struct user), 0, 0, 0,
                                      user_hash, user_compare, NULL, NULL);

    // Here we'll load some users into the hash map. Each set operation
    // performs a copy of the data that is pointed to in the second argument.
    hashmap_set(map, &(struct user){.name = "Dale", .age = 44});
    hashmap_set(map, &(struct user){.name = "Roger", .age = 68});
    hashmap_set(map, &(struct user){.name = "Jane", .age = 47});

    struct user *user;

    printf("\n-- get some users --\n");
    user = hashmap_get(map, &(struct user){.name = "Jane"});
    printf("%s age=%d\n", user->name, user->age);

    user = hashmap_get(map, &(struct user){.name = "Roger"});
    printf("%s age=%d\n", user->name, user->age);

    user = hashmap_get(map, &(struct user){.name = "Dale"});
    printf("%s age=%d\n", user->name, user->age);

    user = hashmap_get(map, &(struct user){.name = "Tom"});
    printf("%s\n", user ? "exists" : "not exists");

    printf("\n-- iterate over all users (hashmap_scan) --\n");
    hashmap_scan(map, user_iter, NULL);

    printf("\n-- iterate over all users (hashmap_iter) --\n");
    size_t iter = 0;
    void *item;
    while (hashmap_iter(map, &iter, &item))
    {
        const struct user *user = item;
        printf("%s (age=%d)\n", user->name, user->age);
    }

    hashmap_free(map);
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