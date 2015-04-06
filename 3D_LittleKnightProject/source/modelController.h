#include <map>
#include <string>
using namespace std;

#include "library/glm.h"
#include "library/math3d.h"

GLMmodel *Demon_head_and_body;
GLMmodel *Demon_leftHand;
GLMmodel *Demon_rightHand;
GLMmodel *Demon_wings;
GLMmodel *Demon_leftLeg;
GLMmodel *Demon_rightLeg;


void setUpModel()
{
    Demon_head_and_body = glmReadOBJ("model\\demon\\head_and_body.obj");
    Demon_rightHand = glmReadOBJ("model\\demon\\lefthand.obj");
    Demon_leftHand = glmReadOBJ("model\\demon\\righthand.obj");
    Demon_wings = glmReadOBJ("model\\demon\\wings.obj");
    Demon_rightLeg = glmReadOBJ("model\\demon\\leftleg.obj");
    Demon_leftLeg = glmReadOBJ("model\\demon\\rightleg.obj");

    glmUnitize(Demon_head_and_body);
    glmUnitize(Demon_leftHand);
    glmUnitize(Demon_rightHand);
    glmUnitize(Demon_wings);
    glmUnitize(Demon_leftLeg);
    glmUnitize(Demon_rightLeg);
}
