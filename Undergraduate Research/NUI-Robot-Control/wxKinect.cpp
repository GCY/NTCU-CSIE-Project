#include "wxKinect.h"

DECLARE_APP(App)
IMPLEMENT_APP(App)

bool App::OnInit()
{
	Frame *frame = new Frame(wxT("Kinect Robot Controller"));

	frame->Show(true);

	return true;
}

Frame::Frame(const wxString &title):wxFrame(NULL,wxID_ANY,title,wxDefaultPosition,wxSize(800,600),wxMINIMIZE_BOX | wxCLOSE_BOX | wxCAPTION | wxSYSTEM_MENU)
{
	InitKinect();
	CreateUI();

	neural_network.load("./data/neural_network.xml");

	cvInitFont(&font,CV_FONT_HERSHEY_COMPLEX,0.5,0.5,1,2,8);

	if(!face_cascade.load(std::string("./haarcascades/haarcascade_frontalface_alt.xml"))){
		wxMessageBox(wxT("err"));
	}
	if(!eyes_cascade.load(std::string("./haarcascades/haarcascade_eye_tree_eyeglasses.xml"))){
		wxMessageBox(wxT("err"));
	}

	temp_angle = 90.0f;

	now_time = last_time = (double)cvGetTickCount() / ((double)cvGetTickFrequency() * MS);
	kick_time = now_time - KICK_DELAY_TIME;
	now_FPS = last_FPS = 0;

	video_writer = NULL;
	original = 0.0f;
	now_state = last_state = STATE_NONE;
	socket = INVALID_SOCKET;

	thread = new Thread(this);
	thread->Create();
	thread->Run();
}

Frame::~Frame()
{
	thread->Delete();

	if(video_writer){
		cvReleaseVideoWriter(&video_writer);
		video_writer = NULL;
	}

	color_stream.destroy();
	device.close();
	openni::OpenNI::shutdown();
	nite::NiTE::shutdown();
}

void Frame::CreateUI()
{
	wxMenu *file = new wxMenu;
	file->Append(wxID_EXIT,wxT("E&xit\tAlt-q"),wxT("exit"));

	view = new wxMenu;
	view->AppendCheckItem(DEBUG_INFORMATION,wxT("Debug"));
	//view->Check(DEBUG_INFORMATION,true);

	wxMenu *camera = new wxMenu;
	camera->Append(ID_START_VIDEO,wxT("Video Start\tCtrl-v"),wxT("Video Start"));
	camera->Append(ID_STOP_VIDEO,wxT("Video Stop\tCtrl-b"),wxT("Video Stop"));

	wxMenu *connect = new wxMenu;
	connect->Append(ID_CONNECT_BLUETOOTH,wxT("C&onnect Bluetooth\tAlt-c"),wxT("Connect Bluetooth"));
	connect->Append(ID_CONNECT_SERIAL_PORT,wxT("C&onnect Serialport\tAlt-c"),wxT("Connect Serial Port"));

	wxMenu *tools = new wxMenu;
	tools->AppendSubMenu(camera,wxT("Video"),wxT("Video"));
	tools->AppendSubMenu(connect,wxT("Connect"),wxT("Connect"));
	tools->Append(ID_SET_SERVO_90,wxT("90\tCtrl-s"),wxT("Set Servo"));
 
	wxMenuBar *bar = new wxMenuBar;
	bar->Append(file,wxT("file"));
	bar->Append(view,wxT("view"));
	bar->Append(tools,wxT("tools"));
	SetMenuBar(bar);

	wxBoxSizer *top = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(top);

	wxBoxSizer *screen_box = new wxBoxSizer(wxHORIZONTAL);
	top->Add(screen_box,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);

	screen = new wxPanel(this,wxID_ANY,wxDefaultPosition,wxSize(SCREEN_WIDTH,SCREEN_HIGHT));
	screen_box->Add(screen,0,wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,5);

	CreateStatusBar(2);
	SetStatusText(wxDateTime::Now().Format());
}

