//Windows includes - DO NOT EDIT AS LOTS OF INTERDEPENDENCE
#include <math.h>						
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>	//Needed for console output (debugging)
#include <gl/freeglut.h>
#include <iostream>
#include <vector>

#ifdef WIN32
#include "gltools.h"
#include <windows.h>		// Must have for Windows platform builds
#include <gl\gl.h>			// Microsoft OpenGL headers (version 1.1 by themselves)
#include <gl\glu.h>			// OpenGL Utilities
#include "glm.h"
#endif

//note that this needs gltools.h and gltools.cpp to be included in the shared/library directory

//can define any number of textures here - we just have 2 images
//these are just integers so you can use them as such
#define Grass			0	
#define Wall			1		
#define Football		2    
#define Tree			3
#define Flower_Orange	4
#define Flower_Yellow	5
#define Target_Red		6
#define Target_Green	7
#define Target_Blue		8
#define Target_Grey		9
#define Background		10
#define Red_Dot			11
#define Xbar			12
#define Ybar			13
#define Xbar_Slider		14
#define Ybar_Slider		15
#define BrickWall		16
#define RedTest			17
#define BlueTest		18
#define Wall2			19
#define TEXTURE_COUNT	20
GLuint  textures[TEXTURE_COUNT];

//below is simply a character array to hold the file names
//note that you may need to replace the below with the full directory root depending on where you put your image files
//if you put them where the exe is then you just need the name as below - THESE IMAGES  ARE IN THE DEBUG FOLDER, YOU CAN ADD ANY NEW ONES THERE 
const char* textureFiles[TEXTURE_COUNT] = { "grass_diff.tga", "old_wall_texture_TGA.tga", "FootballCompleteMap.tga","palmBranchA.tga", 
"orangeFlowerFinal5.tga","yellowFlowerFinal.tga", "targetRed.tga", "targetGreen.tga", "targetBlue.tga",
"targetDull.tga", "stormydays_large.tga", "RedDot.tga", "fillBarHorizontal.tga",
"fillBarVerticalR.tga", "xBarSlider.tga", "yBarSlider.tga" ,"brick_texture_lo_res.tga","XbarSliderTest.tga","YbarSliderTest.tga","Wall2.tga" };


//for lighting if you want to experiment with these
GLfloat mKa[4] = { 0.11f,0.06f,0.11f,1.0f }; //ambient
GLfloat mKd[4] = { 0.43f,0.47f,0.54f,1.0f }; //diffuse
GLfloat mKs[4] = { 1.0f,1.0f,1.0f,1.0f }; //specular
GLfloat mKe[4] = { 0.5f,0.5f,0.0f,1.0f }; //emission
//spot position and direction
GLfloat	 lightPos[] = { 0.0, 100.0, 300.0, 0.0f };
GLfloat  spotDir[] = { 50.0, 25.0, 0.0 };

GLfloat	 lightPos2[] = { 50.0, 100.0, 300.0, 0.0f };
GLfloat  spotDir2[] = { 50.0, 15.0, 0.0 };

GLfloat	 lightPos3[] = { -50.0, 100.0, 300.0, 0.0f };
GLfloat  spotDir3[] = { 50.0, 15.0, 0.0 };


