#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define SERVER "hmif.cf"
#define PORT 9999
#define NPROB 100


void recv_until(int sock, char *buf, char *data) {
   bzero((char *) buf, 512);
   int i = 0;
   while (strstr(buf, data) == NULL) {
	   read(sock, buf+i, 1);
	   i++;
   }
}

void getValidSubnet(char *subnet, char *host) {
   //puts(host);
   bzero((char *) subnet, 512);
   char * pch = strrchr(host,'.');
   int id_last_dot_found = pch-host;
   strncpy(subnet, host, id_last_dot_found+1);
   strcat(subnet,"0/24\n");

}

void countHosts(char *subnet, char *host) {
  //puts(subnet);
   bzero((char *) host, 512);
   char * pch = strrchr(subnet,'/');
   int id_last_dot_found = pch-subnet;
   //printf("STRLEN : %d\n",strlen(host));
   /*char n_mask[2];
   strcpy(n_mask, host+id_last_dot_found+1);
   puts(n_mask);*/
   int nmask = atoi(subnet+id_last_dot_found+1);
   long long nhost = 1;
   nhost = (nhost<<(32-nmask));
   char num[11];
   snprintf (num, sizeof(num), "%lld",nhost);

   strcat(host, num);
   strcat(host, "\n");

}

long long getBase10(char * addr,int nmask) {
  long long num = 0;
  int n,e_id,s_id=0,cnt=4;
  char bil[4];
  char * pch;
  for (int i = 0; i < 3; i++){
    bzero((char * ) bil, 4);
    pch = strchr(addr+s_id,'.');
    e_id = pch - addr;
    strncpy(bil, addr+s_id, e_id-s_id);
    n = atoi(bil);
    num += n;
    num = (num << 8);
    s_id = e_id+1;
  }
    //Last
    bzero((char *) bil,4);
    strcpy(bil,addr+s_id);
    n = atoi(bil);
    num += n;
    long long res = 1;
    res = (res << nmask) - 1;
    res = (res << (32-nmask));


    res = res & num;
    return res;
}
int isSubnetValid(char *subnet, char *host) {
    //puts(subnet);
    //puts(host);
    //printf("\n");
    char * pch = strrchr(subnet,'/');
    int id_last_dot_found = pch-subnet;
    int nmask = atoi(subnet+id_last_dot_found+1);


   long long i_subnet, i_host;
   i_subnet = getBase10(subnet,nmask);
   i_host = getBase10(host,nmask);
   long long res = i_subnet ^ i_host;
   if (res == 0) {
     return 1;
   } else return 0;

}

int main(int argc, char *argv[]) {
   int sockfd, portno, n, i;
   struct sockaddr_in serv_addr;
   struct hostent *server;

   char buffer[512];

   /* Create a socket point */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }

   server = gethostbyname(SERVER);

   if (server == NULL) {
      fprintf(stderr,"ERROR, no such host\n");
      exit(0);
   }

   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
   serv_addr.sin_port = htons(PORT);

   /* Now connect to the server */
   if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR connecting");
      exit(1);
   }

   recv_until(sockfd, buffer, "NIM: ");
   printf("NIM: ");
   scanf("%s", buffer);
   write(sockfd, buffer, strlen(buffer));

   recv_until(sockfd, buffer, "NIM: ");
   printf("Verify NIM: ");
   scanf("%s", buffer);
   write(sockfd, buffer, strlen(buffer));

   recv_until(sockfd, buffer, "\n");
   printf("%s", buffer);

   char host[512];
   char subnet[512];

   // Phase 1
   for (i = 0; i < NPROB; i++) {
      recv_until(sockfd, buffer, "Host: ");
      recv_until(sockfd, host, "\n");
      host[strlen(host)-1] = '\0';
      recv_until(sockfd, buffer, "Subnet: ");
      getValidSubnet(subnet, host);
      write(sockfd, subnet, strlen(subnet));
   }
   recv_until(sockfd, buffer, "\n");
   printf("%s", buffer);
   //printf("DONE PHASE-1\n");
   // Phase 2
   for (i = 0; i < NPROB; i++) {
      recv_until(sockfd, buffer, "Subnet: ");
      recv_until(sockfd, subnet, "\n");
      subnet[strlen(subnet)-1] = '\0';
      recv_until(sockfd, buffer, "Number of Hosts: ");
      countHosts(subnet, host);
      write(sockfd, host, strlen(host));
   }
   recv_until(sockfd, buffer, "\n");
   printf("%s", buffer);

   //printf("DONE PHASE-2\n");

   // Phase 3
   for (i = 0; i < NPROB; i++) {
      recv_until(sockfd, buffer, "Subnet: ");
      recv_until(sockfd, subnet, "\n");
      subnet[strlen(subnet)-1] = '\0';
      recv_until(sockfd, buffer, "Host: ");
      recv_until(sockfd, host, "\n");
      host[strlen(host)-1] = '\0';
      if (isSubnetValid(subnet, host)) {
         write(sockfd, "T\n", 2);
	  } else {
         write(sockfd, "F\n", 2);
	 }
   }
   recv_until(sockfd, buffer, "\n");
   printf("%s", buffer);
   close(sockfd);

   //printf("DONE PHASE-3\n");
   return 0;
}