void Frame::InitKinect()
{
	openni::OpenNI::initialize();
	nite::NiTE::initialize();

	device.open(openni::ANY_DEVICE);
	device.setImageRegistrationMode(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR);

	user_tracker.create();
	user_tracker.setSkeletonSmoothingFactor(0.1f);

	color_mode.setFps(30);
	color_mode.setResolution(SCREEN_WIDTH,SCREEN_HIGHT);
	color_mode.setPixelFormat(openni::PIXEL_FORMAT_RGB888);

	color_stream.setVideoMode(color_mode);
	color_stream.create(device,openni::SENSOR_COLOR);
	color_stream.start();
}

void Frame::Display()
{
	now_time = (double)cvGetTickCount()  / ((double)cvGetTickFrequency() * MS);
	++now_FPS;

	SetStatusText(wxDateTime::Now().Format());

	color_stream.readFrame(&color_frame);
	user_tracker.readFrame(&user_tracker_frame);

	cv::Mat RGBMat(color_frame.getHeight(),color_frame.getWidth(),CV_8UC3,(void*)color_frame.getData());
	FindFace(RGBMat);
	IplImage color_image(RGBMat);
	cvCvtColor(&color_image,&color_image,CV_RGB2BGR);

	IplImage *select_image = cvCreateImage(cvGetSize(&color_image),IPL_DEPTH_8U,3);
	cvCopyImage(&color_image,select_image);


	const nite::Array<nite::UserData> &users = user_tracker_frame.getUsers();
	for(int i = 0;i < users.getSize();++i){
		const nite::UserData &user = users[i];

		if(user.isNew()){
			user_tracker.startSkeletonTracking(user.getId());
			user_tracker.startPoseDetection(user.getId(),nite::POSE_PSI);
		}
		else if(user.isLost()){
		}

		if(user.isVisible()){
			const nite::Skeleton &skeleton = user.getSkeleton();
			nite::SkeletonJoint joints[15];

			if(skeleton.getState() == nite::SKELETON_TRACKED){
				joints[0] = skeleton.getJoint(nite::JOINT_HEAD);
				joints[1] = skeleton.getJoint(nite::JOINT_NECK);
				joints[2] = skeleton.getJoint(nite::JOINT_LEFT_SHOULDER);
				joints[3] = skeleton.getJoint(nite::JOINT_RIGHT_SHOULDER);
				joints[4] = skeleton.getJoint(nite::JOINT_LEFT_ELBOW);
				joints[5] = skeleton.getJoint(nite::JOINT_RIGHT_ELBOW);
				joints[6] = skeleton.getJoint(nite::JOINT_LEFT_HAND);
				joints[7] = skeleton.getJoint(nite::JOINT_RIGHT_HAND);
				joints[8] = skeleton.getJoint(nite::JOINT_TORSO);
				joints[9] = skeleton.getJoint(nite::JOINT_LEFT_HIP);
				joints[10] = skeleton.getJoint(nite::JOINT_RIGHT_HIP);
				joints[11] = skeleton.getJoint(nite::JOINT_LEFT_KNEE);
				joints[12] = skeleton.getJoint(nite::JOINT_RIGHT_KNEE);
				joints[13] = skeleton.getJoint(nite::JOINT_LEFT_FOOT);
				joints[14] = skeleton.getJoint(nite::JOINT_RIGHT_FOOT);
			}
			if(view->IsChecked(DEBUG_INFORMATION)){
				{
					char str[255] = {0};
					sprintf(str,"%.3fcm",original / CM); 
					cvPutText(select_image,str,cvPoint(20,20),&font,CV_RGB(220,20,10));
				}
				{
					char str[255] = {0};
					sprintf(str, "%d left kick count",KikcIntegral::LeftFoot::count);
					cvPutText(select_image,str,cvPoint(20,50),&font,CV_RGB(250,0,0));
					sprintf(str, "%d right kick count",KikcIntegral::RightFoot::count);
					cvPutText(select_image,str,cvPoint(190,50),&font,CV_RGB(250,0,0));
				}
			}
			double aPoint_z[15];
			for( int  s = 0; s < 15; ++ s )  {
				aPoint_z[s] = joints[s].getPosition().z;
			}
			CvPoint2D32f point[15];
			for(int i = 0;i < 15;++i){
				const nite::Point3f &pos = joints[i].getPosition();
				user_tracker.convertJointCoordinatesToDepth(pos.x,pos.y,pos.z,&(point[i].x),&(point[i].y));
			}

			double leftElbow = 0; 
			double leftShoulder = 0; 
			double rightElbow = 0; 
			double rightShoulder = 0;

			{     // 2:左肩  4:左肘 6:左腕 
				CvPoint3D32f shoulder = cvPoint3D32f(point[2].x,point[2].y,aPoint_z[2]);
				CvPoint3D32f elbow = cvPoint3D32f(point[4].x,point[4].y,aPoint_z[4]);
				CvPoint3D32f wrist = cvPoint3D32f(point[6].x,point[6].y,aPoint_z[6]);

				CvPoint3D32f elbowShoulder = cvPoint3D32f((point[2].x-point[4].x), (point[2].y-point[4].y), (aPoint_z[2]-aPoint_z[4]));
				CvPoint3D32f elbowWrist = cvPoint3D32f((point[6].x-point[4].x), (point[6].y-point[4].y), (aPoint_z[6]-aPoint_z[4]));
			
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
				cvCircle(select_image,reflectionPoint,3,CV_RGB(0,255,255),2);
			
				if( (reflectWrist.y-wrist.y) < 0){ //若手肘至對稱點的連線y分量小於零,即手腕位於下半部
					leftElbow = 360 - leftElbow;
				}
				if(view->IsChecked(DEBUG_INFORMATION)){
					char str[255] = {0};
					sprintf(str, "%.4f",leftElbow);
					cvPutText(select_image, str, cvPoint(elbow.x-75,elbow.y+15), &font, CV_RGB(110,210,100));
				}
			}
			{     // 2:左肩  4:左肘   1:脖子 8:身體 
		    
				CvPoint3D32f x = cvPoint3D32f(point[4].x,point[4].y,aPoint_z[4]);
				CvPoint3D32f y = cvPoint3D32f(point[2].x,point[2].y,aPoint_z[2]);
				CvPoint3D32f z = cvPoint3D32f((point[2].x+point[3].x)/2,(point[2].y+point[3].y)/2,(aPoint_z[2]+aPoint_z[3])/2);
			
				CvPoint3D32f yx = cvPoint3D32f(point[4].x-point[2].x,point[4].y-point[2].y, aPoint_z[4]-aPoint_z[2]);
				CvPoint3D32f yz = cvPoint3D32f(point[1].x-point[8].x, point[1].y-point[8].y, aPoint_z[1]-aPoint_z[8]);
			
				double scalarYX = sqrt(pow(yx.x,2)+pow(yx.y,2)+pow(yx.z,2)); // 純量 = 分量平方相加開根號 
				double scalarYZ = sqrt(pow(yz.x,2)+pow(yz.y,2)+pow(yz.z,2));
			
				double dotProduct = ((yx.x*yz.x)+(yx.y*yz.y)+(yx.z*yz.z)); // 內積 = 分量相乘後相加 

				// cosA = (AB*AC)/(|AB|*|AC|)
				leftShoulder = acos(dotProduct/(scalarYX*scalarYZ))*RADIAN;
				if(view->IsChecked(DEBUG_INFORMATION)){
					char str[255] = {0};
					sprintf(str, "%.4f",leftShoulder);
					cvPutText(select_image, str, cvPoint(y.x-75,y.y+15), &font, CV_RGB(110,210,100));
				}
			}
			{		// 3:右肩 5:右肘 7:右腕 
				CvPoint3D32f x = cvPoint3D32f(point[3].x,point[3].y,aPoint_z[3]);      
				CvPoint3D32f y = cvPoint3D32f(point[5].x,point[5].y,aPoint_z[5]);
				CvPoint3D32f z = cvPoint3D32f(point[7].x,point[7].y,aPoint_z[7]);

				CvPoint3D32f yx = cvPoint3D32f(point[3].x-point[5].x,point[3].y-point[5].y,aPoint_z[3]-aPoint_z[5]);
				CvPoint3D32f yz = cvPoint3D32f(point[7].x-point[5].x,point[7].y-point[5].y,aPoint_z[7]-aPoint_z[5]);
			
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
				cvCircle(select_image,reflectionPoint,3,CV_RGB(0,255,255),2);
			
				if( (reflectWrist.y-z.y) < 0){ //若手肘至對稱點的連線y分量小於零,即手腕位於下半部
					rightElbow = 360 - rightElbow;
				}			
			
				//out << rightElbow << " ";
				if(view->IsChecked(DEBUG_INFORMATION)){
					char str[255] = {0};
					sprintf(str, "%.4f",rightElbow);
					//cv::putText(cImageBGR, cv::Point(yPos.x+15,yPos.z+15),str, CV_FONT_HERSHEY_PLAIN, CV_RGB(0,0,250));
					//cv::putText(cImageBGR, str, cv::Point(y.x+15,y.y+15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(110,210,100), 2, CV_AA);
					cvPutText(select_image, str, cvPoint(y.x+15,y.y+15), &font, CV_RGB(110,210,100));
				}
			}
			{     // 3:右肩 5:右肘 8:身體 
				CvPoint3D32f x = cvPoint3D32f(point[5].x,point[5].y,aPoint_z[5]);      
				CvPoint3D32f y = cvPoint3D32f(point[3].x,point[3].y,aPoint_z[3]);

				CvPoint3D32f yx = cvPoint3D32f(point[5].x-point[3].x,point[5].y-point[3].y,aPoint_z[5]-aPoint_z[3]);
				CvPoint3D32f yz = cvPoint3D32f(point[1].x-point[8].x,point[1].y-point[8].y, aPoint_z[1]-aPoint_z[8]);

				double scalarYX = sqrt(pow(yx.x,2)+pow(yx.y,2)+pow(yx.z,2)); // 純量 = 分量平方相加開根號 
				double scalarYZ = sqrt(pow(yz.x,2)+pow(yz.y,2)+pow(yz.z,2));
			
				double dotProduct = ((yx.x*yz.x)+(yx.y*yz.y)+(yx.z*yz.z)); // 內積 = 分量相乘後相加 

				// cosA = (AB*AC)/(|AB|*|AC|)
				rightShoulder = acos(dotProduct/(abs(scalarYX)*abs(scalarYZ)))*RADIAN;
				if(view->IsChecked(DEBUG_INFORMATION)){
					char str[255] = {0};
					sprintf(str, "%.4f",rightShoulder);
					cvPutText(select_image, str, cvPoint(y.x+15,y.y+15), &font, CV_RGB(110,210,100));
				}
			}
			
			OpenNN::Vector<double> input_data(4);
			OpenNN::Vector<double> output_data(9);

			input_data[0] = leftElbow;
			input_data[1] = leftShoulder;
			input_data[2] = rightElbow;
			input_data[3] = rightShoulder;

			output_data = neural_network.calculate_outputs(input_data);

			bool flag = true;
			for(int i = 0;i < 9;++i){
				int val = floor(output_data[i]+0.5);
				if(val == 1){
					if(view->IsChecked(DEBUG_INFORMATION)){		
						char str[255] = {0};
						sprintf(str, "%s%i", "action",i);
						cvPutText(select_image, str, cvPoint(170,20), &font, CV_RGB(110,210,100));
					}
					flag = false;
					break;
				}
			}
			if(flag){
				if(view->IsChecked(DEBUG_INFORMATION)){
					char str[255] = {0};
					sprintf(str, "%s","NoN");
					cvPutText(select_image, str, cvPoint(170,20), &font, CV_RGB(110,210,100));
				}
			}

			/* FSM-State Set */
			if(floor(output_data[7] + 0.5f)){
				original = joints[8].getPosition().z;
			}
			if(((joints[8].getPosition().z - original) / CM) > USER_STEP_RANGE){
				now_state = STATE_BACK;
			}
			else if(((joints[8].getPosition().z - original) / CM) < -USER_STEP_RANGE){
				now_state = STATE_FRONT;
			}
			else{
				if((now_time - kick_time) > KICK_DELAY_TIME){
					if(floor(output_data[0] + 0.5f)){
						now_state = STATE_STOP;
					}
					else if(floor(output_data[1] + 0.5f)){
						now_state = STATE_TURN_LEFT;
					}
					else if(floor(output_data[2] + 0.5f)){
						now_state = STATE_TURN_RIGHT;
					}
					else if(floor(output_data[3] + 0.5f)){
						now_state = STATE_FRONT_UP;
					}
					else if(floor(output_data[4] + 0.5f)){
						now_state = STATE_BACK_UP;
					}
					else if(floor(output_data[5] + 0.5f)){
						now_state = STATE_HAND_LEVEL;
					}
					else if(floor(output_data[6] + 0.5f)){
						now_state = STATE_HAND_UP;
					}
					else if(floor(output_data[8] + 0.5f)){
					}
					else{
						//now_state = STATE_STOP;
					}
				}
				/* Kick Integral Computing */
				{
					KikcIntegral::LeftFoot::n = aPoint_z[13] / CM;
					KikcIntegral::LeftFoot::diff = ( KikcIntegral::LeftFoot::n -  KikcIntegral::LeftFoot::n_1);
					KikcIntegral::LeftFoot::n_1 =  KikcIntegral::LeftFoot::n;
				
					if( KikcIntegral::LeftFoot::diff < 5 &&  KikcIntegral::LeftFoot::diff > -5 &&
						KikcIntegral::LeftFoot::integral_plus > 50 &&  KikcIntegral::LeftFoot::integral_minus < -50){
							if( KikcIntegral::LeftFoot::elapse_up > 50 &&  KikcIntegral::LeftFoot::elapse_up < 800 &&
								KikcIntegral::LeftFoot::elapse_down > 50 &&  KikcIntegral::LeftFoot::elapse_down < 800){
									++KikcIntegral::LeftFoot::count;
									now_state = STATE_LEFT_FOOT_KICK;
									kick_time = (double)cvGetTickCount()  / ((double)cvGetTickFrequency() * MS);
							}
							KikcIntegral::LeftFoot::elapse_up = 0;
							KikcIntegral::LeftFoot::elapse_down = 0;
							KikcIntegral::LeftFoot::integral_plus = 0;
							KikcIntegral::LeftFoot::integral_minus = 0;
					}
					else if( KikcIntegral::LeftFoot::diff > 5 &&  KikcIntegral::LeftFoot::diff < 280){
						KikcIntegral::LeftFoot::integral_plus += KikcIntegral::LeftFoot::diff;
						KikcIntegral::LeftFoot::elapse_up += 40;
					}
					else if( KikcIntegral::LeftFoot::diff < -5 &&  KikcIntegral::LeftFoot::diff > -200){
						KikcIntegral::LeftFoot::integral_minus += KikcIntegral::LeftFoot::diff;
						KikcIntegral::LeftFoot::elapse_down += 40;
					}
				}
				{
					KikcIntegral::RightFoot::n = aPoint_z[14] / CM;
					KikcIntegral::RightFoot::diff = ( KikcIntegral::RightFoot::n -  KikcIntegral::RightFoot::n_1);
					KikcIntegral::RightFoot::n_1 =  KikcIntegral::RightFoot::n;
				
					if( KikcIntegral::RightFoot::diff < 5 &&  KikcIntegral::RightFoot::diff > -5 &&
						KikcIntegral::RightFoot::integral_plus > 50 &&  KikcIntegral::RightFoot::integral_minus < -50){
							if( KikcIntegral::RightFoot::elapse_up > 50 &&  KikcIntegral::RightFoot::elapse_up < 800 &&
								KikcIntegral::RightFoot::elapse_down > 50 &&  KikcIntegral::RightFoot::elapse_down < 800){
									++KikcIntegral::RightFoot::count;
									now_state = STATE_RIGHT_FOOT_KICK;
									kick_time = (double)cvGetTickCount()  / ((double)cvGetTickFrequency() * MS);
							}
							KikcIntegral::RightFoot::elapse_up = 0;
							KikcIntegral::RightFoot::elapse_down = 0;
							KikcIntegral::RightFoot::integral_plus = 0;
							KikcIntegral::RightFoot::integral_minus = 0;
					}
					else if( KikcIntegral::RightFoot::diff > 5 &&  KikcIntegral::RightFoot::diff < 280){
						KikcIntegral::RightFoot::integral_plus += KikcIntegral::RightFoot::diff;
						KikcIntegral::RightFoot::elapse_up += 40;
					}
					else if( KikcIntegral::RightFoot::diff < -5 &&  KikcIntegral::RightFoot::diff > -200){
						KikcIntegral::RightFoot::integral_minus += KikcIntegral::RightFoot::diff;
						KikcIntegral::RightFoot::elapse_down += 40;
					}
				}
			}

			/* Send State Command to Robot */
			if(now_state != last_state && socket != INVALID_SOCKET){
				if(now_state == STATE_BACK){
					Send("BR\xff\x41\x19");
				}
				else if(now_state == STATE_FRONT){
					Send("BR\xff\x41\x32");
				}
				else if(now_state == STATE_STOP){
					Send("BR\xff\x41\xff");
				}
				else if(now_state == STATE_TURN_RIGHT){
					Send("BR\xff\x41\x2d");
				}
				else if(now_state == STATE_TURN_LEFT){
					Send("BR\xff\x41\x37");
				}
				else if(now_state == STATE_FRONT_UP){
					Send("BR\xff\x41\x46");
				}
				else if(now_state == STATE_BACK_UP){
					Send("BR\xff\x41\x41");
				}
				else if(now_state == STATE_LEFT_FOOT_KICK){
					Send("BR\xff\x41\x1e");
				}
				else if(now_state == STATE_RIGHT_FOOT_KICK){
					Send("BR\xff\x41\x14");
				}
				else if(now_state == STATE_HAND_UP){
					Send("BR\xff\x41\x28");
				}
				else if(now_state == STATE_HAND_LEVEL){
					Send("BR\xff\x41\x23");
				}
				last_state = now_state;
			}

			cvLine(select_image,cvPoint(point[0].x,point[0].y),cvPoint(point[1].x,point[1].y),CV_RGB(255,0,0),3);
			cvLine(select_image,cvPoint(point[1].x,point[1].y),cvPoint(point[2].x,point[2].y),CV_RGB(255,0,0),3);
			cvLine(select_image,cvPoint(point[1].x,point[1].y),cvPoint(point[3].x,point[3].y),CV_RGB(255,0,0),3);
			cvLine(select_image,cvPoint(point[2].x,point[2].y),cvPoint(point[4].x,point[4].y),CV_RGB(255,0,0),3);
			cvLine(select_image,cvPoint(point[3].x,point[3].y),cvPoint(point[5].x,point[5].y),CV_RGB(255,0,0),3);
			cvLine(select_image,cvPoint(point[4].x,point[4].y),cvPoint(point[6].x,point[6].y),CV_RGB(255,0,0),3);
			cvLine(select_image,cvPoint(point[5].x,point[5].y),cvPoint(point[7].x,point[7].y),CV_RGB(255,0,0),3);
			cvLine(select_image,cvPoint(point[1].x,point[1].y),cvPoint(point[8].x,point[8].y),CV_RGB(255,0,0),3);
			cvLine(select_image,cvPoint(point[8].x,point[8].y),cvPoint(point[9].x,point[9].y),CV_RGB(255,0,0),3);
			cvLine(select_image,cvPoint(point[8].x,point[8].y),cvPoint(point[10].x,point[10].y),CV_RGB(255,0,0),3);
			cvLine(select_image,cvPoint(point[9].x,point[9].y),cvPoint(point[11].x,point[11].y),CV_RGB(255,0,0),3);
			cvLine(select_image,cvPoint(point[10].x,point[10].y),cvPoint(point[12].x,point[12].y),CV_RGB(255,0,0),3);
			cvLine(select_image,cvPoint(point[11].x,point[11].y),cvPoint(point[13].x,point[13].y),CV_RGB(255,0,0),3);
			cvLine(select_image,cvPoint(point[12].x,point[12].y),cvPoint(point[14].x,point[14].y),CV_RGB(255,0,0),3);

			if(view->IsChecked(DEBUG_INFORMATION)){
				cvLine(select_image,cvPoint((color_frame.getWidth() / 2) - SERVO_CORRECTION_RANGE,(color_frame.getHeight() / 2) - SERVO_CORRECTION_RANGE),
					cvPoint((color_frame.getWidth() / 2) - SERVO_CORRECTION_RANGE,(color_frame.getHeight() / 2) + SERVO_CORRECTION_RANGE),CV_RGB(0,0,255),3);
				cvLine(select_image,cvPoint((color_frame.getWidth() / 2) + SERVO_CORRECTION_RANGE,(color_frame.getHeight() / 2) - SERVO_CORRECTION_RANGE),
					cvPoint((color_frame.getWidth() / 2) + SERVO_CORRECTION_RANGE,(color_frame.getHeight() / 2) + SERVO_CORRECTION_RANGE),CV_RGB(0,0,255),3);
			}
			if(point[8].x < (color_frame.getWidth() / 2) - SERVO_CORRECTION_RANGE){
				temp_angle -= 0.25f;
				if(temp_angle < 1.0f){
					temp_angle = 1.0f;
				}
				angles[0] = temp_angle;
			}
			else if(point[8].x > (color_frame.getWidth() / 2) + SERVO_CORRECTION_RANGE){
				temp_angle += 0.25f;
				if(temp_angle > 179.0f){
					temp_angle = 179.0f;
				}
				angles[0] = temp_angle;
			}

			for(int i = 0;i < 15;++i){
				if(joints[i].getPositionConfidence() > 0.5f){
					cvCircle(select_image,cvPoint(point[i].x,point[i].y),3,CV_RGB(0,255,0),2);
				}
				else{
					cvCircle(select_image,cvPoint(point[i].x,point[i].y),3,CV_RGB(0,0,255),2);
				}
			}
		}
	}

	if(now_time - last_time > RESET_TIME){
		last_FPS = now_FPS;
		now_FPS = 0;
		run_time_sec += 1;
		last_time = (double)cvGetTickCount()  / ((double)cvGetTickFrequency() * MS);
		
		KikcIntegral::LeftFoot::elapse_up = 0;
		KikcIntegral::LeftFoot::elapse_down = 0;
		KikcIntegral::LeftFoot::integral_plus = 0;
		KikcIntegral::LeftFoot::integral_minus = 0;

		KikcIntegral::RightFoot::elapse_up = 0;
		KikcIntegral::RightFoot::elapse_down = 0;
		KikcIntegral::RightFoot::integral_plus = 0;
		KikcIntegral::RightFoot::integral_minus = 0;
	}

	if(view->IsChecked(DEBUG_INFORMATION)){
		char str[255] = {0};
		sprintf(str, "%d/sec",run_time_sec);
		cvPutText(select_image, str, cv::Point(460,20), &font, CV_RGB(255,0,0));
		sprintf(str, "%d FPS",last_FPS / (int)(RESET_TIME / MS));
		cvPutText(select_image, str, cv::Point(540,20), &font, CV_RGB(255,0,0));
	}

	if(video_writer){
		cvWriteFrame(video_writer,select_image);
	}

	wxClientDC dc(screen);
	cvConvertImage(select_image,select_image,CV_CVTIMG_SWAP_RB);
	unsigned char *data;
	cvGetRawData(select_image,&data);
	wxImage *image = new wxImage(select_image->width,select_image->height,data,true);
	wxBitmap *bitmap = new wxBitmap(*image);
	int x,y,width,height;
	dc.GetClippingBox(&x,&y,&width,&height);
	dc.DrawBitmap(*bitmap,x,y);
	delete image;
	delete bitmap;

	cvReleaseImage(&select_image);

	serial.Write(angles,SERVO_MAX);
}

