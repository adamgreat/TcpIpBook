#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>

void errorHandling(const char* message);

int main(int argc, char* argv[])
{
    if(argc != 3){
        printf("Usage: %s <ip> <port> \n", argv[0]);
        exit(1);
    }

    struct sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = inet_addr(argv[1]);
    sockAddr.sin_port = htons(atoi(argv[2]));

    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    
    int timeLive = 64;
    setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&timeLive, sizeof(timeLive));

    FILE* fp = fopen("news.txt", "r");
    if(fp == NULL){
        errorHandling("fopen() error!");
    }

    char buf[256];
    while(!feof(fp)){
        if(fgets(buf, sizeof(buf), fp) == NULL){
            errorHandling("fgets() error!");
        }
        sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)& sockAddr, sizeof(sockAddr));

        sleep(3);
    }

    fclose(fp);
    close(sock);
    return 0;
}

void errorHandling(const char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);

    exit(1);
}