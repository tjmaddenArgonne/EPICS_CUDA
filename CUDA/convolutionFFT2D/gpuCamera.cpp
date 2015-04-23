// softwareCam.cpp : Defines the entry point for the DLL application.
//

//#include "stdafx.h"
#include "glviewer.h"
#include "gpuCamera.h"
//#include "corecoFPGACam.h"
#include <stdio.h>
#include "stopWatch.h"
#include "thread.h"

using namespace genCamControllerEnum;
using namespace glviewer_stuff;

int_string gpuCamera::paramStrs[] = {
	{		is_running ,			"gpu_is_running"},
	{		log_file_ptr,			"gpu_log_file_ptr"},
	{		is_log, 				"gpu_is_log"},
	{		gpu_callback,	"gpu_callback"},
	{		run_counter,			"gpu_run_counter"},
	{		sleep_ms,				"gpu_sleep_ms"},
	{		is_sleep,				"gpu_is_sleep"},
	{		setup_gpu,				"gpu_setup_gpu"},
	{		is_setup_gpu,				"gpu_is_setup_gpu"},
	{		run_ffts,				"gpu_run_ffts"},

	{		xfer2gpu,				"gpu_xfer2gpu"},
	{		shut_gpu,				"gpu_shut_gpu"},
	{		dark_sub,				"gpu_dark_sub"},
	{		gain_mult,				"gpu_gain_mult"},
	{		set_dark,				"gpu_set_dark"},
	{		set_gain,				"gpu_set_gain"},

	{		keep_gpu_running,				"gpu_keep_gpu_running"},
			
	{		host_set_dark,				"gpu_host_set_dark"},
	{		host_dark_sub,				"gpu_host_dark_sub"},
	{		gpu_host_fft,				"gpu_host_fft"},
	{		gpu_host_ret_filt,			"gpu_host_ret_filt"},
	{is_gpusetup,"is_gpusetup"},
	{hi_speed_view,"hi_speed_view"},


	{		last_starter_param,		"w_last_starter_param"}

		};
	
			
			



// This is the constructor of a class that has been exported.
// see softwareCam.h for the class definition
gpuCamera::gpuCamera() :
	lf("gpuCamera_log.txt")
{
	
	num_inherits++;
	this->paramIntStrArray[num_inherits] = paramStrs;
	this->num_params[num_inherits]=num_param_strs;

	p=0;
	pg=0;
	p_conv_inv=0;
	pg_inv=0;
	fftmags=0;
	 imagew=0;
	 imagewF=0;
	 gauss_filt=0;
	 gauss_filtF=0;
	 convF=0;
	 conv=0;

	//serial_port = 0;
	lf.log("gpuCamera constructing");
	// set callback to do nothing default....
	putPtrParam(gpu_callback,(const void*)genCamController::defaultCallback);
	callbackFunction = (int (*)(callbackReasons reason, void*))getPtrParam(gpu_callback);

	putIntParam(gpu_host_ret_filt,0);
	putIntParam(dark_sub,0);
	putIntParam(gain_mult,0);
	putIntParam(set_gain,0);
	putIntParam(set_dark,0);

	putIntParam(host_dark_sub,0);
	putIntParam(host_set_dark,0);

	putIntParam(is_gpusetup,0);

	putIntParam(gpu_host_fft,0);
	putIntParam(is_setup_gpu,0);
	/// init all params or else the client will not see them...they will be undefined...
	putIntParam(is_running ,1);
	putPtrParam(log_file_ptr, (void*)&lf);
	putIntParam(is_log,1);
	putIntParam(run_counter,0);
	putIntParam(sleep_ms,500);
	putIntParam(is_sleep,0);// suspend...

	putIntParam(setup_gpu,1);
	putIntParam(run_ffts,0);
	putIntParam(shut_gpu,0);
	putIntParam(keep_gpu_running,0);

	putIntParam(hi_speed_view,0);
	is_view_init=false;

	putIntParam(xfer2gpu,0);

	putIntParam(gpu_img_sizex,1024);
	putIntParam(gpu_img_sizey,1024);



	clearChanges();
			
}


