#if _MSC_VER < 1600 // Visual Studio 2008 and older doesn't have stdint.h...
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int8 uint8_t;
#endif

// STL Header
#include <iostream>
#include <fstream>
//#include <cmath>
#include <sstream>
#include <string>
#include <cstring>

#include <math.h>
#include <windows.h>
  
// OpenCV Header
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
  
// o1. OpenNI Header
#include <OpenNI.h>
  
// n1. NiTE Header
#include <NiTE.h>

// OpenNN Header
#include <opennn.h>
  
// namespace
using namespace std;
using namespace openni;
using namespace nite;

const double RADIAN = 57.32f;
bool flag = true;
double orignal = 0.0f;
  
int main( int argc, char **argv )
{
	fstream out("data.txt",ios::out);
	CvVideoWriter *video_writer;
	video_writer = cvCreateVideoWriter("video.avi",CV_FOURCC('P','I','M','1'),25,cvSize(640,480),1);
  // o2. Initial OpenNI
  OpenNI::initialize();
  
  // o3. Open Device
  Device  mDevice;
  mDevice.open( ANY_DEVICE );
  
  // o4. create depth stream
  VideoStream mDepthStream;
  mDepthStream.create( mDevice, SENSOR_DEPTH );
  // o4a. set video mode
  VideoMode mDMode;
  mDMode.setResolution( 640, 480 );
  mDMode.setFps( 30 );
  mDMode.setPixelFormat( PIXEL_FORMAT_DEPTH_1_MM );
  mDepthStream.setVideoMode( mDMode);

  // o5. Create color stream
  VideoStream mColorStream;
  mColorStream.create( mDevice, SENSOR_COLOR );
  // o5a. set video mode
  VideoMode mCMode;
  mCMode.setResolution( 640, 480 );
  mCMode.setFps( 30 );
  mCMode.setPixelFormat( PIXEL_FORMAT_RGB888 );
  mColorStream.setVideoMode( mCMode);

  // o6. image registration
  mDevice.setImageRegistrationMode( IMAGE_REGISTRATION_DEPTH_TO_COLOR );
  
  // n2. Initial NiTE
  NiTE::initialize();
  
  // n3. create user tracker
  UserTracker mUserTracker;
  mUserTracker.create( &mDevice );
  mUserTracker.setSkeletonSmoothingFactor( 0.1f );
  
  // create OpenCV Window
  cv::namedWindow( "User Image",  CV_WINDOW_AUTOSIZE );
  
  // p1. start
  mColorStream.start();
  mDepthStream.start();
  
  while( true )
  {
    // main loop
    // p2-p5 ...
  // p2. prepare background
cv::Mat cImageBGR;
// p2a. get color frame
VideoFrameRef mColorFrame;
mColorStream.readFrame( &mColorFrame );
// p2b. convert data to OpenCV format
const cv::Mat mImageRGB( mColorFrame.getHeight(), mColorFrame.getWidth(),
                         CV_8UC3, (void*)mColorFrame.getData() );
// p2c. convert form RGB to BGR
cv::cvtColor( mImageRGB, cImageBGR, CV_RGB2BGR );
 
// p3. get user frame
UserTrackerFrameRef  mUserFrame;
mUserTracker.readFrame( &mUserFrame );
 
// p4. get users data
const nite::Array<UserData>& aUsers = mUserFrame.getUsers();
for( int i = 0; i < aUsers.getSize(); ++ i )
{
  const UserData& rUser = aUsers[i];
 
  // p4a. check user status
  if( rUser.isNew() )
  {
    // start tracking for new user
    mUserTracker.startSkeletonTracking( rUser.getId() );
  }
 
  if( rUser.isVisible() )
  {
    // p4b. get user skeleton
    const Skeleton& rSkeleton = rUser.getSkeleton();
    if( rSkeleton.getState() == SKELETON_TRACKED )
    {
      // p4c. build joints array
      SkeletonJoint aJoints[15];
      aJoints[ 0] = rSkeleton.getJoint( JOINT_HEAD );
      aJoints[ 1] = rSkeleton.getJoint( JOINT_NECK );
      aJoints[ 2] = rSkeleton.getJoint( JOINT_LEFT_SHOULDER );
      aJoints[ 3] = rSkeleton.getJoint( JOINT_RIGHT_SHOULDER );
      aJoints[ 4] = rSkeleton.getJoint( JOINT_LEFT_ELBOW );
      aJoints[ 5] = rSkeleton.getJoint( JOINT_RIGHT_ELBOW );
      aJoints[ 6] = rSkeleton.getJoint( JOINT_LEFT_HAND );
      aJoints[ 7] = rSkeleton.getJoint( JOINT_RIGHT_HAND );
      aJoints[ 8] = rSkeleton.getJoint( JOINT_TORSO );
      aJoints[ 9] = rSkeleton.getJoint( JOINT_LEFT_HIP );
      aJoints[10] = rSkeleton.getJoint( JOINT_RIGHT_HIP );
      aJoints[11] = rSkeleton.getJoint( JOINT_LEFT_KNEE );
      aJoints[12] = rSkeleton.getJoint( JOINT_RIGHT_KNEE );
      aJoints[13] = rSkeleton.getJoint( JOINT_LEFT_FOOT );
      aJoints[14] = rSkeleton.getJoint( JOINT_RIGHT_FOOT );
 
      // p4d. convert joint position to image
      cv::Point2f aPoint[15];
	  double aPoint_z[15];
	  /*body*/
	  {
		  /*
		  if(flag){
			  orignal = aJoints[8].getPosition().z;
			  flag = false;
		  }
		  char str[30];
		  sprintf(str, "%.3f cm",aJoints[8].getPosition().z / 10.0f);
		  cv::putText(cImageBGR, str, cv::Point(20,20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(110,210,100), 2, CV_AA);
		  if(((aJoints[8].getPosition().z - orignal) / 10.0f) > 35.0f){
			  sprintf(str, "b",aJoints[8].getPosition().z / 10.0f);
			  cv::putText(cImageBGR, str, cv::Point(220,20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(110,210,100), 2, CV_AA);
		  }
		  else if(((aJoints[8].getPosition().z - orignal) / 10.0f) < -35.0f){
			  sprintf(str, "f",aJoints[8].getPosition().z / 10.0f);
			  cv::putText(cImageBGR, str, cv::Point(220,20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(110,210,100), 2, CV_AA);
		  }
		  else{
			  sprintf(str, "org",aJoints[8].getPosition().z / 10.0f);
			  cv::putText(cImageBGR, str, cv::Point(220,20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(110,210,100), 2, CV_AA);
		  }
		  */
	  }
	  /*Left Foot*/
	  {
		  /*
		  char str[30];
		  sprintf(str, "%.3f cm",aJoints[13].getPosition().z / 10.0f);
		  cv::putText(cImageBGR, str, cv::Point(20,50), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(110,210,100), 2, CV_AA);
		  */
	  }
	  /*Right Foot*/
	  {
		  /*
		  char str[30];
		  sprintf(str, "%.3f cm",aJoints[14].getPosition().z / 10.0f);
		  cv::putText(cImageBGR, str, cv::Point(120,50), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(110,210,100), 2, CV_AA);
		  */
	  }
      for( int  s = 0; s < 15; ++ s )
      {
		aPoint_z[s] = aJoints[s].getPosition().z;
      }
      for( int  s = 0; s < 15; ++ s )
      {
        const Point3f& rPos = aJoints[s].getPosition();
		mUserTracker.convertJointCoordinatesToDepth( 
                                 rPos.x, rPos.y, rPos.z,
                                 &(aPoint[s].x), &(aPoint[s].y));
		//aPoint_z[s] = rPos.z;
      }

	  double leftElbow = 0; 
	  double leftShoulder = 0; 
	  double rightElbow = 0; 
	  double rightShoulder = 0;

	  {     // 2:左肩  4:左肘 6:左腕 
      	    CvPoint3D32f shoulder = cvPoint3D32f(aPoint[2].x,aPoint[2].y,aPoint_z[2]);
			CvPoint3D32f elbow = cvPoint3D32f(aPoint[4].x,aPoint[4].y,aPoint_z[4]);
			CvPoint3D32f wrist = cvPoint3D32f(aPoint[6].x,aPoint[6].y,aPoint_z[6]);

			CvPoint3D32f elbowShoulder = cvPoint3D32f((aPoint[2].x-aPoint[4].x), (aPoint[2].y-aPoint[4].y), (aPoint_z[2]-aPoint_z[4]));
			CvPoint3D32f elbowWrist = cvPoint3D32f((aPoint[6].x-aPoint[4].x), (aPoint[6].y-aPoint[4].y), (aPoint_z[6]-aPoint_z[4]));
			
			double scalarElbowShoulder = sqrt(pow(elbowShoulder.x,2)+pow(elbowShoulder.y,2)+pow(elbowShoulder.z,2)); // 純量 = 分量平方相加開根號 
			double scalarElbowWrist = sqrt(pow(elbowWrist.x,2)+pow(elbowWrist.y,2)+pow(elbowWrist.z,2));
			
			double dotProduct = ((elbowShoulder.x*elbowWrist.x)+(elbowShoulder.y*elbowWrist.y)+(elbowShoulder.z*elbowWrist.z)); // 內積 = 分量相乘後相加 
			
			double cosLeftElbow;

			// cosA = (AB*AC)/(|AB|*|AC|)
			cosLeftElbow = dotProduct/(abs(scalarElbowShoulder)*abs(scalarElbowWrist));
			leftElbow = acos(dotProduct/(abs(scalarElbowShoulder)*abs(scalarElbowWrist))) * RADIAN;
			
			// 取手肘與對稱點連線 若y分量為負,則角度為360-x
			CvPoint projection = cvPoint(elbowShoulder.x*(scalarElbowWrist*cosLeftElbow)/scalarElbowShoulder, elbowShoulder.y*(scalarElbowWrist*cosLeftElbow)/scalarElbowShoulder);
			CvPoint reflectWrist = cvPoint(elbow.x+projection.x, elbow.y+projection.y);

			if(elbowWrist.y < 0){ //手腕朝上
				if(reflectWrist.y < wrist.y){ //投影點高於手腕
					reflectWrist.y = wrist.y+1; //投影點不超過手腕
				}
			}
			if(elbowWrist.y > 0){ //手腕朝下
				if(reflectWrist.y > wrist.y){ //投影點低於手腕
					reflectWrist.y = wrist.y-1; //投影點不低於手腕
				}
			}

			CvPoint reflectionPoint = cvPoint(reflectWrist.x,reflectWrist.y);
			cv::circle( cImageBGR, reflectionPoint, 3, cv::Scalar( 0, 255, 255 ), 2 );
			
			if( (reflectWrist.y-wrist.y) < 0){ //若手肘至對稱點的連線y分量小於零,即手腕位於下半部
				leftElbow = 360 - leftElbow;
			}
			
			out << leftElbow << " ";
			char str[30];
			//sprintf(str, "%.4f %.4f",scalarElbowWrist,scalarElbowShoulder);
			//cv::putText(cImageBGR, str, cv::Point(80,20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(110,210,100), 2, CV_AA);
			sprintf(str, "%.4f",leftElbow);
			cv::putText(cImageBGR, str, cv::Point(elbow.x-75,elbow.y+15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(110,210,100), 2, CV_AA);
	  }
	  {     // 2:左肩  4:左肘   1:脖子 8:身體 
		    
            CvPoint3D32f x = cvPoint3D32f(aPoint[4].x,aPoint[4].y,aPoint_z[4]);
			CvPoint3D32f y = cvPoint3D32f(aPoint[2].x,aPoint[2].y,aPoint_z[2]);
			CvPoint3D32f z = cvPoint3D32f((aPoint[2].x+aPoint[3].x)/2,(aPoint[2].y+aPoint[3].y)/2,(aPoint_z[2]+aPoint_z[3])/2);
			
			CvPoint3D32f yx = cvPoint3D32f(aPoint[4].x-aPoint[2].x,aPoint[4].y-aPoint[2].y, aPoint_z[4]-aPoint_z[2]);
			CvPoint3D32f yz = cvPoint3D32f(aPoint[1].x-aPoint[8].x, aPoint[1].y-aPoint[8].y, aPoint_z[1]-aPoint_z[8]);
			
			double scalarYX = sqrt(pow(yx.x,2)+pow(yx.y,2)+pow(yx.z,2)); // 純量 = 分量平方相加開根號 
			double scalarYZ = sqrt(pow(yz.x,2)+pow(yz.y,2)+pow(yz.z,2));
			
			double dotProduct = ((yx.x*yz.x)+(yx.y*yz.y)+(yx.z*yz.z)); // 內積 = 分量相乘後相加 

			if(_isnan(dotProduct/(scalarYX*scalarYZ))){
			    char str[30];
				//sprintf(str, "%s", action[i]);
				sprintf(str, "%.5f", dotProduct);
				cv::putText(cImageBGR, str, cv::Point(20,80), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(110,210,100), 2, CV_AA);
				sprintf(str, "%.5f", scalarYX*scalarYZ);
				cv::putText(cImageBGR, str, cv::Point(40,80), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(110,210,100), 2, CV_AA);
			}

			// cosA = (AB*AC)/(|AB|*|AC|)
			leftShoulder = acos(dotProduct/(scalarYX*scalarYZ))*RADIAN;
			out << leftShoulder << " ";
			
			char str[30];
			sprintf(str, "%.4f",leftShoulder);
			cv::putText(cImageBGR, str, cv::Point(y.x-75,y.y+15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(110,210,100), 2, CV_AA);
	  }
  	  {		// 3:右肩 5:右肘 7:右腕 
            CvPoint3D32f x = cvPoint3D32f(aPoint[3].x,aPoint[3].y,aPoint_z[3]);      
			CvPoint3D32f y = cvPoint3D32f(aPoint[5].x,aPoint[5].y,aPoint_z[5]);
			CvPoint3D32f z = cvPoint3D32f(aPoint[7].x,aPoint[7].y,aPoint_z[7]);

			CvPoint3D32f yx = cvPoint3D32f(aPoint[3].x-aPoint[5].x,aPoint[3].y-aPoint[5].y,aPoint_z[3]-aPoint_z[5]);
			CvPoint3D32f yz = cvPoint3D32f(aPoint[7].x-aPoint[5].x,aPoint[7].y-aPoint[5].y,aPoint_z[7]-aPoint_z[5]);
			
			double scalarYX = sqrt(pow(yx.x,2)+pow(yx.y,2)+pow(yx.z,2)); // 純量 = 分量平方相加開根號 
			double scalarYZ = sqrt(pow(yz.x,2)+pow(yz.y,2)+pow(yz.z,2));
			
			double dotProduct = ((yx.x*yz.x)+(yx.y*yz.y)+(yx.z*yz.z)); // 內積 = 分量相乘後相加 
			
			double cosRightElbow = dotProduct/(abs(scalarYX)*abs(scalarYZ));
			// cosA = (AB*AC)/(|AB|*|AC|)
			rightElbow = acos(dotProduct/(abs(scalarYX)*abs(scalarYZ))) * RADIAN;
			
			// 取手肘與對稱點連線 若y分量為負,則角度為360-x
			CvPoint projection = cvPoint(yx.x*(scalarYZ*cosRightElbow)/scalarYX, 
										 yx.y*(scalarYZ*cosRightElbow)/scalarYX);
			CvPoint reflectWrist = cvPoint(y.x+projection.x, y.y+projection.y);
			if(yz.y < 0){ //手腕朝上
				if(reflectWrist.y < z.y){ //投影點高於手腕
					reflectWrist.y = z.y+1; //投影點不超過手腕
				}
			}
			if(yz.y > 0){ //手腕朝下
				if(reflectWrist.y > z.y){ //投影點低於手腕
					reflectWrist.y = z.y-1; //投影點不低於手腕
				}
			}

			CvPoint reflectionPoint = cvPoint(reflectWrist.x,reflectWrist.y);
			cv::circle( cImageBGR, reflectionPoint, 3, cv::Scalar( 0, 255, 255 ), 2 );
			
			if( (reflectWrist.y-z.y) < 0){ //若手肘至對稱點的連線y分量小於零,即手腕位於下半部
				rightElbow = 360 - rightElbow;
			}			
			
			out << rightElbow << " ";

			char str[30];
			sprintf(str, "%.4f",rightElbow);
			//cv::putText(cImageBGR, cv::Point(yPos.x+15,yPos.z+15),str, CV_FONT_HERSHEY_PLAIN, CV_RGB(0,0,250));
			cv::putText(cImageBGR, str, cv::Point(y.x+15,y.y+15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(110,210,100), 2, CV_AA);
	  }
	  {     // 3:右肩 5:右肘 8:身體 
            CvPoint3D32f x = cvPoint3D32f(aPoint[5].x,aPoint[5].y,aPoint_z[5]);      
			CvPoint3D32f y = cvPoint3D32f(aPoint[3].x,aPoint[3].y,aPoint_z[3]);
			//CvPoint3D32f z = cvPoint3D32f(aPoint[8].x,aPoint[8].y,aPoint_z[8]);

			CvPoint3D32f yx = cvPoint3D32f(aPoint[5].x-aPoint[3].x,aPoint[5].y-aPoint[3].y,aPoint_z[5]-aPoint_z[3]);
			CvPoint3D32f yz = cvPoint3D32f(aPoint[1].x-aPoint[8].x,aPoint[1].y-aPoint[8].y, aPoint_z[1]-aPoint_z[8]);

			double scalarYX = sqrt(pow(yx.x,2)+pow(yx.y,2)+pow(yx.z,2)); // 純量 = 分量平方相加開根號 
			double scalarYZ = sqrt(pow(yz.x,2)+pow(yz.y,2)+pow(yz.z,2));
			
			double dotProduct = ((yx.x*yz.x)+(yx.y*yz.y)+(yx.z*yz.z)); // 內積 = 分量相乘後相加 

			// cosA = (AB*AC)/(|AB|*|AC|)
			rightShoulder = acos(dotProduct/(abs(scalarYX)*abs(scalarYZ)))*RADIAN;
			out << rightShoulder << " ";
			
			char str[30];
			sprintf(str, "%.4f",rightShoulder);
			cv::putText(cImageBGR, str, cv::Point(y.x+15,y.y+15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(110,210,100), 2, CV_AA);
	  }
	  
		double leftLeg = 0;
		double rightLeg = 0;
		
	  {		// 9:左臀 11:左膝
			/*
		    CvPoint3D32f x = cvPoint3D32f(aPoint[11].x,aPoint[11].y,aPoint_z[11]);
		    CvPoint3D32f y = cvPoint3D32f(aPoint[9].x,aPoint[9].y,aPoint_z[9]);
			
			CvPoint3D32f yx = cvPoint3D32f(x.x-y.x, x.y-y.y, x.z-y.z);
			CvPoint3D32f vertical = cvPoint3D32f(0,1,0); // 向下垂直向量
			
			double scalarYX = sqrt(pow(yx.x,2)+pow(yx.y,2)+pow(yx.z,2)); // 純量 = 分量平方相加開根號 
			double scalarVertical = sqrt(pow(vertical.x,2)+pow(vertical.y,2)+pow(vertical.z,2));
			
			double dotProduct = ((yx.x*vertical.x)+(yx.y*vertical.y)+(yx.z*vertical.z)); // 內積 = 分量相乘後相加 
			*/
			cv::Point2f x = cv::Point2f(aPoint[11].x,aPoint[11].y);
		    cv::Point2f y = cv::Point2f(aPoint[9].x,aPoint[9].y);
			
			cv::Point2f yx = cv::Point2f(x.x-y.x, x.y-y.y);
			cv::Point2f vertical = cv::Point2f(0,1); // 向下垂直向量
			
			double scalarYX = sqrt(pow(yx.x,2)+pow(yx.y,2)); // 純量 = 分量平方相加開根號 
			double scalarVertical = sqrt(pow(vertical.x,2)+pow(vertical.y,2));
			
			double dotProduct = ((yx.x*vertical.x)+(yx.y*vertical.y)); // 內積 = 分量相乘後相加 
			// cosA = (AB*AC)/(|AB|*|AC|)
			leftLeg = acos(dotProduct/(abs(scalarYX)*abs(scalarVertical)))*RADIAN;	
			
			char str[30];
			sprintf(str, "%.4f",leftLeg);
			cv::putText(cImageBGR, str, cv::Point(y.x-75,y.y+15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(110,210,100), 2, CV_AA);
	  }

	  {		// 10:右臀 12:右膝
		    cv::Point2f x = cv::Point2f(aPoint[12].x,aPoint[12].y);
		    cv::Point2f y = cv::Point2f(aPoint[10].x,aPoint[10].y);
			
			cv::Point2f yx = cv::Point2f(x.x-y.x, x.y-y.y);
			cv::Point2f vertical = cv::Point2f(0,1); // 向下垂直向量
			
			double scalarYX = sqrt(pow(yx.x,2)+pow(yx.y,2)); // 純量 = 分量平方相加開根號 
			double scalarVertical = sqrt(pow(vertical.x,2)+pow(vertical.y,2));
			
			double dotProduct = ((yx.x*vertical.x)+(yx.y*vertical.y)); // 內積 = 分量相乘後相加 

			// cosA = (AB*AC)/(|AB|*|AC|)
			rightLeg = acos(dotProduct/(abs(scalarYX)*abs(scalarVertical)))*RADIAN;	
			char str[30];
			sprintf(str, "%.4f",rightLeg);
			cv::putText(cImageBGR, str, cv::Point(y.x+15,y.y+15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(110,210,100), 2, CV_AA);
	  }


	  {		// 以下進行判斷
		    out << "0 0 0 0 0 0 0 0 0" << endl;
			
			OpenNN::Vector<double> input_data(4);
		    OpenNN::Vector<double> output_data(9);

			OpenNN::NeuralNetwork neural_network("data/neural_network.xml");

			input_data[0] = leftElbow;
			input_data[1] = leftShoulder;
			input_data[2] = rightElbow;
			input_data[3] = rightShoulder;

			output_data = neural_network.calculate_outputs(input_data);

		    
			bool flag = true;
			for(int i = 0;i < 9;++i){
				int val = floor(output_data[i]+0.5);
					/*
					char str[30];
					sprintf(str, "%s%i: %d","action",i,val);
					cv::putText(cImageBGR, str, cv::Point(20,20+i*30), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(110,210,100), 2, CV_AA);
					*/
				if(val == 1){
					
					char str[30];
					sprintf(str, "%s%i", "action",i);
					cv::putText(cImageBGR, str, cv::Point(20,20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(110,210,100), 2, CV_AA);
					flag = false;
					break;
					
				}
			}
			if(flag){
				
				char str[30];
				sprintf(str, "%s","NoN");
				cv::putText(cImageBGR, str, cv::Point(20,20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(110,210,100), 2, CV_AA);
				
			}
	  }
 
      // p4e. draw line
      cv::line( cImageBGR, aPoint[ 0], aPoint[ 1], cv::Scalar( 255, 0, 0 ), 3 );
      cv::line( cImageBGR, aPoint[ 1], aPoint[ 2], cv::Scalar( 255, 0, 0 ), 3 );
      cv::line( cImageBGR, aPoint[ 1], aPoint[ 3], cv::Scalar( 255, 0, 0 ), 3 );
      cv::line( cImageBGR, aPoint[ 2], aPoint[ 4], cv::Scalar( 255, 0, 0 ), 3 );
      cv::line( cImageBGR, aPoint[ 3], aPoint[ 5], cv::Scalar( 255, 0, 0 ), 3 );
      cv::line( cImageBGR, aPoint[ 4], aPoint[ 6], cv::Scalar( 255, 0, 0 ), 3 );
      cv::line( cImageBGR, aPoint[ 5], aPoint[ 7], cv::Scalar( 255, 0, 0 ), 3 );
      cv::line( cImageBGR, aPoint[ 1], aPoint[ 8], cv::Scalar( 255, 0, 0 ), 3 );
      cv::line( cImageBGR, aPoint[ 8], aPoint[ 9], cv::Scalar( 255, 0, 0 ), 3 );
      cv::line( cImageBGR, aPoint[ 8], aPoint[10], cv::Scalar( 255, 0, 0 ), 3 );
      cv::line( cImageBGR, aPoint[ 9], aPoint[11], cv::Scalar( 255, 0, 0 ), 3 );
      cv::line( cImageBGR, aPoint[10], aPoint[12], cv::Scalar( 255, 0, 0 ), 3 );
      cv::line( cImageBGR, aPoint[11], aPoint[13], cv::Scalar( 255, 0, 0 ), 3 );
      cv::line( cImageBGR, aPoint[12], aPoint[14], cv::Scalar( 255, 0, 0 ), 3 );
 
      // p4f. draw joint
      for( int  s = 0; s < 15; ++ s )
      {
        if( aJoints[s].getPositionConfidence() > 0.5 )
          cv::circle( cImageBGR, aPoint[s], 3, cv::Scalar( 0, 0, 255 ), 2 );
        else
          cv::circle( cImageBGR, aPoint[s], 3, cv::Scalar( 0, 255, 0 ), 2 );
      }
	  IplImage temp(cImageBGR);
	  cvWriteFrame(video_writer,&temp);
    }
  }
}

// p5. show image
cv::imshow( "User Image", cImageBGR );
    // p6. check keyboard
    if( cv::waitKey( 1 ) == 'q' )
      break;
  }
  
  // p7. stop
  mUserTracker.destroy();
  mColorStream.destroy();
  mDepthStream.destroy();
  mDevice.close();
  NiTE::shutdown();
  OpenNI::shutdown();

  cvReleaseVideoWriter(&video_writer);
  
  return 0;
}
