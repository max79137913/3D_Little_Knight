#include <al\al.h>
#include "al\alut.h"

#include "leadingRole.h"

#include <math.h>
#include <stdio.h>
#include <map>
#include <string>
#include <iostream>
#include <vector>
using namespace std;

class glFrame;

#include "DemonRole.h"

extern int timeCount;
extern map<char*,int> getTextureNumMap();
extern struct BoundingBox getBoundingBox(GLMmodel *model);
struct BoundingBox returnMergeBoundingBox(struct BoundingBox aBD, struct BoundingBox bBD);
extern struct BoundingBox returnNewBoundingBox(struct BoundingBox BD, M3DMatrix44f modelViewMatrix);
extern bool checkCollisionByObjectInGame(ObjectInGame a, ObjectInGame b);
extern bool checkCollisionByBoundingBox(struct BoundingBox aBD, struct BoundingBox bBD);

extern vector<ObjectInGame> groundObject;
extern vector<DemonRole> demonObject;
extern GLFrame    frameCamera;

extern ALuint	source[3];

LeadingRole::LeadingRole()
{
    head = glmReadOBJ("model\\leadingRole\\head.obj");
    body = glmReadOBJ("model\\leadingRole\\body.obj");
    rightHand = glmReadOBJ("model\\leadingRole\\lefthand.obj");
    leftHand = glmReadOBJ("model\\leadingRole\\righthand.obj");
    axe = glmReadOBJ("model\\leadingRole\\axe.obj");
    rightLeg = glmReadOBJ("model\\leadingRole\\leftleg.obj");
    leftLeg = glmReadOBJ("model\\leadingRole\\rightleg.obj");

    glmUnitize(head);
    glmUnitize(body);
    glmUnitize(leftHand);
    glmUnitize(rightHand);
    glmUnitize(axe);
    glmUnitize(leftLeg);
    glmUnitize(rightLeg);

    m3dLoadVector3(headRotate, 0, 0, 0);
    m3dLoadVector3(bodyRotate, 0, 0, 0);
    m3dLoadVector3(leftHandRotate, 0, 0, 0);
    m3dLoadVector3(rightHandRotate, 0, 0, 0);
    m3dLoadVector3(leftLegRotate, 0, 0, 0);
    m3dLoadVector3(rightLegRotate, 0, 0, 0);

    faceRight = true;
    walkLoop = 0;

    xSpeed = 0;
    ySpeed = 0;
    state = normalState;
    health = 100;
    maxHealth = 100;
    moveS = -1;
}

void LeadingRole::initBoundbox()
{
    //define bounding box-\/
    headBD = getBoundingBox(head);
    bodyBD = getBoundingBox(body);
    leftHandBD = getBoundingBox(leftHand);
    rightHandBD = getBoundingBox(rightHand);
    axeBD = getBoundingBox(axe);
    leftLegBD = getBoundingBox(leftLeg);
    rightLegBD = getBoundingBox(rightLeg);

    glPushMatrix();
        glLoadIdentity();
        M3DMatrix44f modelViewMatrix;
        struct BoundingBox newBD;

        glPushMatrix();
            glTranslatef(0.045, -0.38, -0.32);
            glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
            newBD = returnNewBoundingBox(bodyBD, modelViewMatrix);
            bodyBD = newBD;
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-0.325,-0.68,-0.09);
            glScalef(0.8, 0.8, 0.8);
            glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
            newBD = returnNewBoundingBox(rightHandBD, modelViewMatrix);
            rightHandBD = newBD;
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-0.325,-0.68,-0.09);
            glScalef(0.8, 0.8, 0.8);
            glTranslatef(-0.025,-0.33,0.75);
            glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
            newBD = returnNewBoundingBox(axeBD, modelViewMatrix);
            axeBD = newBD;
            axeBD.xMin-=0.5;
            axeBD.xMax+=0.5;
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.33,-0.036,-0.07);
            glScalef(1.47, 1.47, 1.47);
            glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
            newBD = returnNewBoundingBox(leftHandBD, modelViewMatrix);
            leftHandBD = newBD;
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-0.12,-1.1,0);
            glScalef(0.18, 0.18, 0.18);
            glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
            newBD = returnNewBoundingBox(rightLegBD, modelViewMatrix);
            rightLegBD = newBD;
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.12,-1.1,0);
            glScalef(0.18, 0.18, 0.18);
            glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
            newBD = returnNewBoundingBox(leftLegBD, modelViewMatrix);
            leftLegBD = newBD;
        glPopMatrix();
    glPopMatrix();
    boundingBox = returnMergeBoundingBox(headBD, bodyBD);
    boundingBox = returnMergeBoundingBox(boundingBox, leftHandBD);
    boundingBox = returnMergeBoundingBox(boundingBox, rightHandBD);
    boundingBox = returnMergeBoundingBox(boundingBox, leftLegBD);
    boundingBox = returnMergeBoundingBox(boundingBox, rightLegBD);

    boundingBox.zMin += 0.9;
    boundingBox.zMax -= 0.9;
    //define bounding box complete-^
}

