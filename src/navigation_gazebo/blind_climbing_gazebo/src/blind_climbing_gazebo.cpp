#include<ros/ros.h>
#include<iostream>
#include<sensor_msgs/NavSatFix.h>
#include<sensor_msgs/Imu.h>
#include<tf/LinearMath/Matrix3x3.h>
#include<tf/LinearMath/Quaternion.h>
#include<geometry_msgs/Twist.h>
#include<sensor_msgs/LaserScan.h>
#include<cmath>
#include<time.h>
#include<unistd.h>
#include <vector>

using namespace std;

const float Pi = 3.1415926535898;
float Aim[2];

long i=0,j=0;
//float aim_x = 8,aim_y = 8;
double aim_angle;
double erro_angle = 3.14;
double speed;
int count=0;
clock_t start_time,cur_time,end_time;
void delay(int sec)
{
	time(&start_time);
	do
	{
		time(&cur_time);
	}while((cur_time - start_time) < sec);
}

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
float x,y;
void GpsDateRecieved(const sensor_msgs::NavSatFix& GpsDate)
{
	x = (GpsDate.latitude - 49.9) * 111000;
	y = -(GpsDate.longitude - 8.9) * 71950;
}

double roll,pitch,yaw;
void ImuDataRecieved(const sensor_msgs::Imu& ImuData)
{
	tf::Quaternion bq(ImuData.orientation.x,ImuData.orientation.y,ImuData.orientation.z,ImuData.orientation.w);
	tf::Matrix3x3(bq).getRPY(roll,pitch,yaw);
}

float scan_ranges[20];
void ScanDataRecieved(const sensor_msgs::LaserScan& ScanData)
{
	for(int i=0;i < 20 ;++i)
	{
		scan_ranges[i] = ScanData.ranges[32 * i];
		if(isnan(scan_ranges[i]) != 0 )  scan_ranges[i] = 100;
	}
}

double NextPointAngleControl(double aim_x,double aim_y)
{
	if( yaw > -Pi/2 && yaw < Pi/2)
	{
		if (tan(yaw) * (aim_x - x) + y < aim_y)
		{
			erro_angle = acos((aim_x-x + tan(yaw) * (aim_y - y)) / ( sqrt( pow(aim_x - x , 2)  + pow( aim_y - y , 2)) * sqrt( 1  + pow( tan(yaw) , 2))));
		}
		if (tan(yaw) * (aim_x - x) + y > aim_y)
		{
			erro_angle = -acos((aim_x-x + tan(yaw) * (aim_y - y)) / ( sqrt( pow(aim_x - x , 2)  + pow( aim_y - y , 2)) * sqrt( 1  + pow( tan(yaw) , 2))));
		}
	}

	if( (yaw < -Pi/2 && yaw > -Pi) || (yaw > Pi/2 && yaw <Pi))
	{
		if (tan(yaw) * (aim_x - x) + y < aim_y)
		{
			erro_angle = -acos((-aim_x+x - tan(yaw) * (aim_y - y)) / ( sqrt( pow(aim_x - x , 2)  + pow( aim_y - y , 2)) * sqrt( 1  + pow( tan(yaw) , 2))));
		}
		if (tan(yaw) * (aim_x - x) + y > aim_y)
		{
			erro_angle = acos((-aim_x+x - tan(yaw) * (aim_y - y)) / ( sqrt( pow(aim_x - x , 2)  + pow( aim_y - y , 2)) * sqrt( 1  + pow( tan(yaw) , 2))));
		}
	}
	speed = 0.2;
} 

float node_x[100];
float node_y[100];
float line_interval = 0.5;
//build nodes function
void BuildNode( )
{
	for(int i = 0;i < 50;++i)
	{
		node_x[i+50] =  line_interval * i;
		node_y[i+50] =  line_interval * i;
		node_x[49-i] = -line_interval - line_interval * i;
		node_y[49-i] = -line_interval - line_interval * i;
	}
}

