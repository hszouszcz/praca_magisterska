#pragma once
#include <GL/gl.h>


class OpenGL_Utils
{
public:
	int drawSine = 0, drawHead = 0, drawAxis = 1, drawNormals = 0, blending = 1, drawLight = 1, drawBackground = 1;

	void drawCentralAxis();
	void drawHelpText();
	void checkTransparency();
	void getFrameAsGLTexture();
	OpenGL_Utils();
	~OpenGL_Utils();
};

