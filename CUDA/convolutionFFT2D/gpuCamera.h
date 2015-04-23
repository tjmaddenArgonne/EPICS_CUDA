// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the SOFTWARECAM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// SOFTWARECAM_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifndef _gpuCamera_H
#define _gpuCamera_H

#include "genCamController.h"
#include "logfile.h"
#include "ccd_exception.h"
#include <math.h>
#include "fftw3.h"

#include <cufft.h>
#include <cutil_inline.h>


#include "convolutionFFT2D_common.h"


//#include "cl_com_port2.h"


// This class is exported from the softwareCam.dll
class gpuCamera : public genCamController
{
	public:
		gpuCamera(void);
		// TODO: add your methods here.

		enum gpuCameraParams
		{
			is_running = genCamControllerEnum::subclassSerial,
			log_file_ptr,
			is_log,
			gpu_callback,
			run_counter,
			sleep_ms,
			is_sleep,
			setup_gpu,
			is_setup_gpu,
			run_ffts,
		
			xfer2gpu,
			shut_gpu,
			dark_sub,
			gain_mult,
			set_dark,
			set_gain,
			keep_gpu_running,
			host_dark_sub,
			host_set_dark,
			gpu_host_fft,

			gpu_host_ret_filt,			
			is_gpusetup,
			hi_speed_view,
			last_starter_param
		};

		enum {num_param_strs=24};

	static int_string paramStrs[];

		virtual int main(int arg);
	virtual void clearPipe(FILE *fp);

	virtual int processCallback(int arg);


protected:

	void gpuCamera::cpuDarkSubtract(
	unsigned short *h_Dst,
	unsigned short *h_Src,
	unsigned short *h_DImg,
	int Xsize,
	int Ysize);
bool is_view_init;

unsigned short *grabbermem,
				*grabbermem2,
				*h_darkImage;

    float
        *h_Data,
        *h_Kernel,
        *h_ResultCPU,
        *h_ResultGPU;

    float
        *d_Data,
        *d_PaddedData,
        *d_Kernel,
        *d_PaddedKernel,
		*d_gainImage,
		*d_darkImage,
		*d_floatData;

    fComplex
        *d_DataSpectrum,
        *d_KernelSpectrum;

    cufftHandle
        fftPlanFwd,
        fftPlanInv;

	unsigned short *d_shortData;
	unsigned short *d_shortData2;
	
	unsigned short *h_ShortResultCPU;
    unsigned int hTimer;
    unsigned int hTimer2;



	void test0(void);


	void setupGPU(void);

void shutGPU(void);
	log_file lf;
//   cl_com_port *serial_port;
	
  int kernelH;
         int kernelW ;
         int kernelY ;
         int kernelX ;
         int   dataH ;
         int   dataW ;
//        const int    fftH = snapTransformSize(dataH + kernelH - 1);
  //      const int    fftW = snapTransformSize(dataW + kernelW - 1);
       int    fftH ;
         int    fftW;



		 //
		 //Stuff for FFTW
		 //
	int fft_xsize;
	int fft_ysize;
// for host fft conv. use FFTW
	fftw_complex *imagew;
	fftw_complex *imagewF;
	fftw_complex *gauss_filt;
	fftw_complex *gauss_filtF;
	fftw_complex *convF;
	fftw_complex *conv;

	double *fftmags;
	fftw_plan p;
	fftw_plan pg;
	fftw_plan pg_inv;
	fftw_plan p_conv_inv;
	fftw_plan p_conv;

	fftw_plan p_a;

	void cpuFFTConv(
		unsigned short* dst_data,
		unsigned short* src_data);

	
	void cpuFFTWSetup(
		int xx, 
		int yy,
		double fwhmd);

	void scaleFFT(fftw_complex *ffx, int flen);
	
	void makeFilt(
		int filtsizex,
		int filtsizey,
		double fwhm,
		fftw_complex *gfilt);


};



#endif
