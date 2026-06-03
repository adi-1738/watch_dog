#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/PointCloud.h>

#include <diagnostic_updater/diagnostic_updater.h>
#include <diagnostic_updater/publisher.h>

diagnostic_updater::Updater* updater;

// Expected frequencies
double velocity_min = 47.0;
double velocity_max = 53.0;

double lidar_min = 9.0;
double lidar_max = 11.0;

double stereo_min = 27.0;
double stereo_max = 33.0;

double slam_min = 9.0;
double slam_max = 11.0;

// Diagnostics
diagnostic_updater::TopicDiagnostic* velocity_diag;
diagnostic_updater::TopicDiagnostic* lidar_diag;
diagnostic_updater::TopicDiagnostic* stereo_diag;
diagnostic_updater::TopicDiagnostic* slam_diag;

void velocityCallback(
const nav_msgs::Odometry::ConstPtr& msg)
{
    velocity_diag->tick(msg->header.stamp);
}

void lidarCallback(
const sensor_msgs::PointCloud::ConstPtr& msg)
{
    lidar_diag->tick(msg->header.stamp);
}

void stereoCallback(
const sensor_msgs::PointCloud::ConstPtr& msg)
{
    stereo_diag->tick(msg->header.stamp);
}

void slamCallback(
const sensor_msgs::PointCloud::ConstPtr& msg)
{
    slam_diag->tick(msg->header.stamp);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "watchdog_node");
    updater = new diagnostic_updater::Updater();
    updater->setHardwareID("vehicle_watchdog");

    ros::NodeHandle nh;

    updater->setHardwareID("vehicle_watchdog");

    velocity_diag =
    new diagnostic_updater::TopicDiagnostic(
        "/estimation/velocity",
        *updater,
        diagnostic_updater::FrequencyStatusParam(
            &velocity_min,
            &velocity_max,
            0.1,
            10),
        diagnostic_updater::TimeStampStatusParam());

    lidar_diag =
    new diagnostic_updater::TopicDiagnostic(
        "/lidar/cone_position_cloud",
        *updater,
        diagnostic_updater::FrequencyStatusParam(
            &lidar_min,
            &lidar_max,
            0.1,
            10),
        diagnostic_updater::TimeStampStatusParam());

    stereo_diag =
    new diagnostic_updater::TopicDiagnostic(
        "/stereo_cone_perception/cones",
        *updater,
        diagnostic_updater::FrequencyStatusParam(
            &stereo_min,
            &stereo_max,
            0.1,
            10),
        diagnostic_updater::TimeStampStatusParam());

    slam_diag =
    new diagnostic_updater::TopicDiagnostic(
        "/slam/map",
        *updater,
        diagnostic_updater::FrequencyStatusParam(
            &slam_min,
            &slam_max,
            0.1,
            10),
        diagnostic_updater::TimeStampStatusParam());

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

    ros::Rate rate(10);

    while(ros::ok())
    {
        ros::spinOnce();

        updater->update();

        rate.sleep();
    }

    delete velocity_diag;
    delete lidar_diag;
    delete stereo_diag;
    delete slam_diag;

    return 0;
}


