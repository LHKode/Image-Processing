#ifndef __Skeletonh
#define __Skeletonh

#include <string>
#include <fstream>
#include <iostream>
#include <string>

#include <Renderable.h>
#include <Movements.h>
#include <Mesh.h>

using namespace std;

class SkeletonNode
  {
    public:
      string name;
      // length of this bone.
      Vector offset;
      // shape
      Mesh * shape;
      // absolute position, in rest state.
      Point position;
      // current transformation  (only if explicitly created)
      Transformation actual;
      // flag for using bvh offset.
      int hasoffset;

      // nodes connected to dis bone.
      int numChildren;
      SkeletonNode * * children;
      SkeletonNode * parent;

      // correction factor used to reorient
      // bones to match those on other skeletons.
      Quaternion changeOri;
      
      SkeletonNode (string nme);
      void addChild (SkeletonNode * c);
  };

class Skeleton : public Renderable
  {
    protected:
      SkeletonNode * root;

      Movements * mov;
      int frame;

    public:
      Skeleton ();
      SkeletonNode * findSkeletonNode (string name);
      SkeletonNode * findSkeletonNode (SkeletonNode * root, string name);

      // adds a new node to the skeleton, returns a pointer to this
      // node.
      SkeletonNode * addBone (string name, string parent);
      // adds a new node to the skeleton returning the pointer to the
      // node. Position of the node is set to the given absolute position.
      SkeletonNode * addBone (string name, string parent, Point position);
      
      void setFrame (int f);
      void setMovements (Movements * m);

      void renderSkeleton (SkeletonNode * root, int frame, Movements * mov, int solid, int wire, int norms);
      void render ();
      void renderWireframe ();
      void renderNormals ();
      
      friend void writeBVH (string filename, Skeleton & skel, Movements * mov);
      
      class iterator
        {
          protected:
            SkeletonNode * current;
            
          public:
            iterator (SkeletonNode * n);
            int operator ++ (int i);
            bool operator != (const iterator & i);
            SkeletonNode * operator * ();
        };
      iterator begin ();
      iterator end ();
      
      // compare this skeleton's rest pose to that of the parameter,
      // and store the updates provided.
      void matchWith (Skeleton & model);

      Vector restCoordinates (string bone, Point p);
      
      // find the joint corresponding to the first/top end of the bone, using the
      // convention that nameHead is the name of this joint.
      SkeletonNode * findBone (string name);
            
      // set the actual position of the joints according to the current pose.  
      void createActualPose ();
      
    protected:
      // recursive calculation of pose changes.  
      void updateMatchWith (Skeleton & model, SkeletonNode * root, Quaternion baseori);
      
      // calculate actual pose for current movement.
      void createActualPose (SkeletonNode * root, Transformation trans);
      
      // write the skeleton description portion of the BVH file.
      static void writeBVHSkeleton (ofstream & fl, SkeletonNode * root, Movements * mov, bool start, int indent);
      // write the motion channels for a given frame to the BVH file.
      static void writeBVHMotion (ofstream & fl, SkeletonNode * root, Movements * mov, int frame);
  };

#endif
// __Skeletonh