void LeadingRole::attack()
{
    if(state == normalState)
    {
        state = attackState1;
    }
    else if(state == attackState1 && rightHandRotate[0] >=-80)
    {
        rightHandRotate[0]-=10;
    }
    else if(state == attackState2 && rightHandRotate[0] <0)
    {
        rightHandRotate[0]+=20;
        bool demonCollision = false;
        for(int i=0; i<demonObject.size(); i++)
        {
            //cout << test2.boundingBox.yMin << ";" << groundObject[i].boundingBox.yMax << endl;
            glPushMatrix();
                glLoadIdentity();
                M3DMatrix44f modelViewMatrix;

                struct BoundingBox newBD;
                struct BoundingBox demonBD;

                applyTransformAndScale();
                //glRotated(rightHandRotate[0], 1, 0, 0);

                glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
                newBD = returnNewBoundingBox(axeBD, modelViewMatrix);

                glLoadIdentity();
                demonObject[i].applyTransformAndScale();

                glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
                demonBD = returnNewBoundingBox(demonObject[i].boundingBox, modelViewMatrix);
            glPopMatrix();

            if(checkCollisionByBoundingBox(newBD, demonBD))
                demonCollision = true;
            if(demonCollision == true)
            {
                demonObject[i].beingAttacked();
                demonObject[i].health -= 10;
                cout << "demon" << endl;
                demonCollision = false;
            }
        }

    }
    else if(state == attackState1 && rightHandRotate[0] < -80)
    {
        alSourcePlay(source[1]);
        state = attackState2;
    }
    else if(state == attackState2 && rightHandRotate[0] >=0)
    {
        alSourceStop(source[1]);
        state = normalState;
    }
}

void LeadingRole::jump()
{
    if(state == normalState)
    {
        ySpeed = 0.8;
        state = jumpState;
        rightHandRotate[0] = 45;
        leftLegRotate[0] = leftHandRotate[0] = -rightHandRotate[0];
        rightLegRotate[0] = rightHandRotate[0];
    }
    else if(state == jumpState)
    {
        ySpeed = ySpeed-0.1;
    }

    frame.MoveUp(ySpeed);
    frameCamera.MoveUp(ySpeed*scaleY);

    bool groundCollision = false;
    for(int i=0; i<groundObject.size(); i++)
    {
        //cout << test2.boundingBox.yMin << ";" << groundObject[i].boundingBox.yMax << endl;
        if(checkCollisionByObjectInGame(*this, groundObject[i]))
            groundCollision = true;
    }
    if(groundCollision == true)
    {
        frame.MoveUp(-ySpeed);
        frameCamera.MoveUp(-ySpeed*scaleY);

        if(ySpeed < 0)
        {
            state = normalState;
            rightHandRotate[0] = 0;
            leftLegRotate[0] = leftHandRotate[0] = -rightHandRotate[0];
            rightLegRotate[0] = rightHandRotate[0];
            return;
        }

        ySpeed = 0;
    }

    //glutTimerFunc(33, jump, i+1);
    //glutPostRedisplay();
}

void LeadingRole::walkFoward(float step)
{
    if(step < 0)
    {
        if(faceRight == true)
        {
            faceRight = !faceRight;
            frame.RotateWorld(M_PI, 0, 1, 0);
        }
        frame.MoveForward(-step);
        frameCamera.MoveRight(-step*scaleX);
        if(collisionGround() == true)
        {
            frame.MoveForward(step);
            frameCamera.MoveRight(step*scaleX);
            return;
        }

        if(walkLoop == 0 && rightHandRotate[0] >= -45 && state == normalState)
        {
            rightHandRotate[0] -= 5;
        }
        else if(walkLoop == 1 && rightHandRotate[0] <= 45 && state == normalState)
        {
            rightHandRotate[0] += 5;
        }
        else if(state == normalState)
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
        frameCamera.MoveRight(-step*scaleX);
        if(collisionGround() == true)
        {
            frame.MoveForward(-step);
            frameCamera.MoveRight(step*scaleX);
            return;
        }

        if(walkLoop == 0 && rightHandRotate[0] <= 45 && state == normalState)
        {
            rightHandRotate[0] += 5;
        }
        else if(walkLoop == 1 && rightHandRotate[0] >= -45 && state == normalState)
        {
            rightHandRotate[0] -= 5;
        }
        else if(state == normalState)
        {
            walkLoop = !walkLoop;
        }
    }
    leftLegRotate[0] = leftHandRotate[0] = -rightHandRotate[0];
    rightLegRotate[0] = rightHandRotate[0];
}

