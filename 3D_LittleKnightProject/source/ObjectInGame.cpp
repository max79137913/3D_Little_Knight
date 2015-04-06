#include "ObjectInGame.h"

ObjectInGame::ObjectInGame()
{
    boundingBox.xMax = 0;
    boundingBox.xMin = 0;
    boundingBox.yMax = 0;
    boundingBox.yMin = 0;
    boundingBox.zMax = 0;
    boundingBox.zMin = 0;

    frame.SetOrigin(0, 0, 0);

    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    className = "ObjectInGame";
}

void ObjectInGame::applyTransformAndScale()
{
    glScalef(scaleX, scaleY, scaleZ);
    frame.ApplyActorTransform();
}
