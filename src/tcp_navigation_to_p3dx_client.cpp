#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ros/ros.h"
#include <geometry_msgs/Pose2D.h>
 char inputBuffer[32]={};
char message[32]={};
bool callback_bool=false;
 void pose_callback(const geometry_msgs::Pose2DConstPtr& pose2D)
{
  int Vx=10*pose2D->x;
  int Vy=10*pose2D->y;
  int Theta=10*pose2D->theta;
   char Vx_char[32]={};
  char Vy_char[32]={};
  char Theta_char[32]={};
  sprintf(Vx_char,"%d",Vx);
  sprintf(Vy_char,"%d",Vy);
  sprintf(Theta_char,"%d",Theta);
   strcpy(message,"a,");
  strcat(message,Vx_char);
  strcat(message,",");
  strcat(message,Vy_char);
  strcat(message,",");
  strcat(message,Theta_char);
  strcat(message,",");
  strcat(message,"*");
  ROS_INFO("%s",message);
  callback_bool=true;
}
 int main(int argc, char ** argv)
{
  struct sockaddr_in serverInfo;
  char buf[32];
  int n;
  int sockfd;
  ros::init(argc, argv, "tf_echo_pose2D_subscriber");
  ros::NodeHandle nh;
  ros::Subscriber tf_echo_pose2D_subscriber=nh.subscribe("/tf_echo_pose2D",10,pose_callback);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  serverInfo.sin_family = AF_INET;
  serverInfo.sin_port = htons(8787);
  inet_pton(AF_INET, "192.168.1.100", &serverInfo.sin_addr.s_addr);
  std::cout << "Start Connect" << std::endl;

  int err=connect(sockfd, (struct sockaddr *)&serverInfo, sizeof(serverInfo));
  if(err==-1)
  {
     ROS_INFO("Connection error");
  }
  else if(err==1)
  {
     ROS_INFO("Connection success");
  }
   while(ros::ok())
  {
    ros::spinOnce();
    if(callback_bool)
    {
       write(sockfd , message , sizeof(message));
       callback_bool=false;
    }
  }
   /* 從伺服器接受資料
  memset(buf, 0, sizeof(buf));
  n = read(sockfd, buf, sizeof(buf));*/
  //printf("\t[Info] Receive %d bytes: %s\n", n, buf);
  close(sockfd);
  return 0;
}