void gpuCamera::clearPipe(FILE *fp)
{
	int ret;
	int counter=0;
	ret = fgetc(fp);
	while (ret != EOF)
	{
		counter++;
		if (counter>100)
			break;
	}

}


int gpuCamera::main(int arg)
{

	printf("starting viewer\n");

	initGL();
	   
 cudaGLSetGLDevice( cutGetMaxGflopsDeviceId() );

       // cutilSafeCall( CUDA_MallocArray(&h_Src, imageW, imageH) );

       initOpenGLBuffers();

	
   glutMainLoop();

 //   cutilExit(argc, argv);

  //  cudaThreadExit();

//	is_view_init=false;

	return(0);
}


int gpuCamera::processCallback(int arg)
{
	FILE *fp;
	char mesg[128];
				stopWatch tx;

	unsigned short *aa,*bb;
//	lf.log("wiiz entering main function");

	// update callback function ptr
	callbackFunction = (int (*)(callbackReasons reason, void*))getPtrParam(gpu_callback);



		try
		{

			callbackFunction = (int (*)(callbackReasons reason, void*))getPtrParam(gpu_callback);


		
			putIntParam(run_counter, getIntParam(run_counter) + 1);

/*
			if (getIntParam(is_gpusetup)==0 || getIntParam(setup_gpu)==1)				
			{
				if (getIntParam(is_gpusetup)==1)
						shutGPU();

				setupGPU();
				
				putIntParam(setup_gpu,0);
			}



			if (getIsChanged(gpu_img_sizex) || getIsChanged(gpu_img_sizey))
			{
				clearChanges(gpu_img_sizex);
				clearChanges(gpu_img_sizey);

				if (getIntParam(gpu_img_sizex) != dataW  || getIntParam(gpu_img_sizey)!=dataH)
				{

					if (getIntParam(is_gpusetup)==1)
						shutGPU();

					setupGPU();
					
				}

			}

			if (getIntParam(gpu_host_fft))
			{
				
				aa=(unsigned short *)getPtrParam(image_mem_ptr2);
				bb=(unsigned short *)getPtrParam(image_mem_ptr);

				cpuFFTConv(aa,bb);

			}

			if (getIntParam(xfer2gpu))
				test0();

			//}

			if (getIntParam(shut_gpu))
				shutGPU();

			
*/
			if (getIntParam(hi_speed_view)==1)
			{

				//printf("d_result %d\n",glviewer_stuff::d_result);
				if (is_view_init==false)
				{
					glviewer_stuff::imageW=getIntParam(gpu_img_sizex);
					glviewer_stuff::imageH=getIntParam(gpu_img_sizey);
					putIntParam(gpu_ret_new_img,0);

					thread t=thread(this);
				
					is_view_init=true;
				}
				else
				{
					//aa=(unsigned short *)getPtrParam(image_mem_ptr2);
				bb=(unsigned short *)getPtrParam(image_mem_ptr);

					glviewer_stuff::imageW=getIntParam(gpu_img_sizex);
					glviewer_stuff::imageH=getIntParam(gpu_img_sizey);

					memcpy(glviewer_stuff::h_buffer,bb,glviewer_stuff::imageW * glviewer_stuff::imageH * sizeof(short));
					glviewer_stuff::h_srcimgptr = h_buffer;
					glviewer_stuff::update_count++;
				
				}



			}


			// call back to client so it can do somehing like update params in EPICS etc.
			callbackFunction(genCamControllerEnum::general, this);


			
		

		}//try
		catch (ccd_exception err)
		{
			lf.log(err.err_mess());
			(*callbackFunction)(call_error,this);

		}//catch
		



	(*callbackFunction)(exiting,this);

	return 0;
}//main


