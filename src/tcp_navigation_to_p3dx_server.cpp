#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ros/ros.h"
#include "tcp_linux/tcpserver.h"
 #define PortNum 8845
 double Vx=0.0 , Vy=0.0 ,Theta=0.0;
 void inputBuffer_to_data(const char*inputBuffer)
{
  double data_Buffer=0;
  int data_Cout=0;
  int minus_cout=0;
  const char client1[16]="a" , client2[16]="b";
   /*client1*/
  if(inputBuffer[0]==client1[0])
  {
     for(int i=2 ; i<=32 ; i++)
     {
        /*讀取資料*/
        if(inputBuffer[i]!=',' && inputBuffer[i]!='*' && inputBuffer[i]!='-')
        {
           data_Buffer=data_Buffer*10+(int)inputBuffer[i]-'0';
        }
        /*判斷正負*/
        else if(inputBuffer[i]=='-')
        {
          minus_cout++;
        }
        /*取出資料*/
        else if(inputBuffer[i]==',')
        {
           data_Cout++;
           if(minus_cout==0)
           {
               if(data_Cout==1)
               {
                 Vx=data_Buffer/10;
                 data_Buffer=0;
               }
               else if(data_Cout==2)
               {
                 Vy=data_Buffer/10;
                 data_Buffer=0;
               }
               else if(data_Cout==3)
               {
                 Theta=data_Buffer/10;
                 data_Buffer=0;
               }
           }
           else if(minus_cout==1)
           {
               if(data_Cout==1)
               {
                 Vx=-data_Buffer/10;
                 data_Buffer=0;
                 minus_cout=0;
               }
               else if(data_Cout==2)
               {
                 Vy=-data_Buffer/10;
                 data_Buffer=0;
                 minus_cout=0;
               }
               else if(data_Cout==3)
               {
                 Theta=-data_Buffer/10;
                 data_Buffer=0;
                 minus_cout=0;
               }
           }
           else
           {
             ROS_INFO("minus error");
           }
        }
        /*讀取結束*/
        else if(inputBuffer[i]=='*')
       {
         data_Cout=0;
         data_Buffer=0;
         minus_cout=0;
         ROS_INFO("receve from client1");
         ROS_INFO("Vx=%f",Vx);
         ROS_INFO("Vy=%f",Vy);
         ROS_INFO("Theta=%f",Theta);
         break;
       }
        /*讀取錯誤*/
        else
        {
          ROS_INFO("data error");
          break;
        }
     }
  }
   /*client2*/
  else if(inputBuffer[0]==client2[0])
  {
    for(int i=2 ; i<=32 ; i++)
    {
       /*讀取資料*/
       if(inputBuffer[i]!=',' && inputBuffer[i]!='*' && inputBuffer[i]!='-')
       {
          data_Buffer=data_Buffer*10+(int)inputBuffer[i];
       }
       /*判斷正負*/
       else if(inputBuffer[i]=='-')
       {
         minus_cout++;
       }
       /*取出資料*/
       else if(inputBuffer[i]==',')
       {
          data_Cout++;
          if(minus_cout==0)
          {
              if(data_Cout==1)
              {
              }
              else if(data_Cout==2)
              {
              }
              else if(data_Cout==3)
              {
              }
          }
          else if(minus_cout==1)
          {
              if(data_Cout==1)
              {
              }
              else if(data_Cout==2)
              {
              }
              else if(data_Cout==3)
              {
              }
          }
          else
          {
            ROS_INFO("minus error");
          }
       }
       /*讀取結束*/
       else if(inputBuffer[i]=='*')
      {
        data_Cout=0;
        data_Buffer=0;
        minus_cout=0;
        printf("%s\n",inputBuffer);
        ROS_INFO("receve from client2");
        break;
      }
       /*讀取錯誤*/
       else
       {
         ROS_INFO("data error");
         break;
       }
    }
  }
   /*client error*/
  else
  {
    ROS_INFO("client error......");
  }
 }
 int main()
{
  char inputBuffer[32] = {} , message[32]={};
  int sockfd , SocketClient,n ;
  struct sockaddr_in serverInfo , clientInfo;
  socklen_t len;
   sockfd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET:Interent Address
   serverInfo.sin_family = AF_INET;
  serverInfo.sin_port = htons(8787);
  serverInfo.sin_addr.s_addr = inet_addr("192.168.1.123");
  bind(sockfd, (struct sockaddr*)&serverInfo, sizeof(serverInfo));
  printf("binding...\n");
   listen(sockfd, 5);
   printf("listening...\n");
  printf("wait for connection...\n");
  len = sizeof(clientInfo);
  SocketClient = accept(sockfd, (struct sockaddr *)&clientInfo, &len);
  printf("Receive connection\n");
   while(1)
  {
      memset(inputBuffer, 0, sizeof(inputBuffer));
      n = read(SocketClient, inputBuffer, sizeof(inputBuffer));
      inputBuffer_to_data(inputBuffer);
  }
   close(sockfd);
  return 0;
}
