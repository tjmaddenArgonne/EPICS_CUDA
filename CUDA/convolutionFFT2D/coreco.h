// AncDemoRotationExpDlg.h : header file
//
#ifdef USE_SAP

#ifndef _CORECO_H
#define _CORECO_H

#include "SapClassBasic.h"
//#include "SapClassGui.h"

//#include "genCamController_S.h"
//#include <AncDlgLogo.h>


//only needed if rtpro firmware will be changed dynamically
#define ALLOW_LOADING_FIRMWARE              TRUE

//description for each design
#define EXPANSION_RTPRO_ROTATION_STR                "Rotation using Expansion Rtpro"

/////////////////////////////////////////////////////////////////////////////
// CAncDemoRotationExpDlg dialog

class coreco
{
// Construction
public:
	coreco(bool is_use_fpga);	// standard constructor

	bool CreateObjects();
	bool DestroyObjects();
	bool DestroyObjectsNoDelete();
	static void XferCallback(SapXferCallbackInfo *pInfo);
	static void SignalCallback(SapAcqCallbackInfo *pInfo);
   void GetSignalStatus();
   void GetSignalStatus(SapAcquisition::SignalStatus signalStatus);


//	bool	m_bEnableRtPro;

	CORACQ hAcq;

	SapAcquisition	*m_Acq;
	SapBuffer		*m_Buffers;
	SapView        *m_View;
   SapTransfer    *m_Xfer;
//   SapRTProDesign *m_RtproDesign;

	//true to call resetServer on initialize...
	bool is_rst_server;

   bool  m_IsSignalDetected;  // TRUE if camera signal is detected

//   unsigned long m_SelectedRtProDesign;

  // SapRTProDesign *NewRtproDesign( SapLocation location);
  // bool DeleteRtproDesign( SapRTProDesign *pRtproDesign);
  // bool InitializeRtProDesignParameters(  SapRTProDesign *pRtProDesign);

  // bool readFPGARegFile(char *filename,int num_vals);
  // bool readFPGARegsToFile(char *filename,int num_vals);
	//bool writeFPGAReg16(int address,int value);

  	virtual bool initialize(int size_x, int size_y);
	// use to override the image size in the ccf file.
	virtual bool initialize(int size_x, int size_y,bool is_force_size);
	void setConfigFileName(char *name);
	//void setFpgaFileName(char *name);
	SapAcquisition* makeAcquision();
 //void loadFPGA();

	int acq_device_number;
	SapLocation *sap_location;

	// set CC pins on the card.
	void setPin(char* pinstr, int val);
	void snap();
	 void grab();
	void freeze();
	void abort();
	bool isFrameAvailable(void);
	bool getFrame(void *mem_ptr);
	bool getFrame(void *mem_ptr, unsigned int *coreco_timestamp);
	bool isMissedFrame(void);
	void clearMissedFrames(void);
	long getTotalMissedFrames(void);
	long getRecentMissedFrames(void);
	void setNumBuffers(int b);
	void makeView(void);

	bool is_force_size;

	bool is_destroyed;

	static volatile long frames_to_cpu;
	static volatile long frame_count;
	static volatile long missed_frames;
	static volatile long recent_missed_frames;
	static volatile bool is_missed_frame;

	static volatile int sap_buffer_count;
	volatile int sensor_width;
	volatile int sensor_height;
volatile int num_buffers;

	// static genCamController* cam_control;

	char camera_format_file[256];
//	char fpga_file[256];
	char acq_server_name[256];
	char device_name[256];
		//SapView        *m_View;
	SapView        *m_View2;
		static coreco *mycard;


};


#endif // !defined(AFX_ANCROTATIONDEMODLG_H__82BFE149_F01E_11D1_AF74_00A0C91AC0FB__INCLUDED_)

#endif