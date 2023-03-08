#ifndef DNSINFOMATION_H
#define DNSINFOMATION_H

//电信、移动、联通的DNS服务器
const char *DNS_SERVER_114 = "114.114.114.114";
//谷歌服务器
const char *DNS_SERVER_GOOGLE = "8.8.8.8";
//本地服务器
const char *DNS_SERVER_LOCAL = "198.41.0.4";
//外国watch服务器，能收到最终IP和权威域名服务器的域名
const char *DNS_SERVER_WATCH = "84.200.69.80";
//外国comodo服务器，收到权威域名服务器域名,首选
const char *DNS_SERVER_COMODO = "8.26.56.26";

#endif