float dist_x[100];
float dist_y[100];
int index_nearest_x;
int index_nearest_y;
float dist_min_x;
float dist_min_y;
void FindNearestNode( )
{
	for(int i=0;i<100;++i)
	{
		dist_x[i] = fabs(x - node_x[i]);
		dist_y[i] = fabs(y- node_y[i]);
	}
	dist_min_x = dist_x[0];
	dist_min_y = dist_y[0];
	for(int i=0;i<100;++i)
	{
		if(dist_min_x > dist_x[i])
		{
			dist_min_x = dist_x[i];
			index_nearest_x = i;
		}

		if(dist_min_y > dist_y[i])
		{
			dist_min_y = dist_y[i];
			index_nearest_y = i;
		}
	}
}

float scan_robot_length = 0.083;
float x_scan_origin;
float y_scan_origin;
void GetScanCoordinate()
{
	x_scan_origin = x - scan_robot_length * cos(yaw);
	y_scan_origin = y - scan_robot_length * sin(yaw);
}

float local_radius_min = 0.5;
float local_radius_max = 1;
vector<int> radius_max_min_x;
vector<int> radius_max_min_y;
int num_min_max_node;
int intj = 0;
vector<int> UnableReachNodeX;
vector<int> UnableReachNodeY;
int numUnableReachNode;
int numUnableReachNodeMax = 20;
//.3  find the circle node; shrinking number of nodes in local_radius_min and  local_radius_max

