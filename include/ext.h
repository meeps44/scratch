#ifndef EXT_H
#define EXT_H
#include <stdint.h>
#include <openssl/sha.h> // SHA1

#define HOP_MAX 35

typedef struct ipv6_address
{
    uint16_t address_short[8];
} address;

typedef struct hop
{
    uint32_t returned_flowlabel;
    uint8_t hopnumber;
    address *hop_address; // Could be a list of address pointers
} hop;

typedef struct traceroute
{
    // For easy route comparison - make a hash of the (source_ip, dest_ip, outgoing_flow_label)-tuple and add it
    // as a variable to the struct?
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
    address *source_ip;
    uint32_t source_asn;
    address *destination_ip;
    uint32_t destination_asn;
    uint8_t path_id[SHA_DIGEST_LENGTH];
    uint8_t hop_count;
    hop *hops[HOP_MAX]; // maximum hop length is 35. any hops longer than that do not get included.
    // Could also be a list of *hop-pointers

} traceroute;

/**
 * @brief Create an address object initialized to zero
 *
 * @return address* Pointer to the new address object
 */
address *createAddress(void);

/**
 * @brief Create a Traceroute object initialized to zero
 *
 * @return traceroute*
 */
traceroute *createTraceroute(void);

/**
 * @brief Create a Hop object initalized to zero
 *
 * @return hop*
 */
hop *createHop(void);

/**
 * @brief Prints a hash digest to a unsigned char buffer.
 * The buffer should be at least size 41 or greater.
 * NB! For successful memcmp() between two buffers, the buffer size needs to
 *  be exactly size 41.
 *
 * @param digest The hash digest in the form of an array of unsigned
 * characters of size SHA_DIGEST_LENGTH.
 * @param s The destination buffer.
 */
void sPrintHash(uint8_t *digest, char *s);

/**
 * @brief Prints a hash digest to STDOUT.
 *
 * @param digest The hash digest in the form of an array of unsigned
 * characters of size SHA_DIGEST_LENGTH.
 */
void printHash(uint8_t digest[]);

/**
 * @brief Creates a hash of all the hops in a path and returns the resulting
 * digest.
 * We define a path as an ordered, indexed set of hops to a destination.
 *
 * @param l Ordered list of address pointers that combined comprise a path.
 * @return uint8_t* List containing the newly created 20-char long SHA1 digest.
 *
 * NB! Code must be linked with libopenSSL in order for this to work.
 * Linkage example: gcc sha1-in-c.c -lcrypto
 */
uint8_t *hashPath(address arr[], int arraySize);

/**
 * @brief Performs ASN-lookup of a given IPv6-address.
 *
 * @param ipv6_address The IPv6-address on which to lookup.
 * @return int The AS number associated with this address
 */
int asnLookup(address *ipv6_address);

/**
 * @brief Parses a raw IPv6-packet and saves the result to a parsed_packet struct.
 *
 * @param packet
 * @return parsed_packet*
 */
// parsed_packet *parseIPv6(packet_t packet);

/**
 * @brief Prints each individual field of a parsed_packet to STDOUT.
 *
 * @param p
 */
// void printParsedPacket(parsed_packet *p);

/**
 * @brief Get the Flow Label value of a parsed_packet.
 *
 * @param p
 * @return int
 */
int getFlowLabel(address *a);

/**
 * @brief Loads a traceroute object at OFFSET offset in FILE filename
 * into memory and returns a pointer to the newly created
 * object.
 *
 * @param fileName
 */
traceroute *readTracerouteFromFile(char *filename, long offset);

/**
 * @brief Write a traceroute object to file in binary format.
 * Uses file locking to avoid race conditions
 *
 * @param t
 */
int writeTracerouteToFile(traceroute *t, char *filename);

/**
 * @brief Prints each individual field of a traceroute object to STDOUT.
 *
 * @param t
 */
int printTraceroute(traceroute *t);

/**
 * @brief Prints each individual field of an address object to STDOUT
 *
 * @param a
 * @return int
 */
int printAddress(address *a);

/**
 * @brief Prints each individual field of a hop object to STDOUT
 *
 * @param h
 * @return int
 */
int printHop(hop *h);