void gpuCamera::setupGPU(void)
{
	int sx;
	int sy;

	putIntParam(is_gpusetup,1);

	sx=getIntParam(gpu_img_sizex);
	sy=getIntParam(gpu_img_sizey);

	cpuFFTWSetup(sx,sy,100.0);

    cutilCheckError( cutCreateTimer(&hTimer) );

    printf("Testing built-in R2C / C2R FFT-based convolution\n");
         kernelH = sy;
         kernelW = sx;
         kernelY = 3;
         kernelX = 4;
           dataH = sy;
           dataW = sx;
//        const int    fftH = snapTransformSize(dataH + kernelH - 1);
  //      const int    fftW = snapTransformSize(dataW + kernelW - 1);
          fftH = sy;
            fftW = sx;

    printf("...allocating memory\n");
        h_Data      = (float *)malloc(dataH   * dataW * sizeof(float));
        h_Kernel    = (float *)malloc(kernelH * kernelW * sizeof(float));
//        grabbermem      = (unsigned short *)malloc(dataH   * dataW * sizeof(unsigned short));

		h_ResultCPU = (float *)malloc(dataH   * dataW * sizeof(float));
        h_ResultGPU = (float *)malloc(fftH    * fftW * sizeof(float));

		h_darkImage = (unsigned short *)malloc(dataH   * dataW * sizeof(short));

        cutilSafeCall( cudaMalloc((void **)&d_Data,   dataH   * dataW   * sizeof(float)) );
		cutilSafeCall( cudaMalloc((void **)&d_floatData,   dataH   * dataW   * sizeof(float)) );
        cutilSafeCall( cudaMalloc((void **)&d_Kernel, kernelH * kernelW * sizeof(float)) );

        cutilSafeCall( cudaMalloc((void **)&d_PaddedData,   fftH * fftW * sizeof(float)) );
        cutilSafeCall( cudaMalloc((void **)&d_PaddedKernel, fftH * fftW * sizeof(float)) );

        cutilSafeCall( cudaMalloc((void **)&d_DataSpectrum,   fftH * (fftW / 2 + 1) * sizeof(fComplex)) );
        cutilSafeCall( cudaMalloc((void **)&d_KernelSpectrum, fftH * (fftW / 2 + 1) * sizeof(fComplex)) );

	      cutilSafeCall( cudaMalloc((void **)&d_shortData, dataH   * dataW   * sizeof(short)) );
	      cutilSafeCall( cudaMalloc((void **)&d_shortData2, dataH   * dataW   * sizeof(short)) );


		          cutilSafeCall( cudaMalloc((void **)&d_gainImage,   dataH   * dataW   * sizeof(float)) );
        cutilSafeCall( cudaMalloc((void **)&d_darkImage,   dataH   * dataW   * sizeof(float)) );

       
//        for(int i = 0; i < dataH * dataW; i++)
//            h_Data[i] = getRand();

		//clear the fileter to 0
        for(int i = 0; i < kernelH * kernelW; i++)
		{
			//h_Kernel[i] = getRand();
			h_Kernel[i] = 0.0;	
		}

		if (0)
		{
		    printf("...generating diric delta  Filter data\n");

		h_Kernel[0]=1.0;
		}
		else
		{
		    printf("...generating Gauusian Low Pass Filter data\n");

		//%square because gauss is sq. log 2 to make it 1/2 where x2/hw2 gives .5
			//%.25 is .5 sq, so fwhm is disx across gauyss, not from top to across.
			float fwhm=5.0;
			float spr=(fwhm*fwhm*.25)/log((float)2.0);
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
			//%make gauss filter.
			//gfilt=zeros(filtsize,filtsize);
			float xx,yy;
			float filtcoef;
			for (int xg=0;xg<kernelW;xg++)
			{
				for (int yg=0;yg<kernelH;yg++)
				{
					xx=((xg+kernelW/2) % kernelW )-kernelW/2;
					yy= ((yg+kernelH/2) %kernelH )-kernelH/2;
					filtcoef = exp(-(  xx*xx + yy*yy  )/spr);
					filtcoef=filtcoef/((float)kernelW);
					h_Kernel[xg*kernelW + yg]=filtcoef;
			   // %gfilt(xg,yg)=exp((xg).^2 + (yg).^2);
				}
			}


		}
//////
	///////
////////
////////


    printf("...creating R2C & C2R FFT plans for %i x %i\n", fftH, fftW);
        cufftSafeCall( cufftPlan2d(&fftPlanFwd, fftH, fftW, CUFFT_R2C) );
        cufftSafeCall( cufftPlan2d(&fftPlanInv, fftH, fftW, CUFFT_C2R) );


		printf("...uploading to GPU  kernel data\n");
        cutilSafeCall( cudaMemcpy(d_Kernel, h_Kernel, kernelH * kernelW * sizeof(float), cudaMemcpyHostToDevice) );
        cutilSafeCall( cudaMemset(d_PaddedKernel, 0, fftH * fftW * sizeof(float)) );
        cutilSafeCall( cudaMemset(d_PaddedData,   0, fftH * fftW * sizeof(float)) );

  printf("...transforming convolution kernel\n");

//	cufftSafeCall( cufftExecR2C(fftPlanFwd, (cufftReal *)d_PaddedKernel, (cufftComplex *)d_KernelSpectrum) );
	cufftSafeCall( cufftExecR2C(fftPlanFwd, (cufftReal *)d_Kernel, (cufftComplex *)d_KernelSpectrum) );

}


