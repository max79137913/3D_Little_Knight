#include <al\al.h>
#include "al\alut.h"

#include "library/gltools.h"	// OpenGL toolkit
#include "library/glframe.h"   // Frame Class
#include <math.h>
#include <time.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

#include "library/glm.h"

#include "textureController.h"
#include "collisionController.h"
#include "modelController.h"
#include "ObjectInGame.h"
#include "leadingRole.h"
#include "DemonRole.h""

int timeElapsed; //in millsec
int timeCount;

//Define glframe(camera)
GLFrame    frameCamera;
GLfloat  secondPersonCamera[] = {0, 0.1, 4.7}; //camera's position
enum CameraState {
    normalCameraState,
    startShakeState,
    shakeLeftState,
    shakeRightState,
    turnRightState,
    turnLeftState,
    endState
} cameraState;
int cameraCounter;
int cameraRotateForMove;

//Define collisionVector
vector<ObjectInGame> groundObject;
vector<bool> grassGround;

vector<DemonRole> demonObject;

LeadingRole role;

int demonCount;

//for openal
const int NUM_BUFFERS = 3;
ALuint	buffer[NUM_BUFFERS];

ALboolean al_bool;
ALsizei size,freq;
ALenum format;
ALvoid *data = NULL;
int ch;

const int NUM_SOURCES = 3;
ALuint	source[NUM_SOURCES];

void addDemonToWorld(float x)
{
    DemonRole newDemon;
    newDemon.frame.MoveUp(15);
    newDemon.frame.MoveRight(x);
    newDemon.frame.MoveForward(-8);
    newDemon.frame.RotateWorld(-M_PI_2, 0, 1, 0);
    newDemon.scaleX = newDemon.scaleY = newDemon.scaleZ = 0.2;
    newDemon.initBoundbox();
    demonObject.push_back(newDemon);
}

