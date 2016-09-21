/*****************************************************
copyright (C), 2014-2015, Lighting Studio. Co.,     Ltd. 
File name：
Author：Jerey_Jobs    Version:0.1    Date: 
Description：
Funcion List: 
*****************************************************/

#include "head.h"
//#include "package.h"

extern void * c_handle(void * arge);

int main()
{
	int sockfd;	/*建立sockfd描述符*/
	int new_fd;	/*建立新的客户端连接描述符*/
	struct sockaddr_in s_addr;
	struct sockaddr_in c_addr;

	ph * foo = (ph*)malloc(sizeof(ph));
	char ph_ch[1024];

	sqlite3 * db;/*定义数据库句柄*/
	char * errmsg;/*存放数据库函数调用返回的错误信息*/

	pthread_t c_thread;	/*线程ID指针*/

//数据库设置
	open_db(&db);	/*打开数据库server.db（没有的话就创建一个）*/
	creat_user_db(db,&errmsg);	/*创建user表*/
	creat_data_db(db,&errmsg);	/*创建data表*/
	creat_online_db(db,&errmsg);
	reg_db(db,&errmsg,"管理员","10000",10000);	//注册管理员

//网络连接设置	
	sockfd = mysocket();	/*创建TCP套接口*/
	mybind(sockfd,&s_addr);	/*绑定地址*/
	mylisten(sockfd);	/*监听，设置最大连接人数*/;

	while(1)
	{
		memset(foo,0,sizeof(ph));
		printf("等待客户端连接...\n");
		new_fd = myaccept(sockfd,&c_addr);	/*接受客户端连线，返回新的套接字*/
		printf("client(ip=%s,port=%d)\n",inet_ntoa(c_addr.sin_addr),ntohs(c_addr.sin_port));	/*打印出连接的客户端信息*/
		foo->sockfd = new_fd;	//将数据存入结构体
		foo->db = db;
		memcpy(ph_ch,foo,sizeof(ph));	//结构体中数据存放到数组
		pthread_create(&c_thread,NULL,c_handle,(void *)ph_ch);/*创建线程，将参数ph_ch转化为void指针类型传递入c_handle函数*/
		free(foo);
	}
}

