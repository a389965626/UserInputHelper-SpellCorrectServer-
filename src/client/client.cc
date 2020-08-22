#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using std::string;
using std::cout;
using std::endl;
using std::to_string;
#define ERR_EXIT(m) \
	do { \
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)

void do_service(int sockfd);

int main()
{
	int peerfd = socket(PF_INET, SOCK_STREAM, 0);
	if(peerfd == -1)
		ERR_EXIT("socket");

	struct sockaddr_in client;
	memset(&client, 0, sizeof(client));
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = inet_addr("172.25.214.250"); //localhost
	//addr.sin_addr.s_addr = INADDR_ANY; //localhost
	client.sin_port = htons(8888);

	socklen_t len = sizeof(client);
	if(connect(peerfd, (struct sockaddr*)&client, len) == -1)
		ERR_EXIT("Connect");

	do_service(peerfd);

	return 0;
}

void do_service(int sockfd)
{
	cout << "请输入待查询单词:" << endl;
	fd_set rdset;
	char recvbuf[1024] = {0};
	char sendbuf[1024] = {0};
	while(1)
	{
		FD_ZERO(&rdset);
		FD_SET(0,&rdset);//监听标准输入
		FD_SET(sockfd,&rdset);
		int ret=select(sockfd+1,&rdset,NULL,NULL,NULL);
		if(ret>0)
		{
			if(FD_ISSET(sockfd,&rdset))  //服务器端有消息过来，证明服务器端打包好的json字符串采用小火车的方式发送过来了
				//我们应该采用小火车的方式来进行接受。然后对json字符串进行解析。
			{
				cout << "查询结果如下:" << endl;
				//read，先接受火车的大小
				size_t count;
				int nread = read(sockfd, &count, sizeof(size_t)); //nread应该是4，表明读到的是4个字节
				if(nread == -1)
				{
					if(errno == EINTR)
						continue;
					ERR_EXIT("read");
				}
				else if(nread == 0)  //写端关闭，读端读到0，
				{
					printf("server close!\n");
					close(sockfd);
					exit(EXIT_SUCCESS);
				}
				else  //表明确实有打包好的json字符串发过来，此时应该先循环接受打包好的string，然后再进行解析
				{
					size_t total=0;
					memset(recvbuf,0,sizeof(recvbuf));//清空
					char *p=recvbuf;
					while(total < count)  //循环接受
					{
						ret=recv(sockfd,p+total,count-total,0);
						total=ret+total;
					}
					string jsonstring(recvbuf);  //将C风格字符串转为C++风格字符串，便于解析json。

					//cout << "before parse" << endl;
					Json::Value root_value;
					Json::Reader reader;
					if(!reader.parse(jsonstring,root_value))
					{
						cout << "parse error" << endl;
					}
					int i;
					int num=root_value.size();
					for(i=0 ; i<num ; ++i)
					{
						Json::Value temp_item=root_value[i];
						string s(to_string(i));
						string strVal = temp_item[s.c_str()].asString();
						cout << strVal << endl;
					}
					//cout << "after parse" << endl;
			        cout << endl << "请继续输入待查询单词或者退出查询:" << endl;
				}
			}
		}

		if(FD_ISSET(0,&rdset))   //如果标准输入有信息到来，证明我们输入了查询单词，应该将其发送给服务器端
		{
			memset(sendbuf,0,sizeof(sendbuf));
			ret=read(0,sendbuf,sizeof(sendbuf)-1);
			if(ret<=0)//标准输入来了ctrl+D等退出的信号，会返回0
			{
				printf("byebye\n");
				close(sockfd);
				exit(0);
			}
			ret=send(sockfd,sendbuf,strlen(sendbuf),0); //必须将换行符发送过去，因为服务器是readline，要读换行符。
			if(ret<=0)
			{
				printf("byebye\n");
				close(sockfd);
				exit(0);
			}
		}
	}
}