void addBlockToWorld(float width, float length, float height, float x, float y, float z, bool grass){
    ObjectInGame ground1;
    ground1.frame.MoveForward(-3);
    //ground1.frame.SetForwardVector(1, 0, 0);
    ground1.boundingBox.xMax = x+width;
    //cout << "addM" << ground1.boundingBox.xMax;
    ground1.boundingBox.xMin = x;
        //cout << "addm" << ground1.boundingBox.xMin;
    ground1.boundingBox.yMax = y+height;
    ground1.boundingBox.yMin = y;
    ground1.boundingBox.zMax = z+length;
    ground1.boundingBox.zMin = z;
    groundObject.push_back(ground1);
    grassGround.push_back(grass);
}
void renderBlock(){
    glEnable(GL_TEXTURE_2D);
    for(int i=0; i<groundObject.size(); i++)
    {
        glPushMatrix();
        groundObject[i].applyTransformAndScale();

        glNormal3f(0.0f, 1.0f, 0.0f);

        glBindTexture(GL_TEXTURE_2D, textures[getTextureNumMap()["groundGrass"]]);
        glBegin(GL_QUADS);
            glTexCoord2f(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.zMax);
            glVertex3d(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMax, groundObject[i].boundingBox.zMax);

            glTexCoord2f(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.zMin);
            glVertex3d(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMax, groundObject[i].boundingBox.zMin);

            glTexCoord2f(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.zMin);
            glVertex3d(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMax, groundObject[i].boundingBox.zMin);

            glTexCoord2f(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.zMax);
            glVertex3d(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMax, groundObject[i].boundingBox.zMax);
        glEnd();
        glBegin(GL_QUADS);
            glTexCoord2f(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMax);
            glVertex3d(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMax, groundObject[i].boundingBox.zMax);

            glTexCoord2f(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMax);
            glVertex3d(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMax, groundObject[i].boundingBox.zMin);

            glTexCoord2f(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMax-0.1);
            glVertex3d(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMax-0.1, groundObject[i].boundingBox.zMin);

            glTexCoord2f(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMax-0.1);
            glVertex3d(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMax-0.1, groundObject[i].boundingBox.zMax);
        glEnd();
        glBegin(GL_QUADS);
            glTexCoord2f(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMax);
            glVertex3d(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMax, groundObject[i].boundingBox.zMax);

            glTexCoord2f(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMax);
            glVertex3d(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMax, groundObject[i].boundingBox.zMin);

            glTexCoord2f(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMax-0.1);
            glVertex3d(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMax-0.1, groundObject[i].boundingBox.zMin);

            glTexCoord2f(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMax-0.1);
            glVertex3d(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMax-0.1, groundObject[i].boundingBox.zMax);
        glEnd();
        glBegin(GL_QUADS);
            glTexCoord2f(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMax);
            glVertex3d(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMax, groundObject[i].boundingBox.zMax);

            glTexCoord2f(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMax);
            glVertex3d(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMax, groundObject[i].boundingBox.zMax);

            glTexCoord2f(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMax-0.1);
            glVertex3d(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMax-0.1, groundObject[i].boundingBox.zMax);

            glTexCoord2f(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMax-0.1);
            glVertex3d(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMax-0.1, groundObject[i].boundingBox.zMax);
        glEnd();
        glBegin(GL_QUADS);
            //glTexCoord2f(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.zMin);
            glTexCoord2f(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMax);
            glVertex3d(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMax, groundObject[i].boundingBox.zMin);

            glTexCoord2f(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMax);
            glVertex3d(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMax, groundObject[i].boundingBox.zMin);

            glTexCoord2f(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMax-0.1);
            glVertex3d(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMax-0.1, groundObject[i].boundingBox.zMin);

            glTexCoord2f(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMax-0.1);
            glVertex3d(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMax-0.1, groundObject[i].boundingBox.zMin);
        glEnd();

        if(grassGround[i] == true)
            glBindTexture(GL_TEXTURE_2D, textures[getTextureNumMap()["groundGrass"]]);
        else
            glBindTexture(GL_TEXTURE_2D, textures[getTextureNumMap()["dirt"]]);
        glBegin(GL_QUADS);
            glVertex3d(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMax-0.1, groundObject[i].boundingBox.zMax);
            glVertex3d(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMax-0.1, groundObject[i].boundingBox.zMin);
            glVertex3d(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMax-0.1, groundObject[i].boundingBox.zMin);
            glVertex3d(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMax-0.1, groundObject[i].boundingBox.zMax);
        glEnd();
        glBegin(GL_QUADS);
            glVertex3d(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMax-0.1, groundObject[i].boundingBox.zMax);
            glVertex3d(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMax-0.1, groundObject[i].boundingBox.zMin);
            glVertex3d(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMin, groundObject[i].boundingBox.zMin);
            glVertex3d(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMin, groundObject[i].boundingBox.zMax);
        glEnd();
        glBegin(GL_QUADS);
            glVertex3d(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMax-0.1, groundObject[i].boundingBox.zMax);
            glVertex3d(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMax-0.1, groundObject[i].boundingBox.zMin);
            glVertex3d(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMin, groundObject[i].boundingBox.zMin);
            glVertex3d(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMin, groundObject[i].boundingBox.zMax);
        glEnd();
        glBegin(GL_QUADS);
            glVertex3d(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMax-0.1, groundObject[i].boundingBox.zMax);
            glVertex3d(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMax-0.1, groundObject[i].boundingBox.zMax);
            glVertex3d(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMin, groundObject[i].boundingBox.zMax);
            glVertex3d(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMin, groundObject[i].boundingBox.zMax);
        glEnd();
        glBegin(GL_QUADS);
            glVertex3d(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMax-0.1, groundObject[i].boundingBox.zMin);
            glVertex3d(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMax-0.1, groundObject[i].boundingBox.zMin);
            glVertex3d(groundObject[i].boundingBox.xMin, groundObject[i].boundingBox.yMin, groundObject[i].boundingBox.zMin);
            glVertex3d(groundObject[i].boundingBox.xMax, groundObject[i].boundingBox.yMin, groundObject[i].boundingBox.zMin);
        glEnd();
        glPopMatrix();
    }
    glDisable(GL_TEXTURE_2D);
}

