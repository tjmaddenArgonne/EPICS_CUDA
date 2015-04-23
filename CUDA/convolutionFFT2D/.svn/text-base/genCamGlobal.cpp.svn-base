#include "genCamController.h"
#include "softwareCam.h"
//#include "corecoCam.h"
#include "display_socket.h"



BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

void makeControllerObjects(int *num_objects, genCamController *objs[],void *callback_func, void **img_data_ptr)
{
	// the programmer has to fill in here. then AD or whatever client can call it to 
	// manufacture all necessary objects.
	// client supplies pointer array  memory to store the obj pointers.
	// nium pf objects is returned.

	objs[0]= new CsoftwareCam();
	//objs[0]= new corecoCam();

	objs[0]->putPtrParam(CsoftwareCam::softwareCam_callback,callback_func);
	//objs[0]->putPtrParam(corecoCam::corecoCam_callback,callback_func);
	
	objs[1] = new display_socket();

			// tell display code aobut this... who generates image data...
		objs[1]->putPtrParam(display_socket::data_source_ptr,(void*)objs[0]);
		objs[1]->putIntParam(display_socket::source_isnew_param,CsoftwareCam::is_new_image);
		objs[1]->putIntParam(display_socket::source_data_param,CsoftwareCam::img_ram_ptr);
		objs[1]->putPtrParam(display_socket::dispsock_callback,callback_func);
	

	if (*img_data_ptr==0)
		*img_data_ptr=objs[0]->getPtrParam(CsoftwareCam::copy_memory);
	else
		objs[0]->putPtrParam(CsoftwareCam::copy_memory,*img_data_ptr);


	*num_objects = 2;
}