//#include "ehci.h"
//#include <stdio.h>
//#include "windows.h"
//
//
//int main(){
//                
//        double glPositMatrix[16];
//        int initialGuess=1;
//        int frameCount=0;
//        
//        //this function creates a window to see debug information
//        //about capturing image and face detection
//        ehciInit();
//		Sleep(500);
//        while(1){
//                //ehciLoop is the main function of the library
//                //it captures a frame and processes it
//                //the EHCI6DFACEDETECT parameter tells ehci to 
//                //run the 6 degrees of freedom algorithm to detect
//                //head translation and rotation
//                //the initialGuess parameter tells ehci to
//                //initialize, when it is set to zero 
//                //and tells ehci to track when it is set to one
//                
//                ehciLoop(EHCI6DFACEDETECT,initialGuess);
//                
//                //returns translation and rotation matrix in openGl format
//                getGlPositMatrix(glPositMatrix);
//                
//                //the dimensions from 12 up to 14 store translation data
//                //depending on the calibration, measures will be roughly proportional
//                //to milimeters. Notice that the center of the camera has coordinates (0,0,0)
//                double headX = glPositMatrix[12];
//                double headY = glPositMatrix[13];
//                double headZ = glPositMatrix[14];
//                
//                printf("Head Coordinates X: %+7.3lf Y: %+7.3lf Z:%+7.3lf ",
//                                headX,headY,headZ);
//                
//                //after 30 frames we will stop the initialization algorithm
//                //and start the tracking algorithm
//                
//                frameCount++;
//                if(frameCount >= 30) initialGuess=0;
//                else printf("[initializing]");
//                
//                printf(" (Press Ctrl+C to finish)\n");
//				SetCursorPos(headX, headY);
//        }
//}
//


#include "cv.h"
#include "highgui.h"
#include "ehci.h"
#include <stdio.h>
#include <ctype.h>
#include <vector>
#include "Mouse.h"
Mouse mouse;
//..
//opengl related includes
#include <glut.h>    // Header File For The GLUT Library
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library
#define NOMINMAX
#include <Windows.h>


GLuint  keyframeTexture;

/* white ambient light at half intensity (rgba) */
GLfloat LightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

/* super bright, full intensity diffuse light. */
GLfloat LightDiffuse[] = { 0.8f, 0.8f, 0.8, 1.0f };

GLfloat LightSpecular[] = { 0.5f, 0.5f, 0.5f, 1.0f };

/* position of light (x, y, z, (position of light)) */
GLfloat LightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f };

//openGl texture to store grabbed frame
GLuint  frameTexture;


/* The number of our GLUT window */
int window;

double projectionMatrix[16];


int initialGuess = 1;
int drawSine = 0, drawHead = 0, drawAxis = 1, drawNormals = 0, blending = 1, drawLight = 1, drawBackground = 1, moveMouse;
int aLastHeadW, aLastHeadH;
int headRefX, headRefY;

struct triangle{
	float vert[3][3];
};

triangle triangles[3036];

void checkTransparency();
void drawGrabbedFrame();

void drawHelpText(){
	int* font = (int*)GLUT_BITMAP_HELVETICA_12;
	//glClear(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);


	int XSize = 640, YSize = 480;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, XSize, YSize, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	glColor3f(255.0f, 255.0f, 255.0f);
	glBegin(GL_QUADS);
	glVertex2f(10, 10);
	glVertex2f(290, 10);
	glVertex2f(290, 40);
	glVertex2f(10, 40);
	glEnd();


	glColor3f(0.0f, 0.0f, 0.0f);

	glRasterPos2f(10, 20);
	const unsigned char text[100] = "Press \'I\' to reinitialize. You must focus this window\n(face the camera in a still position)";
	for (int i = 0; i < 100; i++)
	{
		glutBitmapCharacter(font, text[i]);
	}


	if (!drawLight) {
		glDisable(GL_LIGHTING);
	}
	else {
		glEnable(GL_LIGHTING);
	}

	glEnable(GL_DEPTH_TEST);




}