//////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering
// context.
void SetupRC()
{
    timeCount = 0;

    setUpModel();
    // Bluish background
    glClearColor(0.0f, 0.0f, .50f, 1.0f );
	glEnable(GL_DEPTH_TEST);

    generateTexture();

    //ground setup
    addBlockToWorld(2, 3, 0.2, -2, 0, 0 ,1);
    addBlockToWorld(0.2, 3, 0.4, -2.2, 0, 0 ,1);
    addBlockToWorld(0.2, 3, 0.6, -2.4, 0, 0 ,1);
    addBlockToWorld(0.2, 3, 0.8, -2.6, 0, 0 ,1);
    addBlockToWorld(0.2, 3, 0.8, -3.4, 0, 0 ,1);
    addBlockToWorld(0.2, 3, 0.6, -3.6, 0, 0 ,1);
    addBlockToWorld(0.2, 3, 0.4, -3.8, 0, 0 ,1);
    addBlockToWorld(11.2, 3, 0.2, -15, 0, 0 ,1);

    addBlockToWorld(2, 3, 0.2, -8, 0.5, 0 ,1);
    addBlockToWorld(0.2, 3, 0.2, -8, 1, 0 ,1);

    addBlockToWorld(0.2, 3, 0.4, -15.2, 0, 0 ,1);
    addBlockToWorld(0.2, 3, 0.6, -15.4, 0, 0 ,1);
    addBlockToWorld(0.2, 3, 0.8, -15.6, 0, 0 ,1);
    addBlockToWorld(0.2, 3, 1, -15.8, 0, 0 ,1);
    addBlockToWorld(0.2, 3, 1.2, -16, 0, 0 ,1);
    addBlockToWorld(0.2, 3, 1.4, -16.2, 0, 0 ,1);
    addBlockToWorld(2.2, 3, 1.6, -18.4, 0, 0 ,1);

    //camero setting
    frameCamera.SetOrigin(secondPersonCamera[0], secondPersonCamera[1], secondPersonCamera[2]);
    frameCamera.MoveUp(0.5);
    cameraState = normalCameraState;
    cameraCounter = 0;
    cameraRotateForMove = 0;

    role.frame.SetOrigin(0, 0, 0);
    role.frame.MoveUp(2);
    //role.frame.MoveRight(0.1);
    role.frame.MoveForward(-5);
    role.frame.RotateWorld(-M_PI_2, 0, 1, 0);
    role.scaleX = role.scaleY = role.scaleZ = 0.3;
    role.initBoundbox();

    //demmon setUp

    addDemonToWorld(-rand()%70+1);
    addDemonToWorld(-rand()%70+1);
    addDemonToWorld(-rand()%70+1);
    addDemonToWorld(-rand()%70+1);
    addDemonToWorld(-rand()%70+1);
    demonCount = 5;
}

