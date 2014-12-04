#ifndef __Simball_h_
#define __Simball_h_

#include <hidapi.h>
#define SIMBALL_VENDOR_ID 7330
#define SIMBALL_PRODUCT_ID 1
#define MAX_SIMBALL 3

class SimBall
{
public:
	 SimBall();
	~SimBall();

	double AYaw;
	double APitch;
	double AInsertion;
	double ARotation;
	double AGrip;
	unsigned short APedals;
	
	hid_device *handle[4];
	struct hid_device_info *device;

	int ConnectToSimBall(int); 
	int DisconnectFromSimBall(int);
	

	int DevIdentificTech(unsigned short IDProd,unsigned short IDVend);
	void ReadSimBallState(hid_device *nr);
	int ReadSimBallRawTech(int nr,int disp,int delay);

private:
	struct hid_device_info *current_device;
	unsigned char buf[80];
	double Yaw, YawMax, YawMin;
	double Pitch, PitchMax,PitchMin;
	double Insertion,  InsertionMax,  InsertionMin;
	double Rotation, RotationMax, 	RotationMin;
	unsigned Quarter;
	double Grip, GripMax, GripMin;
	
protected:
	void ScaleStates(void);
};
#endif // #ifndef __Simball_