void gpuCamera::shutGPU(void)
{
		putIntParam(is_gpusetup,0);

	    printf("...shutting down\n");
        cutilCheckError( cutDeleteTimer(hTimer) );

        cufftSafeCall( cufftDestroy(fftPlanInv) );
        cufftSafeCall( cufftDestroy(fftPlanFwd) );

        cutilSafeCall( cudaFree(d_DataSpectrum)   );
        cutilSafeCall( cudaFree(d_KernelSpectrum) );
        cutilSafeCall( cudaFree(d_PaddedData)   );
        cutilSafeCall( cudaFree(d_PaddedKernel) );
        cutilSafeCall( cudaFree(d_Data)   );
        cutilSafeCall( cudaFree(d_Kernel) );
        cutilSafeCall( cudaFree(d_shortData) );
		cutilSafeCall( cudaFree(d_floatData) );
		

        cutilSafeCall( cudaFree(d_shortData2) );
        cutilSafeCall( cudaFree(d_gainImage) );
        cutilSafeCall( cudaFree(d_darkImage) );


        free(h_ResultGPU);
        free(h_ResultCPU);
        free(h_Data);
        free(h_Kernel);
		free(h_darkImage);

}
void gpuCamera::test0(void)
{
 double gpuTime;
stopWatch timer1;
stopWatch timer2;

int sx,sy;

		sx=getIntParam(gpu_img_sizex);
	sy=getIntParam(gpu_img_sizey);


/*
	printf("Setting up Dalsa Grabber");

	coreco_card = new coreco(false);
	coreco_card->setConfigFileName("D:/corecofiles/c1k1k16bit.ccf");
	coreco_card->setNumBuffers(4);
	coreco_card->initialize(1024,1024,true);

	printf("Starting GRAB Mode");
	coreco_card->grab();
*/
        cutilCheckError( cutResetTimer(hTimer) );
        cutilCheckError( cutStartTimer(hTimer) );
		timer1.tic();

		grabbermem=(unsigned short *)getPtrParam(image_mem_ptr);
		grabbermem2=(unsigned short *)getPtrParam(image_mem_ptr2);

     //   for(int i = 0; i < dataH * dataW; i++)
     //       h_Data[i] = (float)(grabbermem[i]);


	


		if (getIntParam(xfer2gpu))		
		{
			cutilSafeCall( cudaMemcpy(d_shortData,   grabbermem,   dataH   * dataW *   sizeof(short), cudaMemcpyHostToDevice) );
			//convertUShort2Float(d_floatData, d_shortData, fftH, fftW);
		}

//        cutilSafeCall( cudaMemset(d_PaddedKernel, 0, fftH * fftW * sizeof(float)) );
 //       cutilSafeCall( cudaMemset(d_PaddedData,   0, fftH * fftW * sizeof(float)) );

	//	 cutilCheckError( cutResetTimer(hTimer2) );
     //   cutilCheckError( cutStartTimer(hTimer2) );
//         gpuTime = cutGetTimerValue(hTimer);
	   // printf("Copytime %f ms\n", gpuTime);

		
    //Not including kernel transformation into time measurement,
    //since convolution kernel is not changed very frequently
  
  //  printf("...running GPU FFT convolution: ");
//        cufftSafeCall( cufftExecR2C(fftPlanFwd, (cufftReal *)d_PaddedData, (cufftComplex *)d_DataSpectrum) );
        cutilSafeCall( cudaThreadSynchronize() );

			if (getIntParam(host_dark_sub))
			{
				//h_ShortResultCPU
				grabbermem=(unsigned short *)getPtrParam(image_mem_ptr);	
				grabbermem2=(unsigned short *)getPtrParam(image_mem_ptr2);

				cpuDarkSubtract(grabbermem2,grabbermem ,h_darkImage, fftH, fftW);
			
			}

			if (getIntParam(host_set_dark))
			{
				//h_ShortResultCPU
				grabbermem=(unsigned short *)getPtrParam(image_mem_ptr);	
				memcpy(h_darkImage,grabbermem,sx*sy*sizeof(short));

			}



		if (getIntParam(set_dark))
		{

			        cutilSafeCall( 
						cudaMemcpy(
							d_shortData,   
							grabbermem,   
							dataH   * dataW *   sizeof(short), 
							cudaMemcpyHostToDevice)
					);

					convertUShort2Float(d_darkImage, d_shortData, fftH, fftW);
					

//					h_darkImage
					//copy to host dark iage...
//					memcpy(h_darkImage,grabbermem,dataH   * dataW *   sizeof(short));
		}

		if (getIntParam(dark_sub))
		{
			//timer2.tic();
			convertUShort2Float(d_Data, d_shortData, fftH, fftW);		
			subtractDarkImage(d_PaddedData,d_Data ,d_darkImage, fftH, fftW);
			convertFloat2UShort(d_shortData,d_PaddedData , fftH, fftW);
			//printf("dark time gpu: %f\n",timer2.toc());
		}



		if (getIntParam(run_ffts))
		{
			convertUShort2Float(d_Data, d_shortData, fftH, fftW);


			cufftSafeCall( cufftExecR2C(fftPlanFwd, (cufftReal *)d_Data, (cufftComplex *)d_DataSpectrum) );
			modulateAndNormalize(d_DataSpectrum, d_KernelSpectrum, fftH, fftW, 1);
			cufftSafeCall( cufftExecC2R(fftPlanInv, (cufftComplex *)d_DataSpectrum, (cufftReal *)d_PaddedData) );

			cutilSafeCall( cudaThreadSynchronize() );

		 // printf("Convert result to shorts\n");
		  convertFloat2UShort(d_shortData,d_PaddedData , fftH, fftW);

				   gpuTime = cutGetTimerValue(hTimer);
		//	printf("ComputeTime+Copytime %f ms\n", gpuTime);

		}
		// dbpf "13SIM1:DLL:gpu_dark_sub","0"
		// dbpf "13SIM1:DLL:gpu_run_ffts","1"
		//
		// dbpf "13SIM1:DLL:gpu_xfer2gpu","1"


	      //    cutilCheckError( cutStopTimer(hTimer2) );
     //   double gpuTime2 = cutGetTimerValue(hTimer2);

		//    printf("%CALC f MPix/s (%f ms)\n", (double)dataH * (double)dataW * 1e-6 / (gpuTime2 * 0.001), gpuTime2);

    //printf("...reading back GPU convolution results\n");
	
	

		if (getIntParam(xfer2gpu))
		{
			cutilSafeCall( cudaThreadSynchronize() );
			//grabbermem2=(unsigned short *)getPtrParam(image_mem_ptr2);
				cutilSafeCall( cudaMemcpy(grabbermem2, d_shortData, fftH * fftW * sizeof(short), cudaMemcpyDeviceToHost) );
		
		}
			
			cutilCheckError( cutStopTimer(hTimer) );
         gpuTime = cutGetTimerValue(hTimer);

    printf("TOTAL %f MPix/s (%f ms)\n", (double)dataH * (double)dataW * 1e-6 / (gpuTime * 0.001), gpuTime);
	printf("Timer1 %f\n",timer1.toc());

}



