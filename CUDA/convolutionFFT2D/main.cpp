/*
 * Copyright 1993-2010 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */
 
 
/*
 * This sample demonstrates how 2D convolutions 
 * with very large kernel sizes 
 * can be efficiently implemented 
 * using FFT transformations.
 */


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cufft.h>
#include <cutil_inline.h>


#include "convolutionFFT2D_common.h"

#include "coreco.h"
#include "stopWatch.h"
#include "tinytiff.h"


coreco *coreco_card;


////////////////////////////////////////////////////////////////////////////////
// Helper functions
////////////////////////////////////////////////////////////////////////////////
int snapTransformSize(int dataSize){
    int hiBit;
    unsigned int lowPOT, hiPOT;

    dataSize = iAlignUp(dataSize, 16);
	
    for(hiBit = 31; hiBit >= 0; hiBit--)
        if(dataSize & (1U << hiBit)) break;

    lowPOT = 1U << hiBit;
    if(lowPOT == dataSize)
        return dataSize;

    hiPOT = 1U << (hiBit + 1);
    if(hiPOT <= 1024)
        return hiPOT;
    else 
        return iAlignUp(dataSize, 512);
}

float getRand(void){
    return (float)(rand() % 16);
}

void test0(void){
unsigned short *grabbermem;

tinytiff tiff;

    float
        *h_Data,
        *h_Kernel,
        *h_ResultCPU,
        *h_ResultGPU;

    float
        *d_Data,
        *d_PaddedData,
        *d_Kernel,
        *d_PaddedKernel;

    fComplex
        *d_DataSpectrum,
        *d_KernelSpectrum;

    cufftHandle
        fftPlanFwd,
        fftPlanInv;

    unsigned int hTimer;


	printf("Setting up Dalsa Grabber");

	coreco_card = new coreco(false);
	coreco_card->setConfigFileName("D:/corecofiles/c1k1k16bit.ccf");
	coreco_card->setNumBuffers(4);
	coreco_card->initialize(1024,1024,true);

	printf("Starting GRAB Mode");
	coreco_card->grab();




    cutilCheckError( cutCreateTimer(&hTimer) );

    printf("Testing built-in R2C / C2R FFT-based convolution\n");
        const int kernelH = 1024;
        const int kernelW = 1024;
        const int kernelY = 3;
        const int kernelX = 4;
        const int   dataH = 1024;
        const int   dataW = 1024;
//        const int    fftH = snapTransformSize(dataH + kernelH - 1);
  //      const int    fftW = snapTransformSize(dataW + kernelW - 1);
      const int    fftH = 1024;
        const int    fftW = 1024;

    printf("...allocating memory\n");
        h_Data      = (float *)malloc(dataH   * dataW * sizeof(float));
        h_Kernel    = (float *)malloc(kernelH * kernelW * sizeof(float));
        grabbermem      = (unsigned short *)malloc(dataH   * dataW * sizeof(unsigned short));

		h_ResultCPU = (float *)malloc(dataH   * dataW * sizeof(float));
        h_ResultGPU = (float *)malloc(fftH    * fftW * sizeof(float));

        cutilSafeCall( cudaMalloc((void **)&d_Data,   dataH   * dataW   * sizeof(float)) );
        cutilSafeCall( cudaMalloc((void **)&d_Kernel, kernelH * kernelW * sizeof(float)) );

        cutilSafeCall( cudaMalloc((void **)&d_PaddedData,   fftH * fftW * sizeof(float)) );
        cutilSafeCall( cudaMalloc((void **)&d_PaddedKernel, fftH * fftW * sizeof(float)) );

        cutilSafeCall( cudaMalloc((void **)&d_DataSpectrum,   fftH * (fftW / 2 + 1) * sizeof(fComplex)) );
        cutilSafeCall( cudaMalloc((void **)&d_KernelSpectrum, fftH * (fftW / 2 + 1) * sizeof(fComplex)) );

        srand(2010);
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


	printf("Check for Frames from Camera\n");
	int checktimes=0;
	while((coreco_card->isFrameAvailable()==false) && checktimes<10)
	{
		printf("Wait from frame 1s- %d\n",checktimes);
		checktimes++;
		Sleep(1000);
	}

	stopWatch st1;
	st1.tic();
	printf("Started Stopwatch");


	if (coreco_card->isFrameAvailable())
	{
		printf("Got a frame, copy to host memory\n ");
		unsigned int corecoticks;
		coreco_card->getFrame((void*)grabbermem,&corecoticks);

		printf("Wrote image to D:/rawfile.tif");
		tiff.tifWr("D:/rawfile.tif",grabbermem,1024,1024);
		printf("CPU convert to float\n");
        for(int i = 0; i < dataH * dataW; i++)
            h_Data[i] = (float)(grabbermem[i]);


	}
	else
	{
		printf("No Frames- generate random data...");
		for(int i = 0; i < dataH * dataW; i++)
            h_Data[i] = getRand();
	}


		
		printf("...uploading to GPU and padding convolution kernel and input data\n");
        cutilSafeCall( cudaMemcpy(d_Kernel, h_Kernel, kernelH * kernelW * sizeof(float), cudaMemcpyHostToDevice) );
        cutilSafeCall( cudaMemcpy(d_Data,   h_Data,   dataH   * dataW *   sizeof(float), cudaMemcpyHostToDevice) );
        cutilSafeCall( cudaMemset(d_PaddedKernel, 0, fftH * fftW * sizeof(float)) );
        cutilSafeCall( cudaMemset(d_PaddedData,   0, fftH * fftW * sizeof(float)) );

		if (0)
		{
			padKernel(
				d_PaddedKernel,
				d_Kernel,
				fftH,
				fftW,
				kernelH,
				kernelW,
				kernelY,
				kernelX
			);

			padDataClampToBorder(
				d_PaddedData,
				d_Data,
				fftH,
				fftW,
				dataH,
				dataW,
				kernelH,
				kernelW,
				kernelY,
				kernelX
			);
		}

    //Not including kernel transformation into time measurement,
    //since convolution kernel is not changed very frequently
    printf("...transforming convolution kernel\n");

//	cufftSafeCall( cufftExecR2C(fftPlanFwd, (cufftReal *)d_PaddedKernel, (cufftComplex *)d_KernelSpectrum) );
	cufftSafeCall( cufftExecR2C(fftPlanFwd, (cufftReal *)d_Kernel, (cufftComplex *)d_KernelSpectrum) );

    printf("...running GPU FFT convolution: ");
        cutilSafeCall( cudaThreadSynchronize() );
        cutilCheckError( cutResetTimer(hTimer) );
        cutilCheckError( cutStartTimer(hTimer) );
//        cufftSafeCall( cufftExecR2C(fftPlanFwd, (cufftReal *)d_PaddedData, (cufftComplex *)d_DataSpectrum) );
        cufftSafeCall( cufftExecR2C(fftPlanFwd, (cufftReal *)d_Data, (cufftComplex *)d_DataSpectrum) );
        modulateAndNormalize(d_DataSpectrum, d_KernelSpectrum, fftH, fftW, 1);
        cufftSafeCall( cufftExecC2R(fftPlanInv, (cufftComplex *)d_DataSpectrum, (cufftReal *)d_PaddedData) );

        cutilSafeCall( cudaThreadSynchronize() );
        cutilCheckError( cutStopTimer(hTimer) );
        double gpuTime = cutGetTimerValue(hTimer);
    printf("%f MPix/s (%f ms)\n", (double)dataH * (double)dataW * 1e-6 / (gpuTime * 0.001), gpuTime);

    printf("...reading back GPU convolution results\n");
        cutilSafeCall( cudaMemcpy(h_ResultGPU, d_PaddedData, fftH * fftW * sizeof(float), cudaMemcpyDeviceToHost) );

	printf("Convert result to shorts\n");
      for(int i = 0; i < dataH * dataW; i++)
            grabbermem[i] = (unsigned short)(h_ResultGPU[i]);


	  printf("Write to tiff\n");
	  tiff.tifWr("D:/fftimg.tif",grabbermem,dataH,dataW);

		if (0)
		{
			printf("...running reference CPU convolution\n");
				convolutionClampToBorderCPU(
					h_ResultCPU,
					h_Data,
					h_Kernel,
					dataH,
					dataW,
					kernelH,
					kernelW,
					kernelY,
					kernelX
				);

			printf("...comparing the results: ");
				double sum_delta2 = 0;
				double sum_ref2   = 0;
				double max_delta_ref = 0;
				for(int y = 0; y < dataH; y++)
					for(int x = 0; x < dataW; x++){
						double  rCPU = (double)h_ResultCPU[y * dataW + x];
						double  rGPU = (double)h_ResultGPU[y * fftW  + x];
						double delta = (rCPU - rGPU) * (rCPU - rGPU);
						double   ref = rCPU * rCPU + rCPU * rCPU;
						if((delta / ref) > max_delta_ref) max_delta_ref = delta / ref;
						sum_delta2 += delta;
						sum_ref2   += ref;
					}
				double L2norm = sqrt(sum_delta2 / sum_ref2);
				printf("rel L2 = %E (max delta = %E)\n", L2norm, sqrt(max_delta_ref));
				printf((L2norm < 1e-6) ? "TEST PASSED\n" : "TEST FAILED\n");
		}
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

        free(h_ResultGPU);
        free(h_ResultCPU);
        free(h_Data);
        free(h_Kernel);
}