void drawCentralAxis(){

	float deltaX, deltaY, deltaZ;
	
	getReferenceCoordinate(&deltaX, &deltaY, &deltaZ);
	
	
	float cx, cy, cz;
	cx = .5 - deltaX;
	cy = -.25 - deltaY;
	cz = 1.0 - deltaZ;


	//std::cout << "Z " << cz << std::endl;
	//Sleep(100);
	//std::cout <<"               Y " << cy << std::endl;
	//Sleep(100);
	//std::cout << "                                X " << cx << std::endl;

	glLineWidth(4.0);
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0);
	glVertex3f(cx, cy, cz);
	glVertex3f(cx + 1000, cy, cz);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.9f, 0.3f, 0.0);
	glVertex3f(cx, cy, cz);
	glVertex3f(cx - 1000, cy, cz);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0);
	glVertex3f(cx, cy, cz);
	glVertex3f(cx, cy + 1000, cz);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.3f, 0.8f, 0.0);
	glVertex3f(cx, cy, cz);
	glVertex3f(cx, cy - 1000, cz);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0);
	glVertex3f(cx, cy, cz);
	glVertex3f(cx, cy, cz + 5.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.5f);
	glVertex3f(cx, cy, cz);
	glVertex3f(cx, cy, cz - 5.0f);
	glEnd();

	glLineWidth(1.0);
	
	
}
//void drawReferenceAxis(){
//
//	glBegin(GL_LINES);
//	glColor3f(1.0f, 0.0f, 0.0);
//	glVertex3f(0.0f, 0.0f, 0.0f);
//	glVertex3f(1000.0f, 0.0f, 0.0f);
//	glEnd();
//	glBegin(GL_LINES);
//	glColor3f(0.9f, 0.3f, 0.0);
//	glVertex3f(0.0f, 0.0f, 0.0f);
//	glVertex3f(-1000.0f, 0.0f, 0.0f);
//	glEnd();
//	/*
//	double modelview[16], projection[16];
//	int viewport[4];
//	float z;
//	//get the projection matrix
//	glGetDoublev( GL_PROJECTION_MATRIX, projection );
//	//get the modelview matrix
//	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
//	//get the viewport
//	glGetIntegerv( GL_VIEWPORT, viewport );
//
//	printf("Projection Matrix:\n");
//	for(int i=0;i<4;i++){
//	for(int j=0;j<4;j++){
//	printf("%lf ",projection[i*4+j]);
//	}
//	printf("\n");
//	}
//	printf("\n");
//
//	printf("Modelview Matrix:\n");
//	for(int i=0;i<4;i++){
//	for(int j=0;j<4;j++){
//	printf("%lf ",modelview[i*4+j]);
//	}
//	printf("\n");
//	}
//	printf("\n");
//
//	printf("Viewport Matrix:\n");
//	for(int i=0;i<4;i++){
//	printf("%d ",viewport[i]);
//	}
//	printf("\n");*/
//
//
//	glBegin(GL_LINES);
//	glColor3f(0.0f, 1.0f, 0.0);
//	glVertex3f(0.0f, 0.0f, 0.0f);
//	glVertex3f(0.0f, 1000.0f, 0.0f);
//	glEnd();
//
//	glBegin(GL_LINES);
//	glColor3f(0.3f, 0.8f, 0.0);
//	glVertex3f(0.0f, 0.0f, 0.0f);
//	glVertex3f(0.0f, -1000.0f, 0.0f);
//	glEnd();
//
//	/*      glBegin(GL_LINES);
//	glColor3f(0.0f,1.0f,0.0);
//	glVertex3f(100.0f, 0.0f,0.0f);
//	glVertex3f(100.0f, 1000.0f,0.0f);
//	glEnd();*/
//
//	glBegin(GL_LINES);
//	glColor3f(0.0f, 0.0f, 1.0);
//	glVertex3f(0.0f, 0.0f, 0.0f);
//	glVertex3f(0.0f, 0.0f, 1000.0f);
//	glEnd();
//
//	glBegin(GL_LINES);
//	glColor3f(0.0f, 0.0f, 0.5f);
//	glVertex3f(0.0f, 0.0f, 0.0f);
//	glVertex3f(0.0f, 0.0f, -1000.0f);
//	glEnd();
//
//}

