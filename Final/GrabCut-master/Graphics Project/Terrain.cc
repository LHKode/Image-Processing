#include <iostream>
#include <vector>
#include <fstream>

#include <Terrain.h>
#include <GL/gl.h>
#include <Boid.h>

#include <math.h>

using namespace std;

Terrain::Terrain (ImageBuffer * h, double mh)
{
  terrain = h;
  maxheight = mh;

  listid = -1;
}

void Terrain::landVertex (int x, int y, Vector * normals)
  {
    double h = getHeight (terrain, x, y, maxheight);
    glTexCoord2f ((double) x / (double) terrain->xDimension (), h / maxheight);
    glNormal3f (normals[y * terrain->xDimension () + x].coord[0],
                normals[y * terrain->xDimension () + x].coord[1],
                normals[y * terrain->xDimension () + x].coord[2]);
    glVertex3f ((double) (x - (terrain->xDimension () / 2)), h, (double) (y - (terrain->yDimension () / 2)));
  }
  
void Terrain::render ()

{
  static int count = 0;
  count++;
  if ((!glIsList (listid)) || (count % 25 == 0))
    {
      cout << "Creating terrain\n";
      if (!glIsList (listid))
        listid = glGenLists (1);
      glNewList (listid, GL_COMPILE);

        Vector * normals = new Vector [terrain->yDimension () * terrain->xDimension ()];
        // get land characteristics.
        for (int i = 0; i < terrain->yDimension (); i++)
          {
            for (int j = 0; j < terrain->xDimension (); j++)
              {
                normals[j * terrain->xDimension () + i] = Vector (0.0, 1.0, 0.0);

                if ((i > 0) && (i < terrain->yDimension () - 1) && (j > 0) && (j < terrain->xDimension () - 1))
                  {
                    double hh = getHeight (terrain, j, i, maxheight);
                    double hl = getHeight (terrain, j-1, i, maxheight);
                    double hr = getHeight (terrain, j+1, i, maxheight);
                    double ht = getHeight (terrain, j, i-1, maxheight);
                    double hb = getHeight (terrain, j, i+1, maxheight);
                    double hlt = getHeight (terrain, j-1, i-1, maxheight);
                    double hrt = getHeight (terrain, j+1, i-1, maxheight);
                    double hlb = getHeight (terrain, j-1, i+1, maxheight);
                    double hrb = getHeight (terrain, j+1, i+1, maxheight);
                    normals[j * terrain->xDimension () + i] = Vector ((hh - hl) - (hh - hr), 10.0, (hh - ht) - (hh - hr));
                    normals[j * terrain->xDimension () + i].normalize ();
                  }
              }
          }
        
        glBegin (GL_TRIANGLES);	

        for (int i = 1; i < terrain->yDimension (); i++)
          {
            for (int j = 1; j < terrain->xDimension (); j++)
              {
                landVertex (i -1, j - 1, normals);
                landVertex (i, j - 1, normals);
                landVertex (i, j, normals);

                landVertex (i -1, j - 1, normals);
                landVertex (i, j, normals);
                landVertex (i -1, j, normals);
              }
          }
        glEnd ();
        
        delete [] normals;
    glEndList ();
  }

  glCallList (listid);
}

void Terrain::renderWireframe ()
  
{  
}

void Terrain::renderNormals ()

{  
}

