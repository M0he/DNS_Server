#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <cstring>
#include <netinet/in.h>
#include <ctype.h>
#include <iostream>
#include <netdb.h>
#include <ifaddrs.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>
#include "ThreadPool.h"
#include "name.h"
using namespace std;
int main(int argc,char *argv[])
{
    if (argc < 2) {
        cout<<"please input dns port!"<<endl;
        cout<<"case : program_name port"<<endl;
        return 1;
    }
    //获取dns端口号
    int dns_port = 0;
    int dns_port_off = 0;
    for (int i = 0; i < strlen(argv[1]); i++) {
        dns_port = *(argv[1] + i ) - '0' + dns_port * 10;       
    }
    //socket绑定
    int socketfd = socket(AF_INET,SOCK_DGRAM,0);
    perror("socket");
    //socket绑定的本地地址
    struct sockaddr_in local_dns_server_address;
    local_dns_server_address.sin_port = htons(dns_port);
    local_dns_server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    local_dns_server_address.sin_family = AF_INET;
    bind(socketfd, (struct sockaddr *)&local_dns_server_address, sizeof(struct sockaddr));
    perror("bind");

    //创建线程池
    ThreadPool<name> * pool = NULL;
    try{
        pool = new ThreadPool<name>;
    } catch(...) {
        printf("线程池内存分配错误！\n");
        return 1;
    }
    socklen_t len = sizeof(struct sockaddr);
    //开始工作
    while (true) {
    name *newName = new name(dns_port_off++, dns_port);
    newName->mainSocketfd = socketfd;
    int count = recvfrom(socketfd,newName->buf,sizeof(newName->buf),0,(struct sockaddr *)&(newName->clientAddress),&len);
    for (int i = 0; i < count ;i++)cout<< (uint16_t)((newName->buf)[i])<<":";
    cout<<endl;
    pool->list_append(newName);
    }
    close(socketfd);
    delete pool;
    return 0;
}

