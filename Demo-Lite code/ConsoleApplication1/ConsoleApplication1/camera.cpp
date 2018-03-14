#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<stdio.h>
//#include"ros/ros.h"
//#include"std_msgs/String.h"
#include<math.h>
#include<sstream>
//#include<std_msgs/Float64.h>
//#include<window.cpp>

using namespace cv;
using namespace std;

int controlnumber;
int controlaccount;
int controltemp1 = 0;
int controltemp2 = 0;
int leftnumber;
int leftaccount;
int rightnumber;
int rightaccount;
const int MaxSpeedValue = 100;
int SpeedValue=0;

//ros::Publisher chatter;
//std_msgs::Float64 msg;
int msg;

int ExtremumValue(int i1, int i2, int i3, int type);
int AbsoluteValue(int i1, int i2);
void on_Trackbar(int, void *);
int RGBprocessing(Mat picture, int rows, int cols, int range);
void varify(int i1, int i2);
void result(int i1,int i2);

void initial(int value1, int value2,int value3)
{
	value1 = 0;
	value2 = 0;
	value3 = 0;
	Mat img = imread("The rule.png");
	if(!img.empty())
	{
	imshow("INSTRUCTION BOOK", img);
	}
}

int main(int argc,char **argv)
{
	//ros::init(argc,argv,"camera");
	//ros::NodeHandle n;
	//ros::Publisher chatter=n.advertise<std_msgs::Float64>("center",10);
	//ros::Rate loop_rate(10);

	//VideoCapture capture(0);
	controlaccount = 0;
	while (1)
	{
		initial(leftnumber, rightnumber,controlnumber);
		createTrackbar("[cm/Sec]", "Video monitor", &SpeedValue, MaxSpeedValue,on_Trackbar);

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

    	Mat image;    
    int cam_idx = 0;  
  
    if (argc == 2) {  
        cam_idx = atoi(argv[1]);  
    }  
  
    VideoCapture cap(cam_idx);  
    if (!cap.isOpened()) {  
        cerr << "Could not open camera." << endl;  
        exit(EXIT_FAILURE);  
    }  
    //namedWindow("Display Image");   
      
   // for(;;){  
      Mat frame;
      cap >> frame;   
     // imshow("Display Image", image);    
       //   waitKey(5);  
    //}  
       
  
   // ros::spin();  
    //return 0; 

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
		//Mat frame;
		//capture.read(frame);
		IplImage temp=IplImage(frame);
		IplImage *src=&temp;
		int row = frame.rows;
		int col = frame.cols;
		flip(frame, frame, 1);
		cvRectangle(src, cvPoint(25, 25), cvPoint(75, 75), CvScalar(255, 0, 0), 3, 4, 0);
		controlnumber = RGBprocessing(frame, 25, 25, 50);
		if (controlnumber > 350 && controlaccount == 0)
		{
			//printf("123 123 123 the number of temp1 is %d \n", controltemp1);
			controltemp1 = controltemp1 + 1;
			controltemp2 = 0;
		}
		if (controltemp1 > 20)
		{
			cvNamedWindow("The output signal");
			controlaccount = 1;
			cvRectangle(src, cvPoint(50, 100), cvPoint(200, 250), CvScalar(0, 0, 255), 3, 4, 0);
			cvRectangle(src, cvPoint(450, 100), cvPoint(600, 250), CvScalar(0, 0, 255), 3, 4, 0);
			leftnumber = RGBprocessing(frame, 100, 50, 150);
			rightnumber = RGBprocessing(frame, 100, 450, 150);
			varify(leftnumber, rightnumber);
			result(leftaccount, rightaccount);
		}
		if (controlnumber > 350 && controlaccount == 1)
		{
			//printf("123 123 the number of temp is %d \n",controltemp2);
			controltemp2 = controltemp2 + 1;
		}
		if (controltemp2 > 35)
		{
			//printf("123	the number of temp2 is %d \n", controltemp2);
			controlaccount = 0;
			controltemp1 = 0;
			destroyWindow("The output signal");
		}
		if(!frame.empty()){
		imshow("Video monitor", frame);
		}
		//printf("the number of rightvalue is %d\n", rightnumber);
		//printf("the number of leftvalue is %d\n", leftnumber);
		//waitKey(5);
		//ros::spinOnce();
	}
	return 0;
}

