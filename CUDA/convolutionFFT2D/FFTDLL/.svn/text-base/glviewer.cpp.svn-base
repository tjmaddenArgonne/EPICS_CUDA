#include "glviewer.h"
#include "convolutionFFT2D_common.h"
namespace glviewer_stuff {

volatile int imageW=1024;
volatile int  imageH=1024;

GLuint gl_PBO, gl_Tex;

uchar4 *h_Src;

unsigned int* h_img = NULL;
 
int lastxsize, lastysize;
int maxsize = 4096;

float	pic_map_x = 1.0;
float	pic_map_y = 1.0;

unsigned int vtimer;

unsigned int *d_colorimg;

volatile int update_count=0;
volatile int update_count_last=0;

 unsigned short *d_srcimg;
 
 volatile void *h_srcimgptr;
unsigned int *d_result;

float bright_gain=1.0;
float bright_offset=0.0;


 void *h_buffer;

int initGL(  )
{
    printf("Initializing GLUT...\n");

	static int argc;
	static char argstr[128]="simDetectorApp.exe";
	char *argv[10];

	argc=1;
	argv[0]=argstr;

	cutilCheckError(cutCreateTimer(&vtimer));

    glutInit(&argc, &argv[0]);
   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
   glutInitWindowSize(512, 512);
    glutInitWindowPosition(128,128);
    glutCreateWindow(argv[0]);
    printf("OpenGL window created.\n");
	    glutIdleFunc(idle);
    glutDisplayFunc(displayFunc);
    glutKeyboardFunc(shutDown);
	glutMotionFunc(mouseFunc);
    glewInit();
    

    return 0;
}

void resizeTexture(void)
{
	glDeleteTextures(1, &gl_Tex);
 // create texture for display
    glGenTextures(1, &gl_Tex);
    glBindTexture(GL_TEXTURE_2D, gl_Tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imageW, imageH, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

}

void initOpenGLBuffers()
{
	h_buffer=malloc(maxsize*maxsize   * sizeof(short));
	
	cutilSafeCall( cudaMalloc((void **)&d_srcimg,   maxsize*maxsize   * sizeof(short)) );
 // create pixel buffer object to store final image
    glGenBuffersARB(1, &gl_PBO);
    glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, gl_PBO);
    glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, maxsize*maxsize*sizeof(int), NULL, GL_STREAM_DRAW_ARB);

    glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
    cutilSafeCall(cudaGLRegisterBufferObject(gl_PBO));

    // create texture for display
    glGenTextures(1, &gl_Tex);
    glBindTexture(GL_TEXTURE_2D, gl_Tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imageW, imageH, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

	h_srcimgptr=0;

	lastxsize=imageW;
	lastysize=imageH;


}

void mouseFunc(int x, int y)
{

	float wx,wy;
	float xn,yn;
	float xf,yf;

	xf=(float)x;
	yf=(float)y;

	wx=(float)glutGet(GLUT_WINDOW_WIDTH);
	wy=(float)glutGet(GLUT_WINDOW_HEIGHT);

	xn=xf/wx;
	yn=yf/wy;

	xn=xn*2 -1.0;
	yn=yn*2 - 1.0;
	
	pic_map_x = xn;
	pic_map_y = yn;


}


void displayFunc(void){

  //  cutilCheckError(cutStartTimer(timer));  

    // execute filter, writing results to pbo
    
	
	if (imageW != lastxsize  || imageH != lastysize)
		resizeTexture();

	lastxsize=imageW;
	lastysize=imageH;

    cutilSafeCall(cudaGLMapBufferObject((void**)&d_result, gl_PBO));

	
	if (update_count>update_count_last)
	{

		cutilCheckError(cutStopTimer(vtimer));
		char fps[256];
        float ifps = 1.f / (cutGetAverageTimerValue(vtimer) / 1000.f);
      //  float ifps = 1.f / (cutGetTimerValue(vtimer) / 1000.f);

sprintf(fps, "Area Detector GPU Callback Rate %3.1f fps", ifps);  
		cutilCheckError(cutResetTimer(vtimer));  
			 cutilCheckError(cutStartTimer(vtimer));  
		glutSetWindowTitle(fps);

	if (h_srcimgptr!=0)
	{
		
		cutilSafeCall( cudaMemcpy(d_srcimg,  (const void*) h_srcimgptr,   imageH   * imageW *   sizeof(short), cudaMemcpyHostToDevice) );
	 shortImg2rgbImage(
		(int*)d_result,
		(short*)d_srcimg,
		imageH,
		imageW,
		bright_gain,
		bright_offset
		);
	}
   update_count = update_count_last;
	}

	cutilSafeCall(cudaGLUnmapBufferObject(gl_PBO));

    // load texture from pbo
    glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, gl_PBO);
    glBindTexture(GL_TEXTURE_2D, gl_Tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, imageW, imageH, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);

