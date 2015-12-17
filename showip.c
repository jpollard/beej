#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];
 
    if (argc != 2) {
        fprintf(stderr, " usage: showip hostname\n");
        return 1;
    } 

    memset(&hints, 0, sizeof hints); // zeros out the hints struct
    hints.ai_family = AF_UNSPEC;     // AF_INET or AF_INET6 to force ip4 or ip6
    hints.ai_socktype = SOCK_STREAM; // TCP - SOCK_DATA = UDP
    
    if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    printf("IP addresses for %s\n\n", argv[1]);
   
    for (p = res; p != NULL; p = p->ai_next) {
        void *addr;
        char *ipver;

        // get pointer to the address itself,
	// different field in ipv5 and ipv6:
        if (p->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf(" %s: %s\n", ipver, ipstr);
    }

    freeaddrinfo(res);
  
    return 0;
}

