#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <unistd.h>


char* get_ip_address() {
static char host[NI_MAXHOST]; // static to ensure it persists after function returns
struct ifaddrs *ifaddr, *ifa;
int family, s;
// Error if you can't get the interface address
if (getifaddrs(&ifaddr) == -1) {
perror("getifaddrs");
return NULL;
}
// Loop through each address
for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
// Skip the interface if it doesn't have an IP
if (ifa->ifa_addr == NULL)
continue;
family = ifa->ifa_addr->sa_family;
// IPv4 Addresses
if (family == AF_INET) {
s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
if (s != 0) {
printf("getnameinfo() failed: %s\n", gai_strerror(s));
continue;
}
// Skip loopback addresses (127.0.0.1)
if (strcmp(host, "127.0.0.1") != 0) {
freeifaddrs(ifaddr); // Free the memory used by getifaddrs
return host;
}
}
}
freeifaddrs(ifaddr); // Free the memory used by getifaddrs
return NULL; // No non-loopback IPv4 address found
}
