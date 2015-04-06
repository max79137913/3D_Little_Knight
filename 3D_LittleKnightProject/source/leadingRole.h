#ifndef leadingRole_h
#define leadingRole_h

#include "ObjectInGame.h"
#include "library/glm.h"
#include "library/math3d.h"

enum LeadingRoleState {
    normalState,
    jumpState,
    attackState1,
    attackState2,
    beingAttackedState
};

class LeadingRole : public ObjectInGame
{
    public:
        LeadingRole();
        ~LeadingRole();
        void initBoundbox();
        void draw();
        void jump();
        void attack();
        void walkFoward(float step);
        bool collisionGround();
        void beingAttacked();
        int moveS;

        int health;
        int maxHealth;

    private:
        float xSpeed;
        float ySpeed;

        bool faceRight;
        bool walkLoop;

        LeadingRoleState state;

        GLMmodel *head;
        M3DVector3f headRotate;
        struct BoundingBox headBD;

        GLMmodel *body;
        M3DVector3f bodyRotate;
        struct BoundingBox bodyBD;

        GLMmodel *leftHand;
        M3DVector3f leftHandRotate;
        struct BoundingBox leftHandBD;

        GLMmodel *rightHand;
        M3DVector3f rightHandRotate;
        struct BoundingBox rightHandBD;

        GLMmodel *axe;
        struct BoundingBox axeBD;

        GLMmodel *leftLeg;
        M3DVector3f leftLegRotate;
        struct BoundingBox leftLegBD;

        GLMmodel *rightLeg;
        M3DVector3f rightLegRotate;
        struct BoundingBox rightLegBD;
};

#endif
