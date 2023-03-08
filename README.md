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


#### 特技

1.  使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2.  Gitee 官方博客 [blog.gitee.com](https://blog.gitee.com)
3.  你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解 Gitee 上的优秀开源项目
4.  [GVP](https://gitee.com/gvp) 全称是 Gitee 最有价值开源项目，是综合评定出的优秀开源项目
5.  Gitee 官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6.  Gitee 封面人物是一档用来展示 Gitee 会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
