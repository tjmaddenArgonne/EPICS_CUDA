// softwareCam.cpp : Defines the entry point for the DLL application.
//

//#include "stdafx.h"
#include <time.h>
#include "display_socket.h"

using namespace genCamControllerEnum;




int_string display_socket::paramStrs[] = {
	{ is_running,"d_is_running"},
	{ dispsock_callback,"d_dispsock_callback"},
	{ run_counter,"d_run_counter"},
	{ num_pics_sent,"d_num_pics_sent"},
	{is_disp_image, "d_is_disp_image"},
	{disp_ip_addr ,"d_disp_ip_addr"},
	{is_suspend ,"d_is_suspend"},
	{data_source_ptr ,"d_data_source_ptr"},
	{source_data_param ,"d_source_data_param"},
	{ source_isnew_param,"d_source_isnew_param"},
	{ sleep_ms,"d_is_sleep_ms"}
};

// This is the constructor of a class that has been exported.
// see softwareCam.h for the class definition
display_socket::display_socket() :
	lf("display_log.txt"),
	RAM1()
{
	WSADATA wsaData;
	lf.log("display socket constructing");

// Initialize Winsock
int iResult = WSAStartup(MAKEWORD(2,0), &wsaData);
//if (iResult != 0) {
 //   printf("WSAStartup failed: %d\n", iResult);
  //  return 1;
//}

	num_inherits++;
	this->paramIntStrArray[num_inherits] = paramStrs;
	this->num_params[num_inherits]=num_param_strs;

	putIntParam(is_running,true);
	putIntParam(run_counter,0);
	putIntParam(is_suspend,false);
	putIntParam(num_pics_sent,0);
	putStringParam(disp_ip_addr,"127.1.1.1");
	putIntParam(is_disp_image,true);
	putIntParam(sleep_ms,1000);
	putPtrParam(data_source_ptr,0);
	data_source = 0;
	putPtrParam(dispsock_callback,(const void*)display_socket::defaultCallback);

	callbackFunction = (int (*)(callbackReasons reason, void*))getPtrParam(dispsock_callback);

	clearChanges();

	//setExe(this);
	//makeThread();
	(*callbackFunction)(make_new_thread,this);
}

int display_socket::main(int arg)
{
	lf.log("display_socket entering main function");

	while (getIntParam(is_running))
	{
		try
		{
			if (getIntParam(is_suspend))
			{
				(*callbackFunction)(thread_suspend,this);

			//	this->suspend();
			}
			else
				Sleep(getIntParam(sleep_ms));//disp at 2fps max

			putIntParam(run_counter, getIntParam(run_counter) + 1);


/////

			// update the pointer to data osurce object that generates images...
			data_source = (genCamController *)getPtrParam(data_source_ptr);

	if (getPtrParam(data_source_ptr) != 0 )
		if(data_source->getIntParam(getIntParam(source_isnew_param)))
	{
	//	MessageBox (NULL, "in aq image", "Maddog", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		lf.log("display socket sees new image...");
	data_source->putIntParam(getIntParam(source_isnew_param),false);


// call data sources mutex as we want its image.
		(*callbackFunction)(grab_mutex,data_source);
//		source_ram->grabMutex();





		image_ram *source_ram = (image_ram*)data_source->getPtrParam(getIntParam(source_data_param));
//		RAM1.setImageSize(source_ram->getImgX(),source_ram->getImgY());
		RAM1.copy(*source_ram);

		//source_ram->releaseMutex();
		(*callbackFunction)(release_mutex,data_source);

		char mzzz[80];
		sprintf(mzzz,"cor size = %i",source_ram->getImgX());
		lf.log(mzzz);

		sprintf(mzzz,"local size = %i",RAM1.getImgX());
		lf.log(mzzz);

		//
		// here is where we connect to IJ to disp iamge.
		//

		// if camera has lots of frames coming in, dont bother displaying it.
		if (getIntParam(is_disp_image))
		{

			sendImageJ2(
				(unsigned short*)RAM1.getMem(),
				RAM1.getImgX(),
				RAM1.getImgX()*RAM1.getImgY()*sizeof(short));

			lf.log("display socket sent Image to Image J");

		}


			//MessageBox (NULL, "released RAM mutex", "Maddog", MB_OK | MB_ICONERROR | MB_TASKMODAL);

		//RAM1.releaseMutex();



	}

/////




		}//try
		catch (ccd_exception err)
		{
			lf.log(err.err_mess());
		}//catch

		(*callbackFunction)(general,this);

	}//while

	lf.log("display_socket leaving main funtionc");
		(*callbackFunction)(exiting,this);
	return 0;
}//main




