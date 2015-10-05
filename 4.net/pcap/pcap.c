#include <pcap.h>
#include <stdio.h>

#include <pcap.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define IS_VALID() do { \
	if( 184 != packet[0] && 184 != packet[0 + ETHER_ADDR_LEN]) \
     	return; \
	} while(0)

#define IS_IP() do { \
	struct sniff_ip *ip = (struct sniff_ip*)(packet + SIZE_ETHERNET); \
	if( 4 != ip->ip_vhl ) \
     	return; \
	} while(0)


/* Ethernet addresses are 6 bytes */
#define ETHER_ADDR_LEN	6
#define SIZE_ETHERNET 14

/* Ethernet header */
struct sniff_ethernet 
{
	u_char ether_dhost[ETHER_ADDR_LEN]; /* Destination host address */
	u_char ether_shost[ETHER_ADDR_LEN]; /* Source host address */
	u_short ether_type; /* IP? ARP? RARP? etc */
};

/* IP header */
struct sniff_ip 
{
	u_char ip_vhl;		/* version << 4 | header length >> 2 */
	u_char ip_tos;		/* type of service */
	u_short ip_len;		/* total length */
	u_short ip_id;		/* identification */
	u_short ip_off;		/* fragment offset field */
#define IP_RF 0x8000		/* reserved fragment flag */
#define IP_DF 0x4000		/* dont fragment flag */
#define IP_MF 0x2000		/* more fragments flag */
#define IP_OFFMASK 0x1fff	/* mask for fragmenting bits */
	u_char ip_ttl;		/* time to live */
	u_char ip_p;		/* protocol */
	u_short ip_sum;		/* checksum */
	struct in_addr ip_src;
    struct in_addr ip_dst; /* source and dest address */
};
#define IP_HL(ip)		(((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)		(((ip)->ip_vhl) >> 4)

/* TCP header */
typedef u_int tcp_seq;

struct sniff_tcp 
{
	u_short th_sport;	/* source port */
	u_short th_dport;	/* destination port */
	tcp_seq th_seq;		/* sequence number */
	tcp_seq th_ack;		/* acknowledgement number */
	u_char th_offx2;	/* data offset, rsvd */
#define TH_OFF(th)	(((th)->th_offx2 & 0xf0) >> 4)
	u_char th_flags;
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80
#define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
	u_short th_win;		/* window */
	u_short th_sum;		/* checksum */
	u_short th_urp;		/* urgent pointer */
};

int t = 1;

void got_packet(u_char *args, const struct pcap_pkthdr *header,
	    const u_char *packet);

void show_ethernet( const u_char *packet );
void show_ip( const u_char *packet );
void show_tcp( const u_char *packet );

int main(int argc, char *argv[])
{
	pcap_t *handle;			/* Session handle */
	char *dev = "eth0";			/* The device to sniff on */
	char errbuf[PCAP_ERRBUF_SIZE];	/* Error string */
	struct bpf_program fp;		/* The compiled filter */
	//char filter_exp[] = "port 38959";	/* The filter expression */
	char filter_exp[] = "port 40278";	/* The filter expression */
	/* char filter_exp[] = "ip";	/* The filter expression */
	bpf_u_int32 mask;		/* Our netmask */
	bpf_u_int32 net;		/* Our IP */
	struct pcap_pkthdr header;	/* The header that pcap gives us */
	const u_char *packet;		/* The actual packet */


	/* Define the device */

/*
	dev = pcap_lookupdev(errbuf);
	if (dev == NULL) {
		fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
		return(2);
	}
*/


	/* Find the properties for the device */
	if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
		fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuf);
		net = 0;
		mask = 0;
	}


	/* Open the session in promiscuous mode */
	handle = pcap_open_live(dev, BUFSIZ, /*malicious*/0, 1000, errbuf);
	if (handle == NULL) {
		fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
		return(2);
	}


	/* Compile and apply the filter */

	if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
		fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
		return(2);
	}

	if (pcap_setfilter(handle, &fp) == -1) {
		fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
		return(2);
	}


    pcap_loop(handle, 1000, got_packet, NULL);


	/* And close the session */
	pcap_close(handle);
	return(0);
 }