/**
 * @brief Convert a traceroute object to JSON.
 *
 * @param t
 * @return char*
 */
char *tracerouteToJSON(traceroute *t);

/**
 * @brief Gets the current time (GMT)
 *
 * @return A struct representation of the current time in GMT
 */
struct tm *getCurrentTime(void);

/**
 * @brief Appends hop-object to the next available spot in the
 * hops-array. Returns -1 if the array is full.
 *
 * @param h
 * @param t
 * @return int
 */
int appendHop(hop *h, traceroute *t);

/**
 * @brief Appends an address-object to the next available spot in the
 * hops-array. Returns -1 if the array is full, 0 if executed correctly.
 *
 * @param a
 * @param t
 * @return int Returns -1 if the array is full, 0 if executed correctly with no
 * errors.
 */
int appendAddress(address *a, traceroute *t, uint8_t hopnumber, uint32_t returned_flowlabel);

/**
 * @brief Converts an IPv6 address object to a C-string (null-terminated
 * array of characters).
 *
 * @param a Pointer to IPv6 address object
 * @return char* Pointer to the first character in the character array
 */
char *addressToString(address *a);

/**
 * @brief Loads a .pt-file into memory and converts its content into an
 * array (or linked-list, or hashmap) of Traceroute-objects.
 *
 * @param filename
 * @return int 0 on success, -1 on error
 */
int ptFileToTraceroute(char *filename);

/**
 * @brief Compares two traceroute paths and returns the hop-number (index)
 * where they diverged. If the paths did not diverge (they are equal), return 0.
 *
 * @param t1
 * @param t2
 * @return int
 */
int compareIndexedPaths(traceroute *t1, traceroute *t2);

/**
 * @brief Compares two traceroute paths.
 *
 * @param t
 * @return 1 if equal, 0 if not equal, -1 on error.
 */
int comparePaths(traceroute *t1, traceroute *t2);

/**
 * @brief Compares all paths to [source], [destination] in file1 against
 * all paths with the same [source], [destination] pair in file2 and
 * returns the hop-number (index) where they diverged. If the paths did not
 * diverge (they are equal), print EQUAL.
 *
 * @param file1
 * @param file2
 * @return *char[] Array of strings in the format:
 * Paths [source], [destination] in [file1], [file2] were EQUAL
 * Paths [source], [destination] in [file1], [file2] were NOT EQUAL. Diverged
 * at: [hopnumber].
 */
char **fCompareIndexedPaths(char *file1, char *file2);

/**
 * @brief Compares all paths to [source], [destination] in file1 against
 * all paths with the same [source], [destination] pair in file2.
 *
 * @param file1
 * @param file2
 * @return *char[] Array of strings in the format:
 * Paths [source], [destination] in [file1], [file2] were EQUAL
 * Paths [source], [destination] in [file1], [file2] were NOT EQUAL
 */
char **fComparePaths(char *file1, char *file2);

/**
 * @brief Creates a filename in the form HOSTNAME-CURRENT_TIME.
 * The filename length is limited to 100 characters.
 *
 * @return Pointer to the filename.
 */
char *createFileName(struct tm *now); // (Might not be needed)

/**
 * @brief Compares two hops and checks if they are equal. (Function
 * might not be needed).
 *
 * @param h1
 * @param h2
 * @return int 1 if equal, 0 if not equal.
 */
int compareHops(hop *h1, hop *h2);

/**
 * @brief Compares two address objects and checks if they are equal.
 *
 * @param a1 Pointer to the first address object.
 * @param a2 Pointer to the second address object.
 * @return int 1 if equal, 0 if not equal.
 */
int compareAddresses(address *a1, address *a2);

/**
 * @brief Writes all traceroute objects in tr_array to filename.
 *
 * @param filename
 * @param tr_arr
 * @param arraySize
 * @return int
 */
int writeTracerouteArrayToFile(char *filename, traceroute *tr_arr[], int arraySize);

/**
 * @brief Reads arraySize number of traceroute objects from filename into
 * array tr_arr.
 *
 * @param filename
 * @param tr_arr
 * @param arraySize
 * @return int
 */
int readTracerouteArrayFromFile(char *filename, traceroute *tr_arr[], int arraySize);

#endif