void loadRaw(char* file){

	float p1[3], p2[3], p3[3];
	FILE* in = fopen("B:\\ehci\\data\\head.raw", "r");
	if (!in){
		printf("Cannot find head.raw file with head vertices. This file should be in a subdirectory data in the directory from which the application is being run.\n");
		exit(EXIT_FAILURE);
	}

	//fscanf(in,"%s",&temp);
	//fscanf(in,"%f",&p1[0]);
	//      printf("Temp %s %f\n",temp,p1[0]);
	//      return;
	int tcount = 0;
	float mScale = 1.0;
	//float deltaX=1.874,deltaY=-1.999,deltaZ=-2.643;
	float deltaX = 0.0, deltaY = 0.0, deltaZ = 0.0;

	while (fscanf(in, "%f%f%f%f%f%f%f%f%f", &p1[0], &p1[1], &p1[2], &p2[0], &p2[1], &p2[2], &p3[0], &p3[1], &p3[2]) != EOF){
		triangles[tcount].vert[0][0] = (p1[0] + deltaX)*mScale;
		triangles[tcount].vert[0][1] = (p1[1] + deltaY)*mScale;
		triangles[tcount].vert[0][2] = (p1[2] + deltaZ)*mScale;

		triangles[tcount].vert[1][0] = (p2[0] + deltaX)*mScale;
		triangles[tcount].vert[1][1] = (p2[1] + deltaY)*mScale;
		triangles[tcount].vert[1][2] = (p2[2] + deltaZ)*mScale;

		triangles[tcount].vert[2][0] = (p3[0] + deltaX)*mScale;
		triangles[tcount].vert[2][1] = (p3[1] + deltaY)*mScale;
		triangles[tcount].vert[2][2] = (p3[2] + deltaZ)*mScale;

		tcount++;
		//printf("Tcount %d\n",tcount);
	}
	fclose(in);
}

