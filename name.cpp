#include "name.h"
//外国comodo服务器，收到权威域名服务器域名,首选
const char *DNS_SERVER_COMODO = "8.26.56.26";

name::name(int off, int dns_port) {
    socketfd = socket(AF_INET,SOCK_DGRAM,0);
    localAddress.sin_port = htons(off +dns_port);
    localAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddress.sin_family = AF_INET;
    bind(socketfd, (struct sockaddr *)&localAddress, sizeof(struct sockaddr));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(DNS_SERVER_COMODO);
    serverAddress.sin_port = htons(53);
    len = sizeof(struct sockaddr);
}

//递归搜索
int name::dfs(uint8_t * temp, uint8_t * buf, int tmp, int next) {
    
    //cout<<"开始递归"<<endl;
    int length = 0;
    for (;isReturn;length ++) {
        if (buf[tmp] == 0) {
            temp[next + length] = 0;

            isReturn = false;
            break;
        } else if (buf[tmp] == 192) {
            int index = buf[tmp + 1];
            length = length + dfs(temp, buf , index, next + length);
            tmp += 2;
        } else {
            temp[next + length] = buf[tmp];   
            tmp ++;
        }
    }
    return length;
}
//解压缩，DNS回应报文是结过压缩的
void name::extend(int count) {
    cout<<"开始扩展"<<endl;
    int pre = 0, next = 0;
    for (;pre < count; pre ++ ) {
        if (buf[pre] != 192) {
            tempBuf[next ++] = buf[pre];
            for (auto n = ns_ptr.begin(); n != ns_ptr.end(); n++) {
                if (*n == (buf + pre)) {//如果是开始位置就就行替换
                temp_ns_ptr.push_back(tempBuf + next - 1);
                cout<<(uint16_t *)*n<<endl;
                }
            }
        } else { 
            int tmp = buf[pre + 1];
            for (auto n = ns_ptr.begin(); n != ns_ptr.end(); n++) {
                if (*n == (buf + pre)) {//如果是开始位置就就行替换
                temp_ns_ptr.push_back(tempBuf + next);
                cout<<(uint16_t *)*n<<endl;
                }
            }
            isReturn = true;
            int len = dfs(tempBuf, buf, tmp ,next);
            pre += 1;
            next += len;
            if (tempBuf[next - 1] == 0) next--;
        }
    }
    
    for (int i = 0; i< next; i++) {
        cout<<(uint16_t)tempBuf[i]<<"/";
    }  
}
//解析域名
int parse_domain(uint8_t * domain,uint8_t * buf ,int i) {
    //问题部分
    int status = 1;//1则指示数量
    int number;
    int index = 0;
    for (;;i++) {
        switch (status) {
            case 1 :
            number = buf[i];
            index ++;  
            if (number == 0) return index;
            status = 2;
            break;
            case 2 :
            int off = 0;
            while (number -- ) {
                domain[index ++] = buf[i + off++];
            }
            i += (off - 1);
            status = 1;
            break;
        }
    }
}
//隔断每个数据部分
int name::parse_author(int count) {
    //标识符号id
    int i = 0;
    for (; i < 2; i++) {
        sendbuf[i] = buf[i];
    } 
    sendbuf[i++] = 1;
    sendbuf[i++] = 32;
    //各个部分数量
    int queryOfnumber,answerOfNumber,authorityOfNumber,additionalOfNumber;
    queryOfnumber = (buf[i++] << 8) + buf[i++];
    answerOfNumber = (buf[i++] << 8) + buf[i++];
    authorityOfNumber = (buf[i++] << 8) + buf[i++];
    additionalOfNumber = (buf[i++] << 8) + buf[i++];
    //解析问题
    for (int n = 0; n< queryOfnumber; n++) {
    i += parse_domain(domain, buf, i);
    int queryType = (buf[i++] << 8) + buf[i++];
    i += 2;//都是IN类型所以跳过        
    }
    cout<<"问题结束n == "<<i;
    //解析回答
    for (int n = 0; n < answerOfNumber; n++) {
    int option = 1;
    switch (option) {
        case 1:
        answer_ptr.push_back(buf + i);
        //answer_ptr_map[buf + i] = 1;
        for (;buf[i] != 0;) {
            i++;
        }
        case 2:
        for (int n = 0; n < 8; n++,i++) {}   
        case 3:
        int number = (buf[i++] << 8) + buf[i++];
        for (int n = 0; n < number; n++,i++) {}
    }
    cout <<"回答结束n== "<<i<<endl;
    }
    //解析权威
    for (int n = 0; n < authorityOfNumber; n++) {
    int option = 1;
    switch (option) {
        case 1:
        ns_ptr.push_back(buf + i);
        //ns_ptr_map[buf + i] = 1;
        for (;buf[i] != 0;) {
            i++;
        }
        case 2:
        for (int n = 0; n < 8; n++,i++) {}   
        case 3:
        int number = (buf[i++] << 8) + buf[i++];
        for (int n = 0; n < number; n++,i++) {}
    }
    cout <<"权威结束n== "<<i<<endl;
    }
    ns_ptr.push_back(buf + i);
    
    extend(count);
    cout<<"已经扩展，并且权威域名服务器已经找出"<<endl;
    for (auto n = temp_ns_ptr.begin(); *n != temp_ns_ptr.back(); n++) {
        //打印域名服务器
        auto next = n;
        next++;
        int option = 1;//1域名2类型3生存时间4长度5域名服务器域名
        auto temp = new uint8_t(30);
        int index = 0;
        for (auto ptr = *n; ptr < *(next); ptr++) {
            switch (option) {
                case 1:
                if (*ptr == 0) {option = 2;}
                else break;
                case 2:
                if ((*ptr << 8 )+ *(++ ptr) != 2) option = 6;
                else {option = 3;nameServerdueue.push_back(temp);}
                ptr++;ptr++;
                break;
                case 3:
                ptr++;ptr++;ptr++;
                option = 4;
                break;
                case 4: 
                ptr++;
                option = 5;
                break;
                case 5:
                temp[index++] = *ptr;
            }
        }
    } 
    return 0;
}
int name::parse_author_ip() {
    int i = 0;
    sendbuf[i] = buf[i++];
    sendbuf[i] = buf[i];i++;
    sendbuf[i++] = 1;
    sendbuf[i++] = 32;
    sendbuf[i++] = 0;//问题数
    sendbuf[i++] = 1;
    sendbuf[i++] = 0;//回答数
    sendbuf[i++] = 0;
    sendbuf[i++] = 0;//授权资源记录数
    sendbuf[i++] = 0;
    sendbuf[i++] = 0;//附加信息数
    sendbuf[i++] = 0;
    for (int n = 0; n < 1; n++) {
        int index = 0;
        while (nameServerdueue[n][index] != 0) {
            sendbuf[i++] = nameServerdueue[n][index];
            index++;
        }
        sendbuf[i++] = 0;
        sendbuf[i++] = 0;
        sendbuf[i++] = 1;//授权资源记录数
        sendbuf[i++] = 0;
        sendbuf[i++] = 1;//附加信息数 
    } 
    return i;
}
void name::process() {
    sendto(socketfd, buf, sizeof(buf), 0, (struct  sockaddr*)&serverAddress, len);
    memset(buf, sizeof(buf), 0);
    int count = recvfrom(socketfd, buf, sizeof(buf), 0, (struct sockaddr *)&serverAddress, &len);
    cout<<"server回应"<<endl;
    for (int i = 0; i < count ;i++)cout<< (uint16_t)(buf[i])<<":";
    if (parse_author(count) == 0) {//因为不是A类型和TXT类型查询，不做处理  
        //sendto(mainSocketfd, buf, sizeof(buf), 0, (struct  sockaddr*)&clientAddress, len); 
    } else {
        throw exception();
    }
    
    if (nameServerdueue.size() == 0) {
        //sendto(mainSocketfd, buf, sizeof(buf), 0, (struct  sockaddr*)&clientAddress, len);  
    }
    count = parse_author_ip();
    for (int i = 0; i < count; i++) {
        cout<<(uint16_t)sendbuf[i]<<"/";
    }
    cout<<endl;
    sendto(socketfd, sendbuf, sizeof(sendbuf), 0, (struct  sockaddr*)&serverAddress, len);
    memset(sendbuf, sizeof(sendbuf), 0);
    count = recvfrom(socketfd, sendbuf, sizeof(sendbuf), 0, (struct sockaddr *)&serverAddress, &len);
    for (int i = 0; i < count ;i++)cout<< (uint16_t)(sendbuf[i])<<":";
    
    cout<<endl;
}