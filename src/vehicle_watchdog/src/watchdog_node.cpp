#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/PointCloud.h>

// Velocity

int velocity_message_count = 0;
ros::Time velocity_start_time;

// Lidar

int lidar_message_count = 0;
ros::Time lidar_start_time;

// Stereo

int stereo_message_count = 0;
ros::Time stereo_start_time;

// SLAM

int slam_message_count = 0;
ros::Time slam_start_time;


void velocityCallback(
const nav_msgs::Odometry::ConstPtr& msg)
{
    velocity_message_count++;

    ros::Time current_time = ros::Time::now();

    if(velocity_start_time.isZero())
    {
        velocity_start_time = current_time;
    }

    double elapsed_time =
    (current_time - velocity_start_time).toSec();

    if(elapsed_time >= 1.0)
    {
        double frequency =
        velocity_message_count / elapsed_time;

        if(frequency < 47.0)
        {
            ROS_WARN("[LOW] Velocity: %.2f Hz",
                     frequency);
        }
        else
        {
            ROS_INFO("[OK] Velocity: %.2f Hz",
                     frequency);
        }

        velocity_message_count = 0;
        velocity_start_time = current_time;
    }
}


void lidarCallback(
const sensor_msgs::PointCloud::ConstPtr& msg)
{
    lidar_message_count++;

    ros::Time current_time = ros::Time::now();

    if(lidar_start_time.isZero())
    {
        lidar_start_time = current_time;
    }

    double elapsed_time =
    (current_time - lidar_start_time).toSec();

    if(elapsed_time >= 1.0)
    {
        double frequency =
        lidar_message_count / elapsed_time;

        if(frequency < 9.0)
        {
            ROS_WARN("[LOW] Lidar: %.2f Hz",
                     frequency);
        }
        else
        {
            ROS_INFO("[OK] Lidar: %.2f Hz",
                     frequency);
        }

        lidar_message_count = 0;
        lidar_start_time = current_time;
    }
}

void stereoCallback(
const sensor_msgs::PointCloud::ConstPtr& msg)
{
    stereo_message_count++;

    ros::Time current_time = ros::Time::now();

    if(stereo_start_time.isZero())
    {
        stereo_start_time = current_time;
    }

    double elapsed_time =
    (current_time - stereo_start_time).toSec();

    if(elapsed_time >= 1.0)
    {
        double frequency =
        stereo_message_count / elapsed_time;

        if(frequency < 27.0)
        {
            ROS_WARN("[LOW] Stereo: %.2f Hz",
                     frequency);
        }
        else
        {
            ROS_INFO("[OK] Stereo: %.2f Hz",
                     frequency);
        }

        stereo_message_count = 0;
        stereo_start_time = current_time;
    }
}

void slamCallback(
const sensor_msgs::PointCloud::ConstPtr& msg)
{
    slam_message_count++;

    ros::Time current_time = ros::Time::now();

    if(slam_start_time.isZero())
    {
        slam_start_time = current_time;
    }

    double elapsed_time =
    (current_time - slam_start_time).toSec();

    if(elapsed_time >= 1.0)
    {
        double frequency =
        slam_message_count / elapsed_time;

        if(frequency < 9.0)
        {
            ROS_WARN("[LOW] SLAM: %.2f Hz",
                     frequency);
        }
        else
        {
            ROS_INFO("[OK] SLAM: %.2f Hz",
                     frequency);
        }

        slam_message_count = 0;
        slam_start_time = current_time;
    }
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "watchdog_node");

    ros::NodeHandle nh;
    ros::Subscriber velocity_sub =
nh.subscribe("/estimation/velocity",
             100,
             velocityCallback);

ros::Subscriber lidar_sub =
nh.subscribe("/lidar/cone_position_cloud",
             100,
             lidarCallback);

ros::Subscriber stereo_sub =
nh.subscribe("/stereo_cone_perception/cones",
             100,
             stereoCallback);

ros::Subscriber slam_sub =
nh.subscribe("/slam/map",
             100,
             slamCallback);
             
               ROS_INFO("Vehicle watchdog started");

    ros::spin();

    return 0;
             
             }



