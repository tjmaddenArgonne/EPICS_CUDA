// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the SOFTWARECAM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// SOFTWARECAM_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <winsock.h>


#ifndef _DISPSOCK_H
#define _DISPSOCK_H


#include "genCamController.h"
#include "logfile.h"
#include "image_ram2.h"
#include "ccd_exception.h"
//#include "softwareCam.h"

// This class is exported from the softwareCam.dll
class display_socket : public genCamController
{
	public:
		display_socket(void);
		// TODO: add your methods here.

		enum display_socket_params
		{
			is_running = genCamControllerEnum::subclassSerial2,		// true uf keep looping main
			dispsock_callback,// ptr to callback
			run_counter,// counts time in main
			num_pics_sent,// counts num images sent
			disp_ip_addr,// where to send the images, IP address
			is_disp_image,//true to dusp this iamges
			is_suspend, //trye to syspend this thread to be reawaken
			data_source_ptr, //ptr to calling genCamController where images are generated
			source_data_param,// parameter in caller where the data is stored..
								//call datasource->getPtrParam(this->getIntParam(source_data_param));
			source_isnew_param ,  // parameter in caller where the is new image flag is stored..
								//call datasource->getBoolParam(this->getIntParam(source_isnew_param));
			sleep_ms
			
		};
		enum {num_param_strs=11};

		static int_string paramStrs[];

		virtual int main(int arg);




protected:

	log_file lf;
	image_ram RAM1;

	// where iamges will come from...
	genCamController *data_source;


	void sendImageJ2(
		unsigned short* ptrx,
		long img_size,
		long datasize);
};

//extern SOFTWARECAM_API int nsoftwareCam;

//SOFTWARECAM_API int fnsoftwareCam(void);


#endif