void drawSinusoidalHead(float scale, int headWidth, int headHeight, int myRefX, int myRefY){
	//TODO: FIND OUT WHY Y IS UPSIDE DOWN

	float deltaX, deltaY, deltaZ;
	getReferenceCoordinate(&deltaX, &deltaY, &deltaZ);

	//this code will map everything from the upper head corner down to the bottom right corner
	/*glBegin(GL_LINES);
	glColor3f(0.0f,0.0f,0.5f);
	glVertex3f(-0.332 - deltaX, +0.332 - deltaY,-0.8663 -deltaZ);
	glVertex3f(-0.332 - deltaX, -1.332 - deltaY,-0.8663 -deltaZ);
	glEnd();

	for(double x=0;x<1.6;x+=0.111){
	glBegin(GL_LINES);

	glVertex3f(-0.332 +x - deltaX, +0.332 - deltaY,sin((-0.332 +x)*3.141593) -deltaZ);
	glVertex3f(-0.332 +x - deltaX, -1.332 - deltaY,sin((-0.332 +x)*3.141593) -deltaZ);
	glEnd();
	}

	glBegin(GL_LINES);
	glVertex3f( 1.332 - deltaX, +0.332 - deltaY,-0.8663 -deltaZ);
	glVertex3f( 1.332 - deltaX, -1.332 - deltaY,-0.8663 -deltaZ);
	glEnd();*/

	glBindTexture(GL_TEXTURE_2D, keyframeTexture);

	//printf("GlRef %f %f %f\n",deltaX,deltaY,deltaZ);

	//this code will map only the texture detail

	double xIncrement = 0.01;
	for (double x = 0; x <= 1.0 - xIncrement; x += xIncrement){
		glBegin(GL_QUADS);
		//glBegin(GL_LINES);
		glColor3f(1.0f, 1.0f, 1.0f);

		glTexCoord2f(x, 0);
		glVertex3f(0 + x - deltaX, +0.0 - deltaY, .5*sin((+x)*3.141593) - deltaZ);//0-deltaZ);//sin((+x)*3.141593) -deltaZ);

		glTexCoord2f(x, 1.0);
		glVertex3f(0 + x - deltaX, -1.0 - deltaY, .5*sin((+x)*3.141593) - deltaZ);//0-deltaZ);//sin((+x)*3.141593) -deltaZ);

		glTexCoord2f(x + xIncrement, 1.0);
		glVertex3f(0 + x + xIncrement - deltaX, -1.0 - deltaY, .5*sin((+x + xIncrement)*3.141593) - deltaZ);//0-deltaZ);//sin((+x+xIncrement)*3.141593) -deltaZ);

		glTexCoord2f(x + xIncrement, 0.0);
		glVertex3f(0 + x + xIncrement - deltaX, 0.0 - deltaY, .5*sin((+x + xIncrement)*3.141593) - deltaZ);//0-deltaZ);//sin((+x+xIncrement)*3.141593) -deltaZ);

		glEnd();
	}

	glBindTexture(GL_TEXTURE_2D, 0);


}
/*
void drawSinusoidalHead(float scale,int headWidth,int headHeight, int myRefX,int myRefY){

float c1x,c1y,c1z;
float c2x,c2y,c2z;
glBindTexture(GL_TEXTURE_2D, keyframeTexture);

float deltaX = 0.0;//-(myRefX/(float)headWidth*5.0-2.5);
float deltaY = 0.0;//myRefY/(float)headHeight*7.5-3.75;
float deltaZ = 0.0;//-4.0f * cos(deltaX/2.5*3.141593);


getReferenceCoordinate(&deltaX,&deltaY,&deltaZ);
//deltaX-=0.5;
//deltaY+=0.5;
//deltaZ-=0.5;

for(int i=0;i<headWidth;i+=1){
float fx = (1.6667 * i/(float)headWidth) - 0.332;
float fy = 0;
float fz = .5*sin(fx*3.141593);
//float deltaX = -((1.6667 * myRefX/(1.0*headWidth)) - 0.332)*scale;
//float deltaY = -((1.6667 * myRefY/(1.0*headHeight)) - 0.332)*scale;
//float deltaZ = -sin(deltaX*3.141593)*scale;
glBegin(GL_LINES);

glColor3f(1.0f,1.0f,1.0);

if(i==0){
c1x = fx*scale+deltaX;
c1y =+0.332*scale   -deltaY;
c1z = fz*scale+deltaZ;
}
//glTexCoord2f(fx*scale-deltaX, -1.3347*scale  -deltaY);
glTexCoord2f( (i*1.0/headWidth)*1.2-0.1, -0.1);

glVertex3f(fx*scale-deltaX, +0.332*scale   -deltaY, fz*scale-deltaZ);

if(i+8>=headWidth){
c2x=fx*scale+deltaX;
c2y= -1.3347*scale  -deltaY;
c2z=fz*scale+deltaZ;
}
glTexCoord2f( (i*1.0/headWidth)*1.2-0.1, 1.1);
//glTexCoord2f(fx*scale-deltaX, +0.332*scale   -deltaY);
glVertex3f(fx*scale-deltaX, -1.3347*scale  -deltaY, fz*scale-deltaZ);
glEnd();

}

glBindTexture(GL_TEXTURE_2D, keyframeTexture);
glColor3f(1.0f,1.0f,1.0);
glBegin(GL_QUADS);
glTexCoord2f(0.0f, 0.0f);glVertex3f(c1x, c1y,  c1z);    // Bottom Left Of The Texture and Quad
glTexCoord2f(1.0f, 0.0f);glVertex3f(c2x, c1y,  c1z);    // Bottom Right Of The Texture and Quad
glTexCoord2f(1.0f, 1.0f); glVertex3f(c2x, c2y,  c2z);   // Top Right Of The Texture and Quad
glTexCoord2f(0.0f, 1.0f);  glVertex3f(c1x, c2y,  c2z);  // Top Left Of The Texture and Quad
glEnd();

}*/

