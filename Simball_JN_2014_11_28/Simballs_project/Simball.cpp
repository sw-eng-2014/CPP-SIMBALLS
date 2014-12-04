#include "Simball.h"

#include <iostream>
#include <windows.h>

using namespace std;

SimBall::SimBall()
{
	 Yaw=0;
	 Pitch=0;
	 Insertion=0;
	 Grip=0;
	 Rotation=0;	
	 Quarter=0;
	 handle[0]=nullptr,handle[1]=nullptr;
	 handle[2]=nullptr, handle[3]=nullptr;
}

SimBall::~SimBall()
{
}

int SimBall::ConnectToSimBall(int nr)
{
	/*********************************************************************************
	 ****************************  JN 28.11.2014 *************************************
	 *********************************************************************************
    	1. Before calling this subroutine another, named SimballConfig, should be launched.
	       We do not know exactly what SimballConfig is doing, but without it Simball 
		   reading does not work.
		2. Simball's address/identifier is stored in hid_device_info.path. Other records
		   are exactly the same for each Simballs.
		3. hid_device_info.path depends only on the socket to which Simball is connected.
		   The order of pluging the USB socket is not important!!!
		4. hid_enumerate(7330, 1) returns Simballs handles starting from the lowest adress.

		5. Calling with nr=0 connects to all installed Simballs, otherwise connects to first nr devices.
	*/	
	
	int i=1;
	char *path_info;
	unsigned short IDvendor, IDproduct;
	wchar_t *ser_num,*product;


	device = hid_enumerate(SIMBALL_VENDOR_ID, SIMBALL_PRODUCT_ID);
	current_device = device;

	if (current_device != NULL){
		do {     // until you have some device
			IDvendor = current_device->vendor_id;
			IDproduct = current_device->product_id;
			ser_num = current_device->serial_number;
			product = current_device->product_string;
			path_info = current_device->path;
			handle[i]=hid_open_path(current_device->path);
	       
			cout << "Device number: "        << i <<endl;
			cout << "The Vendor ID is: "     << IDvendor <<endl;
			cout << "The Product ID is: "    << IDproduct << endl;
			cout << "Product string is: "    << product << endl;
			cout << "Path:  " 	             << path_info << endl;
			cout << "handle[" <<i << "] is:" << handle[i] << endl;
			cout << "-----------------------------------------------------------------------------" << endl << endl;
			current_device = current_device->next; i++;
			if ((i==nr)||(i==MAX_SIMBALL+1))  return i-1;
		}while (current_device != NULL); // until you have some device
	}
	return i-1;
}

int SimBall::DisconnectFromSimBall(int nr)
	
	/*********************************************************************************
	 ****************************  JN 28.11.2014 *************************************
	 *********************************************************************************
	 Calling with nr=0 disconnects all connected Simballs, 
	 otherwise disconnects first nr devices.
	*/
{
	 int i;
	 for (i=1; i<=MAX_SIMBALL ; i++){
		if (handle[i]!= nullptr) {
			cout << "Device number: "        << i <<" is closed " << endl;
			cout << "handle[" <<i << "] is:" << handle[i] << endl;
			hid_close(handle[i]);
			cout << "-----------------------------------------------------------------------------" << endl << endl;
		}
			if (nr==i)  return i;
	 }
	 return i;	 
	
}

int SimBall::DevIdentificTech(unsigned short IDProd,unsigned short IDVend)
{
	/*********************************************************************************
	 *********** device identification technical program JN 28.11.2014 ***************
	 *********************************************************************************
     Important!!!. 
	 Before calling this subroutine another, named SimballConfig, should be launched.
	 We do not know exactly what SimballConfig is doing, but without it Simball 
	 reading does not work !!!
	  tested variants: (IDProd,IDVend) =(0,0), (1118,673), (7330, 1)
	  device = hid_enumerate(0,0);     //when we write (0,0) we see all devices
	  device = hid_enumerate(1118,673); //numbers corresponding to the xbox joystick 
	  when we write these numbers we only see the xbox device
	  device = hid_enumerate(7330, 1);  //numbers corresponding to the Simball joystick
	*/

    int i=1;
	char *path_info;
	unsigned short IDvendor, IDproduct;
	wchar_t *ser_num,*product;

	device = hid_enumerate(IDProd,IDVend);    
	current_device=device;
	if (current_device != NULL){
		do {     // until you have some device
			IDvendor = current_device->vendor_id;
			IDproduct = current_device->product_id;
			ser_num = current_device->serial_number;
			product = current_device->product_string;
			path_info = current_device->path;

			cout << "Device number: "     << i <<endl;
			cout << "The Vendor ID is: "  << IDvendor <<endl;
			cout << "The Product ID is: " << IDproduct << endl;
			cout << "Product string is: " << product << endl;
			cout << "Path:  " 	          << path_info << endl;
			cout << "-----------------------------------------------------------------------------" << endl << endl;
			current_device = current_device->next; i++;
		}while (current_device != NULL); // until you have some device
	}
	hid_free_enumeration(device); 
	Sleep(20);	//20 ms delay because the rapid call of hid_exit results in error 
	hid_exit();
	return i-1;
}