void on_Trackbar(int, void *)
{
	printf("the velocity of speed now: %d \n", SpeedValue);
}

int RGBprocessing(Mat picture,int rows,int cols,int range)
{
	int value = 0;
	for (int i = rows; i < (rows+range); i++)
	{
		for (int j = cols; j < (cols+range); j++)
		{
			if (picture.at<Vec3b>(i, j)[2] > 95 && picture.at<Vec3b>(i, j)[1] > 40 && picture.at<Vec3b>(i, j)[0] > 20)
			{
				if (ExtremumValue(picture.at<Vec3b>(i, j)[0], picture.at<Vec3b>(i, j)[1], picture.at<Vec3b>(i, j)[2], 1) - ExtremumValue(picture.at<Vec3b>(i, j)[0], picture.at<Vec3b>(i, j)[1], picture.at<Vec3b>(i, j)[2], 0) > 15)
				{
					if (AbsoluteValue(picture.at<Vec3b>(i, j)[1], picture.at<Vec3b>(i, j)[2]) > 15 && picture.at<Vec3b>(i, j)[2] > picture.at<Vec3b>(i, j)[1] && picture.at<Vec3b>(i, j)[2] > picture.at<Vec3b>(i, j)[0])
						value = value + 1;
					//printf("number of leftnumber is %d \n", value);
				}
			}
			else if (picture.at<Vec3b>(i, j)[2] > 220 && picture.at<Vec3b>(i, j)[1] > 210 && picture.at<Vec3b>(i, j)[0] > 170)
			{
				if (AbsoluteValue(picture.at<Vec3b>(i, j)[2], picture.at<Vec3b>(i, j)[1]) <= 15 && picture.at<Vec3b>(i, j)[2] > picture.at<Vec3b>(i, j)[0] && picture.at<Vec3b>(i, j)[1] > picture.at<Vec3b>(i, j)[0])
					value = value + 1;
				//printf("number of leftnumber is %d \n", value);
			}
			
		}
	}
	return value;
}

int ExtremumValue(int i1, int i2, int i3, int type)
{
	int temp;
	if (type == 0)			  //minima
	{
		if (i1 < i2) temp = i1;
		else temp = i2;
		if (i3 < temp) temp = i3;
	}
	else if (type == 1)		  //maxima
	{
		if (i1 < i2) temp = i2;
		else temp = i1;
		if (temp < i3) temp = i3;
	}
	return temp;
}

int AbsoluteValue(int i1, int i2)
{
	int temp;
	if (i1 < i2)
		temp = i2 - i1;
	else
		temp = i1 - i2;
	return temp;
}

void varify(int i1, int i2)
{
	if (i1 >5000)
		 leftaccount = leftaccount + 1;
	else leftaccount = 0;
	if (i2 > 5000)
		rightaccount = rightaccount + 1;
	else rightaccount = 0;
	printf("the number of leftaccount is %d \n", leftaccount);
	printf("the number of rightaccount is %d \n", rightaccount);
}

void result(int i1, int i2)
{
	Mat img;
	if (i1 > 5 && i2 > 5)
	    {	img = imread("go.jpg");
		//msg.data=1;
	    }	
	else if (i1 > 5 && i2 <=5)
		{	img = imread("leftturn.jpg");
		//	msg.data=2;
		}
	else if (i1 <= 5 && i2 >5)
		{	img = imread("rightturn.jpg");
			//msg.data=3;
		}
	else
		{	img = imread("stop.jpg");
			if(!img.empty())
			{
			imshow("The output signal", img);
			}			
			//msg.data=0;
		}
	//chatter.publish(msg);
	//ros::spinOnce();
}
