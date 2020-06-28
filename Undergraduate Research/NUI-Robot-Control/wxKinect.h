#ifndef __WX_KINECT__
#define __WX_KINECT__

#if _MSC_VER < 1600
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int8 uint8_t;
#else
#include <stdint.h>
#endif

#include <Winsock2.h>
#include <Ws2bth.h>
#include <bthsdpdef.h>
#include <BluetoothAPIs.h>
#include <windows.h>

#include <wx/wx.h>

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include <OpenNI.h>
#include <NiTE.h>

#include <opennn.h>

#include "serialport.h"
#include "bluetoothconnectargsdlg.h"
#include "serialportconnectargsdlg.h"

typedef ULONGLONG bt_addr, *pbt_addr, BT_ADDR, *PBT_ADDR;

const int SCREEN_WIDTH = 640;
const int SCREEN_HIGHT = 480;

const int SERVO_CORRECTION_RANGE = 35;

const int FACE_WIDTH = 100;
const int FACE_HEIGHT = 120;

const int SERVO_MAX = 1;

const double CM = 10.0f;
const double USER_STEP_RANGE = 35.0f; //cm

const double MS = 1000.0f;
const double RESET_TIME = 1000.0f;
const double KICK_DELAY_TIME = 5000.0;

const double RADIAN = 57.32f;

int run_time_sec = 0;

enum{
	ID_CONNECT_BLUETOOTH = 100,
	ID_CONNECT_SERIAL_PORT,
	ID_START_VIDEO,
	ID_STOP_VIDEO,
	ID_SET_SERVO_90
};

enum{
	DEBUG_INFORMATION = 0
};

enum{
	STATE_NONE = 1000,
	STATE_STOP,
	STATE_FRONT,
	STATE_BACK,
	STATE_TURN_RIGHT,
	STATE_TURN_LEFT,
	STATE_FRONT_UP,
	STATE_BACK_UP,
	STATE_LEFT_FOOT_KICK,
	STATE_RIGHT_FOOT_KICK,
	STATE_HAND_UP,
	STATE_HAND_LEVEL
};

namespace KikcIntegral{
	namespace LeftFoot{
		int n = 0;
		int n_1 = 0;
		int diff = 0;
		int elapse_up = 0;
		int elapse_down = 0;
		int integral_plus = 0;
		int integral_minus = 0;
		int count = 0;
	};
  
	namespace RightFoot{
		int n = 0;
		int n_1 = 0;
		int diff = 0;
		int elapse_up = 0;
		int elapse_down = 0;
		int integral_plus = 0;
		int integral_minus = 0;
		int count = 0;
	};
}

class App:public wxApp
{
public:
	bool OnInit();
};

class Frame:public wxFrame
{
public:
	Frame(const wxString&);
	~Frame();

	void CreateUI();
	void InitKinect();

	void Display();

	void OnConnectBluetooth(wxCommandEvent&);
	void OnConnectSerialPort(wxCommandEvent&);
	void OnStartVideo(wxCommandEvent&);
	void OnStopVideo(wxCommandEvent&);
	void OnSetServo90(wxCommandEvent&);
	void OnExit(wxCommandEvent&);

	void FindFace(cv::Mat&);

	void Send(const char*);
	void Send(const char*,unsigned char,unsigned char);
private:
	friend class Thread;
	Thread *thread;

	openni::Device device;
	nite::UserTracker user_tracker;
	openni::VideoMode color_mode;
	openni::VideoStream color_stream;
	openni::VideoFrameRef color_frame;
	nite::UserTrackerFrameRef user_tracker_frame;
	int max_depth;

	//cv::CascadeClassifier face_cascade;
	//cv::CascadeClassifier eyes_cascade;

	int now_FPS,last_FPS;
	int select_mode;
	double original;
	int now_state,last_state;
	long long now_time,last_time,kick_time;
	wxMenu *view;

	CvFont font;
	CvVideoWriter *video_writer;
	cv::CascadeClassifier face_cascade;
	cv::CascadeClassifier eyes_cascade;
	
	wxPanel *screen;

	WSADATA wsadata;
	SOCKET socket;
	SOCKADDR_BTH sockaddr_bth;

	OpenNN::NeuralNetwork neural_network;

	SerialPort serial;
	unsigned char angles[SERVO_MAX];
	float temp_angle;

	DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(Frame,wxFrame)
	EVT_MENU(ID_CONNECT_BLUETOOTH,Frame::OnConnectBluetooth)
	EVT_MENU(ID_CONNECT_SERIAL_PORT,Frame::OnConnectSerialPort)
	EVT_MENU(ID_START_VIDEO,Frame::OnStartVideo)
	EVT_MENU(ID_STOP_VIDEO,Frame::OnStopVideo)
	EVT_MENU(ID_SET_SERVO_90,Frame::OnSetServo90)
	EVT_MENU(wxID_EXIT,Frame::OnExit)
END_EVENT_TABLE()

class Thread:public wxThread
{
public:
	Thread(Frame*);

	void* Entry();
private:
	Frame *frame;
};

#endif