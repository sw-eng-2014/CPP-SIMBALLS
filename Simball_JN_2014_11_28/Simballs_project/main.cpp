// getting data from symballs
//SimballConfig.exe must be on execution (located on the folder)

#include <windows.h>
#include <iostream> // <...> used when is a library outside the project ; "..." used when we have the headers in the directory or subdirectory
#include "hidapi.h"
#include "Simball.h"


using namespace std;


SimBall simball, simball_Left, simball_Right,simball_camera;
int num_res, i;




int main(){

	num_res=simball.DevIdentificTech(7330,1);
	cout << "Total number of devices: "     << num_res <<endl;
//--------------------------------------------------------------------------
	num_res=simball.ConnectToSimBall(0);
	cout << "Total number of connected devices: "     << num_res <<endl;
//--------------------------------------------------------------------------
//	for ( i = 0;  i < 10000 ; ++i) {
//		num_res=simball.ReadSimBallRawTech(0,1,100);
//	}  
//--------------------------------------------------------------------------
	//showing the information	
	for ( i = 0;  i < 1000000 ; ++i) {
			simball_Left.ReadSimBallState(simball.handle[1]);
			cout << simball.handle[1] << " "<< int (simball_Left.AYaw);
		    cout << " " << int (simball_Left.APitch);
		    cout << " " << int (simball_Left.AInsertion);
			cout << " " << simball_Left.ARotation;
		   	cout << " " << simball_Left.AGrip ;
		   	cout << " " << simball_Left.APedals ;
			cout << "   " ;
	      //----------------------------------------------------
			simball_Right.ReadSimBallState(simball.handle[2]);
		    cout << " " <<simball.handle[2] <<" " << int (simball_Right.AYaw);
		    cout << " " << int (simball_Right.APitch);
		    cout << " " << int (simball_Right.AInsertion);
			cout << " " << simball_Right.ARotation;
		   	cout << " " << simball_Right.AGrip ;
		 	cout << " " << simball_Left.APedals ;
			cout << "   " ;
			//----------------------------------------------------
			simball_camera.ReadSimBallState(simball.handle[3]);
		    cout <<" " <<simball.handle[3] << " " << int (simball_camera.AYaw);
		    cout << " " << int (simball_camera.APitch);
		    cout << " " << int (simball_camera.AInsertion);
			cout << " " << simball_camera.ARotation;
		   	cout << " " << simball_camera.AGrip;
		 	cout << " " << simball_Left.APedals;
			cout << endl;
			Sleep (100);
		};
	num_res= simball.DisconnectFromSimBall(0);
	hid_free_enumeration(simball.device); //we free enumeration
	Sleep (50);
	hid_exit(); //in the future should be in other place (it depends on ogre...)
	return 0;
}