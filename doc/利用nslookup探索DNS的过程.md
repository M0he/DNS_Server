###nslookup的基本信息和用法

nslookup是一个查询域名信息，并进行DNS问题判断的Linux工具。

在nslookup环境下可以输入“set type=a”查询主机的域名或者“set type=ns“查询域名的地址。

”server 域名服务器ip“切换成其他域名服务器进行域名查询。

###使用nslookup的目的

1、为了获取本地DNS服务器可以进行DNS请求的根域名服务器ip地址。

2、为了研究整个完整的DNS请求是如何进行的。

###实验1:查询根域名服务器及其com服务器

使用nslookup向10.8.8.18查询根域名服务器的结果

![根服务器地址](picture/%E6%A0%B9%E5%9F%9F%E5%90%8D%E6%9C%8D%E5%8A%A1%E5%99%A8%E5%9F%9F%E5%90%8D.pic.jpg)


切换成查询IP模式查询b.root-servers.net.根域名服务器的IP地址，并且切换成这个根DNS服务器查询com的域名

![com服务器地址](picture/com%E5%9F%9F%E5%90%8D%E6%9C%8D%E5%8A%A1%E5%99%A8.pic.jpg)

###实验2:根据com服务器查询bilibili.com的域名服务器并且返回最后得到的IP

查询com服务器的ip，并且切换到这个服务器查询bilibil负载均衡DNS服务器域名及其对应IP地址

![bilibili负载均衡服务器地址](picture/bilibili%E8%B4%9F%E8%BD%BD%E5%9D%87%E8%A1%A1%E6%9C%8D%E5%8A%A1%E5%99%A8.pic.jpg)

切换到bilibil负载均衡DNS服务器域名对应的IP地址查询最终的域名及其IP地址

![bilibili最终的地址](picture/bilibili%E6%9C%80%E7%BB%88%E7%9A%84%E5%9C%B0%E5%9D%80.pic.jpg)

###通过以上两个实验我们得知一条DNS查询需要经过最少四个DNS服务器（本地域名服务器、根域名服务器、顶级域名服务器、DNS负载均衡服务器）

在DNS查询中本地域名服务器是DNS的核心，所以本地服务器要做的工作最多，如果本地DNS服务器的性能提高了，那无疑能提高DNS查询的速度。

目前能想到的提高本地DNS服务器性能的策略

1、把服务器搭建在一个稳定并且充分利用硬件的操作系统之上，所以redhat操作系统是适合的，在当前的阶段可以将实验放在CentOS这个仿redhat操作系统上面进行。而且Linux是个多用户多任务的操作系统，DNS服务器有时需要多个运维工程师进行管理，所以DNS服务器只能选择Linux作为其运行环境。

2、利用Linux下epoll这个机制来进行高并发地监听IO。

3、使用线程池来提高对cpu的利用率，并且减少了线程创建及销毁带来的额外开销。

4、使用高性能的MySql来做DNS的本地缓存，把发生过的DNS查询缓存到本地，使其大部分的查询不必要经过完整的过程就能得到很好的结果，还有就是能够接受邮件协议申请的域名注册信息。