//void drawHeadModel(float scale, int headWidth, int headHeight, int myRefX, int myRefY){
//
//	float deltaX = 0.0;//-(myRefX/(float)headWidth*5.0-2.5);
//	float deltaY = 0.0;//myRefY/(float)headHeight*7.5-3.75;
//	float deltaZ = 0.0;//-4.0f * cos(deltaX/2.5*3.141593);
//
//
//	getReferenceCoordinate(&deltaX, &deltaY, &deltaZ);
//	deltaX -= 0.5;
//	deltaY += 0.5;
//	deltaZ += 0.5;
//	//printf("Reference %f %f %f (%f)\n",deltaX,deltaY,deltaZ,scale);
//
//	for (int i = 0; i<3036; i++){
//		//glColor3f (i/3036.0, 0.0, 0.0);
//
//
//		float v1x = triangles[i].vert[1][0] - triangles[i].vert[0][0];
//		float v1y = triangles[i].vert[1][1] - triangles[i].vert[0][1];
//		float v1z = triangles[i].vert[1][2] - triangles[i].vert[0][2];
//
//		float v2x = triangles[i].vert[2][0] - triangles[i].vert[0][0];
//		float v2y = triangles[i].vert[2][1] - triangles[i].vert[0][1];
//		float v2z = triangles[i].vert[2][2] - triangles[i].vert[0][2];
//
//		float nx = 5 * (v1y*v2z - v2y*v1z);
//		float ny = 5 * (v1z*v2x - v2z*v1x);
//		float nz = 5 * (v1x*v2y - v2x*v1y);
//
//		// calculate the length of the vector
//		float len = (float)(sqrt((nx * nx) + (ny * ny) + (nz * nz)));
//
//		// avoid division by 0
//		if (len == 0.0f)
//			len = 1.0f;
//
//		// reduce to unit size
//		nx /= len;
//		ny /= len;
//		nz /= len;
//
//
//		glNormal3f(nx, ny, nz);
//
//		if (drawNormals){
//			glBegin(GL_LINES);
//
//			glColor3f(1.0f, 0.0f, 0.0);
//
//			glVertex3f(scale* (triangles[i].vert[0][0] + deltaX),
//				scale* (triangles[i].vert[0][1] + deltaY),
//				scale* (triangles[i].vert[0][2] + deltaZ));
//
//			glVertex3f(scale* (triangles[i].vert[0][0] + deltaX + nx),
//				scale* (triangles[i].vert[0][1] + deltaY + ny),
//				scale* (triangles[i].vert[0][2] + deltaZ + nz));
//
//			glEnd();
//		}
//
//
//		glBegin(GL_TRIANGLES);
//		glColor4d(1.0, 1.0, 1.0, 0.65);
//
//
//
//		for (int j = 0; j<3; j++){
//			glVertex3f(scale* (triangles[i].vert[j][0]) - deltaX,
//				scale* (triangles[i].vert[j][1]) - deltaY,
//				scale* (triangles[i].vert[j][2]) - deltaZ);
//		}
//		glEnd();
//
//	}
//
//
//}


void checkTransparency(){
	//dealing with transparency
	if (blending){
		glEnable(GL_BLEND);
		//glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else{
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}
}

/*
* grabs frame from webcam and loads it in an OpenGL texture
*/

void getFrameAsGLTexture(){

	IplImage* currentFrame = getCurrentFrame();




	// texture 1 (poor quality scaling)
	glBindTexture(GL_TEXTURE_2D, frameTexture);   // 2d texture (x and y size)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, 0x8191, GL_TRUE); //generate mipmaps

	//   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
	//   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture

	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	// 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image,
	// border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.


	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, currentFrame->width, currentFrame->height,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, currentFrame->imageData);

	/*glTexImage2D(GL_TEXTURE_2D, 0, 3, currentFrame->width, currentFrame->height,
	0, GL_RGB, GL_UNSIGNED_BYTE, currentFrame->imageData);*/
}

