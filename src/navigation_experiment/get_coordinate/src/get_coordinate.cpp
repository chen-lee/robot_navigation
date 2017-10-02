#include <string>
#include <ros/ros.h>                           // 包含ROS的头文件
#include <sensor_msgs/JointState.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <boost/asio.hpp>                  //包含boost库函数
#include <boost/bind.hpp>
#include <math.h>
#include "std_msgs/String.h"              //ros定义的String数据类型
#include <stdio.h> /* Standard input/output definitions */
#include <string.h> /* String function definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h> /* File control definitions */
#include <errno.h> /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include "get_coordinate/position.h"

int distanceBaseA = 0;	
int distanceBaseB = 0;
int distanceBaseC = 0;
int BaseAx =4370,BaseAy = 0;//int BaseAx = 0,BaseAy = 790 + 6 * 930 + 520; 6120
int BaseBx = 0,BaseBy =  0;//int BaseBx = 600 + 6 * 870 + 50,BaseBy =  790 + 6 * 930 + 520;
int BaseCx = 0,BaseCy = 5590;//7420;//int BaseCx = 600 + 3 * 870,BaseCy = 0;

/*
 * @brief Open serial port with the given device name
 *
 * @return The file descriptor on success or -1 on error.
 */

int open_port(char *port_device)
{
    int fd; /* File descriptor for the port */

    fd = open(port_device, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)
    {
        perror("open_port: Unable to open /dev/ttyS0 - ");
    }
    else
        fcntl(fd, F_SETFL, 0);

    return (fd);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "position");
    ros::NodeHandle n;
    ros::Publisher pub = n.advertise<get_coordinate::position>("position",1);
    struct termios options;

    int fd=open_port("/dev/ttyUSB0");
    if(fd==-1){
        return -1;
    }

    tcgetattr(fd, &options);


    //Set the baud rates to 38400...
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);


    //Enable the receiver and set local mode...
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~CSIZE; /* Mask the character size bits */
    options.c_cflag |= CS8; /* Select 8 data bits */

    //No parity
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;


    //Set the new options for the port...
    tcsetattr(fd, TCSANOW, &options);

    int ax,ay,az,gx,gy,gz;
    int cricleCnt = 0;
    char buf[1024];
    char* pos=buf;

    while(ros::ok()){
        ssize_t n=read(fd, pos, 1);
        if(n==1){
            if(*pos=='\n' ){
                *(pos+1)=0;
                sscanf(buf,"%d,%d,%d,%d,%d,%d", &ax, &ay, &az, &gx, &gy, &gz);
                if (ax%10 == 1)
                {
                  distanceBaseA = (ax - 1) / 10;
                }
                 if (ax%10 == 2)
                {
                  distanceBaseB = (ax - 2) / 10;
                }
                if (ax%10 == 3)
                {
                  distanceBaseC = (ax - 3) / 10;
                }
                if(cricleCnt++ ==5)
                {
                    
                    cricleCnt = 0;
                    int A = BaseAx - BaseCx;
                    int B = BaseAy - BaseCy;
                    float C = pow(BaseAx,2) - pow(BaseCx,2) + pow(BaseAy,2) - pow(BaseCy,2) + pow(distanceBaseC,2) - pow(distanceBaseA,2);
                    int D = BaseBx - BaseCx;
                    int E = BaseBy - BaseCy;
                    float F = pow(BaseBx,2) - pow(BaseCx,2) + pow(BaseBy,2) - pow(BaseCy,2) + pow(distanceBaseC,2) - pow(distanceBaseB,2);

                    int x = (B * F - E * C) / (2 * B *D - 2 * A * E);
                    int y = (A * F - D * C) / (2 * A * E - 2 * B * D);
                    get_coordinate::position msg;
                    msg.positionX = x;
                    msg.positionY = y;
                    pub.publish(msg);
                    printf("DISTANCE: %d %d %d %d %d\n", distanceBaseA,distanceBaseB,distanceBaseC,x,y);

                }
                //printf("DISTANCE: %d %d %d\n", distanceBaseA,distanceBaseB,distanceBaseC);
                //printf("DISTANCE: %d\n", ax);

                pos=buf;
            }else{
                pos++;
            }
        }
    }


    close(fd);
}