void LeadingRole::draw()
{
    if(timeCount%100 == 0 && health<maxHealth)
    {
        health+=10;
    }

    if(state == jumpState)
    {
        jump();
    }
    else if(state == beingAttackedState)
    {
        beingAttacked();
    }
    else if(state == attackState1 || state == attackState2)
    {
        attack();
    }
    else
    {
        ySpeed = ySpeed-0.1;
        frame.MoveUp(ySpeed);
        frameCamera.MoveUp(ySpeed*scaleY);

        bool groundCollision = collisionGround();
        if(groundCollision == true)
        {
            frame.MoveUp(-ySpeed);
            frameCamera.MoveUp(-ySpeed*scaleY);

            ySpeed = 0;
        }
    }

    if(moveS == 0)
    {
        walkFoward(0.15f);
    }
    else if(moveS == 1)
    {
        walkFoward(-0.15f);
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    extern GLuint  textures[1];
    int textureNum = getTextureNumMap()["leadingRole"];
    glBindTexture(GL_TEXTURE_2D, textures[textureNum]);

    glPushMatrix();

        //health line
        glPushMatrix();
            glTranslatef(0, 1, -0.5);
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

        glmDraw(head, GLM_SMOOTH | GLM_TEXTURE);

        glPushMatrix();
            glTranslatef(0.045, -0.38, -0.32);
            glmDraw(body, GLM_SMOOTH | GLM_TEXTURE);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-0.325,-0.68,-0.09);
            glScalef(0.8, 0.8, 0.8);

            glTranslatef(0.2, 0.3, 0);
            glRotatef(rightHandRotate[0], 1, 0, 0);
            glTranslatef(-0.2, -0.3, 0);
            glmDraw(rightHand, GLM_SMOOTH | GLM_TEXTURE);

            glPushMatrix();
                glTranslatef(-0.025,-0.33,0.75);
                glmDraw(axe, GLM_SMOOTH | GLM_TEXTURE);
            glPopMatrix();

        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.33,-0.036,-0.07);
            glScalef(1.47, 1.47, 1.47);

            glTranslatef(-0, -0.25, 0);
            glRotatef(leftHandRotate[0], 1, 0, 0);
            glTranslatef(0, 0.25, 0);
            glmDraw(leftHand, GLM_SMOOTH | GLM_TEXTURE);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-0.12,-1.1,0);
            glScalef(0.18, 0.18, 0.18);

            glTranslatef(0, 1, 0);
            glRotatef(rightLegRotate[0], 1, 0, 0);
            glTranslatef(0, -1, 0);
            glmDraw(rightLeg, GLM_SMOOTH | GLM_TEXTURE);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.12,-1.1,0);
            glScalef(0.18, 0.18, 0.18);

            glTranslatef(0, 1, 0);
            glRotatef(leftLegRotate[0], 1, 0, 0);
            glTranslatef(0, -1, 0);
            glmDraw(leftLeg, GLM_SMOOTH | GLM_TEXTURE);
        glPopMatrix();

    glPopMatrix();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void LeadingRole::beingAttacked()
{
    if(state == normalState)
    {
        state = beingAttackedState;
        ySpeed = 0.5;
        xSpeed = 0.3;
    }
    if(state == beingAttackedState)
    {
        ySpeed = ySpeed-0.1;
        frame.MoveUp(ySpeed);
        frameCamera.MoveUp(ySpeed*scaleY);

        if(faceRight == true)
        {
            frame.MoveForward(-xSpeed);
            frameCamera.MoveRight(xSpeed*scaleX);
        }
        else
        {
            frame.MoveForward(-xSpeed);
            frameCamera.MoveRight(-xSpeed*scaleX);
        }

        bool collision = collisionGround();
        if(collision == true)
        {
            frame.MoveUp(-ySpeed);
            frameCamera.MoveUp(-ySpeed*scaleY);

            while(collision == true)
            {
                frame.MoveUp(0.1);
                frameCamera.MoveUp(0.1*scaleY);
                collision = collisionGround();
            }

            xSpeed = 0;
            ySpeed = 0;
            state = normalState;
            //cout << "normal" << endl;
        }
    }
}


bool LeadingRole::collisionGround()
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

LeadingRole::~LeadingRole()
{
    free(head);
    free(body);
    free(leftHand);
    free(rightHand);
    free(axe);
    free(leftLeg);
    free(rightLeg);
}