void Frame::OnStartVideo(wxCommandEvent &event)
{
	if(video_writer == NULL){
		wxFileDialog dlg(this,wxT("Save Video"),wxT("."),wxT("*.avi"),wxT("AVI files (*.avi)|*.avi"),wxSAVE | wxOVERWRITE_PROMPT);
		if(dlg.ShowModal() == wxID_OK){
			video_writer = cvCreateVideoWriter(dlg.GetPath().mb_str(),CV_FOURCC('P','I','M','1'),25,cvSize(640,480),1);
		}
	}
	else{
		wxMessageBox(wxT("Starting!"),wxT("Error"));
	}
}

void Frame::OnStopVideo(wxCommandEvent &event)
{
	if(video_writer){
		cvReleaseVideoWriter(&video_writer);
		video_writer = NULL;
	}
}

void Frame::OnConnectBluetooth(wxCommandEvent &event)
{
    BluetoothConnectArgsDialog dlg(this,wxID_ANY,wxT("Connect Bluetooth"),wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE);

    if(dlg.ShowModal() == wxID_OK){
		
		if(socket != INVALID_SOCKET){
			shutdown(socket,SD_SEND);
			WSACleanup ();
			closesocket(socket);
		}

		BT_ADDR aSddr;
		std::istringstream iss(std::string(dlg.GetDeviceAddr(dlg.GetDevicePath()).mb_str()));
		iss >> std::hex >> aSddr;

		WSAStartup(MAKEWORD(2, 2), &wsadata);

		socket = ::socket(AF_BTH,SOCK_STREAM,BTHPROTO_RFCOMM);
		memset (&sockaddr_bth,0,sizeof(sockaddr_bth));
		sockaddr_bth.addressFamily = AF_BTH;
		sockaddr_bth.btAddr = aSddr;
		sockaddr_bth.port = 1;

		if(connect(socket,(SOCKADDR*)&sockaddr_bth,sizeof(sockaddr_bth)) != SOCKET_ERROR){
			Send("BR\xff\x41\xff");
		}
		else{
			wxMessageBox(wxT("can't connect"));
			closesocket(socket);
			WSACleanup();
		}
    }
}

