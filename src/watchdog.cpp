#include <ros/ros.h>
#include <std_msgs/String.h>

#include <diagnostic_updater/diagnostic_updater.h>
#include <diagnostic_updater/publisher.h>

diagnostic_updater::HeaderlessTopicDiagnostic* topic_diagnostic;

void callback(const std_msgs::String::ConstPtr& msg)
{
    topic_diagnostic->tick();
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "watchdog_node");

    ros::NodeHandle nh;

    diagnostic_updater::Updater updater;

    updater.setHardwareID("publisher_monitor");

    double min_freq = 10.0;
    double max_freq = 10.0;

    diagnostic_updater::FrequencyStatusParam freq_param(
        &min_freq,
        &max_freq,
        0.1,
        10);

    topic_diagnostic =
        new diagnostic_updater::HeaderlessTopicDiagnostic(
            "publisher_topic",
            updater,
            freq_param);

    ros::Subscriber sub =
        nh.subscribe("publisher_topic", 100, callback);

    ros::Rate loop_rate(10);

    while (ros::ok())
    {
        updater.update();

        ros::spinOnce();

        loop_rate.sleep();
    }

    return 0;
}
