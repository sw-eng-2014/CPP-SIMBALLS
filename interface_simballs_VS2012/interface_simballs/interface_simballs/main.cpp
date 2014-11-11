// getting data from symballs
#include <windows.h>
#include <iostream> // <...> used when is a library outside the project ; "..." used when we have the headers in the directory or subdirectory
#include "hidapi.h"
using namespace std;

#define SYMBALL_VENDOR_ID 7330
#define SYMBALL_PRODUCT_ID 1

int main(){
	hid_device_ *handle;
	
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
			
			//printf_s("The product string is i: %s\n",product);
			cout << "Product string is: " << endl;
			cout << product << endl;

			// printf_s("The path is: %s\n", path_info);
			cout << "Path:" << endl;
			cout << path_info << endl;


			current_device = current_device->next;
			i++;
		} while (current_device != NULL);
	}

	hid_free_enumeration(device); //we free enumeration
	hid_free_enumeration(current_device); //we free enumeration


	hid_device *device2 = hid_open(SYMBALL_VENDOR_ID,SYMBALL_PRODUCT_ID,NULL); //while NULL it will open only the first

	const size_t BUFSIZE = 80;
	unsigned char * buffer = new unsigned char[BUFSIZE];

	for (unsigned n = 0; n < 100; ++n) {
		// buff clear
		for (size_t i = 0; i < BUFSIZE; ++i)
			buffer[i] = 0;

		hid_read(device2, buffer, BUFSIZE);

		for (size_t i = 0; i < BUFSIZE; ++i)
			printf("%.2x", buffer[i]);
		cout << endl;

		//sleep
		Sleep(1000);
	}
	
	//data inside buffer
	delete[] buffer;

	//pause to see the command window
	cout << "Press enter" << endl;
	int in;
	in = getchar();

	hid_exit(); //in the future should be in other place (it depends on ogre...)
	hid_init(); //in the future should be at the end of execution to avoid memory leaks

	return 0;
}