void FindCircleNode()
{
	numUnableReachNode = UnableReachNodeX.size();
	//保持不可达节点数量在max范围内
	//if(numUnableReachNode >= numUnableReachNodeMax )
	//{
	 // UnableReachNodeX.erase(UnableReachNodeX.begin());
	//  UnableReachNodeY.erase(UnableReachNodeY.begin());
	//}
	//判断
	if(numUnableReachNode > 1)
	{
	  if(UnableReachNodeX[numUnableReachNode - 1] == UnableReachNodeX[numUnableReachNode - 2] && UnableReachNodeY[numUnableReachNode - 1] == UnableReachNodeY[numUnableReachNode - 2])
	  {
	    local_radius_max+=0.5;
	    local_radius_min+=0.5;
	  }
	  else
	  {
	    local_radius_max-=0.5;
	    local_radius_min-=0.5;
	    if(local_radius_min<=0.5)   local_radius_min=0.5;
	    if(local_radius_max<=1)  local_radius_max=1;
	  }
	}
	
	for(int i=0;i<local_radius_max/line_interval + 1;++i)
		for(int j=0;j<local_radius_max/line_interval+1;++j)
		{
			int mark1 = 0;
			if(pow((node_x[index_nearest_x+i]-x_scan_origin),2)+pow((node_y[index_nearest_y+j]-y_scan_origin),2) <= pow(local_radius_max,2) && pow((node_x[index_nearest_x+i]-x_scan_origin),2)+pow((node_y[index_nearest_y+j]-y_scan_origin),2) >= pow(local_radius_min,2))
			{
			  //cout <<"发现与不可达区域一致" <<index_nearest_x + i<<"    "<<index_nearest_y +j << endl;
			  for(int ij = 0;ij < numUnableReachNode;++ij)
			  {
			    if(((index_nearest_x + i) == UnableReachNodeX[ij]) && index_nearest_y + j == UnableReachNodeY[ij])
			    {
			      cout <<"发现与不可达区域一致" <<index_nearest_x + i<<"    "<<index_nearest_y +j << endl;
			      mark1++;
			      //break;
			    }
			  }
			  if(mark1 == 0)
			  {
				radius_max_min_x.push_back(index_nearest_x+i);
				radius_max_min_y.push_back(index_nearest_y+j);
				//cout << index_nearest_x + i <<"   " <<index_nearest_y+j
			  }
			}
			if(pow((node_x[index_nearest_x-i-1]-x_scan_origin),2)+pow((node_y[index_nearest_y+j]-y_scan_origin),2) <= pow(local_radius_max,2) && pow((node_x[index_nearest_x-i-1]-x_scan_origin),2)+pow((node_y[index_nearest_y+j]-y_scan_origin),2) >= pow(local_radius_min,2))
			{
			   for(int ij = 0;ij < numUnableReachNode;++ij)
			  {
			    if(index_nearest_x - i - 1 == UnableReachNodeX[ij] && index_nearest_y + j==UnableReachNodeY[ij])
			    {
			       cout <<"发现与不可达区域一致" <<index_nearest_x + i<<"    "<<index_nearest_y +j << endl;
			      mark1 = 1;
			     // break;
			    }
			      
			  }
			  if(mark1 == 0)
			  {
				radius_max_min_x.push_back(index_nearest_x-i-1);
				radius_max_min_y.push_back(index_nearest_y+j);
			  }
			}
			if(pow((node_x[index_nearest_x-i-1]-x_scan_origin),2)+pow((node_y[index_nearest_y-j-1]-y_scan_origin),2) <= pow(local_radius_max,2) && pow((node_x[index_nearest_x-i-1]-x_scan_origin),2)+pow((node_y[index_nearest_y-j-1]-y_scan_origin),2) >= pow(local_radius_min,2))
			{
			  for(int ij = 0;ij < numUnableReachNode;++ij)
			  {
			    if(index_nearest_x - i -1 == UnableReachNodeX[ij] && index_nearest_y - j - 1==UnableReachNodeY[ij])
			    {
			      cout <<"发现与不可达区域一致" <<index_nearest_x + i<<"    "<<index_nearest_y +j << endl;
			      mark1 = 1;
			      //break;
			    }
			  }
			  if(mark1 == 0)
			  {
			    radius_max_min_x.push_back(index_nearest_x-i-1);
			    radius_max_min_y.push_back(index_nearest_y-j-1);
			  }
			}
			if(pow((node_x[index_nearest_x+i]-x_scan_origin),2)+pow((node_y[index_nearest_y-j-1]-y_scan_origin),2) <= pow(local_radius_max,2) && pow((node_x[index_nearest_x+i]-x_scan_origin),2)+pow((node_y[index_nearest_y-j-1]-y_scan_origin),2) >= pow(local_radius_min,2))
			{
			   for(int ij = 0;ij < numUnableReachNode;++ij)
			  {
			    if(index_nearest_x + i == UnableReachNodeX[ij] && index_nearest_y-j-1 == UnableReachNodeY[ij])
			    {
			     cout <<"发现与不可达区域一致" <<index_nearest_x + i<<"    "<<index_nearest_y +j << endl;
			      mark1 = 1;
			      //break;
			    }
			  }
			  if(mark1 == 0)
			  {
				radius_max_min_x.push_back(index_nearest_x+i);
				radius_max_min_y.push_back(index_nearest_y-j-1);
			  }
			}
		}
	num_min_max_node = radius_max_min_x.size();
}

