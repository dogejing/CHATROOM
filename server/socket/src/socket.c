/*****************************************************
copyright (C), 2014-2015, Lighting Studio. Co.,     Ltd. 
File name：
Author：Jerey_Jobs    Version:0.1    Date: 
Description：
Funcion List: 
*****************************************************/

#include "head.h"

int mysocket()/*调用socket函数创建TCP协议套接口*/
{
	int sockfd;	/*建立sockfd描述符*/

	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)/*IPV4,TCP*/
	{
		fprintf(stderr,"socket error:%s\n",strerror(errno));
		exit(1);
	}

	return sockfd;
}

void mybind(int sockfd,struct sockaddr_in * s_addr)/*绑定地址，bind对sockfd定位*/
{
	memset(s_addr,0,sizeof(struct sockaddr_in));/*对结构体s_addr初始化*/
	s_addr->sin_family = AF_INET;/*采用IPV4网络协议*/
	s_addr->sin_port   = htons(PORTNUMBER);/*设置端口号*/
	s_addr->sin_addr.s_addr = INADDR_ANY;/*服务器自动填充本机ip*/

	int opt = 1;
	if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)) < 0)
	{
		perror("setsockopt failed");
		exit(1);
	}

	if(bind(sockfd,(struct sockaddr *)s_addr,sizeof(struct sockaddr)) == -1)
	{
		fprintf(stderr,"bind error:%s\n",strerror(errno));
		exit(1);
	}
}

void mylisten(int sockfd)/*等待连接*/
{
	if(listen(sockfd,LISTEN_MAX) == -1)
	{
		fprintf(stderr,"listen error:%s\n",strerror(errno));
		exit(1);
	}
}

int myaccept(int sockfd,struct sockaddr_in * c_addr)/*接受socket连线*/
{
	int new_fd;
	int addrlen = sizeof(struct sockaddr);

	memset(c_addr,0,sizeof(struct sockaddr_in));/*结构体初始化*/
	if((new_fd = accept(sockfd,(struct sockaddr *)c_addr,&addrlen)) == -1)
	{
		fprintf(stderr,"accept error:%s\n",strerror(errno));
		exit(1);
	}

	return new_fd;
}
