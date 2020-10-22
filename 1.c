#include <pthread.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>


#define SERVER_PORT  6666
#define SERVER_IP    "192.168.71.23"

void send_msg(void*arg)
{
    int sockCli  = *(int*)arg;
    
    char writebuf[256];     //申请一个发送数据缓存区
    bzero(writebuf,256);
    while(1)
    {
        printf("客户端:>");
        scanf("%s",writebuf);
        if(strncmp(writebuf,"quit", 4) == 0)    //如果客户端发送的数据为"quit"，则退出。
            break;
        write(sockCli, writebuf, strlen(writebuf));   //发送数据
    }
    

}





int main(int argc, char *argv[])
{
    //创建客户端套接字号，并检测是否创建成功
    int sockCli;
    sockCli = socket(AF_INET, SOCK_STREAM, 0);
    if(sockCli == -1)
        perror("socket");

    //创建一个地址信息结构体，并对其内容进行设置
    struct sockaddr_in addrSer;     
    addrSer.sin_family = AF_INET;         //使用AF_INET协议族
    addrSer.sin_port = htons(SERVER_PORT);  //设置端口号
    addrSer.sin_addr.s_addr = inet_addr(SERVER_IP);   //设置服务器ip

    // bind(sockCli,(struct sockaddr*)&addrCli, sizeof(struct sockaddr));    //将套接字地址与所创建的套接字号联系起来

    //创建一个与服务器的连接，并检测连接是否成功
    socklen_t addrlen = sizeof(struct sockaddr);
    int res = connect(sockCli,(struct sockaddr*)&addrSer, addrlen);
    if(res == -1)
    {
        perror("connect");
        return 0;
    }
        
    else
    {
        printf("Client Connect Server OK.\n");
    }
    
    pthread_t tid ;
	pthread_create(&tid,NULL,(void*)send_msg,&sockCli);


    
    char readbuf[256];     //申请一个接收数据缓存区
    while(1)
    {
        
        bzero(readbuf,256);
        read(sockCli, readbuf, 256);    //接收来自服务器的数据
        printf("服务器:> %s\n",readbuf);
        
       
    }
    close(sockCli);       //关闭套接字
    return 0;
}