float left_slope;
float right_slope;
vector<int> sector_x;
vector<int> sector_y;
int num_sector;
void FindSectorNode()
{
	left_slope = tan(yaw + Pi/6);
	right_slope = tan(yaw - Pi/6);
	for (int i = 0; i < num_min_max_node; ++i)
	{
		//sector area's all in x >  x_scan_origin
		if(yaw - Pi/6>-Pi/2 && yaw + Pi/6<Pi/2)
		{
			if(left_slope * (node_x[radius_max_min_x[i]] - x_scan_origin) + y_scan_origin > node_y[radius_max_min_y[i]] && right_slope * (node_x[radius_max_min_x[i]] - x_scan_origin) + y_scan_origin < node_y[radius_max_min_y[i]])
			{
				sector_x.push_back(radius_max_min_x[i]);
				sector_y.push_back(radius_max_min_y[i]);
			}
				
		}
		//sector area's in x>0 and x<0	
		else if( yaw - Pi/6<-Pi/2 && yaw + Pi/6>-Pi/2)
		{
			if(left_slope * (node_x[radius_max_min_x[i]] - x_scan_origin) + y_scan_origin > node_y[radius_max_min_y[i]] && right_slope * (node_x[radius_max_min_x[i]] - x_scan_origin) + y_scan_origin > node_y[radius_max_min_y[i]])
			{
				sector_x.push_back(radius_max_min_x[i]);
				sector_y.push_back(radius_max_min_y[i]);
			}
				
		}
		//sector area's in x>0 and x<0	
		//sector area's all in x<x_scan_origin and y<y_scan_origin...........................................................
		else if(yaw - Pi/6>-Pi && yaw + Pi/6<-Pi/2)
		{
			if(left_slope * (node_x[radius_max_min_x[i]] - x_scan_origin) + y_scan_origin < node_y[radius_max_min_y[i]] && right_slope * (node_x[radius_max_min_x[i]] - x_scan_origin) + y_scan_origin > node_y[radius_max_min_y[i]])
			{
				sector_x.push_back(radius_max_min_x[i]);
				sector_y.push_back(radius_max_min_y[i]);
			}
				
		}
		else if(yaw - Pi/6>Pi/2 && yaw + Pi/6<Pi)
		{
			if(left_slope * (node_x[radius_max_min_x[i]] - x_scan_origin) + y_scan_origin < node_y[radius_max_min_y[i]] && right_slope * (node_x[radius_max_min_x[i]] - x_scan_origin) + y_scan_origin > node_y[radius_max_min_y[i]])
			{
				sector_x.push_back(radius_max_min_x[i]);
				sector_y.push_back(radius_max_min_y[i]);
			}
				
		}
		else if(yaw - Pi/6<Pi/2 && yaw + Pi/6>Pi/2)
		{
			if(left_slope * (node_x[radius_max_min_x[i]] - x_scan_origin) + y_scan_origin < node_y[radius_max_min_y[i]] && right_slope * (node_x[radius_max_min_x[i]] - x_scan_origin) + y_scan_origin < node_y[radius_max_min_y[i]])
			{
				sector_x.push_back(radius_max_min_x[i]);
				sector_y.push_back(radius_max_min_y[i]);
			}
				
		}
		else
		{
			if(left_slope * (node_x[radius_max_min_x[i]] - x_scan_origin) + y_scan_origin < node_y[radius_max_min_y[i]] && right_slope * (node_x[radius_max_min_x[i]] - x_scan_origin) + y_scan_origin > node_y[radius_max_min_y[i]])
			{
				sector_x.push_back(radius_max_min_x[i]);
				sector_y.push_back(radius_max_min_y[i]);
			}
				
		}
	}
	num_sector = sector_x.size();
}

