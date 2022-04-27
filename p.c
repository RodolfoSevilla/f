#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <netinet/in.h>

#define BUF_SIZE 500;

int main(int argc, char *argv[])
{

if (argc != 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(EXIT_FAILURE);
    }
int soc = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
int i = 1;
 setsockopt (soc,SOL_SOCKET,SO_BROADCAST,&i,sizeof(i));
char str[50] = "Online: ";

strcat(str, argv[1]) ;
//strcat(str, "\0");
printf("%s\n",str);
struct sockaddr_in hints;
    hints.sin_family = AF_INET;    // Allow IPv4 or IPv6
    hints.sin_port = htons(8221);
     inet_pton(PF_INET,"10.10.13.255",&hints.sin_addr);

     sendto(soc, str, strlen(str), 0, (const struct sockaddr *)&hints , sizeof(hints));
sleep(2);
char str2[50] = "Offline: ";
strncat(str2, argv[1],strlen(argv[1])) ;
strncat(str2, "\0", 1);
sendto(soc, str2, strlen(str2), 0, (const struct sockaddr *)&hints , sizeof(hints));
printf("%s\n",str2);
}
