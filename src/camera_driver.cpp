#include <ros/ros.h>
#include <image_transport/image_transport.h>

#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

using namespace std;
using namespace cv;

VideoCapture *capture;

int imageNumber;
int key;

int main(int argc, char** argv) 
{
	ros::init(argc, argv, "cameleon_camera");
	ros::NodeHandle n = ros::NodeHandle("~");
	image_transport::ImageTransport it(n);
	//Change manually parameters of camera at 192.168.1.6
        //Copy the url for different params
        VideoCapture cap("rtsp://192.168.1.6/h264.sdp?res=half&x0=0&y0=0&x1=2048&y1=1536&qp=20&ratelimit=10000&doublescan=0&ssn=16570"); // open the default camera
	if(!cap.isOpened())  // check if we succeeded
		return -1;

	Mat edges;
	cv_bridge::CvImagePtr cv_ptr;
	image_transport::Publisher imagePub = it.advertise("/output_video", 1);

	ros::Rate r(10); 
	Mat frame;
	while (ros::ok())
	{
		cap >> frame;
		sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
		imagePub.publish(msg);
		ros::spinOnce();
		r.sleep();
	}
	return 0;
}

