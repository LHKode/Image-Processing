#include <Renderable.h>
#include <Geometry.h>
#include <Timer.h>
#include <TextureMap.h>

class Water : public Renderable
  {
    protected:
      int     MaxSwell;
      int     SurfaceResolution;
    
      /// real time lock for surface ripple.
      Timer            lasttime;
      /// surface texture.
      TextureMap *     textSurface;
   
      /// Wave details.
      Vector    *   swelldir;
      double    *   swellamp;
      double    *   swellfreq;
    
      /// number of active waves. This defaults to one with standard settings,
      /// but is the first overridden.
      int              numswell;

      /// calculate swell at a point. Set curnormal to the normal at the point.
      double swellsize (double x , double z, Vector & curnormal);
    
      public:
        // specify the number of waves to be used, and the size of the grid used.
        Water (int swelllimit = 2, int gridsize = 10);
        /// Set the direction (x and z components), amplitude and frequency of a
        /// wave swell.
        void AddSwell (Vector dir, double amp, double freq);
      
        virtual void render ();
        virtual void renderWireframe ();
        virtual void renderNormals ();
  };