void got_packet(u_char *args, const struct pcap_pkthdr *header,
	    const u_char *packet)
{

    int size_ip = 0, size_tcp = 0;
    int i = 0, pos = 0;

    struct sniff_ip *ip = NULL;
    struct sniff_tcp *tcp = NULL;
    char *payload = NULL;

/*
    IS_VALID();
    IS_IP();
*/


    printf("\n\n-------------------------------------\n");
    printf("Got a packet: %d\n\n", t++);    

    show_ethernet( packet );
    show_ip( packet );


/*
	ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
	size_ip = IP_HL(ip)*4;
	if (size_ip < 20) {
		printf("   * Invalid IP header length: %u bytes\n", size_ip);
		return;
	}
	tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
	size_tcp = TH_OFF(tcp)*4;
	if (size_tcp < 20) {
		printf("   * Invalid TCP header length: %u bytes\n", size_tcp);
		return;
	}
	payload = (u_char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);
*/

    printf("-------------------------------------\n");
}

void show_ethernet( const u_char *packet )
{
    int i = 0;
    struct sniff_ethernet *ethernet = (struct sniff_ethernet*)(packet);

    printf("Ethernet:\n    ETHER_ADDR_LEN: %d\n", ETHER_ADDR_LEN );
    printf("    src MAC: ");

    for( i = 0; i < 6; i++)
    {
        if ( 5 == i )
            printf("%02x\n", (unsigned)ethernet->ether_dhost[i]);
        else
        	printf("%02x:", (unsigned)ethernet->ether_shost[i]);
    }

    printf("    dst MAC: ");
    for( i = 0; i < 6; i++)
    {
        if ( 5 == i )
            printf("%02x\n", (unsigned)ethernet->ether_dhost[i]);
        else
        	printf("%02x:", (unsigned)ethernet->ether_dhost[i]);
    }

    printf("    Version: %d\n", ethernet->ether_type);
}


void show_ip( const u_char *packet )
{


    struct sniff_ip *ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
    struct sniff_tcp *tcp = NULL;

    int i = 0;
    int ip_size = 100;
    int size_ip , size_tcp;

    printf("IP:\n");

    /*
    printf("    IP version: %u %d <<4:%d >>2%d", *ver, ip->ip_vhl, ip->ip_vhl << 4, ip->ip_vhl >> 2);
	*/

    unsigned char *tmp = (unsigned char *)(packet + SIZE_ETHERNET);

    tmp = (unsigned char *)(packet + SIZE_ETHERNET + 12);
    printf("    src:");
    for( i = 0; i < 4; i++)
    {
        if ( 3 == i )
            printf("%d\n", tmp[i]);
        else
        	printf("%d.", tmp[i]);
    }

	tmp = (unsigned char *)(packet + SIZE_ETHERNET + 16);
	printf("    dst:");
	for( i = 0; i < 4; i++)
	{
		if ( 3 == i )
			printf("%d\n", tmp[i]);
		else
			printf("%d.", tmp[i]);
	}
	printf("\n");

    /* print source and destination IP addresses */
    printf("       Vers: %d\n", ip->ip_vhl >> 4);
	printf("       From: %s\n", inet_ntoa(ip->ip_src));
	printf("         To: %s\n", inet_ntoa(ip->ip_dst));

	/* determine protocol */
	switch(ip->ip_p) {
		case IPPROTO_TCP:
			printf("   Protocol: TCP\n");
			break;
		case IPPROTO_UDP:
			printf("   Protocol: UDP\n");
			return;
		case IPPROTO_ICMP:
			printf("   Protocol: ICMP\n");
			return;
		case IPPROTO_IP:
			printf("   Protocol: IP\n");
			return;
		default:
			printf("   Protocol: unknown\n");
			return;
	}


	ip_size = 4*(ip->ip_vhl & 0x0f);

	if(IPPROTO_TCP != ip->ip_p || ip_size > 20)
		return;

	printf("TCP:\n");
	unsigned char* tcp_t = (unsigned char *)(packet + SIZE_ETHERNET + ip_size);
	uint16_t *sport = (uint16_t *)tcp_t;
	printf("    IP size:%d\n", ip_size);
	printf("    src Port:%d %d\n", sport[0], ntohs(sport[0]));
	printf("    dst Port:%d %d\n", sport[1], ntohs(sport[1]));




	size_ip = IP_HL(ip)*4;
	if (size_ip < 20) {
		printf("   * Invalid IP header length: %u bytes\n", size_ip);
		return;
	}


	tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
	size_tcp = TH_OFF(tcp)*4;
	if (size_tcp < 20) {
		printf("   * Invalid TCP header length: %u bytes\n", size_tcp);
		return;
	}

	printf("   Src port: %d\n", ntohs(tcp->th_sport));
	printf("   Dst port: %d\n", ntohs(tcp->th_dport));




}

void show_tcp( const u_char *packet )
{



}



