void drawGrabbedFrame(){

	getFrameAsGLTexture();

	glBindTexture(GL_TEXTURE_2D, frameTexture);   // choose the texture to use.

	glDisable(GL_LIGHTING);


	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);


	glLoadIdentity();

	glBegin(GL_QUADS);

	// Front Face (note that the texture's corners have to match the quad's corners)
	glNormal3f(0.0f, 0.0f, 1.0f);                              // front face points out of the screen on z.
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);


	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0, 0.0f);     // Bottom Left Of The Texture and Quad


	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, -1.0, 0.0f);     // Bottom Right Of The Texture and Quad




	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);     // Top Right Of The Texture and Quad


	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glEnd();




	glClear(GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, NULL);
	/*
	glBegin(GL_QUADS);

	// Front Face (note that the texture's corners have to match the quad's corners)
	glNormal3f( 0.0f, 0.0f, 1.0f);                              // front face points out of the screen on z.
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glTexCoord2f(0.0f, 0.0f);

	glVertex3f(0, 0,  1.0f);    // Bottom Left Of The Texture and Quad


	glTexCoord2f(1.0f, 0.0f);
	glVertex3f( 640, 0,  1.0f); // Bottom Right Of The Texture and Quad



	glTexCoord2f(1.0f, 1.0f);
	glVertex3f( 640,  480,  -1);        // Top Right Of The Texture and Quad


	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0,  480,  -1);
	glEnd();
	*/


	if (!drawLight) {
		glDisable(GL_LIGHTING);
	}
	else {
		glEnable(GL_LIGHTING);
	}


}


