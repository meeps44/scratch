#include <stdint.h>

typedef struct ipv6_address
{
    long high_order_bits;
    long low_order_bits;
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
    char timestamp[30];
    address source_ip;
    uint32_t source_asn;
    address destination_ip;
    uint32_t destination_asn;
    char path_id[20];
    uint32_t hop_count;

    hop hops[35]; // maximum hop length is 35. any hops longer than that do not get included.

} traceroute;