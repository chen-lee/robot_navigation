#include <ros/ros.h>                      
#include <math.h>
#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <assert.h> 
#include <termios.h> 
#include "get_coordinate/position.h"
using namespace std;

class get_coordinate_two_dimension
{
public:
    get_coordinate_two_dimension(char *port_device,int BaseAx,int BaseAy,
                                 int BaseBx,int BaseBy,int BaseCx,int BaseCy, int rate);
    void get_raw_data();
    void print_info();
    void parser_distance();
    void parser_position();
    void thread_work();
    ~get_coordinate_two_dimension();
   
private:
    int distanceBaseA;	
    int distanceBaseB;
    int distanceBaseC;
    int BaseAx, BaseAy;
    int BaseBx, BaseBy;
    int BaseCx, BaseCy;
    int x,y;
    int fd;
    int raw_data;
    char buffer[1024];
    char* buf;
    int rate;
    int rate_cnt;
    ros::NodeHandle n;
    ros::Publisher pub;
};

get_coordinate_two_dimension::get_coordinate_two_dimension(char *port_device,int BaseAx,int BaseAy,
                                                            int BaseBx,int BaseBy,int BaseCx,int BaseCy,int rate):
                                                            BaseAx(BaseAx),BaseAy(BaseAy),BaseBx(BaseBx),
                                                            BaseBy(BaseBy),BaseCx(BaseCx),BaseCy(BaseCy),rate(rate)
{
    cout << "construct funtion"  << endl;
    buf = buffer;
    //open serial port
    fd = open(port_device, O_RDWR | O_NOCTTY | O_NDELAY);
    cout << "fd:" << fd  << endl;
	assert(fd != -1); 
    fcntl(fd, F_SETFL, 0);
    //serial initial
    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~CSIZE; 
    options.c_cflag |= CS8; 
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    tcsetattr(fd, TCSANOW, &options);
    //ros initial
    //ros::init();
    pub = n.advertise<get_coordinate::position>("position",1);
}

void get_coordinate_two_dimension::get_raw_data()
{   
    while(1)
    {
	    size_t n = read( fd, buf, 1);
        if( n == 1)  
        {
		    if( *buf == '\n')
			{
			    *(buf + 1) = 0;
				sscanf( buffer, "%d", &raw_data);
				buf = buffer;
                break;
			}
			else
			{
			    buf++;
			}
		}
   }
}

void get_coordinate_two_dimension::print_info()
{
    printf("DISTANCE: %d %d %d  position:%d %d\n", 
                     distanceBaseA,distanceBaseB,
                     distanceBaseC,x,y);
}

void get_coordinate_two_dimension::parser_distance()
{
    if(raw_data % 10 == 1)
    {
        distanceBaseA = (raw_data - 1) / 10;
    }
    if(raw_data % 10 == 2)
    {
        distanceBaseB = (raw_data - 2) / 10;
    }
    if(raw_data % 10 == 3)
    {
        distanceBaseC = (raw_data - 3) / 10;
    }
}

void get_coordinate_two_dimension::parser_position()
{
    if(rate_cnt++ ==rate)
    {
        rate_cnt = 0;
        int A = BaseAx - BaseCx;
        int B = BaseAy - BaseCy;
        float C = pow(BaseAx,2) - pow(BaseCx,2) + pow(BaseAy,2) 
                  - pow(BaseCy,2) + pow(distanceBaseC,2) 
                  - pow(distanceBaseA,2);
        int D = BaseBx - BaseCx;
        int E = BaseBy - BaseCy;
        float F = pow(BaseBx,2) - pow(BaseCx,2) + pow(BaseBy,2) 
                  - pow(BaseCy,2) + pow(distanceBaseC,2) 
                  - pow(distanceBaseB,2);
        x = (B * F - E * C) / (2 * B *D - 2 * A * E);
        y = (A * F - D * C) / (2 * A * E - 2 * B * D);
        
        get_coordinate::position msg;
        msg.positionX = x;
        msg.positionY = y;
        pub.publish(msg);

        print_info();
    }
}

void get_coordinate_two_dimension::thread_work()
{
    while( ros::ok() )
    {
        get_raw_data();
        parser_distance();
        parser_position();
    }
}

get_coordinate_two_dimension::~get_coordinate_two_dimension()
{
    close(fd);
}
 
int main(int argc, char **argv)
{
    get_coordinate_two_dimension *p = new get_coordinate_two_dimension( 
                                     "/dev/ttyUSB0", 4370, 0, 0, 0, 0, 5590,5);
    p->thread_work();
    delete p;
}