void keyframeRelated(){
	static int created = 0;
	if (isEhciTextureCreated()){
		if (!created){
			//created=1;
			glDeleteTextures(1, &keyframeTexture);
			glGenTextures(1, &keyframeTexture);
			glBindTexture(GL_TEXTURE_2D, keyframeTexture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			GLImage glTexture = getEhciGlTexture();
			glTexImage2D(GL_TEXTURE_2D, 0, 3, glTexture.sizeX, glTexture.sizeY, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, glTexture.data);
			//printf("Created texture %d x %d\n",glTexture.sizeX,glTexture.sizeY);
		}
	}
}


//image sent to ehci so that it can generate frames for head tracking

char *data = NULL;

void generateTexture(int width, int height){
	int dataPtr = width*height * 3 - 1;
	if (data == NULL)
		data = (char*)malloc(width*height * 3);

	glReadPixels(0, 0, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
	if (!getGeneratedImage()){
		setGeneratedImage(cvCreateImage(cvSize(width, height),
			IPL_DEPTH_8U, 3));
	}
	//TODO: try to use the pointer for increased performance
	//generatedImage->imageData = data;

	for (int y = 0; y<height; y++) {
		uchar* ptr = (uchar*)(
			getGeneratedImage()->imageData + y * getGeneratedImage()->widthStep
			);
		for (int x = 0; x<getGeneratedImage()->width; x++) {
			ptr[3 * x + 0] = data[dataPtr--];
			ptr[3 * x + 1] = data[dataPtr--];
			ptr[3 * x + 2] = data[dataPtr--];
		}
	}
	//free(data);

}

/* The main drawing function. */

void DrawGLScene(void)
{
	
	double width = 320.0f;
	double height = 240.0f;
	static int bootstrap = 0;
	CvMatr32f rotation_matrix = new float[9];
	CvVect32f translation_vector = new float[3];
	double glPositMatrix[16];
	//cvWaitKey(0);
	int detected = 0;
	if (getGeneratedImage()){
		detected = ehciLoop(EHCI6DFACEDETECT, initialGuess, getGeneratedImage());
	}
	else{
		detected = ehciLoop(EHCI6DFACEDETECT, initialGuess, 0);
	}
	//detected= ehciLoop(EHCI6DFACEDETECT,initialGuess);

	keyframeRelated();


	getReferenceHeadBounds(&headRefX, &headRefY, &aLastHeadW, &aLastHeadH);

	getGlPositMatrix(glPositMatrix);
	double euler_x = atan2(glPositMatrix[9], glPositMatrix[10]);
	double euler_y = atan2(-glPositMatrix[8], sqrt((glPositMatrix[9] * glPositMatrix[9]) + (glPositMatrix[10] * glPositMatrix[10])));
	double euler_z = atan2(glPositMatrix[4], glPositMatrix[0]);
	//Sleep(1000);
	std::cout << euler_x /*<< "  " << euler_y << "  " << euler_z*/ << std::endl;
	if (moveMouse)
	mouse.move_mouse(euler_x, euler_y);
	/*std::cout << "*******GLPOSIT MATRIX X Y Z************" << std::endl;
	Sleep(200);
	std::cout << " X            " << glPositMatrix[12] << std::endl;
	Sleep(200);
	std::cout << "               Y            " << glPositMatrix[12] << std::endl;
	Sleep(200);
	std::cout << "                             z            " << glPositMatrix[12] << std::endl;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	system("cls");*/
	checkTransparency();

	if (drawBackground)
		drawGrabbedFrame();

	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition); // set light position.

	if (initialGuess){
		setInitialRTMatrix(rotation_matrix, translation_vector);
		updateGlPositMatrix(rotation_matrix, translation_vector);
	}


	if (initialGuess){
		//TODO: get points from orthogonal view
		/*
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		float mScale = 10000;

		glOrtho(-10*mScale/headWidth*0.05,+10*mScale/headWidth,-9*mScale/headHeight,+10*mScale/headHeight*0.1,-1000.0f,100.0f);

		glBegin(GL_TRIANGLES);
		for(int i=0;i<3036;i++){
		//glColor3d ((GLdouble)i/3036.0, 0.0, 0.0);
		glColor3d (0.0, 0.0, 1.0);
		for(int j=0;j<3;j++){
		glVertex3f( triangles[i].vert[j][0],triangles[i].vert[j][1],triangles[i].vert[j][2]);
		}
		}
		glEnd();

		glBegin(GL_LINES);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(0, 0,1.0f);
		glVertex3f(100, 100,1.0f);
		glEnd();*/
		bootstrap = 0;
	}
	else{

		//drawGrabbedFrame();

		//loads reference point pose into OpenGL modelview matrix
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixd(projectionMatrix);

		//look in +z direction, the same as POSIT
		gluLookAt(0, 0, 0, 0, 0, +1.0, 0, -1, 0);


		glMatrixMode(GL_MODELVIEW);
		
		glLoadMatrixd(glPositMatrix);
		
		


		if (drawSine)
			drawSinusoidalHead(getEhciModelScale(), aLastHeadW, aLastHeadH, headRefX, headRefY);

		//scale should be the same as Sinusoidal, but the head
		//is 5 units wide, so it's 5 times lower

		//drawHeadModel(1.6*EHCIMODELSCALE/5.0f,aLastHeadW,aLastHeadH,headRefX,headRefY);




		if (bootstrap)
			generateTexture(320, 240);


		if (drawAxis){
		 	drawCentralAxis();
			//drawReferenceAxis();
		}

		//if (drawHead)
			//drawHeadModel(1.4f / 5.0f, aLastHeadW, aLastHeadH, headRefX, headRefY);

		//glTranslatef(200.0,0,0.0f);
		//glutWireSphere(80.0, 200, 200);
	}
	if (initialGuess){
		bootstrap = 1;
	}


	drawHelpText();

	glutSwapBuffers();
	//cvWaitKey();

	//automatic reinitialization
	//TODO: reactivate automatic reinitialization
	if (detected) initialGuess = 0;
	//else initialGuess=1;
	
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void ReSizeGLScene(GLsizei Width, GLsizei Height)
{
	if (Height == 0)                          // Prevent A Divide By Zero If The Window Is Too Small
		Height = 1;

	glViewport(0, 0, Width, Height);                // Reset The Current Viewport And Perspective Transformation

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

/* The function called whenever a normal key is pressed. */
void keyPressed(unsigned char key, int x, int y)
{

	switch (key) {
	case 27: // kill everything.
		/* shut down our window */
		glutDestroyWindow(window);
		/* exit the program...normal termination. */
		exit(1);
		break; // redundant.

	case 'a':
		drawAxis = !drawAxis;
		break;
	case 's':
		drawSine = !drawSine;
		break;
	case 'h':
	case 'H':
		drawHead = !drawHead;
		break;
	case 'm':
		moveMouse = !moveMouse;
		break;
	case 'd':
	case 'D':
		drawBackground = !drawBackground;
		break;
	case 'b':
	case 'B':
		blending = !blending;
		break;
	case 'n':
	case 'N':
		drawNormals = !drawNormals;
		break;
	case 'i':
	case 'I':
		initialGuess = !initialGuess;
		break;
	case 76:
	case 108: // switch the lighting
		drawLight = !drawLight;
		if (!drawLight) {
			glDisable(GL_LIGHTING);
		}
		else {
			glEnable(GL_LIGHTING);
		}
		break;


	default:
		break;
	}
}

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(GLsizei Width, GLsizei Height)      // We call this right after our OpenGL window is created.
{
	double width = (double)Width;
	double height = (double)Height;
	setGLProjectionMatrix(projectionMatrix);


	glEnable(GL_TEXTURE_2D);                    // Enable texture mapping.


	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   // This Will Clear The Background Color To Black
	glClearDepth(1.0);                              // Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);                   // The Type Of Depth Test To Do
	glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
	glShadeModel(GL_SMOOTH);                        // Enables Smooth Color Shading

	//float specReflection[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	//glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);
	//glMateriali(GL_FRONT, GL_SHININESS, 96);

	//float mcolor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);



	glEnable(GL_COLOR_MATERIAL);

	//float colorBlue[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colorBlue);
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);





	//glPolygonMode(GL_FRONT, GL_LINE);
	//glPolygonMode(GL_BACK, GL_LINE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();                               // Reset The Projection Matrix

	gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);       // Calculate The Aspect Ratio Of The Window

	glMatrixMode(GL_MODELVIEW);

	// set up light number 1.
	//    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);  // add lighting. (ambient)

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightAmbient);

	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);  // add lighting. (diffuse).
	glEnable(GL_LIGHT1);                             // turn light 1 on.
	glEnable(GL_LIGHTING);

	//generate texture
	glGenTextures(1, &frameTexture);



}