// Useful lighting colour values
GLfloat  whiteLightBright[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat  redLight[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat  greenLight[] = { 0.0f, 1.0f, 0.0f, 1.0f };
GLfloat  blueLight[] = { 0.0f, 1.0f, 1.0f, 1.0f };
GLfloat  whiteLightLessBright[] = { 0.8f, 0.8f, 0.8f, 1.0f };


//we need these for the texture loader
//they are to do with the image format and size
GLint iWidth, iHeight, iComponents;
GLenum eFormat;
// this is a pointer to memory where the image bytes will be held 
GLbyte* pBytes0;

//camera
GLfloat cameraX = 0.0;
GLfloat cameraY = 100.0;
GLfloat cameraZ = 550.0;

//boolean values to move the camera and ball
bool moveCamera = false;
bool moveBall = false;
//Future declaration methods
void drawTargets();

GLint Wwidth;
GLint Wheight;
GLint chances = 10;
GLfloat Score = 0.0;

//Bar Position
GLfloat xBarSlider = -165.0f;
GLfloat yBarSlider = -71.15;

//Ball Position
GLfloat xStart = 0.0f;
GLfloat yStart = 20.0f;
GLfloat zStart = 280.0f;

//Destination
GLfloat xBall = 0.0f;
GLfloat yBall = 250.0f;
GLfloat zBall = -398.0f;

//Speeds
GLfloat xSpeed = 3;
GLfloat ySpeed = 3;
GLfloat zSpeed = 3;

//Rotate the skybox
GLfloat backgroundRotation = 0.0f;
//Rotate the Ball
GLfloat ballRotation = 0.0f;

//Move camera coordinates
GLfloat moveCameraX = 0.0f;
GLfloat moveCameraY = 30.0f;
GLfloat moveCameraZ = 300.0f;

GLfloat moveCameraCenterX = 0.0f;
GLfloat moveCameraCenterY =	0.0f;
GLfloat moveCameraCenterZ = 0.0f;

//Targets in the wall
bool target1 = false;
bool target2 = false;
bool target3 = false;
bool target4 = false;
bool target5 = false;
bool target6 = false;
bool target7 = false;
bool target8 = false;
bool target9 = false;
bool cameraFollow = false;

using namespace std;

//----------------This section is for the Game UI Objects-----------------

//Create the grass floor
void drawGrassFloor() {
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, textures[Grass]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(-4.0, -4.0);
	glVertex3f(-1000, -1000, 0.0);
	glTexCoord2f(4.0, -4.0);
	glVertex3f(1000, -1000, 0.0);
	glTexCoord2f(4.0, 4.0);
	glVertex3f(1000, 1000, 0.0);
	glTexCoord2f(-4.0, 4.0);
	glVertex3f(-1000, 1000, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
//Create the walls - for all the three walls (Use transformation for the 3 walls)
void drawBrickWalls(int image) {
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(-4.0, -4.0);
	glVertex3f(-1000, -1000, 0.0);
	glTexCoord2f(4.0, -4.0);
	glVertex3f(1000, -1000, 0.0);
	glTexCoord2f(4.0, 4.0);
	glVertex3f(1000, 1000, 0.0);
	glTexCoord2f(-4.0, 4.0);
	glVertex3f(-1000, 1000, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
//Create the flowers - for orange and yellow flowers (Use the yellow texture and the orange texture)
void drawFlowers(int image) {
	glEnable(GL_TEXTURE_2D);
	glColor4f(0.9, 0.5, 0.7, 1); //aplha value is for transparancy
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-20, 0.0, 100.0);
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(20, 0.0, 100.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(20, 50.0, 100.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-20, 50.0, 100.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
//Create the Trees
void drawTree() {
	glEnable(GL_TEXTURE_2D);
	glColor4f(0.6, 0.9, 0.7, 1); //alpha value is for transparency
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[Tree]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-50.0, 0.0, 100.0);
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(50.0, 0.0, 100.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(50.0, 250.0, 100.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-50.0, 250.0, 100.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

//-----------------------------------------------------------------------------
//Create the sky Box - Using BB Code
// here we declare an array of vertices for the cube
GLfloat cubeVertexData[108] =
    {
        //object vertex coordinates for cube made from triangles
        // Data layout for each line below is:
        // positionX, positionY, positionZ
        //wind counter-clockwise
		
		//1
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
		//2
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, 0.5f,
		//3
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        //4
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        //5
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        //6
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f

    };
//the texture coordinates - work will skybox texture of this shape +--
GLfloat textureCoordsSkyBox[72] = {
        //face 1
        0.75,0.33,      //    0,1,
        0.75,0.67,     //    1,1,
        0.5,0.33,     //    0,0,
        0.5,0.33,     //    0,0,
        0.75,0.67,    //    1,0,
        0.5,0.67,   //    1,1,

        //face 2
        0.5,1.0, //    1,1,
        0.25,1, //    0,1,
        0.5,0.67, //    1,0,
        0.5,0.67, //    1,0,
        0.25,1.0, //    0,1,
        0.25,0.67, //    1,1,
        //face 3
        0,0.67,//    1,1,
        0,0.33,//    0,1,
        0.25,0.67,//    1,0,
        0.25,0.67,//    1,0,
        0,0.33,//    0,1,
        0.25,0.33,//    0,0,
        //face 4
        0.25,0.0,//    0,1,
        0.5,0.0,//    1,1,
        0.25,0.33,//    0,0,
        0.25,0.33,//    0,0,
        0.5,0.0,//    1,1,
        0.5,0.33,//    0,0,
        //face 5
        0.5,0.67,//    1,0,
        0.25,0.67,//    0,0,
        0.5,0.33,//    1,1,
        0.5,0.33,//    1,1,
        0.25,0.67,//    0,0,
        0.25,0.33,//    0,1,
        //face 6
        0.75,0.33,//    1,1,
        1.0,0.33,//    0,1,
        0.75,0.67,//    1,0,
        0.75,0.67,//    1,0,
        1.0,0.33,//    0,1,
        1.0,0.67//    0,0


    };
//the lighting normals - all facing out from each face
GLfloat gCubeVertexdataNormals[108] =
    {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,

        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f

    };
//-----------------------------------------------------------------------------



//Set projection to the ortho 2D
void setOrthographicProjection() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, Wwidth, 0, Wheight);
	glScalef(1, -1, 1);
	glTranslatef(0, -Wheight, 0);
	glMatrixMode(GL_MODELVIEW);
}
//Reset projection to the default
void resetPerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
//Create the sky box
void CreateSkyBox() {
	glFrontFace(GL_CW); //texture the inside

	//bind your texture here
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[Background]);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, gCubeVertexdataNormals);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, cubeVertexData);
	//   glTexCoordPointer(2, GL_FLOAT, 0, textureCoords);
	glTexCoordPointer(2, GL_FLOAT, 0, textureCoordsSkyBox);
	// draw a cube - type - start number - number of vertices to draw (so 3 for single triangle)
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
//Display the sky Box 
void displaySkyBox() {
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0f, 400.0f, -590.0f); //Translate cube to the center of the scene
	glScalef(1800, 1800, 1800); //Scale the Cube in order to view inside
	glRotatef(backgroundRotation, 0, 1, 0);
	CreateSkyBox(); // Setting the sky box
	glPopMatrix();
}
//==Create the 3 Walls and Grass Floor==
void CreateMainScene() {
	//Create the Grass
	glPushMatrix();
	glTranslatef(0.0, 0.0, -400.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	drawGrassFloor();
	glPopMatrix();

	//Create the Middle Wall
	glPushMatrix();
	glTranslatef(0.0, -700.0, -400.0);
	drawBrickWalls(Wall2);
	glPopMatrix();

	//Create the Left Wall
	glPushMatrix();
	glTranslatef(500, -700.0, -200);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	drawBrickWalls(Wall2);
	glPopMatrix();

	//Create the Right Wall
	glPushMatrix();
	glTranslatef(-500, -700.0, -200);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	drawBrickWalls(Wall2);
	glPopMatrix();

}
//Draw the Horizontal Bar (X Bar)
void drawXbar() {
	glEnable(GL_TEXTURE_2D);
	glColor4f(0.8, 0.8, 0.8, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[Xbar]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-20.0, 0.0, 5.0);
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(20.0, 0.0, 5.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(20.0, 10.0, 5.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-20.0, 10.0, 5.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
//Draw the Vertical Bar (Y Bar)
void drawYbar() {
	glEnable(GL_TEXTURE_2D);
	glColor4f(0.8, 0.8, 0.8, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[Ybar]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-7, 0.0, 5.0);
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(7, 0.0, 5.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(7, 40.0, 5.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-7.0, 40.0, 5.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
//Draw the Horizontal Bar Slider (X Bar Slider)
void drawXbarSlider() {
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, textures[RedTest]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);

	glTexCoord2f(-4.0, -4.0);
	glVertex3f(-1, 0.0, 5.0);

	glTexCoord2f(4.0, -4.0);
	glVertex3f(1, 0.0, 5.0);

	glTexCoord2f(4.0, 4.0);
	glVertex3f(1, 14.0, 5.0);

	glTexCoord2f(-4.0, 4.0);
	glVertex3f(-1, 14.0, 5.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
//Draw the Vertical Bar Slider (Y Bar Slider)
void drawYbarSlider() {
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[BlueTest]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-9, 0.0, 5.0);
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(9, 0.0, 5.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(9, 2, 5.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-9, 2, 5.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
//==Draw the HUD==
void drawHUD() {
	glTranslatef(0.0, 0.0, -200.0f);
	//Display horizontal Slider
	glPushMatrix();
	glTranslatef(xBarSlider, 88.0f, 0.0f);
	drawXbarSlider();
	glPopMatrix();
	//Display horizontal bar
	glPushMatrix();
	glTranslatef(-165.0f, 90.0f, 0.0f);
	drawXbar();
	glPopMatrix();

	//Display vertical Slider
	glPushMatrix();
	glTranslatef(165, yBarSlider, 0.0);
	drawYbarSlider();
	glPopMatrix();
	//Display vertical bar
	glPushMatrix();
	glTranslatef(165, -90, 0.0f);
	drawYbar();
	glPopMatrix();
}
//==Create Trees==
void sceneTrees() {
	//Draw Tree Right - 1
	glPushMatrix();
	glTranslatef(550, 0, -200);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	drawTree();
	glPopMatrix();
	//Draw Tree Right - 2
	glPushMatrix();
	glTranslatef(570, 0, -100);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	drawTree();
	glPopMatrix();
	//Draw Tree Right - 3
	glPushMatrix();
	glTranslatef(570, 0, 0);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	drawTree();
	glPopMatrix();
	//Draw Tree Right - 4
	glPushMatrix();
	glTranslatef(570, 0, 100);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	drawTree();
	glPopMatrix();
	//Draw Tree Right - 5
	glPushMatrix();
	glTranslatef(570, 0, 200);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	drawTree();
	glPopMatrix();
	//Draw Tree Right- 6
	glPushMatrix();
	glTranslatef(570, 0, 300);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	drawTree();
	glPopMatrix();
	//Draw Tree Left - 1
	glPushMatrix();
	glTranslatef(-550, 0, -200);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	drawTree();
	glPopMatrix();
	//Draw Tree Left - 2
	glPushMatrix();
	glTranslatef(-570, 0, -100);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	drawTree();
	glPopMatrix();
	//Draw Tree Left - 3
	glPushMatrix();
	glTranslatef(-570, 0, 0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	drawTree();
	glPopMatrix();
	//Draw Tree Left - 4
	glPushMatrix();
	glTranslatef(-570, 0, 100);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	drawTree();
	glPopMatrix();
	//Draw Tree Left - 5
	glPushMatrix();
	glTranslatef(-570, 0, 200);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	drawTree();
	glPopMatrix();
	//Draw Tree Left - 6
	glPushMatrix();
	glTranslatef(-570, 0, 300);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	drawTree();
	glPopMatrix();

}
//==Create Flowers==
void sceneFlowers() {
	//Draw Flowers - 1
	glPushMatrix();
	glTranslatef(400, 0, -430);
	drawFlowers(Flower_Orange);
	glPopMatrix();
	//Draw Flowers - 2
	glPushMatrix();
	glTranslatef(350, 0, -430);
	drawFlowers(Flower_Yellow);
	glPopMatrix();
	//Draw Flowers - 3
	glPushMatrix();
	glTranslatef(300, 0, -430);
	drawFlowers(Flower_Orange);
	glPopMatrix();
	//Draw Flowers - 4
	glPushMatrix();
	glTranslatef(250, 0, -430);
	drawFlowers(Flower_Yellow);
	glPopMatrix();
	//Draw Flowers - 5
	glPushMatrix();
	glTranslatef(200, 0, -430);
	drawFlowers(Flower_Orange);
	glPopMatrix();
	//Draw Flowers - 6
	glPushMatrix();
	glTranslatef(150, 0, -430);
	drawFlowers(Flower_Yellow);
	glPopMatrix();
	//Draw Flowers - 7
	glPushMatrix();
	glTranslatef(100, 0, -430);
	drawFlowers(Flower_Orange);
	glPopMatrix();
	//Draw Flowers - 8
	glPushMatrix();
	glTranslatef(50, 0, -430);
	drawFlowers(Flower_Yellow);
	glPopMatrix();
	//Draw Flowers - 9
	glPushMatrix();
	glTranslatef(0, 0, -430);
	drawFlowers(Flower_Orange);
	glPopMatrix();
	//Draw Flowers - 10
	glPushMatrix();
	glTranslatef(-50, 0, -430);
	drawFlowers(Flower_Yellow);
	glPopMatrix();
	//Draw Flowers - 11
	glPushMatrix();
	glTranslatef(-100, 0, -430);
	drawFlowers(Flower_Orange);
	glPopMatrix();
	//Draw Flowers - 12
	glPushMatrix();
	glTranslatef(-150, 0, -430);
	drawFlowers(Flower_Yellow);
	glPopMatrix();
	//Draw Flowers - 13
	glPushMatrix();
	glTranslatef(-200, 0, -430);
	drawFlowers(Flower_Orange);
	glPopMatrix();
	//Draw Flowers - 14
	glPushMatrix();
	glTranslatef(-250, 0, -430);
	drawFlowers(Flower_Yellow);
	glPopMatrix();
	//Draw Flowers - 15
	glPushMatrix();
	glTranslatef(-300, 0, -430);
	drawFlowers(Flower_Orange);
	glPopMatrix();
	//Draw Flowers - 16
	glPushMatrix();
	glTranslatef(-350, 0, -430);
	drawFlowers(Flower_Yellow);
	glPopMatrix();
	//Draw Flowers - 17
	glPushMatrix();
	glTranslatef(-400, 0, -430);
	drawFlowers(Flower_Orange);
	glPopMatrix();

}
//Draw the middle point for ball initial position
void CreateMiddlePoint(int image) {
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);

	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-5.0, -5.0, 0.0);
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(5.0, -5.0, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(5.0, 5.0, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-5.0, 5.0, 0.0);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}
//==Create Middle point for ball initial position ==
void drawMiddlePoint() {
	glPushMatrix();
	glTranslatef(0.0f, 5.0f, 300);
	glRotatef(90, 20.0f, 0.0f, 0.0f);
	glScalef(5, 5, 5);
	CreateMiddlePoint(Target_Grey);
	glPopMatrix();
}
//Draw the red aim
void drawAim() {
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, textures[RedTest]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-10, -10, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(10, -10, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(10, 10, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-10, 10, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
//Set the Aim initial position
void setAim() {
	glPushMatrix();
	glTranslatef(xBall, yBall, zBall);
	drawAim();
	glPopMatrix();
}

//Create the Score Text
void drawText(const char* string, float x, float y, GLfloat r, GLfloat g, GLfloat b) {
	GLint j = strlen(string);

	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	for (GLint c = 0; c < j; c++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, string[c]);
	}
}
//Draw Score Text
char scoreTextString[12];
void drawingScoreText(GLfloat scale)
{
	glPushMatrix();

	//draw the text offset from the box
	glTranslatef(350.0, 70.0, 0.0);
	glColor3f(0.0, 1.0, 0.0);
	sprintf_s(scoreTextString, "Score:%3.0f", Score);

	
	glRotatef(180.0, 1.0, 0.0, 0.0); // To rotate the text (mirroring)
	glScalef(scale, scale, scale); // To scale the size of the text

	drawText(scoreTextString, 500, 100, 1, 0, 0);
	glPopMatrix();
}
//Draw Number of chances 
char numOfChances[22];
void drawingNumOfChance(GLfloat scale)
{
	glPushMatrix();

	//draw the text offset from the box
	glTranslatef(670.0, 70.0, 0.0);
	glColor3f(0.0, 1.0, 0.0);
	sprintf_s(numOfChances, "Chances :%3.0d", chances);

	
	glRotatef(180.0, 1.0, 0.0, 0.0); // To rotate the text (mirroring)
	glScalef(scale, scale, scale); // To scale the size of the text

	drawText(numOfChances, 500, 100, 0, 0, 1);
	glPopMatrix();
}
//Set the Text to the Screen - This function will allow to draw the text within 2D projection
void setTextScreen() {
	//Setting the Score Text
	glPushMatrix();
	glLoadIdentity();
	setOrthographicProjection();
	drawingScoreText(0.3f);
	glPopMatrix();
	resetPerspectiveProjection();
	//Setting the number of chances Text
	glPushMatrix();
	glLoadIdentity();
	setOrthographicProjection();
	drawingNumOfChance(0.3f);
	glPopMatrix();
	resetPerspectiveProjection();
}



//----------------This section is for the Objects and Classes for the Game----------
//Class for Football
class objFootball {
private:
	GLfloat X; // ball x
	GLfloat Y; // ball y
	GLfloat Z; // ball z
	GLfloat Radius; // ball radius
	GLUquadricObj* quadricFootball;

public:
	objFootball(GLfloat x, GLfloat y, GLfloat z, GLfloat r) {
		X = x;
		Y = y;
		Z = z;
		Radius = r;

		glPushMatrix();
		glTranslatef(X, Y, Z);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[Football]);
		glColor3f(0.5, 0.5, 0.5);
		quadricFootball = gluNewQuadric();
		gluQuadricTexture(quadricFootball, GL_TRUE);
		gluSphere(quadricFootball, Radius, 60, 35);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	GLfloat getX() { //return the x value of the football
		return X;
	}

	GLfloat getY() { //return the y value of the football
		return Y;
	}

	GLfloat getZ() { //return the z value of the football
		return Z;
	}

	GLfloat getRadius() { // return the radius of the football
		return Radius;
	}
};
//Class for the Targets
class objTargets {
private:
	bool hit; //boolean value to check whether the target is hit by ball
public:
	objTargets(int image, float width, float height, bool h) {
		hit = h;

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glColor4f(0.8, 0.8, 0.8, 1);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDisable(GL_CULL_FACE);
		glColor3f(1.0, 1.0, 1.0);
		glEnable(GL_TEXTURE_2D);
		if (hit == false) { // if the ball has not hit the target
			glBindTexture(GL_TEXTURE_2D, textures[image]); // it will keep the default texture
		}
		else if (hit == true) { //if the ball hits target
			glBindTexture(GL_TEXTURE_2D, textures[Target_Grey]); // it will set the texture to the grey
		}
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-width, -height, 0.0);
		glTexCoord3f(1.0, 0.0, 0.0);
		glVertex3f(width, -height, 0.0);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(width, height, 0.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-width, height, 0.0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}

	bool getHit() { //method to get the status of the target
		return hit;
	}

	void setHit() { //method to set the target as hit
		hit = true;
	}

};
//Class for the bounding sphere collision - When drawing give the same coordinate as the targets. This sphere will be use to check the collision detection
//When a target object is drawn on the wall, a bounding sphere will drawn in the same position.
class objBoundingSphere {
private:

	GLfloat X; //bonding sphere x
	GLfloat Y; //bounding sphere y
	GLfloat Z; //bounding sphere z
	GLfloat Radius; //bounding sphere radius
	GLUquadricObj* quadricSphere;
	bool hit = false; //bool value to check the collision

public:
	objBoundingSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat r) {
		X = x;
		Y = y;
		Z = z;
		Radius = r;

		glPushMatrix();
		glTranslatef(X, Y, Z);
		glEnable(GL_TEXTURE_2D);
		glColor3f(0.5, 0.5, 0.5);
		quadricSphere = gluNewQuadric();
		gluSphere(quadricSphere, Radius, 60, 35);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	GLfloat getX() { //return the x position of the sphere
		return X;
	}

	GLfloat getY() { //return the y position of the sphere
		return Y;
	}

	GLfloat getZ() { //return the z position of the sphere
		return Z;
	}

	GLfloat getRadius() { //return the radius of the sphere
		return Radius;
	}

	bool getHit() { //return the hit status of the sphere
		return hit;
	}

	void setTrue() { //set hit status to be true
		hit = true;
	}


};


//=====================Vertex Arrays=============================
//Create the Football object
objFootball* objFootball_1;	//Football Object
//Vertex Arrays to store the objects
std::vector < objTargets > obj_targets;	
std::vector < objBoundingSphere > obj_boundingSphere;


//----------------Create elements using object classes---------------------------------
void drawFootball() {
	glPushMatrix();
	//glRotatef(ballRotation, 0, 1, 0);
	objFootball_1 = new objFootball(xStart, yStart, zStart, 20.0f); 
	glPopMatrix();
}
//Create the Targets for the wall and add to the vertex array
void drawTargets() {
//--------------Blue Targets------------------
	//Blue Target - 1
	glPushMatrix();
	glTranslatef(0.0, 150, -399.0);
	objTargets* blueTarget1 = new objTargets(Target_Blue, 40, 40, target1);
	glPopMatrix();
	//Blue Target - 2
	glPushMatrix();
	glTranslatef(120, 200, -399.0);
	objTargets* blueTarget2 = new objTargets(Target_Blue, 40, 40, target2);
	glPopMatrix();
//--------------Red Targets------------------
	//Red Target - 1
	glPushMatrix();
	glTranslatef(120, 100, -399.0);
	objTargets* redTarget1 = new objTargets(Target_Red, 30, 30, target3);
	glPopMatrix();
	//Red Target - 2
	glPushMatrix();
	glTranslatef(-280, 140, -399.0);
	objTargets* redTarget2 = new objTargets(Target_Red, 30, 30, target4);
	glPopMatrix();
	//Red Target - 3
	glPushMatrix();
	glTranslatef(-260, 220, -399.0);
	objTargets* redTarget3 = new objTargets(Target_Red, 30, 30, target5);
	glPopMatrix();
	//Red Target - 4
	glPushMatrix();
	glTranslatef(230, 200, -399.0);
	objTargets* redTarget4 = new objTargets(Target_Red, 30, 30, target6);
	glPopMatrix();
//--------------Green Targets------------------
	//Green Target - 1
	glPushMatrix();
	glTranslatef(250, 100, -399.0);
	objTargets* greenTarget1 = new objTargets(Target_Green, 50, 50, target7);
	glPopMatrix();
	//Green Target - 2
	glPushMatrix();
	glTranslatef(-180, 100, -399.0);
	objTargets* greenTarget2 = new objTargets(Target_Green, 50, 50, target8);
	glPopMatrix();
	//Green Target - 3
	glPushMatrix();
	glTranslatef(-100, 220, -399.0);
	objTargets* greenTarget3 = new objTargets(Target_Green, 50, 50, target9);
	glPopMatrix();
	//Add Target objects to the obj_targets array
	obj_targets.push_back(*blueTarget1);
	obj_targets.push_back(*blueTarget2);
	obj_targets.push_back(*redTarget1);
	obj_targets.push_back(*redTarget2);
	obj_targets.push_back(*redTarget3);
	obj_targets.push_back(*redTarget4);
	obj_targets.push_back(*greenTarget1);
	obj_targets.push_back(*greenTarget2);
	obj_targets.push_back(*greenTarget3);
}
//Create the Bounding Spheres for the Collision detection
void drawBoundingSphere() {
	//Bounding spheres for the blue targerts
	//Using the Same position from the targets in order
	//Sphere - 1 , target number - 1
	glPushMatrix();
	objBoundingSphere* blueBoundingSphere1 = new objBoundingSphere(0, 150, -399, 15);
	glPopMatrix();
	//Sphere - 2 , target number - 2
	glPushMatrix();
	objBoundingSphere* blueBoundingSphere2 = new objBoundingSphere(120, 200, -399, 15);
	glPopMatrix();

	//Bounding sphere for the red targets
	//sphere - 1 , target number - 3
	glPushMatrix();
	objBoundingSphere* redBoundingSphere3 = new objBoundingSphere(120, 100, -399, 10);
	glPopMatrix();
	//sphere - 2 , target number - 4
	glPushMatrix();
	objBoundingSphere* redBoundingSphere4 = new objBoundingSphere(-280, 140, -399, 10);
	glPopMatrix();
	//sphere - 3 , target number - 5
	glPushMatrix();
	objBoundingSphere* redBoundingSphere5 = new objBoundingSphere(-260, 220, -399.0, 10);
	glPopMatrix();
	//sphere - 4 , target number - 6
	glPushMatrix();
	objBoundingSphere* redBoundingSphere6 = new objBoundingSphere(230, 200, -399.0, 10);
	glPopMatrix();

	//Bounding sphere for the green targets
	//sphere - 1 , target number - 7
	glPushMatrix();
	objBoundingSphere* greenBoundingSphere7 = new objBoundingSphere(250, 100, -399, 20);
	glPopMatrix();
	//sphere - 2 , target number - 8
	glPushMatrix();
	objBoundingSphere* greenBoundingSphere8 = new objBoundingSphere(-180, 100, -399, 20);
	glPopMatrix();
	//sphere - 3 , target number - 9
	glPushMatrix();
	objBoundingSphere* greenBoundingSphere9 = new objBoundingSphere(-100, 220, -399, 20);
	glPopMatrix();
	//Adding the bounding spheres to the obj_boundingSphere vertex array
	obj_boundingSphere.push_back(*blueBoundingSphere1);
	obj_boundingSphere.push_back(*blueBoundingSphere2);
	obj_boundingSphere.push_back(*redBoundingSphere3);
	obj_boundingSphere.push_back(*redBoundingSphere4);
	obj_boundingSphere.push_back(*redBoundingSphere5);
	obj_boundingSphere.push_back(*redBoundingSphere6);
	obj_boundingSphere.push_back(*greenBoundingSphere7);
	obj_boundingSphere.push_back(*greenBoundingSphere9);
	obj_boundingSphere.push_back(*greenBoundingSphere8);
}


//----------------This Section is for the Game Logics-------------------------------
//Reset the Aim 
void resetAim() {
	xBall = 0.0f;
	yBall = 250.0f;
	zBall = -398.0f;

	xBarSlider = -165.0f;
	yBarSlider = -71.15;
}
//Reset the Ball coordinates
void resetBall() {
	//Resets the ball position
	xStart = 0.0f;
	yStart = 20.0f;
	zStart = 280.0f;
	moveBall = false;
	cameraFollow = false;
	moveCameraX = 0.0f;
	moveCameraY = 30.0f;
	moveCameraZ = 300.0f;

	moveCameraCenterX = 0.0f;
	moveCameraCenterY = 0.0f;
	moveCameraCenterZ = 0.0f;

	xSpeed = 3;
	ySpeed = 1;
	zSpeed = 3;
}
//Reset the Game variables
void resetGame() {
	resetBall();
	resetAim();
	target1 = false;
	target2 = false;
	target3 = false;
	target4 = false;
	target5 = false;
	target6 = false;
	target7 = false;
	target8 = false;
	target9 = false;
	cameraFollow = false;
	Score = 0;
	chances = 10;
}
//Controls of the keyboard arrow keys
void userSpecialControls(int key, int x, int y) {

	//Controls the y of the ball
	if ((key == GLUT_KEY_UP) && (moveBall==false)) { //Only when the user press the up-arrow key and the ball is not moving
		yBall = yBall + 5; // Increasing the y value of the ball to travel at the end
		ySpeed += 0.1f; // Setting the speed of the y value change 
		yBarSlider = yBarSlider + 0.35; // Increasing the y position slider of the y bar
	}
	else if ((key == GLUT_KEY_DOWN) && (moveBall == false)) { //Only when the user press the down-arrow key and the ball is not moving
		yBall = yBall - 5; // Decreasing the y value of the ball to travel at the end
		ySpeed -= 0.1f; // Setting the speed of the y value change
		yBarSlider = yBarSlider - 0.35; // Decreasing the y position slider of the y bar
	}

	//Controls the x of the ball
	if ((key == GLUT_KEY_RIGHT) && (moveBall == false)) { //Only when the user press the right-arrow key and the ball is not moving
		xBall = xBall + 5; // Increasing the x value of the ball to travel at the end
		xBarSlider = xBarSlider + 0.2; // Increasing the x position slider of the x bar
	}
	else if ((key == GLUT_KEY_LEFT) && (moveBall == false)) { //Only when the user press the left-arrow key and the ball is not moving
		xBall = xBall - 5;  // Decreasing the x value of the ball to travel at the end
		xBarSlider = xBarSlider - 0.2; // Decreasing the x position slider of the x bar
	}

	//Settingt the minium y value that can be set to the ball to travel down
	if (yBall < 10) {
		yBall = 10;
	}
	//Settingt the maxium y value that can be set to the ball to travel up
	else if (yBall > 535) {
		yBall = 535;
	}
	//Settingt the minium x value that can be set to the ball to travel left
	if (xBall < -490) {
		xBall = -490;
	}
	//Settingt the maximum x value that can be set to the ball to travel right
	else if (xBall > 490) {
		xBall = 490;
	}
	//Settingt the maximum x value that can be set to the x bar slider - right
	if (xBarSlider > -145.4f) {
		xBarSlider = -145.4f;
	}
	//Settingt the minimum x value that can be set to the x bar slider - left
	else if (xBarSlider < -184.6) {
		xBarSlider = -184.6;
	}
	//Settingt the maximum y value that can be set to the y bar slider - up
	if (yBarSlider > -50.01f) {
		yBarSlider = -50.01f;
	}
	//Settingt the minimum y value that can be set to the y bar slider - down
	else if (yBarSlider < -89.5) {
		yBarSlider = -89.5;
	}
	// Recalling the render scene
	glutPostRedisplay();
}
//Controls of the letter keys
void userControls(unsigned char key, int x, int y) {
	if (moveBall != true && chances != 0) { // this can be apply if the ball is not moving and the number of chances have to be not zero
		if (key == 32) { //if the space bar pressed ball will move
			chances--; //chances will be reduce by one for each enter of space key
			moveBall = true; // set ball to move
		}
	}


	if (key == 'r') { // if the r pressed game will reset
		resetGame(); // calling the reset function
	}
	else if (key == 'f') { // if the f pressed game will follow the camers
		cameraFollow = !cameraFollow; //set bool value to follow the camera
	}

	if (chances == 0) { // if the chances are 0 game will pause
		moveBall = false;
	}

}

//----------------Display Method----------------------------------------------------
void RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	// Save the matrix state and do the rotations
	setTextScreen(); // calling the function to draw the score text and the number of chances text
	drawHUD(); // calling the function to draw the x and y bar with sliders
	//Reseting the game after 0 chances or the maximum score has achived
	if ((chances == 0) || (Score==210)){
		resetGame();
	}
	//This is the default camera view
	if (cameraFollow == false) {
		gluLookAt(cameraX, cameraY, cameraZ,//eye
			0.00, 0.00, 0.00,//centre
			0.00, 1.00, 0.00);//up
	}
	//If the user presses f this will follow the camera
	else if (cameraFollow == true) {
		gluLookAt(moveCameraX, moveCameraY, moveCameraZ,//eye
			moveCameraCenterX, moveCameraCenterY, -360.00,//centre
			0.00, 1.00, 0.00);//up
	}
	//Creating the Football
	drawFootball();
	//Display the Aim
	setAim();
	//Creating the main scene including the Grass floor and the 3 walls
	CreateMainScene();
	//Display the sky Box
	displaySkyBox();
	//Display the Targets
	drawTargets();
	//Adding the bounding spheres to check the collision
	drawBoundingSphere();
	//Displaying the trees
	sceneTrees();
	//Display flowers
	sceneFlowers();
	//Display the middle point
	drawMiddlePoint();

	glutSwapBuffers();
}


// This function does any needed initialization on the rendering
// context.
void SetupRC()
{
	//textures

	GLuint texture;
	// allocate a texture name
	glGenTextures(1, &texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// photoshop is a good converter to targa (TGA)
	//the gltLoadTGA method is found in gltools.cpp and is orignally from the OpenGL SuperBible book
	//there are quite a few ways of loading images
	// Load textures in a for loop
	glGenTextures(TEXTURE_COUNT, textures);
	//this puts the texture into OpenGL texture memory
 //   glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); - not defined so probably need to update GLEW - not needed here so ignore
	for (int iLoop = 0; iLoop < TEXTURE_COUNT; iLoop++)
	{
		// Bind to next texture object
		glBindTexture(GL_TEXTURE_2D, textures[iLoop]);

		// Load texture data, set filter and wrap modes
		//note that gltLoadTGA is in the glm.cpp file and not a built-in openGL function
		pBytes0 = gltLoadTGA(textureFiles[iLoop], &iWidth, &iHeight,
			&iComponents, &eFormat);

		glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes0);

		//set up texture parameters

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		//try these too
	   // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
		// glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		free(pBytes0);
	}

	//enable textures
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_DEPTH_TEST);	// Hidden surface removal    
	glFrontFace(GL_CCW);// Counter clock-wise polygons face out
 //	glEnable(GL_CULL_FACE);		// Do not calculate inside

//    glCullFace(GL_FRONT_AND_BACK);

// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_POINT_SMOOTH);
	// Setup and enable light 0
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLightBright);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLightBright);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_SPECULAR, mKs);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60.0f);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, whiteLightBright);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, mKs);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.0f);
	//glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT2, GL_DIFFUSE, whiteLightBright);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPos3);
	glLightfv(GL_LIGHT2, GL_SPECULAR, mKs);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 60.0f);
	//glEnable(GL_LIGHT2);

	// Enable colour tracking
	glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// All materials hereafter have full specular reflectivity
	// with a high shine
	glMaterialfv(GL_FRONT, GL_SPECULAR, mKs);
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	// Black blue background clear colour
	glClearColor(0.0f, 0.0f, 0.03f, 1.0f);
}

void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}
//Change the window size
void ChangeSize(int w, int h)
{
	GLfloat fAspect;
	Wwidth = w;
	Wheight = h;

	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Calculate aspect ratio of the window
	fAspect = (GLfloat)w / (GLfloat)h;

	// Set the perspective coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0f, fAspect, 1.0, 2000.0);
	// Modelview matrix reset
	glMatrixMode(GL_MODELVIEW);
}
//Check the collision
void checkCollision() {

	//Calculating the minimum distance that can be have with the football and the bounding sphere.
	//Minimum distance for two spheres are the addition of their radius.
	//This function will check with all the spheres from the sphere array
	GLfloat distance = objFootball_1->getRadius() + obj_boundingSphere[0].getRadius();

	//Checking the collision
	//Checking the collision between the football and the sphere using x,y,z position from the objects
	//If output of the conditions are true there will be collision between the football and the sphere
	if (objFootball_1->getX() <= (obj_boundingSphere[0].getX() + distance) && objFootball_1->getX() >= (obj_boundingSphere[0].getX() - distance)
		&& objFootball_1->getY() <= (obj_boundingSphere[0].getY() + distance) && objFootball_1->getY() >= (obj_boundingSphere[0].getY() - distance)
		&& objFootball_1->getZ() <= (obj_boundingSphere[0].getZ() + distance) && objFootball_1->getZ() >= (obj_boundingSphere[0].getZ() - distance)) {

		//If the output of the conditions are true - collision happened
		if (target1 == false) { //Checking the target is not collided
			Score = Score + 20; //Adding the score point by 20
			target1 = true; // if not collided setting as collided (This will set the target 1 txeture to the grey color)
		}

		resetBall(); // Reseting the ball positions
	}
	else if (objFootball_1->getX() <= (obj_boundingSphere[1].getX() + distance) && objFootball_1->getX() >= (obj_boundingSphere[1].getX() - distance)
		&& objFootball_1->getY() <= (obj_boundingSphere[1].getY() + distance) && objFootball_1->getY() >= (obj_boundingSphere[1].getY() - distance)
		&& objFootball_1->getZ() <= (obj_boundingSphere[1].getZ() + distance) && objFootball_1->getZ() >= (obj_boundingSphere[1].getZ() - distance)) {
		//If the output of the conditions are true - collision happened
		if (target2 == false) { //Checking the target is not collided
			Score = Score + 20; //Adding the score point by 20
			target2 = true; // if not collided setting as collided (This will set the target 2 txeture to the grey color)
		}

		resetBall(); // Reseting the ball positions
	}
	else if (objFootball_1->getX() <= (obj_boundingSphere[2].getX() + distance) && objFootball_1->getX() >= (obj_boundingSphere[2].getX() - distance)
		&& objFootball_1->getY() <= (obj_boundingSphere[2].getY() + distance) && objFootball_1->getY() >= (obj_boundingSphere[2].getY() - distance)
		&& objFootball_1->getZ() <= (obj_boundingSphere[2].getZ() + distance) && objFootball_1->getZ() >= (obj_boundingSphere[2].getZ() - distance)) {
		//If the output of the conditions are true - collision happened
		if (target3 == false) { //Checking the target is not collided
			Score = Score + 30; //Adding the score point by 30
			target3 = true; // if not collided setting as collided (This will set the target 3 txeture to the grey color)
		}

		resetBall(); // Reseting the ball positions
	}
	else if (objFootball_1->getX() <= (obj_boundingSphere[3].getX() + distance) && objFootball_1->getX() >= (obj_boundingSphere[3].getX() - distance)
		&& objFootball_1->getY() <= (obj_boundingSphere[3].getY() + distance) && objFootball_1->getY() >= (obj_boundingSphere[3].getY() - distance)
		&& objFootball_1->getZ() <= (obj_boundingSphere[3].getZ() + distance) && objFootball_1->getZ() >= (obj_boundingSphere[3].getZ() - distance)) {
		//If the output of the conditions are true - collision happened
		if (target4 == false) { //Checking the target is not collided
			Score = Score + 30; //Adding the score point by 30
			target4 = true; // if not collided setting as collided (This will set the target 4 txeture to the grey color)
		}

		resetBall(); // Reseting the ball positions
	}
	else if (objFootball_1->getX() <= (obj_boundingSphere[4].getX() + distance) && objFootball_1->getX() >= (obj_boundingSphere[4].getX() - distance)
		&& objFootball_1->getY() <= (obj_boundingSphere[4].getY() + distance) && objFootball_1->getY() >= (obj_boundingSphere[4].getY() - distance)
		&& objFootball_1->getZ() <= (obj_boundingSphere[4].getZ() + distance) && objFootball_1->getZ() >= (obj_boundingSphere[4].getZ() - distance)) {
		//If the output of the conditions are true - collision happened
		if (target5 == false) { //Checking the target is not collided
			Score = Score + 30; //Adding the score point by 30
			target5 = true;  // if not collided setting as collided (This will set the target 5 txeture to the grey color)
		}

		resetBall();  // Reseting the ball positions
	}
	else if (objFootball_1->getX() <= (obj_boundingSphere[5].getX() + distance) && objFootball_1->getX() >= (obj_boundingSphere[5].getX() - distance)
		&& objFootball_1->getY() <= (obj_boundingSphere[5].getY() + distance) && objFootball_1->getY() >= (obj_boundingSphere[5].getY() - distance)
		&& objFootball_1->getZ() <= (obj_boundingSphere[5].getZ() + distance) && objFootball_1->getZ() >= (obj_boundingSphere[5].getZ() - distance)) {
		//If the output of the conditions are true - collision happened
		if (target6 == false) { //Checking the target is not collided
			Score = Score + 10;  //Adding the score point by 10
			target6 = true; // if not collided setting as collided (This will set the target 6 txeture to the grey color)
		}

		resetBall();  // Reseting the ball positions
	}
	else if (objFootball_1->getX() <= (obj_boundingSphere[6].getX() + distance) && objFootball_1->getX() >= (obj_boundingSphere[6].getX() - distance)
		&& objFootball_1->getY() <= (obj_boundingSphere[6].getY() + distance) && objFootball_1->getY() >= (obj_boundingSphere[6].getY() - distance)
		&& objFootball_1->getZ() <= (obj_boundingSphere[6].getZ() + distance) && objFootball_1->getZ() >= (obj_boundingSphere[6].getZ() - distance)) {
		//If the output of the conditions are true - collision happened
		if (target7 == false) { //Checking the target is not collided
			Score = Score + 10; //Adding the score point by 10
			target7 = true; // if not collided setting as collided (This will set the target 7 txeture to the grey color)
		}

		resetBall(); // Reseting the ball positions
	}
	else if (objFootball_1->getX() <= (obj_boundingSphere[7].getX() + distance) && objFootball_1->getX() >= (obj_boundingSphere[7].getX() - distance)
		&& objFootball_1->getY() <= (obj_boundingSphere[7].getY() + distance) && objFootball_1->getY() >= (obj_boundingSphere[7].getY() - distance)
		&& objFootball_1->getZ() <= (obj_boundingSphere[7].getZ() + distance) && objFootball_1->getZ() >= (obj_boundingSphere[7].getZ() - distance)) {
		//If the output of the conditions are true - collision happened
		if (target9 == false) { //Checking the target is not collided
			Score = Score + 10; //Adding the score point by 30
			target9 = true; // if not collided setting as collided (This will set the target 9 txeture to the grey color)
		}

		resetBall(); // Reseting the ball positions
	}
	else if (objFootball_1->getX() <= (obj_boundingSphere[8].getX() + distance) && objFootball_1->getX() >= (obj_boundingSphere[8].getX() - distance)
		&& objFootball_1->getY() <= (obj_boundingSphere[8].getY() + distance) && objFootball_1->getY() >= (obj_boundingSphere[8].getY() - distance)
		&& objFootball_1->getZ() <= (obj_boundingSphere[8].getZ() + distance) && objFootball_1->getZ() >= (obj_boundingSphere[8].getZ() - distance)) {
		//If the output of the conditions are true - collision happened
		if (target8 == false) { //Checking the target is not collided
			Score = Score + 10;  //Adding the score point by 30
			target8 = true; // if not collided setting as collided (This will set the target 8 txeture to the grey color)
		}

		resetBall(); // Reseting the ball positions
	}
	else if (objFootball_1->getX() <= (obj_boundingSphere[9].getX() + distance) && objFootball_1->getX() >= (obj_boundingSphere[9].getX() - distance)
		&& objFootball_1->getY() <= (obj_boundingSphere[9].getY() + distance) && objFootball_1->getY() >= (obj_boundingSphere[9].getY() - distance)
		&& objFootball_1->getZ() <= (obj_boundingSphere[9].getZ() + distance) && objFootball_1->getZ() >= (obj_boundingSphere[9].getZ() - distance)) {
		//If the output of the conditions are true - collision happened
		if (target9 == false) { //Checking the target is not collided
			Score = Score + 30;  //Adding the score point by 30
			target9 = true;  // if not collided setting as collided (This will set the target 8 txeture to the grey color)
		}

		resetBall(); // Reseting the ball positions
	}
	else if (objFootball_1->getZ() <= -398.0f + 20) { // Checking the maximum z value for ball to travel
		resetBall(); // Reseting the ball positions
	}
}
//Timer function to animate
void TimerFunc(int value)
{
	if (moveBall != false) {

		checkCollision(); //Calling collision function
		//Moving the camera with ball
		moveCameraCenterX += 0.1;
		moveCameraZ = moveCameraZ - 3;
		//=================Moving the ball=======================
		//Minimum y value that the ball can be travel at the end
		if (yStart != yBall + 20) { //if it is above the minum ball y coordinate will increment
			yStart++;
		}
		//Incrementing the z value of the ball
		zStart = zStart - zSpeed;
		//Minimum x value that the ball can be travel at the end
		if (xStart != xBall) { //if it is above the minum ball x coordinate will change 
			if (xStart > xBall) {
				xStart = xStart - xSpeed; //Moving to the  left
			}
			else if (xStart < xBall) {
				xStart = xStart + xSpeed; //Moving to the right
			}
		}
	} 

	glutTimerFunc(10, TimerFunc, 1);
	glutPostRedisplay();

	//Skybox rotation increment
	backgroundRotation = backgroundRotation + 0.1;
	if (backgroundRotation > 360.0f) //background rotation for the sky box
	{
		backgroundRotation = 0.0f;
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1280, 720); //Setting the window size
	glutCreateWindow("3D Football Game"); //Setting the window title
	init(); //Calling the init method

	glutReshapeFunc(ChangeSize); //Calling the reshape function
	glutDisplayFunc(RenderScene); //Calling the display function
	glutSpecialFunc(userSpecialControls); //Calling the special keyboard function
	glutKeyboardFunc(userControls); //Calling the keyboard function
	glutTimerFunc(10, TimerFunc, 1); //Calling the timer function
	SetupRC(); //calling the setup rc function
	glutMainLoop();
	return 0;
}

