#ifndef DemonRole_cpp
#define DemonRole_cpp

#include "DemonRole.h"
#include "LeadingRole.h"

#include <math.h>
#include <stdio.h>
#include <map>
#include <string>
#include <iostream>
#include <vector>
using namespace std;

extern map<char*,int> getTextureNumMap();
extern struct BoundingBox getBoundingBox(GLMmodel *model);
extern struct BoundingBox returnMergeBoundingBox(struct BoundingBox aBD, struct BoundingBox bBD);
extern struct BoundingBox returnNewBoundingBox(struct BoundingBox BD, M3DMatrix44f modelViewMatrix);
extern bool checkCollisionByObjectInGame(ObjectInGame a, ObjectInGame b);
extern vector<ObjectInGame> groundObject;

extern GLMmodel *Demon_head_and_body;
extern GLMmodel *Demon_leftHand;
extern GLMmodel *Demon_rightHand;
extern GLMmodel *Demon_wings;
extern GLMmodel *Demon_leftLeg;
extern GLMmodel *Demon_rightLeg;

extern LeadingRole role;

DemonRole::DemonRole()
{
    m3dLoadVector3(head_and_bodyRotate, 0, 0, 0);
    m3dLoadVector3(leftHandRotate, 0, 0, 0);
    m3dLoadVector3(rightHandRotate, 0, 0, 0);
    m3dLoadVector3(leftLegRotate, 0, 0, 0);
    m3dLoadVector3(rightLegRotate, 0, 0, 0);

    inGround = 0;
    faceRight = 1;
    walkLoop = 0;
    walkRight = 1;
    health = 100;
    maxHealth = 100;

    state = normalDemonState;
}

void DemonRole::walkFoward(float step)
{
    if(step < 0)
    {
        if(faceRight == true)
        {
            faceRight = !faceRight;
            frame.RotateWorld(M_PI, 0, 1, 0);
        }
        frame.MoveForward(-step);
        if(walkLoop == 0 && rightHandRotate[0] >= -45 && state == normalDemonState)
        {
            rightHandRotate[0] -= 5;
        }
        else if(walkLoop == 1 && rightHandRotate[0] <= 45 && state == normalDemonState)
        {
            rightHandRotate[0] += 5;
        }
        else if(state == normalDemonState)
        {
            walkLoop = !walkLoop;
        }
    }
    else
    {
        if(faceRight == false)
        {
            faceRight = !faceRight;
            frame.RotateWorld(-M_PI, 0, 1, 0);
        }
        frame.MoveForward(step);
        if(walkLoop == 0 && rightHandRotate[0] <= 45 && state == normalDemonState)
        {
            rightHandRotate[0] += 5;
        }
        else if(walkLoop == 1 && rightHandRotate[0] >= -45 && state == normalDemonState)
        {
            rightHandRotate[0] -= 5;
        }
        else if(state == normalDemonState)
        {
            walkLoop = !walkLoop;
        }
    }
    leftLegRotate[0] = leftHandRotate[0] = -rightHandRotate[0];
    rightLegRotate[0] = rightHandRotate[0];
}


void DemonRole::initBoundbox()
{
    //define bounding box-\/

    head_and_bodyBD = getBoundingBox(Demon_head_and_body);
    leftHandBD = getBoundingBox(Demon_leftHand);
    rightHandBD = getBoundingBox(Demon_rightHand);
    wingsBD = getBoundingBox(Demon_wings);
    leftLegBD = getBoundingBox(Demon_leftLeg);
    rightLegBD = getBoundingBox(Demon_rightLeg);

    glPushMatrix();
        glLoadIdentity();
        M3DMatrix44f modelViewMatrix;
        struct BoundingBox newBD;

        //glScalef(scaleX, scaleY, scaleZ);

        /*glPushMatrix();
            glTranslatef(0.045, -0.38, -0.32);
            glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
            newBD = returnNewBoundingBox(head_and_bodyBD, modelViewMatrix);
            head_and_bodyBD = newBD;
        glPopMatrix();*/
        glPushMatrix();
            glTranslatef(-0.1,0.0,-0.14);
            glScalef(0.5, 0.5, 0.5);
            glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
            newBD = returnNewBoundingBox(rightHandBD, modelViewMatrix);
            rightHandBD = newBD;
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.25,0,-0.2);
            glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
            newBD = returnNewBoundingBox(wingsBD, modelViewMatrix);
            wingsBD = newBD;
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.07,-0.07,-0.15);
            glScalef(0.5, 0.5, 0.5);
            glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
            newBD = returnNewBoundingBox(leftHandBD, modelViewMatrix);
            leftHandBD = newBD;
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-0,-0.75,-0.1);
            glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
            newBD = returnNewBoundingBox(rightLegBD, modelViewMatrix);
            rightLegBD = newBD;
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.03,-0.7,-0.084);
            glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
            newBD = returnNewBoundingBox(leftLegBD, modelViewMatrix);
            leftLegBD = newBD;
        glPopMatrix();
    glPopMatrix();
    boundingBox = returnMergeBoundingBox(head_and_bodyBD, leftHandBD);
    boundingBox = returnMergeBoundingBox(boundingBox, rightHandBD);
    boundingBox = returnMergeBoundingBox(boundingBox, leftLegBD);
    boundingBox = returnMergeBoundingBox(boundingBox, rightLegBD);
    //define bounding box complete-^
}