void display_socket::sendImageJ2(
	unsigned short* ptrx,
	long img_size,
	long datasize)
{

	int sockfd;
	int sleeptimer = 0;
	int sleeplimit = 200;


	struct sockaddr_in dest_addr;   // will hold the destination addr
	//AllocConsole();
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sockfd  == -1) // do some error checking!
	{
		perror("socket");
		throw ccd_exception("Cant make socket for some reason");

	}
	struct timeval tv;
	int milliseconds = 2000;
	tv.tv_sec = milliseconds / 1000 ;
	tv.tv_usec = ( milliseconds % 1000) * 1000  ;


    setsockopt (sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&milliseconds, sizeof(int));
    setsockopt (sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&milliseconds, sizeof(int));

	dest_addr.sin_family = AF_INET;          // host byte order
	dest_addr.sin_port = htons(4455);   // short, network byte order
	dest_addr.sin_addr.s_addr = inet_addr(getStringParam(disp_ip_addr));
	memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);

	// don't forget to error check the connect()!
	if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof dest_addr)!= -1)
	{
			int len,bytes_sent,tosend,tot_sent,kk;
			//MessageBox (NULL, "to send 4", "Maddog", MB_OK | MB_ICONERROR | MB_TASKMODAL);
 			if (img_size == 256)
				bytes_sent = send(sockfd, "8\n\r", 3, 0);
			else if (img_size == 512)
				bytes_sent = send(sockfd, "4\n\r", 3, 0);
			else if (img_size ==1024)
				bytes_sent = send(sockfd, "2\n\r", 3, 0);
			else if (img_size ==2048)
				bytes_sent = send(sockfd, "1\n\r", 3, 0);
			else if (img_size ==4096)
				bytes_sent = send(sockfd, "3\n\r", 3, 0);
			else
			{
				closesocket(sockfd);
				throw ccd_exception("imageJ_controller::sendImageJ Invalid Image size");
			}

			Sleep(20);
			//MessageBox (NULL, "to send D", "Maddog", MB_OK | MB_ICONERROR | MB_TASKMODAL);
			bytes_sent = send(sockfd, "d\n\r", 3, TCP_NODELAY);
			Sleep(20);
			//MessageBox (NULL, "to send data", "Maddog", MB_OK | MB_ICONERROR | MB_TASKMODAL);


			unsigned char *imgptr = (unsigned char*)ptrx;
			int tot_to_send = datasize;


			char rcvbuf[128];


				 sleeptimer = 0;

				tosend = datasize;
				tot_sent = 0;

				 clock_t timestart = clock()*CLOCKS_PER_SEC;
				 sleeplimit = 2; //2 second wait
				clock_t elapsetime;
				while (tot_sent<tosend)
				{
					//bytes_sent= send(sockfd, &zero, 1, TCP_NODELAY);
					bytes_sent= send(sockfd, (const char*)imgptr, tosend -tot_sent, 0);
					imgptr+=bytes_sent;
					tot_sent +=bytes_sent;
					elapsetime = clock()*CLOCKS_PER_SEC -timestart;
					if (elapsetime > sleeplimit)
					{
						closesocket(sockfd);
						throw ccd_exception("imageJ_controller::sendImageJ Cannot send to Image J");
					}

					//Sleep(1);
				}



			//MessageBox (NULL, "to send X", "Maddog", MB_OK | MB_ICONERROR | MB_TASKMODAL);
			//bytes_sent = send(sockfd, "X\n\r", 3, TCP_NODELAY);

	}
	else
	{
		closesocket(sockfd);
		throw ccd_exception("display socket:sendIJ2 Cannot connect to Image J");
	}



	closesocket(sockfd);



}