void test1(void){
    float
        *h_Data,
        *h_Kernel,
        *h_ResultCPU,
        *h_ResultGPU;

    float
        *d_Data,
        *d_Kernel,
        *d_PaddedData,
        *d_PaddedKernel;

    fComplex
        *d_DataSpectrum0,
        *d_KernelSpectrum0,
        *d_DataSpectrum,
        *d_KernelSpectrum;

    cufftHandle fftPlan;

    unsigned int hTimer;
    cutilCheckError( cutCreateTimer(&hTimer) );

    printf("Testing custom R2C / C2R FFT-based convolution\n");
        const uint fftPadding = 16;
        const int kernelH = 7;
        const int kernelW = 6;
        const int kernelY = 3;
        const int kernelX = 4;
        const int   dataH = 2000;
        const int   dataW = 2000;
        const int    fftH = snapTransformSize(dataH + kernelH - 1);
        const int    fftW = snapTransformSize(dataW + kernelW - 1);

    printf("...allocating memory\n");
        h_Data      = (float *)malloc(dataH   * dataW * sizeof(float));
        h_Kernel    = (float *)malloc(kernelH * kernelW * sizeof(float));
        h_ResultCPU = (float *)malloc(dataH   * dataW * sizeof(float));
        h_ResultGPU = (float *)malloc(fftH    * fftW * sizeof(float));

        cutilSafeCall( cudaMalloc((void **)&d_Data,   dataH   * dataW   * sizeof(float)) );
        cutilSafeCall( cudaMalloc((void **)&d_Kernel, kernelH * kernelW * sizeof(float)) );

        cutilSafeCall( cudaMalloc((void **)&d_PaddedData,   fftH * fftW * sizeof(float)) );
        cutilSafeCall( cudaMalloc((void **)&d_PaddedKernel, fftH * fftW * sizeof(float)) );

        cutilSafeCall( cudaMalloc((void **)&d_DataSpectrum0,   fftH * (fftW / 2) * sizeof(fComplex)) );
        cutilSafeCall( cudaMalloc((void **)&d_KernelSpectrum0, fftH * (fftW / 2) * sizeof(fComplex)) );
        cutilSafeCall( cudaMalloc((void **)&d_DataSpectrum,    fftH * (fftW / 2 + fftPadding) * sizeof(fComplex)) );
        cutilSafeCall( cudaMalloc((void **)&d_KernelSpectrum,  fftH * (fftW / 2 + fftPadding) * sizeof(fComplex)) );

    printf("...generating random input data\n");
        srand(2010);
        for(int i = 0; i < dataH * dataW; i++)
            h_Data[i] = getRand();
        for(int i = 0; i < kernelH * kernelW; i++)
            h_Kernel[i] = getRand();

    printf("...creating C2C FFT plan for %i x %i\n", fftH, fftW / 2);
        cufftSafeCall( cufftPlan2d(&fftPlan, fftH, fftW / 2, CUFFT_C2C) );

    printf("...uploading to GPU and padding convolution kernel and input data\n");
        cutilSafeCall( cudaMemcpy(d_Data,   h_Data,   dataH   * dataW *   sizeof(float), cudaMemcpyHostToDevice) );
        cutilSafeCall( cudaMemcpy(d_Kernel, h_Kernel, kernelH * kernelW * sizeof(float), cudaMemcpyHostToDevice) );
        cutilSafeCall( cudaMemset(d_PaddedData,   0, fftH * fftW * sizeof(float)) );
        cutilSafeCall( cudaMemset(d_PaddedKernel, 0, fftH * fftW * sizeof(float)) );

        padDataClampToBorder(
            d_PaddedData,
            d_Data,
            fftH,
            fftW,
            dataH,
            dataW,
            kernelH,
            kernelW,
            kernelY,
            kernelX
        );

        padKernel(
            d_PaddedKernel,
            d_Kernel,
            fftH,
            fftW,
            kernelH,
            kernelW,
            kernelY,
            kernelX
        );

    //CUFFT_INVERSE works just as well...
    const int FFT_DIR = CUFFT_FORWARD;

    //Not including kernel transformation into time measurement,
    //since convolution kernel is not changed very frequently
    printf("...transforming convolution kernel\n");
        cufftSafeCall( cufftExecC2C(fftPlan, (cufftComplex *)d_PaddedKernel, (cufftComplex *)d_KernelSpectrum0, FFT_DIR) );
        spPostprocess2D(d_KernelSpectrum, d_KernelSpectrum0, fftH, fftW / 2, fftPadding, FFT_DIR);

    printf("...running GPU FFT convolution: ");
        cutilSafeCall( cudaThreadSynchronize() );
        cutilCheckError( cutResetTimer(hTimer) );
        cutilCheckError( cutStartTimer(hTimer) );

        cufftSafeCall( cufftExecC2C(fftPlan, (cufftComplex *)d_PaddedData, (cufftComplex *)d_DataSpectrum0, FFT_DIR) );

        spPostprocess2D(d_DataSpectrum, d_DataSpectrum0, fftH, fftW / 2, fftPadding, FFT_DIR);
        modulateAndNormalize(d_DataSpectrum, d_KernelSpectrum, fftH, fftW, fftPadding);
        spPreprocess2D(d_DataSpectrum0, d_DataSpectrum, fftH, fftW / 2, fftPadding, -FFT_DIR);

        cufftSafeCall( cufftExecC2C(fftPlan, (cufftComplex *)d_DataSpectrum0, (cufftComplex *)d_PaddedData, -FFT_DIR) );

        cutilSafeCall( cudaThreadSynchronize() );
        cutilCheckError( cutStopTimer(hTimer) );
        double gpuTime = cutGetTimerValue(hTimer);
    printf("%f MPix/s (%f ms)\n", (double)dataH * (double)dataW * 1e-6 / (gpuTime * 0.001), gpuTime);

    printf("...reading back GPU FFT results\n");
        cutilSafeCall( cudaMemcpy(h_ResultGPU, d_PaddedData, fftH * fftW * sizeof(float), cudaMemcpyDeviceToHost) );

    printf("...running reference CPU convolution\n");
        convolutionClampToBorderCPU(
            h_ResultCPU,
            h_Data,
            h_Kernel,
            dataH,
            dataW,
            kernelH,
            kernelW,
            kernelY,
            kernelX
        );

    printf("...comparing the results: ");
        double sum_delta2 = 0;
        double sum_ref2   = 0;
        double max_delta_ref = 0;
        for(int y = 0; y < dataH; y++)
            for(int x = 0; x < dataW; x++){
                double  rCPU = (double)h_ResultCPU[y * dataW + x];
                double  rGPU = (double)h_ResultGPU[y * fftW  + x];
                double delta = (rCPU - rGPU) * (rCPU - rGPU);
                double   ref = rCPU * rCPU + rCPU * rCPU;
                if((delta / ref) > max_delta_ref) max_delta_ref = delta / ref;
                sum_delta2 += delta;
                sum_ref2   += ref;
            }
        double L2norm = sqrt(sum_delta2 / sum_ref2);
        printf("rel L2 = %E (max delta = %E)\n", L2norm, sqrt(max_delta_ref));
        printf((L2norm < 1e-6) ? "TEST PASSED\n" : "TEST FAILED\n");

    printf("...shutting down\n");
        cutilCheckError( cutDeleteTimer(hTimer) );
        cufftSafeCall( cufftDestroy(fftPlan) );

        cutilSafeCall( cudaFree(d_KernelSpectrum) );
        cutilSafeCall( cudaFree(d_DataSpectrum)   );
        cutilSafeCall( cudaFree(d_KernelSpectrum0) );
        cutilSafeCall( cudaFree(d_DataSpectrum0)   );
        cutilSafeCall( cudaFree(d_PaddedKernel) );
        cutilSafeCall( cudaFree(d_PaddedData)   );
        cutilSafeCall( cudaFree(d_Kernel) );
        cutilSafeCall( cudaFree(d_Data)   );

        free(h_ResultGPU);
        free(h_ResultCPU);
        free(h_Kernel);
        free(h_Data);
}

