#ifndef collisionController_h
#define collisionController_h

#include "ObjectInGame.h"
#include "library/math3d.h"
#include <algorithm>

struct BoundingBox boxes[5];
int boxindex = 0;

void DefineCollisionBoxes(GLMmodel *model, char *part_of_the_name);

bool checkCollisionByBoundingBox(struct BoundingBox aBD, struct BoundingBox bBD)
{
    return !(aBD.xMin >= bBD.xMax || aBD.xMax <= bBD.xMin ||
             aBD.yMin >= bBD.yMax || aBD.yMax <= bBD.yMin ||
             aBD.zMin >= bBD.zMax || aBD.zMax <= bBD.zMin);
}

struct BoundingBox returnMergeBoundingBox(struct BoundingBox aBD, struct BoundingBox bBD)
{
    struct BoundingBox newBD;
    newBD.xMax = max(aBD.xMax, bBD.xMax);
    newBD.xMin = min(aBD.xMin, bBD.xMin);
    newBD.yMax = max(aBD.yMax, bBD.yMax);
    newBD.yMin = min(aBD.yMin, bBD.yMin);
    newBD.zMax = max(aBD.zMax, bBD.zMax);
    newBD.zMin = min(aBD.zMin, bBD.zMin);
    return newBD;
}

struct BoundingBox returnNewBoundingBox(struct BoundingBox BD, M3DMatrix44f modelViewMatrix)
{
    M3DVector3f points[8];
    M3DVector3f pointsT[8];

    struct BoundingBox newBD;

    //建出bounding box的長方體

    m3dLoadVector3(points[0], BD.xMin, BD.yMin, BD.zMin);
    m3dLoadVector3(points[1], BD.xMax, BD.yMin, BD.zMin);
    m3dLoadVector3(points[2], BD.xMin, BD.yMin, BD.zMax);
    m3dLoadVector3(points[3], BD.xMax, BD.yMin, BD.zMax);
    m3dLoadVector3(points[4], BD.xMin, BD.yMax, BD.zMin);
    m3dLoadVector3(points[5], BD.xMax, BD.yMax, BD.zMin);
    m3dLoadVector3(points[6], BD.xMin, BD.yMax, BD.zMax);
    m3dLoadVector3(points[7], BD.xMax, BD.yMax, BD.zMax);

    for(int i=0 ; i<8 ; i++)
    {
        m3dTransformVector3(pointsT[i], points[i], modelViewMatrix);
    }
    for(int i=0 ; i<8 ; i++)
    {
        if(i==0)
        {
            newBD.xMax = newBD.xMin = pointsT[0][0];
            newBD.yMax = newBD.yMin = pointsT[0][1];
            newBD.zMax = newBD.zMin = pointsT[0][2];
        }
        else
        {
            newBD.xMax = max(newBD.xMax, pointsT[i][0]);
            newBD.yMax = max(newBD.yMax, pointsT[i][1]);
            newBD.zMax = max(newBD.zMax, pointsT[i][2]);
            newBD.xMin = min(newBD.xMin, pointsT[i][0]);
            newBD.yMin = min(newBD.yMin, pointsT[i][1]);
            newBD.zMin = min(newBD.zMin, pointsT[i][2]);
        }
    }
    return newBD;
}

bool checkCollisionByObjectInGame(ObjectInGame a, ObjectInGame b)
{
    glPushMatrix();
        glLoadIdentity();

        M3DMatrix44f modelViewMatrix;

        struct BoundingBox aNewBD;
        a.applyTransformAndScale();
        glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
        aNewBD = returnNewBoundingBox(a.boundingBox, modelViewMatrix);

        glLoadIdentity();

        struct BoundingBox bNewBD;
        b.applyTransformAndScale();
        glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
        bNewBD = returnNewBoundingBox(b.boundingBox, modelViewMatrix);

        //cout << modelViewMatrix[0] << " " << modelViewMatrix[4] << " " << modelViewMatrix[8] << " " << modelViewMatrix[12] << endl;
         //cout << modelViewMatrix[1] << " " << modelViewMatrix[5] << " " << modelViewMatrix[9] << " " << modelViewMatrix[13] << endl;
         //cout << modelViewMatrix[2] << " " << modelViewMatrix[6] << " " << modelViewMatrix[10] << " " << modelViewMatrix[14] << endl;
         //cout << modelViewMatrix[3] << " " << modelViewMatrix[7] << " " << modelViewMatrix[11] << " " << modelViewMatrix[15] << endl;

        //cout << aNewBD.xMax << ";" << bNewBD.xMin << endl;
    glPopMatrix();

    return checkCollisionByBoundingBox(aNewBD, bNewBD);
}

struct BoundingBox getBoundingBox(GLMmodel *model)
{
    boxindex = 0;
    DefineCollisionBoxes(model, "");
    for(int i=0; i<boxindex; i++)
    {
        if(boxes[i].xMax != -100)
        {
            cout << i << "f" << boxes[i].xMax << endl;
            return boxes[i];
        }
    }
}

struct BoundingBox CreateCollisionBox(GLMmodel *model, GLMgroup * object)
{
    struct BoundingBox box;
    box.xMax = -100;
    box.xMin = 100;
    box.yMax = -100;
    box.yMin = 100;
    box.zMax = -100;
    box.zMin = 100;
// GLM doesn't store each vertex together with the object that owns it. It doesn't have that notion. In GLM object don't have vertex, they have triangles. And each triangle is actually an index in the triangle list of the object.
    for(int i = 0; i < object->numtriangles; i++)
    {
// for each vertex of the triangle pmodel1->triangles[object->triangles[i]]
// calculate min and max
        for (int j=0;j<3;j++)
        {
            GLuint index = model->triangles[object->triangles[i]].vindices[j];
            GLfloat x = model->vertices[index*3 + 0];
            GLfloat y = model->vertices[index*3 + 1];
            GLfloat z = model->vertices[index*3 + 2];

            if (box.xMin>x) box.xMin = x;
            if (box.yMin>y) box.yMin = y;
            if (box.zMin>z) box.zMin = z;

            if (box.xMax<x) box.xMax = x;
            if (box.yMax<y) box.yMax = y;
            if (box.zMax<z) box.zMax = z;
        }
    }

    return box;
}

void AddCollisionBox(GLMmodel *model, GLMgroup * object)
{
	boxes[boxindex] = CreateCollisionBox(model,object);
	boxindex++;
}

void DefineCollisionBoxes(GLMmodel *model, char *part_of_the_name)
{
	GLMgroup *group = model->groups;
	while(group)
	{
		if (strstr(group->name,part_of_the_name))
			AddCollisionBox(model,group);
		group = group->next;
	}
}

#endif