    // display results
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex2f(-pic_map_x, -pic_map_y);
    glTexCoord2f(1, 1); glVertex2f(pic_map_x, -pic_map_y);
    glTexCoord2f(1, 0); glVertex2f(pic_map_x, pic_map_y);
    glTexCoord2f(0, 0); glVertex2f(-pic_map_x, pic_map_y);
    glEnd();
	//glFlush();

    glDisable(GL_TEXTURE_2D);


    glutSwapBuffers();
	//glutPostRedisplay();

	//Sleep(100);

   // cutilCheckError(cutStopTimer(timer));  

  //  computeFPS();
}



void idle()
{
    glutPostRedisplay();
}


/*

void displayFunc(void){
	//cutStartTimer(hTimer);
    TColor *d_dst = NULL;
	size_t num_bytes;

   // if(frameCounter++ == 0) cutResetTimer(hTimer);
    // DEPRECATED: cutilSafeCall(cudaGLMapBufferObject((void**)&d_dst, gl_PBO));
    cutilSafeCall(cudaGraphicsMapResources(1, &cuda_pbo_resource, 0));
	cutilCheckMsg("cudaGraphicsMapResources failed");
    cutilSafeCall(cudaGraphicsResourceGetMappedPointer((void**)&d_dst, &num_bytes, cuda_pbo_resource));
	cutilCheckMsg("cudaGraphicsResourceGetMappedPointer failed");

    cutilSafeCall( CUDA_Bind2TextureArray()                      );

    runImageFilters(d_dst);

    cutilSafeCall( CUDA_UnbindTexture()     );
    // DEPRECATED: cutilSafeCall(cudaGLUnmapBufferObject(gl_PBO));
	cutilSafeCall(cudaGraphicsUnmapResources(1, &cuda_pbo_resource, 0));

	if (g_bFBODisplay) {
		g_FrameBufferObject->bindRenderPath();
	}

    // Common display code path
	{
        glClear(GL_COLOR_BUFFER_BIT);

        glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, imageW, imageH, GL_RGBA, GL_UNSIGNED_BYTE, BUFFER_DATA(0) );
        glBegin(GL_TRIANGLES);
            glTexCoord2f(0, 0); glVertex2f(-1, -1);
            glTexCoord2f(2, 0); glVertex2f(+3, -1);
            glTexCoord2f(0, 2); glVertex2f(-1, +3);
        glEnd();
        glFinish();
    }

	if (g_bFBODisplay) {
		g_FrameBufferObject->unbindRenderPath();
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    if (g_CheckRender && g_CheckRender->IsQAReadback() && g_Verify) {
        printf("> (Frame %d) readback BackBuffer\n", frameCount);
        if (g_bFBODisplay) {
            g_CheckRender->readback( imageW, imageH, g_FrameBufferObject->getFbo() );
        } else {
            g_CheckRender->readback( imageW, imageH );
        }
        g_CheckRender->savePPM ( sOriginal[g_Kernel], true, NULL );
        if (!g_CheckRender->PPMvsPPM(sOriginal[g_Kernel], sReference[g_Kernel], MAX_EPSILON_ERROR, 0.15f)) {
            g_TotalErrors++;
        }
        g_Verify = false;
    }

    if(frameCounter == frameN){
        frameCounter = 0;
        if(g_FPS){
            printf("FPS: %3.1f\n", frameN / (cutGetTimerValue(hTimer) * 0.001) );
            g_FPS = false;
        }
    }

	glutSwapBuffers();

	cutStopTimer(hTimer);
	computeFPS();

	glutPostRedisplay();
}

*/




void shutDown(unsigned char k, int , int )
{
    switch (k){
        case '\033':
        case 'q':
        case 'Q':
          
        break;

        case '1':
			bright_gain+=1.0;
       
        break;

        case '2':
			bright_gain=bright_gain-1.0;
           
        break;

        case '3':
			bright_offset+=3000.0;
          
        break;

        case '4':
            bright_offset=bright_offset-3000.0;
        break;

        case ' ':
            
        break;

        case 'n':
           
        break;

        case 'N':
           
        break;

        case 'l':
            
        break;

        case 'L':
            
        break;

        case 'f' : case 'F':
          
        break;

        case '?':
            
        break;
    }
}




};