int SimBall::ReadSimBallRawTech(int nr,int disp,int delay)
	/*********************************************************************************
	 *********** read SimBall raw data technical program JN 28.11.2014 ***************
	 *********************************************************************************
		 Important!!!. 
		 Before calling this subroutine another, named SimballConfig, should be launched.
		 We do not know exactly what SimballConfig is doing, but without it Simball 
		 reading does not work !!!
		 Calling with nr=0 reads all connected Simballs, otherwise reads first nr devices.
		 disp determines which SimBall data should be displayed,
		 delay determines delay between two consecutive displays.
	*/
{
    int r;
	int i,j,k;
	 for (i=1; i<=MAX_SIMBALL ; i++){
		if (handle[i]!= nullptr) {
			for (k = 0; k < sizeof(buf); ++k)	buf[k] = 0;
			r = hid_read(handle[i], buf, sizeof(buf));
		//	cout << "Device number: "        << i <<" is read " << endl;
		//	cout << "handle[" <<i << "] is:  " << handle[i] << endl;
		//	cout << r <<"  characters were read" << endl;
			if (i==disp){
				cout << disp <<"  "; for (j = 0; j < r; j++)	printf("%.2x", buf[j]);  cout << endl;
				Sleep(delay);
			}
		//	cout << endl <<"-----------------------------------------------------------------------------" << endl << endl;
		}
		if (nr==i)  return i;
	 }
	 return i;	
}
void SimBall::ReadSimBallState(hid_device *nr)
	/*********************************************************************************
	 ** read SimBall data into Simball (left, right, camera)structure JN 28.11.2014 **
	 *********************************************************************************
		 Important!!!. 
		 Before calling this subroutine another, named SimballConfig, should be launched.
		 We do not know exactly what SimballConfig is doing, but without it Simball 
		 reading does not work !!!
	*/
{  
	 int r,k;
	 if (nr!= nullptr) {
		for ( k = 0; k < sizeof(buf); ++k)	buf[k] = 0;
		r = hid_read(nr, buf, sizeof(buf));
		Yaw = ((buf[20]<<24) | (buf[19]<<16) | (buf[18]<<8) | buf[17] );
	/*  Yaw coordinate -- 890080 to -608573;  1073000000 offset ;   (buf[17] is swinging)
		Yaw=Yaw-1073000000;		Yaw=Yaw-890080;
		Yaw=Yaw*84/(890080+608573);		Yaw=-Yaw-42;
		if (YawMax<Yaw) YawMax=Yaw;	if (YawMin>Yaw) YawMin=Yaw;
		cout << nr <<"   " << long int (Yaw) << "   " << YawMax << "   " << YawMin << endl;
	*/ 
	//	ScaleStates();	cout << nr <<"   " << long int (AYaw) << endl;

		Pitch = ((buf[12]<<24) | (buf[11]<<16) | (buf[10]<<8) | (buf[9]));
	//  ScaleStates();	cout << nr <<"   " << long int (APitch) << endl;

		Insertion = ((buf[36]<<8) | (buf[35]));
//		ScaleStates();	cout << nr <<"   " << long int (AInsertion) << endl;

		Rotation = ((buf[27]<<8) | (buf[26])); Quarter = buf[28]>>6;
//		ScaleStates();	cout << nr <<"   " << long int (ARotation) << endl;

		Grip = ((buf[32]<<16) | (buf[31]<<8) | (buf[30]));
		//	ScaleStates();	cout << nr <<"   " << long int (AGrip) << endl;
		//  trigger --from 64 to 654-- growing=closing; decreasing=opening; (buf[30] is swinging)

		APedals = buf[45]; //plugged right: yellow=8 and blue=4; plugged left: yellow=2 and blue=1; both=addition;
		ScaleStates();	
		//cout << nr <<"   " << (Pedals) << endl;

	 }
}