void gpuCamera::cpuDarkSubtract(
	unsigned short *h_Dst,
	unsigned short *h_Src,
	unsigned short *h_DImg,
	int Xsize,
	int Ysize)
{

	int datasize;
	datasize=Xsize*Ysize;
	for (int k=0; k<datasize;k++)
	{
		if (h_Src[k] > h_DImg[k])
			h_Dst[k]=h_Src[k] - h_DImg[k];
		else
			h_Dst[k]=0;
	}


}

void gpuCamera::cpuFFTConv(
   unsigned short* dst_data,
   unsigned short* src_data)
{
	int k;
	stopWatch tt;

	int fftlen = fft_xsize*fft_ysize;
	// convert image data to double, copy to fft double memory.
	
	tt.tic();
	printf("FFT Filtering on HOST\n");
	for (k=0; k<fftlen; k++)
	{
		imagew[k][0]=(double)src_data[k];
		imagew[k][1]=0.0;

	}

	fftw_execute(p);
	scaleFFT(imagew, fft_xsize*fft_ysize);

	if (getIntParam(gpu_host_ret_filt)==0)// for returning the raw filter... debugging
	{
		for (k=0; k<fftlen; k++)
		{
			// real part
			convF[k][0] = imagewF[k][0]*gauss_filtF[k][0] - imagewF[k][1]*gauss_filtF[k][1];
			//imag part
			convF[k][1] = imagewF[k][0]*gauss_filtF[k][1] + imagewF[k][1]*gauss_filtF[k][0];
		}
	}
	else
	{
		for (k=0; k<fftlen; k++)
		{
			// real part
			convF[k][0] = gauss_filtF[k][0];
			//imag part
			convF[k][1] = gauss_filtF[k][1];
		}
	}
		
		// do inverse FFT to find filtered image.
		fftw_execute(p_conv_inv);

		//scaleFFT(conv, fft_xsize*fft_ysize);

		
	for (k=0; k<fftlen; k++)
	{
		dst_data[k] = (unsigned short)(conv[k][0]/(1024.0*1024.0));

	}

	printf("Host FFT %fms\n",tt.toc()*1000.0);

}




