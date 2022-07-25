#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* TRACEROUTE_FORMAT_IN = "%[^,], %d, %d";
const char* TRACEROUTE_FORMAT_OUT = "%s, %d, %d\n";

typedef struct ipv6_address  {
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
    char timestamp[50];
    address source_ip;
    uint32_t source_asn;
    address destination_ip;
    uint32_t destination_asn;
    uint8_t path_id[20];
    uint8_t hop_count;
    hop hops[35]; // maximum hop length is 35. any hops longer than that do not get included.
    // Could also be a list of *hop-pointers

} traceroute;

int main(void)
{
    traceroute t1 = {
        .outgoing_tcp_port = 1,
        .timestamp = NULL,
        .source_ip = NULL,
        .source_asn = 4,
        .destination_ip = NULL,
        .destination_asn = 52,
        .path_id = {0},
        .hops = NULL,
    };

    traceroute t2 = {
        .outgoing_tcp_port = 100,
        .timestamp = NULL,
        .source_ip = NULL,
        .source_asn = 4,
        .destination_ip = NULL,
        .destination_asn = 52,
        .path_id = {0},
        .hops = NULL,
    };

    FILE *file;
    int result;
    if ((result = fopen("test.txt", "w+")) == 0)
    {
        return 1;
    }

    /* Write to file */
    fprintf(file, TRACEROUTE_FORMAT_OUT, t1.timestamp, t1.hop_count, t1.destination_asn);

    /* Read in from file */
    fseek(file, 0, SEEK_SET); // Read from beginning of file.
    traceroute *t3 = malloc(sizeof(traceroute));
    // fread(t3, sizeof(traceroute), 1, file);
    fscanf(file, TRACEROUTE_FORMAT_IN, 50, t2.timestamp, t2.hop_count, t2.destination_asn);



    // EOF TEST //
    // ref.: https://www.tutorialspoint.com/c_standard_library/c_function_fopen.htm
    FILE *fp;
    int c;
    
    fp = fopen("file.txt","r");
    while(1) {
        c = fgetc(fp);
        if( feof(fp) ) { 
            break ;
        }
        printf("%c", c);
    }
    fclose(fp);
    // END EOF TEST //

    return EXIT_SUCCESS;
}