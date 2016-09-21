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

int myconnect(int sockfd,struct sockaddr_in * s_addr,char *ip)/*连接服务器*/
{
	bzero(s_addr,sizeof(struct sockaddr_in));/*对结构体s_addr初始化*/
	s_addr->sin_family = AF_INET;/*采用IPV4网络协议*/
	s_addr->sin_port   = htons(PORTNUMBER);/*设置端口号*/
	s_addr->sin_addr.s_addr = inet_addr(ip);/*填充服务器ip*/
	if(connect(sockfd,(struct sockaddr *)s_addr,sizeof(struct sockaddr)) == -1)
	{
		fprintf(stderr,"connect error:%s\n",strerror(errno));
		exit(1);
	}
}

