
/** class Critter
*/

#include <ImageBuffer.h>
#include <Timer.h>

class Critter
  {
    private:
      static const int maxInvisible = 80;

    public:
      float x;
      float y;
      float deltax;
      float deltay;
      float height;
      int invisible;

      Timer movement;

    private:
    // height map.
      ImageBuffer * heights;
      
      double maxlevel;

    // create a new critter, passing it terrain details.
    public:
    Critter (ImageBuffer * h, double ml);
    void reposition ();
    void move ();
    void draw ();
    private:
      void v1 ();
      void v2 ();
      void v3 ();
      void v4 ();
 };
