#ifndef ObjectInGame_h
#define ObjectInGame_h

#include <string>
using namespace std;

#include "library/gltools.h"	// OpenGL toolkit
#include "library/glframe.h"   // Frame Class

struct BoundingBox {
   float xMin, xMax, yMin, yMax, zMin, zMax;
};

class ObjectInGame
{
    public:
        ObjectInGame();
        void applyTransformAndScale();

        struct BoundingBox boundingBox;
        GLFrame frame; //�N��ثeobject�b�@�ɮy�ФW����m
        float scaleX;
        float scaleY;
        float scaleZ;
        string className;
};

#endif