void test2(void){
    float
        *h_Data,
        *h_Kernel,
        *h_ResultCPU,
        *h_ResultGPU;

    float
        *d_Data,
        *d_Kernel,
        *d_PaddedData,
        *d_PaddedKernel;

    fComplex
        *d_DataSpectrum0,
        *d_KernelSpectrum0;

    cufftHandle
        fftPlan;

    unsigned int hTimer;
    cutilCheckError( cutCreateTimer(&hTimer) );

    printf("Testing updated custom R2C / C2R FFT-based convolution\n");
        const int kernelH = 7;
        const int kernelW = 6;
        const int kernelY = 3;
        const int kernelX = 4;
        const int dataH = 2000;
        const int dataW = 2000;
        const int fftH = snapTransformSize(dataH + kernelH - 1);
        const int fftW = snapTransformSize(dataW + kernelW - 1);

    printf("...allocating memory\n");
        h_Data      = (float *)malloc(dataH   * dataW * sizeof(float));
        h_Kernel    = (float *)malloc(kernelH * kernelW * sizeof(float));
        h_ResultCPU = (float *)malloc(dataH   * dataW * sizeof(float));
        h_ResultGPU = (float *)malloc(fftH    * fftW * sizeof(float));

        cutilSafeCall( cudaMalloc((void **)&d_Data,   dataH   * dataW   * sizeof(float)) );
        cutilSafeCall( cudaMalloc((void **)&d_Kernel, kernelH * kernelW * sizeof(float)) );

        cutilSafeCall( cudaMalloc((void **)&d_PaddedData,   fftH * fftW * sizeof(float)) );
        cutilSafeCall( cudaMalloc((void **)&d_PaddedKernel, fftH * fftW * sizeof(float)) );

        cutilSafeCall( cudaMalloc((void **)&d_DataSpectrum0,   fftH * (fftW / 2) * sizeof(fComplex)) );
        cutilSafeCall( cudaMalloc((void **)&d_KernelSpectrum0, fftH * (fftW / 2) * sizeof(fComplex)) );

    printf("...generating random input data\n");
        srand(2010);
        for(int i = 0; i < dataH * dataW; i++)
            h_Data[i] = getRand();
        for(int i = 0; i < kernelH * kernelW; i++)
            h_Kernel[i] = getRand();

    printf("...creating C2C FFT plan for %i x %i\n", fftH, fftW / 2);
        cufftSafeCall( cufftPlan2d(&fftPlan, fftH, fftW / 2, CUFFT_C2C) );

    printf("...uploading to GPU and padding convolution kernel and input data\n");
        cutilSafeCall( cudaMemcpy(d_Data,   h_Data,   dataH   * dataW *   sizeof(float), cudaMemcpyHostToDevice) );
        cutilSafeCall( cudaMemcpy(d_Kernel, h_Kernel, kernelH * kernelW * sizeof(float), cudaMemcpyHostToDevice) );
        cutilSafeCall( cudaMemset(d_PaddedData,   0, fftH * fftW * sizeof(float)) );
        cutilSafeCall( cudaMemset(d_PaddedKernel, 0, fftH * fftW * sizeof(float)) );

        padDataClampToBorder(
            d_PaddedData,
            d_Data,
            fftH,
            fftW,
            dataH,
            dataW,
            kernelH,
            kernelW,
            kernelY,
            kernelX
        );

        padKernel(
            d_PaddedKernel,
            d_Kernel,
            fftH,
            fftW,
            kernelH,
            kernelW,
            kernelY,
            kernelX
        );

    //CUFFT_INVERSE works just as well...
    const int FFT_DIR = CUFFT_FORWARD;

    //Not including kernel transformation into time measurement,
    //since convolution kernel is not changed very frequently
    printf("...transforming convolution kernel\n");
        cufftSafeCall( cufftExecC2C(fftPlan, (cufftComplex *)d_PaddedKernel, (cufftComplex *)d_KernelSpectrum0, FFT_DIR) );

    printf("...running GPU FFT convolution: ");
        cutilSafeCall( cudaThreadSynchronize() );
        cutilCheckError( cutResetTimer(hTimer) );
        cutilCheckError( cutStartTimer(hTimer) );

        cufftSafeCall( cufftExecC2C(fftPlan, (cufftComplex *)d_PaddedData, (cufftComplex *)d_DataSpectrum0, FFT_DIR) );
        spProcess2D(d_DataSpectrum0, d_DataSpectrum0, d_KernelSpectrum0, fftH, fftW / 2, FFT_DIR);
        cufftSafeCall( cufftExecC2C(fftPlan, (cufftComplex *)d_DataSpectrum0, (cufftComplex *)d_PaddedData, -FFT_DIR) );

        cutilSafeCall( cudaThreadSynchronize() );
        cutilCheckError( cutStopTimer(hTimer) );
        double gpuTime = cutGetTimerValue(hTimer);
    printf("%f MPix/s (%f ms)\n", (double)dataH * (double)dataW * 1e-6 / (gpuTime * 0.001), gpuTime);

    printf("...reading back GPU FFT results\n");
        cutilSafeCall( cudaMemcpy(h_ResultGPU, d_PaddedData, fftH * fftW * sizeof(float), cudaMemcpyDeviceToHost) );

    printf("...running reference CPU convolution\n");
        convolutionClampToBorderCPU(
            h_ResultCPU,
            h_Data,
            h_Kernel,
            dataH,
            dataW,
            kernelH,
            kernelW,
            kernelY,
            kernelX
        );

    printf("...comparing the results: ");
        double sum_delta2 = 0;
        double sum_ref2   = 0;
        double max_delta_ref = 0;
        for(int y = 0; y < dataH; y++)
            for(int x = 0; x < dataW; x++){
                double  rCPU = (double)h_ResultCPU[y * dataW + x];
                double  rGPU = (double)h_ResultGPU[y * fftW  + x];
                double delta = (rCPU - rGPU) * (rCPU - rGPU);
                double   ref = rCPU * rCPU + rCPU * rCPU;
                if((delta / ref) > max_delta_ref) max_delta_ref = delta / ref;
                sum_delta2 += delta;
                sum_ref2   += ref;
            }
        double L2norm = sqrt(sum_delta2 / sum_ref2);
        printf("rel L2 = %E (max delta = %E)\n", L2norm, sqrt(max_delta_ref));
        printf((L2norm < 1e-6) ? "TEST PASSED\n" : "TEST FAILED\n");

    printf("...shutting down\n");
        cutilCheckError( cutDeleteTimer(hTimer) );
        cufftSafeCall( cufftDestroy(fftPlan) );

        cutilSafeCall( cudaFree(d_KernelSpectrum0) );
        cutilSafeCall( cudaFree(d_DataSpectrum0) );
        cutilSafeCall( cudaFree(d_PaddedKernel) );
        cutilSafeCall( cudaFree(d_PaddedData)   );
        cutilSafeCall( cudaFree(d_Kernel) );
        cutilSafeCall( cudaFree(d_Data)   );

        free(h_ResultGPU);
        free(h_ResultCPU);
        free(h_Kernel);
        free(h_Data);
}



int main(int argc, char **argv){
    //Use command-line specified CUDA device, otherwise use device with highest Gflops/s
    if(cutCheckCmdLineFlag(argc, (const char **)argv, "device"))
        cutilDeviceInit(argc, argv);
    else
        cudaSetDevice(cutGetMaxGflopsDeviceId());

    test0();
//    test1();
 //   test2();

    cudaThreadExit();
    cutilExit(argc, argv);
}