float scan_min = 0.45;//minmum  value:0.45m
float scan_max = 2;//maxmum value:5m
vector<int> scan_x;
vector<int> scan_y;
vector<int> scan_min_index;
int num_scan;
float node_angle;
float difference_angle[20];
float scan_angle[20];
vector<float> sectorAngle;
int scan_overflow_index;
float min_difference_angle;
int min_difference_angle_index;
float angle_increment = 60.0/19;
float medianYaw;
void FindScanNode()
{
	//Get every scan angle,获得每种情况下的激光传感器中每个间隔的角度
	if(yaw - Pi/6 > -Pi && yaw + Pi/6 < Pi)
	{
		for(int i = 0;i < 20;++i)
		{
			scan_angle[i] = yaw - 30.0 * Pi / 180 + angle_increment * Pi / 180 * i; 
			//cout <<" yaw - Pi/6 > -Pi && yaw + Pi/6 < Pi " << endl;
			//cout << endl;
		}
	}
	else
	{
		 // cout <<" else " << endl;
		  if(yaw > 0 && yaw + Pi/6 > Pi)
		  {
			    for(int i = 0;i < 20;++i)
			    {
				    scan_angle[i] = yaw - 30.0 * Pi / 180 + angle_increment * Pi / 180 * i; 
			//cout << endl;
			     }
		   }
		  if(yaw < 0 && yaw - Pi / 6 < -Pi)
		  {
			    medianYaw =2 * Pi + yaw;
			    for(int i = 0;i < 20;++i)
			    {
				    scan_angle[i] = medianYaw - 30.0 * Pi / 180 + angle_increment * Pi / 180 * i; 
				    //cout << endl;
			    }
		  }
	}
	// Get node's angle 
	for(int i = 0;i < num_sector; ++i)
	{
		if(yaw - Pi/6 > -Pi && yaw + Pi/6 < Pi)
		{
			if(node_y[sector_y[i]] > y_scan_origin)
				node_angle = acos((node_x[sector_x[i]]-x_scan_origin) / ( sqrt( pow(node_x[sector_x[i]] - x_scan_origin , 2)  + pow( node_y[sector_y[i]] - y_scan_origin , 2)) ));	
			else
				node_angle = -acos((node_x[sector_x[i]]-x_scan_origin) / ( sqrt( pow(node_x[sector_x[i]] - x_scan_origin , 2)  + pow( node_y[sector_y[i]] - y_scan_origin , 2)) ));
		}
		else
		{
			if(node_y[sector_y[i]] > y_scan_origin)
				node_angle = acos((node_x[sector_x[i]]-x_scan_origin) / ( sqrt( pow(node_x[sector_x[i]] - x_scan_origin , 2)  + pow( node_y[sector_y[i]] - y_scan_origin , 2)) ));	
			else
				node_angle =2 * Pi -acos((node_x[sector_x[i]]-x_scan_origin) / ( sqrt( pow(node_x[sector_x[i]] - x_scan_origin , 2)  + pow( node_y[sector_y[i]] - y_scan_origin , 2)) ));
		}
		
		sectorAngle.push_back(node_angle);
		//cout <<"node_angle : "<< node_angle << endl;
		for (int j = 0; j < 20; ++j)
		{
			difference_angle[j] = fabs(node_angle - scan_angle[j]);
		}		
		min_difference_angle = difference_angle[0];
		min_difference_angle_index = 0;
		for (int ii = 0; ii < 20; ++ii)
		{
			if(difference_angle[ii] < min_difference_angle)
			{
				min_difference_angle = difference_angle[ii];
				min_difference_angle_index = ii;
			}
		}
		if(sqrt(pow((node_x[sector_x[i]]-x_scan_origin),2)+pow((node_y[sector_y[i]]-y_scan_origin),2))< scan_ranges[min_difference_angle_index]-1)
		{
			scan_x.push_back(sector_x[i]) ;
			scan_y.push_back(sector_y[i]) ;
			scan_min_index.push_back(min_difference_angle_index);
		}
	}
	 num_scan = scan_x.size();		
}

vector<float> dist_scan_node_aim;
//记录机器人已经到达的节点
vector<int>  RobotReachNodeX;
vector<int>  RobotReachNodeY;
float min_scan_node_aim;
int index_min_scan_node_aim; 
float CurAimX;
float CurAimY;
float lastCurAimX;
float lastCurAimY;