void DemonRole::draw()
{
    if(state == normalDemonState)
    {
        if(inGround == 0)
        {
            frame.MoveUp(-0.1);
            bool groundCollision = false;
            for(int i=0; i<groundObject.size(); i++)
            {
                //cout << test2.boundingBox.yMin << ";" << groundObject[i].boundingBox.yMax << endl;
                if(checkCollisionByObjectInGame(*this, groundObject[i]))
                    groundCollision = true;
            }
            if(groundCollision == true)
            {
                frame.MoveUp(0.1);
                inGround = 1;
            }
        }

        if(walkRight == 1 && inGround == 1)
        {
            walkFoward(0.05f);
            //frame.MoveUp(-0.1);
            bool groundCollision = collisionGround();
            if(groundCollision == true)
            {
                walkFoward(-0.05f);
                walkRight = 0;
            }
            //frame.MoveUp(0.1);
        }
        else if(inGround == 1)
        {
            walkFoward(-0.05f);
            //frame.MoveUp(-0.1);
            bool groundCollision = collisionGround();
            if(groundCollision == true)
            {
                walkFoward(0.05f);
                walkRight = 1;
            }
        //frame.MoveUp(0.1);
        }

        ySpeed = ySpeed-0.1;
        frame.MoveUp(ySpeed);

        bool groundCollision = collisionGround();
        if(groundCollision == true)
        {
            frame.MoveUp(-ySpeed);

            ySpeed = 0;
        }

        if(checkCollisionByObjectInGame(*this, role))
        {
            role.beingAttacked();
            role.health -= 10;
        }
    }
    else if(state == beingAttackedDemonState)
    {
        beingAttacked();
    }
    else if(state == deadDemonState)
    {
        return;
    }

    if(health == 0)
    {
        state = deadDemonState;
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    extern GLuint  textures[4];
    int textureNum = 3;
    glBindTexture(GL_TEXTURE_2D, textures[textureNum]);

    glPushMatrix();
    //glRotated(90, 0,1,0);
        glPushMatrix();
            glTranslatef(0, 1.5, -0.5);
            glColor3f(1, 0, 0);
            double lineLength = 1.2*health/maxHealth;
            glBegin(GL_QUADS);
                glVertex3d(0, -0.1, 0);
                glVertex3d(0, -0.1, lineLength);
                glVertex3d(0, 0.1, lineLength);
                glVertex3d(0, 0.1, 0);
            glEnd();
            glColor3f(1, 1, 1);
        glPopMatrix();

        glmDraw(Demon_head_and_body, GLM_SMOOTH | GLM_TEXTURE);
            //cout << "draw " << Demon_head_and_body << endl;

        glPushMatrix();
            glTranslatef(-0.1,0.0,-0.14);
            glScalef(0.5, 0.5, 0.5);

            glTranslatef(0, 0.2, 0);
            glRotatef(rightHandRotate[0], 1, 0, 0);
            glTranslatef(0, -0.2, 0);
            glmDraw(Demon_rightHand, GLM_SMOOTH | GLM_TEXTURE);

        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.25,0,-0.2);
            glRotatef(10, 0, 1, 0);
            glmDraw(Demon_wings, GLM_SMOOTH | GLM_TEXTURE);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.07,-0.07,-0.15);
            glScalef(0.5, 0.5, 0.5);

            glTranslatef(0, 0.3, 0);
            glRotatef(leftHandRotate[0], 1, 0, 0);
            glTranslatef(0, -0.3, 0);
            glmDraw(Demon_leftHand, GLM_SMOOTH | GLM_TEXTURE);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-0,-0.75,-0.1);
            //glScalef(0.18, 0.18, 0.18);

            glTranslatef(0, 0.15, 0);
            glRotatef(rightLegRotate[0], 1, 0, 0);
            glTranslatef(0, -0.15, 0);
            glmDraw(Demon_rightLeg, GLM_SMOOTH | GLM_TEXTURE);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.03,-0.7,-0.084);
            //glScalef(0.18, 0.18, 0.18);

            glTranslatef(0, 0.15, 0);
            glRotatef(leftLegRotate[0], 1, 0, 0);
            glTranslatef(0, -0.15, 0);
            glmDraw(Demon_leftLeg, GLM_SMOOTH | GLM_TEXTURE);
        glPopMatrix();

    glPopMatrix();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void DemonRole::beingAttacked()
{
    if(state == normalDemonState)
    {
        state = beingAttackedDemonState;
        ySpeed = 0.5;

        M3DVector3f roleOrigin;
        role.frame.GetOrigin(roleOrigin);
        M3DVector3f selfOrigin;
        frame.GetOrigin(selfOrigin);

        //cout << selfOrigin[0] <<" "<<roleOrigin[0]<<endl;

        if(selfOrigin[0]*scaleX<roleOrigin[0]*role.scaleX)
        {

            if(faceRight == 1)
                xSpeed = -0.3;
            else
                xSpeed = 0.3;
        }
        else
        {

            if(faceRight == 1)
                xSpeed = 0.3;
            else
                xSpeed = -0.3;
        }

    }
    if(state == beingAttackedDemonState)
    {
        //cout << ySpeed;
        ySpeed = ySpeed-0.1;
        frame.MoveUp(ySpeed);
        frame.MoveForward(xSpeed);
        inGround = 0;
        bool collision = collisionGround();
        if(collision == true)
        {
            frame.MoveUp(-ySpeed);
            xSpeed = 0;
            ySpeed = 0;

            while(collision == true)
            {
                frame.MoveUp(0.1);
                collision =  collisionGround();
            }
            state = normalDemonState;
        }
    }
}

bool DemonRole::collisionGround()
{
    bool groundCollision = false;
    for(int i=0; i<groundObject.size(); i++)
    {
            //cout << test2.boundingBox.yMin << ";" << groundObject[i].boundingBox.yMax << endl;
        if(checkCollisionByObjectInGame(*this, groundObject[i]))
            groundCollision = true;
    }
    return groundCollision;
}

DemonRole::~DemonRole()
{
    ;
}

#endif
