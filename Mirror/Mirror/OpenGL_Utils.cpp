//#define _CRT_SECURE_NO_WARNINGS
//#include "OpenGL_Utils.h"
//#include <stdio.h>
//#include <ehci.h>
//#include "glut.h"
//#include "gl/GL.h"
//
//
//
//void OpenGL_Utils::drawCentralAxis(){
//
//	float deltaX, deltaY, deltaZ;
//
//	getReferenceCoordinate(&deltaX, &deltaY, &deltaZ);
//
//
//	float cx, cy, cz;
//	cx = .5 - deltaX;
//	cy = -.25 - deltaY;
//	cz = 1.0 - deltaZ;
//
//
//	//std::cout << "Z " << cz << std::endl;
//	//Sleep(100);
//	//std::cout <<"               Y " << cy << std::endl;
//	//Sleep(100);
//	//std::cout << "                                X " << cx << std::endl;
//
//	glLineWidth(4.0);
//	glBegin(GL_LINES);
//	glColor3f(1.0f, 0.0f, 0.0);
//	glVertex3f(cx, cy, cz);
//	glVertex3f(cx + 1000, cy, cz);
//	glEnd();
//
//	glBegin(GL_LINES);
//	glColor3f(0.9f, 0.3f, 0.0);
//	glVertex3f(cx, cy, cz);
//	glVertex3f(cx - 1000, cy, cz);
//	glEnd();
//
//	glBegin(GL_LINES);
//	glColor3f(0.0f, 1.0f, 0.0);
//	glVertex3f(cx, cy, cz);
//	glVertex3f(cx, cy + 1000, cz);
//	glEnd();
//
//	glBegin(GL_LINES);
//	glColor3f(0.3f, 0.8f, 0.0);
//	glVertex3f(cx, cy, cz);
//	glVertex3f(cx, cy - 1000, cz);
//	glEnd();
//
//	glBegin(GL_LINES);
//	glColor3f(0.0f, 0.0f, 1.0);
//	glVertex3f(cx, cy, cz);
//	glVertex3f(cx, cy, cz + 5.0f);
//	glEnd();
//
//	glBegin(GL_LINES);
//	glColor3f(0.0f, 0.0f, 0.5f);
//	glVertex3f(cx, cy, cz);
//	glVertex3f(cx, cy, cz - 5.0f);
//	glEnd();
//
//	glLineWidth(1.0);
//
//
//}
//
//void OpenGL_Utils::drawHelpText()
//{
//	int* font = (int*)GLUT_BITMAP_HELVETICA_12;
//	//glClear(GL_DEPTH_BUFFER_BIT);
//	glDisable(GL_DEPTH_TEST);
//	glDisable(GL_LIGHTING);
//
//
//	int XSize = 640, YSize = 480;
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//
//	glOrtho(0, XSize, YSize, 0, 0, 1);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//
//
//	glColor3f(255.0f, 255.0f, 255.0f);
//	glBegin(GL_QUADS);
//	glVertex2f(10, 10);
//	glVertex2f(290, 10);
//	glVertex2f(290, 40);
//	glVertex2f(10, 40);
//	glEnd();
//
//
//	glColor3f(0.0f, 0.0f, 0.0f);
//
//	glRasterPos2f(10, 20);
//	const unsigned char text[100] = "Press \'I\' to reinitialize. You must focus this window\n(face the camera in a still position)";
//	for (int i = 0; i < 100; i++)
//	{
//		glutBitmapCharacter(font, text[i]);
//	}
//
//
//	if (!drawLight) {
//		glDisable(GL_LIGHTING);
//	}
//	else {
//		glEnable(GL_LIGHTING);
//	}
//
//	glEnable(GL_DEPTH_TEST);
//
//
//}
//
//
//void OpenGL_Utils::checkTransparency(){
//	//dealing with transparency
//	if (blending){
//		glEnable(GL_BLEND);
//		//glDisable(GL_DEPTH_TEST);
//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	}
//	else{
//		glDisable(GL_BLEND);
//		glEnable(GL_DEPTH_TEST);
//	}
//}
//
//
//
//OpenGL_Utils::OpenGL_Utils()
//{
//}
//
//
//OpenGL_Utils::~OpenGL_Utils()
//{
//}
