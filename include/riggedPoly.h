#pragma once
#include "components.h"
#include "assets.h"
#include "collisionFuncs.h" // only used for smallestAngle function
#include <queue>

class Joint{
    private:
        Polygon* poly;
    
    public:
        complex<double> posRelAsset;
        RigidBody rb;

        //used to change distance from join rb 
        double scale;
        // indices of vertexes puppeted by joint
        vector<int> attachedVerts;
        int numVerticies;

        void init(Polygon* poly, vector<int>* indices,complex<double> initPos);
        void update(double dt);
        void render();

};

class RiggedPoly{
    public:
        Polygon poly;
        vector<Joint*> joints;
        int numJoints;

        // visual assets attached to the joints
        vector<VisualPolygon*> visPolys;
        int numVisPolys;

        void init(vector<complex<double>>* polyAsset, vector<jointInfo>* jointData, tuple<int,int,int> color, int ID);

        void update(double dt);
        void render(Screen* screen);
};

// joints must be fed in head to tail
// accomidates snakes of variable segment width and length
// requires fixed framerate
class Snake{
    private:
        vector<complex<double>> pathTraveled;
        int pathTraveledLen;

        int headIndex;
        //where on the path each vertex is relative to the head index
        //i th vertex index = headIndex + vertexIndexOffsets[i]
        vector<int> vertexIndexOffsets;

        vector<double> segmentLens;
        
        // will need setter for adjustable speed
        double speed;
        double turnSpeed;
    public:
        RiggedPoly rp;

        double rot;


        void init(vector<complex<double>>* polyAsset, vector<jointInfo>* jointData, 
                    tuple<int,int,int> color, double speed, double turnSpeed, int ID,double spf);

        void spawn(complex<double> pos, double rot,double spf);

        // turn in number between -1 and 1 for 
        void update(double turn, double dt);
        void render(Screen* screen);
        
};


//**************//
//Skeleton Stuff//
//**************//

// used to avoid tree traversal during runtime
struct link{
    int headIndex;
    int tailIndex;
    double rigidity;
    complex<double> relPos;
    double relAngle;
    double len;
};
// put into vector, ith object corrisponds to ith joint in rigidpoly
// holds infomration on which other joints are connected
// used to adjust rotation and joint scaling to maintain constant width
struct jointLink{
    // index of joint puppeting this one
    int base;
    double relAngle;
    // indices of joints this joint puppets
    vector<int> branches;
    int numBranches;
};
// one base joint which controls movment other joints follow with a preference to remain straight
class Skeleton{
    private:
        vector<link*> links;
        vector<jointLink*> jointLinks;
        int numLinks;

    public:
        RiggedPoly rp;
        
        // points to head joints rigidbody
        RigidBody* rb;

        void init(vector<complex<double>>* polyAsset, vector<jointInfo>* jointData, 
                 vector<linkInfo>* linking, tuple<int,int,int> color,int ID);

        void spawn(complex<double> pos, double rot);

        void update(double dt);
        void render(Screen* screen);
};