void FindOptimalNode(float aim_x, float aim_y)
{
  //如果节点不可达，则删除该节点，如下：
//   	if(intj == 1)
// 	{
// 		    //将不可到达的节点记录在动态矩阵中
// 		    cout <<"将不可到达的节点记录在动态矩阵中" << radius_max_min_x[index_min_scan_node_aim]<<"  "<<radius_max_min_y[index_min_scan_node_aim]<<endl;
// 		    UnableReachNodeX.push_back(radius_max_min_x[index_min_scan_node_aim]);
// 		    UnableReachNodeY.push_back(radius_max_min_y[index_min_scan_node_aim]);
// 		    //去掉圆形区域中不可达的节点
// 		    radius_max_min_x.erase(radius_max_min_x.begin() + index_min_scan_node_aim);
// 		    radius_max_min_y.erase(radius_max_min_y.begin() + index_min_scan_node_aim );
// 		    
// 		    for(int i=0;i<num_min_max_node;++i)
// 		    {
// // 		scan_x.pop_back();
// // 		scan_y.pop_back();
// 			  dist_scan_node_aim.pop_back();
// // 		scan_min_index.pop_back();
// 		    }	
// 		    num_min_max_node = radius_max_min_x.size();
// 	}
	lastCurAimX = CurAimX;
	lastCurAimY = CurAimY;
	if(pow(aim_x - x,2) + pow(aim_y - y,2) >= pow(scan_max , 2))
	{
		for(int i = 0;i < num_min_max_node;++i)
		{
			dist_scan_node_aim.push_back(pow(node_x[radius_max_min_x[i]] - aim_x,2) + pow(node_y[radius_max_min_y[i]] - aim_y,2));
		}
		min_scan_node_aim = dist_scan_node_aim[0];
		index_min_scan_node_aim = 0;
		for(int i = 0;i < num_min_max_node;++i)
		{
			if(dist_scan_node_aim[i] < min_scan_node_aim)
			{
				index_min_scan_node_aim = i;
				min_scan_node_aim = dist_scan_node_aim[i];
			}
		}
		CurAimX =node_x[radius_max_min_x[index_min_scan_node_aim]];
		CurAimY = node_y[radius_max_min_y[index_min_scan_node_aim]];
	}
	if(pow(aim_x - x,2) + pow(aim_y - y,2) < pow(scan_max , 2))
	{
	  	for(int i = 0;i < num_min_max_node;++i)
		{
			dist_scan_node_aim.push_back(pow(node_x[radius_max_min_x[i]] - aim_x,2) + pow(node_y[radius_max_min_y[i]] - aim_y,2));

		}
		CurAimX =aim_x;
		CurAimY = aim_y;
	}
}

void PointToTarget(float aim_x,float aim_y)
{
	ros::NodeHandle nh;
	ros::Publisher pub_run = nh.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/navi",1000);
	geometry_msgs::Twist run;
	erro_angle  = 3.14;
	//cout << "start point to target!"<< endl;
	//cout << "warting"<<endl;
	//ros::Rate rate(50);
	while(fabs(erro_angle) * 180/Pi>= 1)
	{
		//cout << "." << " " ;
		fflush(stdout);
		usleep(100000);//100ms
		ros::spinOnce();
		if( yaw > -Pi/2 && yaw < Pi/2)
		{
			if (tan(yaw) * (aim_x - x) + y < aim_y)
			{
				erro_angle = acos((aim_x-x + tan(yaw) * (aim_y - y)) / ( sqrt( pow(aim_x - x , 2)  + pow( aim_y - y , 2)) * sqrt( 1  + pow( tan(yaw) , 2))));
			}
			if (tan(yaw) * (aim_x - x) + y > aim_y)
			{
				erro_angle = -acos((aim_x-x + tan(yaw) * (aim_y - y)) / ( sqrt( pow(aim_x - x , 2)  + pow( aim_y - y , 2)) * sqrt( 1  + pow( tan(yaw) , 2))));
			}
		}
		if( (yaw < -Pi/2 && yaw > -Pi) || (yaw > Pi/2 && yaw <Pi))
		{
			if (tan(yaw) * (aim_x - x) + y < aim_y)
			{
				erro_angle = -acos((-aim_x+x - tan(yaw) * (aim_y - y)) / ( sqrt( pow(aim_x - x , 2)  + pow( aim_y - y , 2)) * sqrt( 1  + pow( tan(yaw) , 2))));
			}
			if (tan(yaw) * (aim_x - x) + y > aim_y)
			{
				erro_angle = acos((-aim_x+x - tan(yaw) * (aim_y - y)) / ( sqrt( pow(aim_x - x , 2)  + pow( aim_y - y , 2)) * sqrt( 1  + pow( tan(yaw) , 2))));
			}
		}
		run.angular.z = 1*erro_angle;
		//run.linear.x = speed;
		pub_run.publish(run);
		//rate.sleep();
	}
	//cout<<endl;
}

