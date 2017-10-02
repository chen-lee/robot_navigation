project:服务机器人的环境认知与导航问题的研究

author:李诚

hardware:
        turtlebot2底座:包含里程计，陀螺仪，具体型号请查看:http://blog.csdn.net/qq_16583687/article/details/64443604;
        uwb模块:Decaware公司的DWM1000模块，具体查看:https://www.decawave.com/products/dwm1000-module;
        蓝牙模块:用于段距离通信，网上一大堆，均可以用

software:
        linux:ubuntu14.04
        ROS：indigo
        Gazebo:用于仿真，需要加GPS插件，http://wiki.ros.org/hector_gazebo_plugins，添加方式见瞿国权的毕业设计论文
        

algorithm:
        瞎子爬山法，又叫贪心算法
        三边定位法
        扩展卡尔曼滤波:http://www.paper.edu.cn/releasepaper/content/201707-90
        som
