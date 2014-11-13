// getting data from symballs
//SimballConfig.exe must be on execution (located on the folder)

#include <windows.h>
#include <iostream> // <...> used when is a library outside the project ; "..." used when we have the headers in the directory or subdirectory
#include "hidapi.h"
using namespace std;

#define SYMBALL_VENDOR_ID 7330
#define SYMBALL_PRODUCT_ID 1

int main(){
	
	//****************************************
	//***********device inicialization********
	//****************************************
	
	struct hid_device_info *device, *current_device;
	// device = hid_enumerate(1118,673); //numbers corresponding to the xbox controller 
	//when we write these numbers we only see the xbox device
	//device = hid_enumerate(7330, 1);    //symballs ID
	device = hid_enumerate(0x00,0x00);     //when we write (0,0) we see all devices
	current_device = device;

	int i;
	i = 1;

	if (current_device != NULL){
		do{
			char *path_info;
			path_info = current_device->path;

			unsigned short IDvendor;
			IDvendor = current_device->vendor_id;

			unsigned short IDproduct;
			IDproduct = current_device->product_id;

			wchar_t *ser_num;
			ser_num = current_device->serial_number;


			wchar_t *product;
			product = current_device->product_string;

			cout << "Device number: " << endl ;
			cout << i <<endl;
			cout << "The Vendor ID is: " << endl;
			cout << IDvendor <<endl;
			cout << "The Product ID is: " << endl ;
			cout << IDproduct << endl;

			cout << "Product string is: " << endl;
			cout << product << endl;

			cout << "Path:" << endl;
			cout << path_info << endl;

			current_device = current_device->next;
			i++;
		} while (current_device != NULL);
	}

	hid_free_enumeration(device); //we free enumeration

	hid_device *device2 = hid_open(SYMBALL_VENDOR_ID,SYMBALL_PRODUCT_ID,NULL); //while NULL it will open only the first

	//****************************************
	//***********simballs data reading********
	//****************************************

	const size_t BUFSIZE = 80;
	unsigned char * buf = new unsigned char[BUFSIZE];
	unsigned long tempX; unsigned long tempY; unsigned long tempZ;
	//unsigned long tempTrigger; 
	unsigned long tempRotation; unsigned long quarter;
	unsigned int pedals; unsigned int scissors;

	unsigned long tempXOld=0; 
	unsigned long tempYOld=0;
	unsigned long tempZOld=0;
	//unsigned long tempTriggerOld=0;
	unsigned long tempRotationOld=0;
	unsigned long quarterOld=0;
	unsigned int pedalsOld=0;
	unsigned int scissorsOld=0;

	for (unsigned n = 0;  n < 10000 ; ++n) {
		// buff clear
		for (size_t i = 0; i < BUFSIZE; ++i)
			buf[i] = 0;

		hid_read(device2, buf, BUFSIZE);

		cout << endl;
		cout << endl;
		for (size_t i = 0; i < BUFSIZE; ++i)
			printf("%.2x", buf[i]);

		tempX = ((buf[20]<<16) | (buf[19]<<8) | (buf[18]));//X coordinate --4188990 to 4194858 (roughly)-- (buf[17] it's important too, but changes too much)
		tempY = ((buf[12]<<16) | (buf[11]<<8) | (buf[10]) ); //Y coordinate --4189022 to 4194913 (roughly)-- (buf[9] it's important too, but changes too much)
		tempZ = buf[36]; //Z coordinate --from 5 to 62--((buf[35]) it's important too, but changes too much)
		//tempTrigger = ((buf[32]<<16) | (buf[31]<<8) | (buf[30])); //buf[32]buf[31] scissors
		scissors=((buf[32]<<8) | (buf[31])); //--from 64 to 654-- growing=closing; decreasing=opening;
		tempRotation = ((buf[27]<<8) | (buf[26]));
		quarter = buf[28]>>6;
		pedals = buf[45]; //plugged right: yellow=8 and blue=4; plugged left: yellow=2 and blue=1; both=addition; 

		cout<<"X: ";
		cout << tempX <<endl;
		cout<<"Y: ";
		cout<<tempY<<endl;
		cout<<"Z: ";
		cout<<tempZ<<endl;
		/*cout<<"Trigger: ";
		cout<<tempTrigger<<endl;*/
		cout<<"Scissors: ";
		cout<<scissors<<endl;
		cout<<"Rotation: ";
		cout<<tempRotation<<endl;
		cout<<"Quarter: ";
		cout<<quarter<<endl;
		cout<<"Pedals: ";
		cout<<pedals<<endl;
		if (pedals==8){
			cout<<"Right plugged yellow pedal"<<endl;
		}else if (pedals==4){
			cout<<"Right plugged blue pedal"<<endl;
		}else if (pedals==2){
			cout<<"Left plugged yellow pedal"<<endl;
		}else if (pedals==1){
			cout<<"Left plugged blue pedal"<<endl;
		}else if (pedals==0xc){
			cout<<"Right plugged both pedals"<<endl;
		}else if (pedals==3){
			cout<<"Left plugged both pedals"<<endl;
		}
		
		// testing byte
		int tmp=buf[6];
		cout<<tmp<<endl;


		unsigned long tempXDif=tempX-tempXOld;
		unsigned long tempYDif=tempY-tempYOld;
		unsigned long tempZDif=tempZ-tempZOld;
		//unsigned long tempTriggerDif=tempTrigger-tempTriggerOld;
		signed int scissorsDif=scissors-scissorsOld;
		unsigned long tempRotationDif=tempRotation-tempRotationOld;
		unsigned long quarterDif=quarter-quarterOld;
		
		//except scissors, it's only testing
		cout<<endl;
		cout<<"Difference with last: "<<endl;
		cout<<"X difference: ";
		cout << tempXDif <<endl;
		cout<<"Y difference: ";
		cout<<tempYDif<<endl;
		cout<<"Z difference: ";
		cout<<tempZDif<<endl;
		/*cout<<"Trigger difference: ";
		cout<<tempTriggerDif<<endl;*/
		cout<<"Rotation difference: ";
		cout<<tempRotationDif<<endl;
		cout<<"Quarter difference: ";
		cout<<quarterDif<<endl;
		cout<<"Scissors difference: ";
		cout<<scissorsDif<<endl;
		if (scissorsDif>0){
			cout<<"Closing scissors"<<endl;
		}else if (scissorsDif<0){
			cout<<"Opening scissors"<<endl;
		}else{
			cout<<"Not moving scissors"<<endl;
		}


		Sleep(600);

		tempXOld=tempX;
		tempYOld=tempY;
		tempZOld=tempZ;
		//tempTriggerOld=tempTrigger;
		tempRotationOld=tempRotation;
		quarterOld=quarter;
		pedalsOld=pedals;
		scissorsOld=scissors;
	}


	//pause to see the command window
	cout << "Press enter" << endl;
	int in;
	in = getchar();

	hid_exit(); //in the future should be in other place (it depends on ogre...)
	hid_init(); //in the future should be at the end of execution to avoid memory leaks

	return 0;


}