void CoutToScreen(void)
{
	cout <<"-------------------cout  to screen---------------------------"<<endl;
	cout << "Robot current position: " << "x : "<< x << "  y : " << y <<" yaw : "<<yaw<<endl;
	cout << "激光传感器原点："  << "x : " <<x_scan_origin <<" y :  " << y_scan_origin << endl;
	cout << "激光传感器角度与测量的长度："<< endl;
	//for(int i = 0; i < 20; ++i)
	//{
		//cout <<"                   序号： "<< i<<"   角度："<<scan_angle[i]*180/Pi <<"   测量长度："<<scan_ranges[i] <<endl;
	//}  
	cout <<"扇形区域节点角度："<<endl;
	//for(int i = 0; i < num_sector; ++i)
	//{
		//cout <<"                   节点序号： "<< sector_x[i] <<" "<<sector_y[i]<<"   节点坐标："<< node_x[sector_x[i]] <<" "<<node_y[sector_y[i]]<<"  节点角度："  << sectorAngle[i] * 180 / Pi <<endl;
	//}  
	cout <<"环形区域节点个数："<< num_min_max_node << endl;
	cout<<"当前局部目标点"<<"x: "<<CurAimX<<"   y: "<<CurAimY<<endl;
	//if(num_scan == 0)
	//{
		//cout << " no scan's node " << endl;
		//cout << "starting turning!" << endl;
		//fflush(stdout);
	//}
	//else
	//{
		for(int i=0;i<num_min_max_node;++i)
		{
			//cout<<"04"<<"num_scan: "<<num_scan<<endl;
			cout<<radius_max_min_x[i]<<"   "<<radius_max_min_y[i]<<"  距离目标点距离: "<<dist_scan_node_aim[i]<<endl;
			//cout << scan_x[i] << "  "<< scan_y[i]<<"  f(x): "<<dist_scan_node_aim[i]<< "  which scan: "<<scan_min_index[i]<<"  length: " <<scan_ranges[scan_min_index[i]]<<endl;
		  
		}	
		cout <<"机器人不可到达的节点："<<endl;
		for(int i=0;i<numUnableReachNode;++i)
		{
		  cout << UnableReachNodeX[i] << "   " <<UnableReachNodeY[i] <<" 坐标：" <<node_x[UnableReachNodeX[i]]<<"   "<<node_y[UnableReachNodeY[i]]<<endl;
		}
	//}

	cout << endl;  
	cout <<"-------------------end---------------------------"<<endl;
}

void IfDistanceSmall(void)
{
  for(int i = 0;i < 20;++i)
  {
    if(scan_ranges[i] <=0.7)
    {
      ros::NodeHandle nh;
      ros::Publisher pub_run = nh.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/navi",1000);
      geometry_msgs::Twist run;
      run.linear.x = 0;
      run.angular.z = 0;
      pub_run.publish(run);
    }
  }
  
}


void DeleVector()
{
	for(int i=0;i<num_min_max_node;++i)
	{
		radius_max_min_x.pop_back();
		radius_max_min_y.pop_back();
	}
// 	for(int i=0;i<num_sector;++i)
// 	{
// 		sector_x.pop_back();
// 		sector_y.pop_back();
// 		sectorAngle.pop_back();
// 	}
 	for(int i=0;i<num_min_max_node;++i)
 	{
// 		scan_x.pop_back();
// 		scan_y.pop_back();
 		dist_scan_node_aim.pop_back();
// 		scan_min_index.pop_back();
 	}	

}