void SimBall::ScaleStates()
	/*********************************************************************************
	 ******************************** JN 28.11.2014 **********************************
	 *********************************************************************************
	 */
{
//************ Yaw axis scaling ********************
  if (Yaw >= 1073292522)
			{
				Yaw -= 1073292522;
				AYaw = -((2.18276880429734E-23) * pow(Yaw,4) + (1.42358667320443E-16) * pow(Yaw,3) + (-9.55806361289564E-11) * pow(Yaw,2) + (7.07838325670724E-5) * Yaw + (-0.143298699532591));			
			}
			else
			{
				if (Yaw <= 1073292000)
				{
					Yaw = 1073292000 - Yaw;
					AYaw = (3.67260705630406E-28) * pow(Yaw,5) + (-8.12658228340524E-22) * pow(Yaw,4) + (5.92841287177993E-16) * pow(Yaw,3) + (-1.73544145016829E-10) * pow(Yaw,2) + (7.34665593302806E-5) * Yaw + (-0.0546009584438984);				
				}
				else
					AYaw = 0;
			}

//************ Pitch axis scaling ********************
	if (Pitch<= 1073290755)
			{
				Pitch = 1073290755 - Pitch;
				APitch = -((3.9553794519495E-28) * pow(Pitch,5) + (-8.88661269210359E-22) * pow(Pitch,4) + (6.63288583225089E-16) * pow(Pitch,3) + (-2.00970171624501E-10) * pow(Pitch,2) + (7.77935679793288E-5) * Pitch + (-0.272107191930087));
			}
			else
			{
				if (Pitch >= 1073294500)
				{
					Pitch = Pitch - 1073294500;
					APitch= (-1.52427292366888E-27) * pow(Pitch,5) + (2.33730224247779E-21) * pow(Pitch,4) + (-1.07776744440637E-15) * pow(Pitch,3) + (1.63637462451549E-10) * pow(Pitch,2) + (5.18188991820584E-5) * Pitch + (0.187435838013565);
				}
				else 
					APitch = 0;
		}

	//************ Insertion axis scaling ********************
	if (Insertion >= 15240)
		AInsertion = 0;
	else
		AInsertion = (-0.0155653803358956) * Insertion + (250.885441917681);

	//************ Trigger  scaling******************
	AGrip = (3.81308560477002E-6) * Grip + (0.000518779280238595);
	AGrip=int(AGrip*1000)/1000.0;

	//************ Rotation  scaling *****************
			switch(Quarter)
			{
			case 2:	
				//I quarter
				if (Rotation < 33500)	ARotation = 0;
				else{
					ARotation = (2.30529210758624E-21) * pow(Rotation,5)+(-2.34941773211879E-16) * pow(Rotation,4)+(-1.40670845230478E-12) * pow(Rotation,3)+(9.76646518060124E-7) * pow(Rotation,2)+(-0.0403246709041148) * Rotation+(507.407005379696);
					ARotation = (int)ARotation;
					if (ARotation > 52 && ARotation < 113) ARotation++;
				} 
				break;
			case 3:			//II quarter
				ARotation = (0.0280318878615769) * Rotation + (114.76277375846);
				ARotation = (int)ARotation;
				break;
			case 1:			//III quarter
				ARotation = (0.0278916122722987) * Rotation + (114.788148386798);
				ARotation = 0 - (int)ARotation;
				break;
			case 0:			//IV quarter
				if (Rotation < 37940) ARotation = 0;
				else{
					ARotation = (4.10766995092384E-16) * pow(Rotation,4)+(-7.27071551863639E-11) * pow(Rotation,3)+(4.85701588046939E-6) * pow(Rotation,2)+(-0.144433863482053) * Rotation+(1609.27611803859);
					ARotation =  - (int)ARotation;
					if (ARotation >70 && ARotation<103) ARotation-=2;
					if (ARotation> 114)	ARotation = 114;
				}
				break;
			default:
				ARotation = 0;
				break;
			}			
}
