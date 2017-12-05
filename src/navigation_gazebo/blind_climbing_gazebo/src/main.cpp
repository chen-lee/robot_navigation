#include <ros/ros.h>
#include <ros_robot_nav.h>
#include <map.h>
using namespace std;

int main(int argc, char** argv)
{
    ros::init(argc, argv, "blind_climbing");
    Robot_Nav rn;
    vector<float> goal_pose(3);
    goal_pose[0] = 10;
    goal_pose[1] = 0;
    rn.climb_goal(goal_pose);
    return 0;
}