void gpuCamera::cpuFFTWSetup(
	int xx, 
	int yy,
	double fwhmd)
{
	fft_xsize=getIntParam(gpu_img_sizex);

	fft_ysize=getIntParam(gpu_img_sizey);

		printf("Redoing HOST FFTW  plans, filters etc.\n");
		fftw_destroy_plan(p);
		//fftw_free(fftin);
		//fftw_free(fftout);
		fftw_free(fftmags);
//		fft_xsize=pArrayOut->dims[0].size ;
//		fft_ysize=pArrayOut->dims[1].size ;
		//fftin = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fft_xsize*fft_ysize);
		//fftout = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fft_xsize*fft_ysize);
		fftmags = (double*) fftw_malloc(sizeof(double) * fft_xsize*fft_ysize);

		fftw_free(imagew);
		fftw_free(imagewF);
		imagew = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fft_xsize*fft_ysize);
		imagewF = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fft_xsize*fft_ysize);



		p = fftw_plan_dft_2d(
			fft_xsize,
			fft_ysize,
			imagew,
			imagewF,
			FFTW_FORWARD,
			FFTW_ESTIMATE);




	fftw_free(gauss_filt);
	gauss_filt = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fft_xsize*fft_ysize);
	fftw_free(gauss_filtF);
	gauss_filtF = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fft_xsize*fft_ysize);

	fftw_destroy_plan(pg);

