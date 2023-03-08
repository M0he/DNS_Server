#pragma once
#include<iostream>
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
#include <deque>
#include<list>
#include<unordered_map>
#include<vector>
#include<algorithm>
using namespace std;
class name {
    public:
    uint8_t buf[1024];
    uint8_t sendbuf[1024];
    int socketfd;
    int mainSocketfd;
    struct sockaddr_in clientAddress;
    struct sockaddr_in serverAddress;
    struct sockaddr_in localAddress;
    socklen_t len;

    public:
    name(int off, int dns_port);
    void process();

    private:
    vector<uint8_t *> nameServerdueue;//权威域名服务器域名
    deque<uint8_t *> nameServerIp;//权威域名服务器ip地址
    list<uint8_t *>answer_ptr;//标记回答开始位置
    list<uint8_t *>ns_ptr;//标记权威开始位置
    list<uint8_t *>temp_ns_ptr;//标记权威开始位置
    unordered_map<uint8_t *,int> ns_ptr_map;
    
    unordered_map<uint8_t *,int> answer_ptr_map;
    uint8_t domain[30];//存放域名
    uint8_t tempBuf[1024];//解压后临时存放的报文
    private:
    int parse_author(int count);//解析头部
    void extend(int count);//解压缩
    int dfs(uint8_t * temp, uint8_t * buf, int tmp, int next);
    int parse_author_ip();
    bool isReturn;

};
