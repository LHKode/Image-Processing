#ifndef __Movementsh
#define __Movementsh

#include <string>
#include <vector>
#include <Renderable.h>
#include <Geometry.h>

class Skeleton;

class Channel
  {
    public:
      Quaternion rot;
      Vector     trans;
  };

typedef std::vector<std::string> ChannelNames;  
  
class Movements
  {
    protected:
    int frames;

    // list of name for the individual channels.
    ChannelNames names;
    // 2 D array of frames and channels.
    Channel * value;

    double frameinterval;

    public:
    Movements (int f, ChannelNames cn, double interval);
    Movements (Movements & m);

    Channel * getChannelEntry (int f, std::string channel);

    int numFrames ();
    double frameInterval ();

    friend Movements * parseBVH (Skeleton & skel, std::string basename);
    
    // convert the motion from one described relative to the original
    // pose of the source skeleton to one described relative to the 
    // original pose of the destination skeleton.
    Movements * convert (Skeleton & source, Skeleton & destination);
  };

#endif
// __Movementsh


