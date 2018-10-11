#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;
using namespace cv;

struct stat st = {0};
int count = 0;
int imgcount =0;

void detectAndDisplay( Mat frame, bool flag);
std::vector<Rect> faces;

String face_cascade_name = "haarcascades/haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;
string window_name = "Capture - Face detection";
string faceToSave;
bool DEBUG = true;

int main( int argc, const char** argv )
{
	CvCapture* capture;
	Mat frame;
	// Load the cascades
	if( !face_cascade.load( face_cascade_name ) )
	{ 
		printf("--(!)Error loading\n"); 
		return -1; 
	};
	if( DEBUG)
	{
		cout << "Enter name of the person \n";
		cin >> faceToSave;
		string dirpath = "/home/sdyavarasetti/Work_Space/FR-OpenCV/FR_test1/faces/" + faceToSave; 
		mkdir(dirpath.c_str(), 0777);
		cout << "Dir is created with name " <<faceToSave << endl;
		cout << "Enter s to save cropped face \n";
	}
	capture = cvCaptureFromCAM( -1 );
	if( capture )
	{
		while( true )
		{
			bool flag = false;
			frame = cvQueryFrame( capture );
			int c = waitKey(2);
			if( (int)c == 27 ) 
			{ 
				break;
			}
			else
				if((int)c == 83 || (int)c==115)
				{
					flag = true;
				}
			if( !frame.empty() )
			{ detectAndDisplay( frame , flag); }
			else
			{ 
				printf(" --(!) No captured frame -- Break!"); 
				break; 
			}
		}
	}
}

void detectAndDisplay( Mat frame , bool flag)
{
	extern std::vector<Rect> faces;
	Mat frame_gray;
	extern int count;
	cvtColor( frame, frame_gray, CV_BGR2GRAY );
	equalizeHist( frame_gray, frame_gray );

	//-- Detect faces
	if(count % 5 == 0)
	{
		face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
		count = 0;
	}
	count++;

	for( size_t i = 0; i < faces.size(); i++ )
	{
		Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
		Rect myROI(faces[i].x,faces[i].y,faces[i].width,faces[i].height);
		rectangle( frame, myROI, Scalar( 0, 255, 0 ), 1, 8, 0 );
		Mat croppedImage;
		Mat(frame,myROI).copyTo(croppedImage);
		resize(croppedImage,croppedImage,Size(200,200),0,0,CV_INTER_AREA);
		if(flag)
		{
			stringstream ss;
			ss << imgcount ;
			string str = ss.str();
			string fileName = "faces/" + faceToSave + "/" + faceToSave + str +".png";
			string savedInfoString = "face saved to " + fileName ;
			cout<< savedInfoString <<endl;
			imwrite(fileName,croppedImage);
			imgcount++;
		}
	}
	imshow( window_name, frame );
}

