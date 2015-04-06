#ifndef DemonRole_h
#define DemonRole_h

#include "ObjectInGame.h"
#include "library/glm.h"
#include "library/math3d.h"

enum DemonRoleState {
    normalDemonState,
    beingAttackedDemonState,
    deadDemonState
};

class DemonRole : public ObjectInGame
{
    public:
        DemonRole();
        ~DemonRole();
        void draw();
        void walkFoward(float step);
        void initBoundbox();
        bool collisionGround();
        void beingAttacked();

        bool inGround;
        bool walkRight;
        int health;
        int maxHealth;
        float xSpeed;
        float ySpeed;
        DemonRoleState state;

    private:
        bool faceRight;
        bool walkLoop;

        M3DVector3f head_and_bodyRotate;
        struct BoundingBox head_and_bodyBD;

        M3DVector3f leftHandRotate;
        struct BoundingBox leftHandBD;

        M3DVector3f rightHandRotate;
        struct BoundingBox rightHandBD;

        struct BoundingBox wingsBD;

        M3DVector3f leftLegRotate;
        struct BoundingBox leftLegBD;

        M3DVector3f rightLegRotate;
        struct BoundingBox rightLegBD;
};

#endif