//	fwhmX1k_last = fwhmX1k;
	fwhmd = 5.0;//piuxels?
	makeFilt(
		fft_xsize,
		fft_ysize,
		fwhmd,
		gauss_filt);

	pg = fftw_plan_dft_2d(
		fft_xsize,
		fft_ysize,
		gauss_filt,
		gauss_filtF,
		FFTW_FORWARD,
		FFTW_ESTIMATE);

	fftw_execute(pg);
	scaleFFT(gauss_filtF, fft_xsize*fft_ysize);

		fftw_free(convF);
		convF = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fft_xsize*fft_ysize);
		fftw_free(conv);
		conv = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fft_xsize*fft_ysize);

	fftw_destroy_plan(p_conv_inv);
			p_conv_inv = fftw_plan_dft_2d(
					fft_xsize,
					fft_ysize,
					convF,
					conv,
					FFTW_BACKWARD,
					FFTW_ESTIMATE);

		printf("Done with HOST FFTSETUP FFTW\n");
	
	



}

void gpuCamera::scaleFFT(fftw_complex *ffx, int flen)
{
	double maxfft=0.0;
	int k;
	double scale;

	scale = (double)flen;
	scale=sqrt(scale);

	//FILE *pFile;

 for (k=0; k<flen; k++)
	{
		ffx[k][0] = ffx[k][0]/scale;
		ffx[k][1] = ffx[k][1]/scale;
	}

}


void gpuCamera::makeFilt(
	int filtsizex,
	int filtsizey,
	double fwhm,
	fftw_complex *gfilt)
{
		//%
	//%filtsize=512;%pixels
	//%fwhm=5%in mm
	//%up_pix_size=.06;%mm/pix


	double spr;
	int k;
	int xg,yg;
	int xx,yy;



	//%square because gauss is sq. log 2 to make it 1/2 where x2/hw2 gives .5
	//%.25 is .5 sq, so fwhm is disx across gauyss, not from top to across.
	spr=(fwhm*fwhm*.25)/log(2.0);

	//%make gauss filter.



	for (xg=0; xg<filtsizex;xg++)
	{
		for (yg=0; yg<filtsizey;yg++)
		{

			xx= ((xg+filtsizex/2) % filtsizex )-filtsizex/2;


			yy= ((yg+filtsizey/2) % filtsizey )-filtsizey/2;



			gfilt[xg + filtsizex*yg][0] =exp(-(xx*xx + yy*yy)/spr);
			gfilt[xg + filtsizex*yg][1] =0.0;

		}
	}

	gfilt[0][0] = 1.0;
	gfilt[0][1] = 0.0;

}






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

void makeControllerObjects(
	int *num_objects, 
	genCamController *objs[],
	void *callback_func,
	void **img_data_ptr)
{
	// the programmer has to fill in here. then AD or whatever client can call it to 
	// manufacture all necessary objects.
	// client supplies pointer array  memory to store the obj pointers.
	// nium pf objects is returned.	
//	objs[0]= new corecoCam();
//	objs[0]->putPtrParam(corecoCam::corecoCam_callback,callback_func);
	
	objs[0] =new gpuCamera();
	// tell serial code where coreco stuff is..
	//objs[1] ->putPtrParam(gpuCamera::coreco_control_ptr,(void *)objs[0]);
	//set the callback, so we can get back to EPICS or whatever client is running..
	objs[0]->putPtrParam(gpuCamera::gpu_callback,callback_func);

	if (*img_data_ptr==0)
		*img_data_ptr=objs[0]->getPtrParam(genCamControllerEnum::image_mem_ptr);
	else
		objs[0]->putPtrParam(genCamControllerEnum::image_mem_ptr,*img_data_ptr);

	*num_objects = 1;
}