float  turningHeading = Pi/12;
float lastHeading;
float intervalYaw;
bool execute = false;
void IfNoOptimalNode()
{
	//cout << "  num_scan: "<<num_scan<<endl;
	execute = true;
	ros::NodeHandle nh;
	ros::Publisher pub_run = nh.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/navi",1000);
	geometry_msgs::Twist run;
	while (num_scan == 0)
	{
		cout << "大循环" << "yaw:"<<yaw * 180/Pi<<endl;
		ros::spinOnce();
		
		lastHeading = yaw;
		if (yaw >=-Pi && yaw + turningHeading <= Pi)
		{
			cout  << "yaw:" <<yaw << "   yaw - lastHeading : " << yaw - lastHeading <<"  lastHeading:"<< lastHeading<<endl;
			while((yaw - lastHeading) < turningHeading)
			{
				cout << "in while"<<endl;	
				ros::spinOnce();
				run.linear.x = 0;
				run.angular.z = Pi/6;
				pub_run.publish(run);
				usleep(20000);//20ms

			}
		}
		if (yaw + turningHeading > Pi && yaw < Pi)
		{
			//printf("3");
			intervalYaw = Pi;
			while(intervalYaw - lastHeading < turningHeading)
			{
				if (yaw < 0)
				{
					intervalYaw = 2 * Pi + yaw;
				}
				ros::spinOnce();
				run.linear.x = 0;
				run.angular.z = Pi/6;
				pub_run.publish(run);
				usleep(20000);//20ms
				cout<<"in while1 " <<endl;
			}
			cout << "jump while1" <<"   num_scan :"<<num_scan<<endl;

		}
		run.angular.z = 0;
		run.linear.x = 0;
		pub_run.publish(run);
		ros::spinOnce();
		DeleVector();
		FindCircleNode();
		FindSectorNode();
		FindScanNode();
		FindOptimalNode(Aim[0],Aim[1]);
		
	}
	cout << "jump while" <<"   num_scan :"<<num_scan<<endl;
	DeleVector();
	sleep(2); //wait 2s
	ros::spinOnce();
	FindCircleNode();
	FindSectorNode();
	FindScanNode();
	FindOptimalNode(Aim[0],Aim[1]);
	CoutToScreen();
}


int main(int argc,char** argv)
{
	cout<<"Starting GPS Date collecting"<<endl;
	ros::init(argc,argv,"gpsheadandrun");
	ros::NodeHandle nh;
	ros::Subscriber sub = nh.subscribe("/sensor_msgs/NavSatFix",1,&GpsDateRecieved);
	ros::Subscriber sub_imu = nh.subscribe("/mobile_base/sensors/imu_data",1,&ImuDataRecieved);
	ros::Subscriber sub_scan = nh.subscribe("/scan",1,&ScanDataRecieved);
	ros::Publisher pub_run = nh.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/navi",1000);
	geometry_msgs::Twist run;
	ros::Rate rate(50);
	cout << "Please input the aim position(like:x  y ):";
	for(int i = 0;i < 2;++i)
	{
		cin >> Aim[i];
	}
	PointToTarget(Aim[0],Aim[1]);
	sleep(2); //wait 2s
	BuildNode();
	CurAimX = x;
	CurAimY = y;
	intj = 0;
	while(ros::ok() && pow(Aim[0]- x,2) + pow(Aim[1] - y,2) >= 0.01)
	{
		ros::spinOnce(); 
		FindNearestNode();
		GetScanCoordinate();
		FindCircleNode();
		FindOptimalNode(Aim[0],Aim[1]);
		CoutToScreen();
		PointToTarget(CurAimX,CurAimY);
		UnableReachNodeX.push_back(radius_max_min_x[index_min_scan_node_aim]);
		UnableReachNodeY.push_back(radius_max_min_y[index_min_scan_node_aim]);
		while(pow(CurAimX- x,2) + pow(CurAimY - y,2) >= 0.01)
		{
		        //cout<<"02"<<endl;
			ros::spinOnce();
			intj = 0;
			for(int inti = 0;inti < 20; ++inti)
			{
			  if(scan_ranges[inti] < 0.7)
			  {
			    intj = 1;
			    //将不可到达的节点记录在动态矩阵中
			   
			    cout <<"距离太小，将重新进行规划" << endl;
			    break;
			  }
			}
			if(intj == 1)  break;
			NextPointAngleControl(CurAimX,CurAimY);
			run.angular.z = 2*erro_angle;
			run.linear.x = speed;
			pub_run.publish(run);
			usleep(20000);//20ms
		}
		erro_angle = 0;
		speed = 0;
		run.angular.z = 2*erro_angle;
		run.linear.x = speed;
		pub_run.publish(run);
		//usleep(2000000);  
		//cout << 01 <<endl;
		DeleVector();
	}
}