// Called to draw scene
void RenderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
        //camera setting
        cameraCounter++;
        if(cameraState == startShakeState)
        {
            cameraState = shakeLeftState;
            cameraCounter = 0;
        }
        else if(cameraCounter == 52)
        {
            alSourceStop(source[2]);
            cameraState = normalCameraState;
            cameraCounter = 0;
        }
        else if(cameraState == shakeLeftState && cameraCounter%4==0)
        {
            //frameCamera.MoveRight(-0.1f);
            cameraState = shakeRightState;
        }
        else if(cameraState == shakeRightState && cameraCounter%4==0)
        {
            //frameCamera.MoveRight(0.1f);
            cameraState = shakeLeftState;
        }

        if(role.moveS == 0 && cameraRotateForMove < 15)
        {
            cameraRotateForMove+=1;
        }
        else if(role.moveS == 1 && cameraRotateForMove > -15)
        {
            cameraRotateForMove-=1;
        }
        else if(role.moveS == -1 && cameraRotateForMove != 0)
        {
            if(cameraRotateForMove>0)
                cameraRotateForMove-=1;
            else
                cameraRotateForMove+=1;
        }

        M3DVector3f roleOrigin;
        M3DVector3f cameraOrigin;
        role.frame.GetOrigin(roleOrigin);
        frameCamera.GetOrigin(cameraOrigin);
        glTranslatef(-cameraOrigin[0], -cameraOrigin[1], -cameraOrigin[2]);
        glTranslatef(roleOrigin[0]*role.scaleX, roleOrigin[1]*role.scaleY, roleOrigin[2]*role.scaleZ);
        glRotatef(cameraRotateForMove, 0, 1, 0);
        glTranslatef(-roleOrigin[0]*role.scaleX, -roleOrigin[1]*role.scaleY, -roleOrigin[2]*role.scaleZ);
        glTranslatef(cameraOrigin[0], cameraOrigin[1], cameraOrigin[2]);

        frameCamera.ApplyCameraTransform();
        //if(cameraCounter == -1)
        //{

        //}


        //render sky
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[getTextureNumMap()["backgroundSky"]]);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex3d(-5, -2, -3);

            glTexCoord2f(3, 0);
            glVertex3d(25, -2, -3);

            glTexCoord2f(3, 1);
            glVertex3d(25, 5, -3);

            glTexCoord2f(0, 1);
            glVertex3d(-5, 5, -3);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        //draw all ground
        renderBlock();

        for(int i=0 ; i<demonObject.size(); i++)
        {
            //cout << "demon "<<i;
            glPushMatrix();
                demonObject[i].applyTransformAndScale();
                demonObject[i].draw();

                M3DVector3f origin;
                demonObject[i].frame.GetOrigin(origin);
                if(demonObject[i].state == deadDemonState || origin[1]<-100)
                {
                    demonObject.erase(demonObject.begin()+i);
                }

            glPopMatrix();
        }

        //draw model
        glPushMatrix();
            //glTranslatef(2, 0, 2);
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
            glAlphaFunc(GL_GREATER, 0.5);
            glEnable(GL_ALPHA_TEST);

            glBindTexture(GL_TEXTURE_2D,textures[getTextureNumMap()["leadingRole"]]);

            role.applyTransformAndScale();
            role.draw();

            glDisable(GL_ALPHA_TEST);
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();


    glPopMatrix();

    M3DVector3f origin;
    role.frame.GetOrigin(origin);
    if(origin[1]<-100 || role.health < 0)
    {
        glPushMatrix();
        frameCamera.SetOrigin(0, 0, 0);
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        frameCamera.ApplyCameraTransform();

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[getTextureNumMap()["gameover"]]);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex3d(-5, -5, -10);

            glTexCoord2f(1, 0);
            glVertex3d(5, -5, -10);

            glTexCoord2f(1, 1);
            glVertex3d(5, 3.5, -10);

            glTexCoord2f(0, 1);
            glVertex3d(-5, 3.5, -10);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        cameraState = endState;
    }
    else if(demonObject.size()==0)
    {
        glPushMatrix();
        frameCamera.SetOrigin(0, 0, 0);
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        frameCamera.ApplyCameraTransform();

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[getTextureNumMap()["win"]]);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex3d(-5, -5, -10);

            glTexCoord2f(1, 0);
            glVertex3d(5, -5, -10);

            glTexCoord2f(1, 1);
            glVertex3d(5, 3.5, -10);

            glTexCoord2f(0, 1);
            glVertex3d(-5, 3.5, -10);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        cameraState = endState;
    }

    // Do the buffer Swap
    glutSwapBuffers();
}

void myKeys(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 'w':
        cameraState = startShakeState;
        break;
    case 's':
        break;
    case 'a':
        break;
    case 'd':
        break;

    case 'z':
        role.attack();
        break;
    }
    glutPostRedisplay();
}

// Respond to arrow keys by moving the camera frame of reference
void SpecialKeys(int key, int x, int y)
{
    if(key == GLUT_KEY_UP)
	{
	    //frameCamera.MoveRight(-0.015f);
	    //frameCamera.RotateWorld(-0.03, 0, 1, 0);
        role.jump();
	}

    if(key == GLUT_KEY_DOWN)
	{
        //frameCamera.MoveRight(0.015f);
	}

    if(key == GLUT_KEY_LEFT)
	{
        role.moveS = 1;

	}

    if(key == GLUT_KEY_RIGHT)
	{
        role.moveS = 0;
	}

    // Refresh the Window
    //glutPostRedisplay();
}
void SpecialKeysUp(int key, int x, int y)
{
    if(key == GLUT_KEY_UP)
	{
	    //frameCamera.MoveRight(-0.015f);
	    //frameCamera.RotateWorld(-0.03, 0, 1, 0);
	}

    if(key == GLUT_KEY_DOWN)
	{
        //frameCamera.MoveRight(0.015f);
	}

    if(key == GLUT_KEY_LEFT)
	{
        role.moveS = -1;
	}

    if(key == GLUT_KEY_RIGHT)
	{
        role.moveS = -1;
	}

    // Refresh the Window
    //glutPostRedisplay();
}


