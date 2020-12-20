    #include <Critter.h>
    #include <Geometry.h>
    #include <Boid.h>
    
    #include <math.h>
    #include <GL/gl.h>
    
    Critter::Critter (ImageBuffer * h, double ml)
      {
        heights = h;
        maxlevel = ml;
        reposition ();
      }
    void Critter::reposition ()
      {
        x = ((float) unitrand ()) * (float) (heights->xDimension () - 1);
        y = ((float) unitrand ()) * (float) (heights->yDimension () - 1);
        deltax = (float) (unitrand () - 0.5);
        deltay = (float) (unitrand () - 0.5);
        invisible = 0;
      }
    void Critter::move ()
      {
        x += deltax;
        y += deltay;

        if ((x < 0.0) || (x > (float) (heights->xDimension () - 1)) ||
            (y < 0.0) || (y > (float) (heights->yDimension () - 1)))
          {
            reposition ();
            invisible = maxInvisible;
          }

        int xpi = (int) x;
        int ypi = (int) y;
        float xp = (float) xpi;
        float yp = (float) ypi;
        height = 1.0f + ((getHeight (heights, xpi, ypi, maxlevel) * ((y - yp) + (x - xp))) +
                  (getHeight (heights, xpi+1, ypi, maxlevel) * ((y - yp) + (1 - (x - xp)))) +
                  (getHeight (heights, xpi, ypi+1, maxlevel) * ((1 - (y - yp)) + (x - xp))) +
                  (getHeight (heights, xpi+1, ypi+1, maxlevel) * ((1 - (y - yp)) + (1 - (x - xp))))) / 4.0f;

        float dirx;
        float diry;
        float pheight;
        
        // affect the landscape - push it up a bit
        changeHeight (heights, xpi, ypi+1, maxlevel, 0.1);

        deltax *= 0.95;
        deltay *= 0.95;

        dirx = -1.0f;
        diry = -1.0f;
        pheight = getHeight (heights, xpi,ypi, maxlevel);
        deltax += ((height - pheight) / maxlevel) * dirx * (0.5f + unitrand ());
        deltay += ((height - pheight) / maxlevel) * diry * (0.5f + unitrand ());
        dirx = 1.0f;
        diry = -1.0f;
        pheight = getHeight (heights, xpi+1, ypi, maxlevel);
        deltax += ((height - pheight) / maxlevel) * dirx * (0.5f + unitrand ());
        deltay += ((height - pheight) / maxlevel) * diry * (0.5f + unitrand ());
        dirx = -1.0f;
        diry = 1.0f;
        pheight = getHeight (heights, xpi, ypi+1, maxlevel);
        deltax += ((height - pheight) / maxlevel) * dirx * (0.5f + unitrand ());
        deltay += ((height - pheight) / maxlevel) * diry * (0.5f + unitrand ());
        dirx = 1.0f;
        diry = 1.0f;
        pheight = getHeight (heights, xpi+1, ypi+1, maxlevel);
        deltax += ((height - pheight) / maxlevel) * dirx * (0.5f + unitrand ());
        deltay += ((height - pheight) / maxlevel) * diry * (0.5f + unitrand ());
      }
    void Critter::v1 ()
      {
        glColor3f (0.5f,
                    0.5f + (float) (invisible + maxInvisible / 4) / 30.0f,
                    0.5f + ((float) invisible / (float) (maxInvisible / 2)));
        glNormal3f (0.0f, 0.0f, -1.0f);
        glVertex3f (0.0f, 0.0f, -0.95f);
      }
    void Critter::v2 ()
      {
        glColor3f ((float) -invisible / 20.0f,
                    (float) (invisible + maxInvisible / 4) / 30.0f,
                    ((float) invisible / (float) (maxInvisible / 2)));
        glNormal3f (1.0f, 0.0f, 0.0f);
        glVertex3f (0.7f, 0.0f, 0.75f);
      }
    void Critter::v3 ()
      {
        glColor3f ((float) -invisible / 20.0f,
                    (float) (invisible + maxInvisible / 4) / 30.0f,
                    ((float) invisible / (float) (maxInvisible / 2)));
        glNormal3f (-1.0f, 0.0f, 0.0f);
        glVertex3f (-0.7f, 0.0f, 0.75f);
      }
    void Critter::v4 ()
      { 
        glColor3f ((float) -invisible / 20.0f,
                    (float) (invisible + maxInvisible / 4) / 30.0f,
                    ((float) invisible / (float) (maxInvisible / 2)));
        glNormal3f (0.0f, 1.0f, 0.0f);
        glVertex3f (0.0f, 0.8f, 0.4f);
      }
    void Critter::draw ()
      {
        glDisable (GL_TEXTURE_2D);
        glDisable (GL_LIGHTING);
        if (invisible <= maxInvisible / 2)
          {
            glPushMatrix ();
            glTranslatef (x - (heights->xDimension () / 2), height, y - (heights->yDimension () / 2));
            glRotatef (180.0f * (float) (atan2 (deltax, deltay) / M_PI), 0.0f, 1.0f, 0.0f);
            glBegin (GL_TRIANGLES);
              v1 (); v2 (); v3 ();
              v1 (); v2 (); v4 ();
              v3 (); v1 (); v4 ();
              v2 (); v3 (); v4 ();
            glEnd ();
            float perturb = 45.0f * (float) sin (movement.interval () * 5.0);
            glPushMatrix ();
              glTranslatef (0.0f, 0.0f, 0.0f);
              glRotatef (perturb, 0.0f, 1.0f, 0.0f);
              glBegin (GL_LINES);
                glVertex3f (0.0f, 0.0f, 0.0f);
                glVertex3f (0.7f, 0.0f, 0.0f);
              glEnd ();
            glPopMatrix ();
            glPushMatrix ();
              glTranslatef (0.0f, 0.0f, 0.0f);
              glRotatef (-perturb, 0.0f, 1.0f, 0.0f);
              glBegin (GL_LINES);
                glVertex3f (0.0f, 0.0f, 0.0f);
                glVertex3f (-0.7f, 0.0f, 0.0f);
              glEnd ();
            glPopMatrix ();

            if (invisible < 0)
              {
                glPushMatrix ();
                  glTranslatef (0.0f, 0.0f, 0.4f);
                  glRotatef (perturb, 0.0f, 1.0f, 0.0f);
                  glBegin (GL_LINES);
                    glVertex3f (0.0f, 0.0f, 0.0f);
                    glVertex3f (0.7f, 0.0f, 0.0f);
                  glEnd ();
                glPopMatrix ();
                glPushMatrix ();
                  glTranslatef (0.0f, 0.0f, 0.4f);
                  glRotatef (-perturb, 0.0f, 1.0f, 0.0f);
                  glBegin (GL_LINES);
                    glVertex3f (0.0f, 0.0f, 0.0f);
                    glVertex3f (-0.7f, 0.0f, 0.0f);
                  glEnd ();
                glPopMatrix ();
              }

            if (invisible < -maxInvisible)
              {
                glPushMatrix ();
                  glTranslatef (0.0f, 0.0f, -0.4f);
                  glRotatef (perturb, 0.0f, 1.0f, 0.0f);
                  glBegin (GL_LINES);
                    glVertex3f (0.0f, 0.0f, 0.0f);
                    glVertex3f (0.7f, 0.0f, 0.0f);
                  glEnd ();
                glPopMatrix ();
                glPushMatrix ();
                  glTranslatef (0.0f, 0.0f, -0.4f);
                  glRotatef (-perturb, 0.0f, 1.0f, 0.0f);
                  glBegin (GL_LINES);
                    glVertex3f (0.0f, 0.0f, 0.0f);
                    glVertex3f (-0.7f, 0.0f, 0.0f);
                  glEnd ();
                glPopMatrix ();
              }
            glPopMatrix ();
          }
        invisible--;
        glEnable (GL_LIGHTING);
      }

