# localDnsServer

#### 介绍
一个在Linux平台下的高并发自制域名解析服务器，起到代替ISP运营商DNS的作用。

#### 软件架构
可以参考的链接https://blog.csdn.net/txwillnottleaveyou/article/details/87617848


#### 安装教程

1.  cd local-dns-server
2.  g++ *.cpp -o main -pthread
3.  ./main 端口号

#### 使用说明

1.  打开另一个终端输入dig @127.0.0.1 -p 端口号 需要解析的域名（比如：www.baidu.com） A


#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request
