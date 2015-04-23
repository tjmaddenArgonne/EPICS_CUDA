
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>

#include <GL/glut.h>

#include <cuda_runtime.h>
#include <cutil_inline.h>
#include <cutil_gl_inline.h>
//#include <cutil_gl_error.h>
#include <cuda_gl_interop.h>

namespace glviewer_stuff
{


int initGL( void );

void idle();

void initOpenGLBuffers();

void displayFunc(void);

void mouseFunc(int x, int y);

void resizeTexture(void);


void shutDown(unsigned char k, int /*x*/, int /*y*/);

extern volatile int imageW, imageH;
extern GLuint gl_PBO, gl_Tex;
extern uchar4 *h_Src;


extern unsigned int* h_img;

extern unsigned int *d_result;

extern unsigned int *d_colorimg;
extern volatile int update_count;
extern volatile int update_count_last;

extern  volatile void *h_srcimgptr;

extern   void *h_buffer;

extern  unsigned short *d_srcimg;
};