void openGLCustomInit(int argc, char** argv, double width, double height){

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

	/* get a 640 x 480 window */
	glutInitWindowSize(width, height);

	/* the window starts at the upper left corner of the screen */
	glutInitWindowPosition(500, 0);

	/* Open a window */
	window = glutCreateWindow("6DOF Head Tracking ");

	/* Register the function to do all our OpenGL drawing. */
	glutDisplayFunc(&DrawGLScene);

	/* Go fullscreen.  This is as soon as possible. */
	//    glutFullScreen();
	//   glutReshapeWindow(640,480);

	/* Even if there are no events, redraw our gl scene. */
	glutIdleFunc(&DrawGLScene);

	/* Register the function called when our window is resized. */
	glutReshapeFunc(&ReSizeGLScene);

	/* Register the function called when the keyboard is pressed. */
	glutKeyboardFunc(&keyPressed);
	
	/* Register the function called when special keys (arrows, page down, etc) are pressed. */
	//    glutSpecialFunc(&specialKeyPressed);

	/* Initialize our window. */
	InitGL(width, height);

	
	/* Start Event Processing Engine */
	glutMainLoop();

}



int main(int argc, char** argv)
{
	char rawFile[] = "head.raw";
	loadRaw(rawFile);

	//      cvNamedWindow( "6dofHead", 1 );
	ehciInit();
	atexit(ehciExit); std::vector<float>vect;
	openGLCustomInit(argc, argv, 320.0, 240.0);
	


	//cvDestroyWindow("6dofHead");

	return 0;
}