void Frame::OnConnectSerialPort(wxCommandEvent &event)
{
    SerialPortConnectArgsDialog dlg(this,wxID_ANY,wxT("Connect Serialport"),wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE);

    if(dlg.ShowModal() == wxID_OK){
		serial.Open(dlg.GetDevicePath(),dlg.GetBaudRate());

		angles[0] = 90;
		serial.Write(angles,SERVO_MAX);
    }
}

void Frame::OnSetServo90(wxCommandEvent &event)
{
	angles[0] = 90;
}

void Frame::FindFace(cv::Mat &RGBMat)
{
	std::vector<cv::Rect> faces;
	cv::Mat frame_gray;

	cvtColor( RGBMat, frame_gray, CV_BGR2GRAY );
	equalizeHist( frame_gray, frame_gray );
	//-- Detect faces
	face_cascade.detectMultiScale( frame_gray, faces, 1.1f, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(FACE_WIDTH,FACE_HEIGHT) );

	std::vector<int> areas;
	for(size_t i = 0; i < faces.size(); i++ ){
		areas.push_back(faces[i].area());
	}

	int temp = 0;
	int max_area = 0;
	for(size_t i = 0; i < areas.size(); i++ ){
		if(areas[i] > temp){
			temp = areas[i];
			max_area = i;
		}
	}

	if(faces.size() > 0){
		cv::Point center( faces[max_area].x + faces[max_area].width*0.5, faces[max_area].y + faces[max_area].height*0.5 );
		cv::ellipse( RGBMat, center, cv::Size( faces[max_area].width*0.5, faces[max_area].height*0.5), 0, 0, 360, cv::Scalar( 0, 255, 0 ), 2, 8, 0 );
 
		cv::Mat faceROI = frame_gray( faces[max_area] );
		std::vector<cv::Rect> eyes;
 
		//-- In each face, detect eyes
		eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, cv::Size(0.1f,0.1f) );
 
		for(size_t j = 0; j < eyes.size(); j++ ){
			cv::Point center( faces[max_area].x + eyes[j].x + eyes[j].width*0.5, faces[max_area].y + eyes[j].y + eyes[j].height*0.5 );
			int radius = cvRound( (eyes[j].width + eyes[max_area].height)*0.25 );
			cv::circle( RGBMat, center, radius, cv::Scalar( 255, 0, 0 ), 3, 8, 0 );
		}
	}
}

void Frame::Send(const char *command)
{
	if(socket != INVALID_SOCKET){
		for(int i = 0;i < (int)strlen(command);++i){
			send(socket,(const char *)&command[i],1, 0 );
			Sleep(50);
		}
	}
	//Sleep(200);
}

void Frame::Send(const char *command,unsigned char servo,unsigned char angle)
{
	if(socket != INVALID_SOCKET){
		send(socket,(const char *)&servo, (int)1, 0 );
		Sleep(50);
		send(socket,(const char *)&angle, (int)1, 0 );
		Sleep(50);
	}
}

void Frame::OnExit(wxCommandEvent &event)
{
	Close();
}

Thread::Thread(Frame *parent):wxThread(wxTHREAD_DETACHED)
{
	frame = parent;
}

void* Thread::Entry()
{
	while(!TestDestroy()){
		frame->Display();
	}

	return NULL;
}