#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <image_transport/image_transport.h>
#include <boost/filesystem.hpp>

cv_bridge::CvImagePtr cv_ptr;

bool visionFlag = false;

void image_cb(const sensor_msgs::ImageConstPtr& img){
  cv_ptr = cv_bridge::toCvCopy(img, "bgr8");
  visionFlag = true;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "cali");
  ros::NodeHandle nh;
  image_transport::ImageTransport it_(nh);

  image_transport::Subscriber sub_vision = it_.subscribe("/image_raw", 1, image_cb);
  ros::Rate rate(5.0f);


  time_t timer = time(NULL);
  struct tm* ltime = localtime(&timer);
  char fn_date[13];
  sprintf(fn_date, "%02d%02d%02d_%02d%02d%02d", (ltime->tm_year) - 100, (ltime->tm_mon) + 1, ltime->tm_mday,
          ltime->tm_hour, ltime->tm_min, ltime->tm_sec);
  std::string dirName(fn_date);

  boost::filesystem::create_directory("/home/jongsik/.ros/" + dirName);

  while (ros::ok())
  {
    if (visionFlag)
    {
      time_t timer = time(NULL);
      struct tm* ltime = localtime(&timer);
      char fn_date[13];
      sprintf(fn_date, "%02d%02d%02d_%02d%02d%02d", (ltime->tm_year) - 100, (ltime->tm_mon) + 1, ltime->tm_mday,
              ltime->tm_hour, ltime->tm_min, ltime->tm_sec);
      std::string fileName(fn_date);
      cv::imwrite("/home/jongsik/.ros/" + dirName + "/" + fileName + ".png", cv_ptr->image);
      ROS_INFO("/home/jongsik/.ros/%s/%s.png", dirName.c_str(), fileName.c_str());
    }
    ros::spinOnce();
    rate.sleep();
  }
}