///////////////////////////////////////////////////////////
// Called by GLUT library when idle (window not being
// resized or moved)
void TimerFunction(int value)
{

    // Redraw the scene with new coordinates
    if(cameraState == endState)
        return;

    glutPostRedisplay();
    timeElapsed = glutGet(GLUT_ELAPSED_TIME);
    timeCount++;

    if(timeCount%909 == 0 && demonCount < 100)
    {
        cameraState = startShakeState;
        alSourcePlay(source[2]);
        addDemonToWorld(-rand()%70+1);
        addDemonToWorld(-rand()%70+1);
        addDemonToWorld(-rand()%70+1);
        addDemonToWorld(-rand()%70+1);
        addDemonToWorld(-rand()%70+1);
        demonCount+=5;
    }

    glutTimerFunc(33,TimerFunction, 1);
}

void ChangeSize(int w, int h)
{
    GLfloat fAspect;

    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0)
        h = 1;

    glViewport(0, 0, w, h);

    fAspect = (GLfloat)w / (GLfloat)h;

    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the clipping volume
    gluPerspective(35.0f, fAspect, 1.0f, 50.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char* argv[])
{
    alutInit(&argc, argv);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800,600);
    glutCreateWindow("3D Final Project");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutKeyboardFunc(myKeys);
    glutSpecialFunc(SpecialKeys);
    glutSpecialUpFunc(SpecialKeysUp);


    alGenBuffers(NUM_BUFFERS, buffer);
	if(alGetError() != AL_NO_ERROR) {
		printf("- Error creating buffers !!\n");
		exit(1);
	} else 	{
		// printf("Created buffers\n");
	}
	alutLoadWAVFile("music\\dark_descent.wav", &format ,&data, &size, &freq, &al_bool);
	alBufferData(buffer[0], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

    alutLoadWAVFile("music\\swing.wav", &format ,&data, &size, &freq, &al_bool);
    alBufferData(buffer[1], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	alutLoadWAVFile("music\\zombie-17.wav", &format ,&data, &size, &freq, &al_bool);
    alBufferData(buffer[2], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	alGetError(); /* clear error */
	alGenSources(NUM_SOURCES, source);
	if(alGetError() != AL_NO_ERROR) {
		printf("- Error creating sources !!\n");
		exit(2);
	}
	alSourcef(source[0], AL_PITCH, 1.0f);
	alSourcef(source[0], AL_GAIN, 1.0f);
	alSource3f(source[0], AL_POSITION, 0, 0, 0);
	alSource3f(source[0], AL_VELOCITY, 0, 0, 0);
	alSourcei(source[0], AL_BUFFER, buffer[0]); //attach buffer
	alSourcei(source[0], AL_LOOPING, AL_TRUE);

	alSourcef(source[1], AL_PITCH, 1.0f);
	alSourcef(source[1], AL_GAIN, 1.5f);
	alSource3f(source[1], AL_POSITION, 0, 0, 0);
	alSource3f(source[1], AL_VELOCITY, 0, 0, 0);
	alSourcei(source[1], AL_BUFFER, buffer[1]); //attach buffer
	alSourcei(source[1], AL_LOOPING, AL_TRUE);

	alSourcef(source[2], AL_PITCH, 1.0f);
	alSourcef(source[2], AL_GAIN, 1.5f);
	alSource3f(source[2], AL_POSITION, 0, 0, 0);
	alSource3f(source[2], AL_VELOCITY, 0, 0, 0);
	alSourcei(source[2], AL_BUFFER, buffer[2]); //attach buffer
	alSourcei(source[2], AL_LOOPING, AL_TRUE);

    alSourcePlay(source[0]);

    SetupRC();
    glutTimerFunc(33, TimerFunction, 1);

    glutMainLoop();

    return 0;
}

