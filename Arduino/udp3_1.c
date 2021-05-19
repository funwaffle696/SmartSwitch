#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
 
#define UDP_TEST_PORT       50001
 
int main(int argC, char* arg[])
{
    struct sockaddr_in addr;

    int sockfd, len = 0, newsockfd;   

    int addr_len = sizeof(struct sockaddr_in);

    char buffer[256];

    int a =0, data; 

    char input[256];

    char output_message[256];

    char bb[256]="";

 

    /* 建立socket，注意必须是SOCK_DGRAM */

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {

        perror ("socket");

        exit(1);

    }

 

    /* 填写sockaddr_in 结构 */

    bzero(&addr, sizeof(addr));

    addr.sin_family = AF_INET;

    addr.sin_port = htons(UDP_TEST_PORT);

    addr.sin_addr.s_addr = htonl(INADDR_ANY) ;// 接收任意IP发来的数据

 

    /* 绑定socket */

    if (newsockfd=bind(sockfd, (struct sockaddr *)&addr, sizeof(addr))<0) {

        perror("connect");

        exit(1);

    }

 

    while(1) {

        bzero(buffer, sizeof(buffer));

        len = recvfrom(sockfd, buffer, sizeof(buffer), 0,

                       (struct sockaddr *)&addr ,&addr_len);

        /* 显示client端的网络地址和收到的字符串消息 */

        /*printf("Received a string from client %s, string is: %s\n",

                inet_ntoa(addr.sin_addr), buffer);*/
        
        printf("Received a string from client %s, %s\n",

                inet_ntoa(addr.sin_addr), buffer);

        /* 将收到的字符串消息返回给client端 */
        
        //---輸出訊息---
        //*output_message = 'a';
        //sendto(sockfd,output_message, len, 0, (struct sockaddr *)&addr, addr_len);

        //bzero(buffer, sizeof(buffer));
        //*buffer = 'c';

        //---輸入所要的輸出訊息
        scanf("%s", input);       

        if(*input=='a')
        {
            //---傳至 arduino (client)           
            *output_message = 'a';
            sendto(sockfd,output_message, len, 0, (struct sockaddr *)&addr, addr_len);
            //---接收 arduino (client) 回傳的值 
            bzero(bb, sizeof(bb));
            recvfrom(sockfd, &bb, sizeof(bb), 0,

                       (struct sockaddr *)&addr ,&addr_len);
            printf("%s\n", bb);
        }
        else if(*input=='b')
        {            
            *output_message = 'b';
            sendto(sockfd,output_message, len, 0, (struct sockaddr *)&addr, addr_len);
            bzero(bb, sizeof(bb));
            recvfrom(sockfd, &bb, sizeof(bb), 0,

                       (struct sockaddr *)&addr ,&addr_len);
            printf("%s\n", bb);
        }
        else
        {
            sendto(sockfd,buffer, len, 0, (struct sockaddr *)&addr, addr_len);
        }
        //---
        

        /*if(*input=='a')
        {
            bzero(buffer, sizeof(buffer));
            *buffer = 'a';
            sendto(sockfd,buffer, len, 0, (struct sockaddr *)&addr, addr_len);
        }
        else if(*input=='b')
        {
            bzero(buffer, sizeof(buffer));
            *buffer = 'b';
            sendto(sockfd,buffer, len, 0, (struct sockaddr *)&addr, addr_len);
        }
        else
        {
            sendto(sockfd,buffer, len, 0, (struct sockaddr *)&addr, addr_len);
        }*/

        
        /*if(a1==0)
        {
            bzero(buffer, sizeof(buffer));
            *buffer = 'c';
            sendto(sockfd,buffer, len, 0, (struct sockaddr *)&addr, addr_len);
            a1=1;            
        }

        if(a1==1)
        {
            bzero(buffer, sizeof(buffer));
            *buffer = 'b';
            sendto(sockfd,buffer, len, 0, (struct sockaddr *)&addr, addr_len);
            a1=0;
        }*/

        //sendto(sockfd,buffer, len, 0, (struct sockaddr *)&addr, addr_len);
        //bzero(buffer, sizeof(buffer));
        //*buffer = 'b';
        //sendto(sockfd,buffer, len, 0, (struct sockaddr *)&addr, addr_len);
        //sendto(sockfd,buffer, len, 0, (struct sockaddr *)&addr, addr_len);
        //sendto(sockfd,a, len, 0, (struct sockaddr *)&addr, addr_len);
    }

 

    return 0;

}
