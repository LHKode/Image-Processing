     #include <Water.h>
     
     #include <math.h>
     
     double Water::swellsize (double x , double z, Vector & curnormal)
      {
        double time = lasttime.interval ();
        int n = numswell;
        if (n == 0)
          n = 1;

        curnormal = Vector (0.0, 0.0, 0.0);
        double res = 0.0;
        int i;
        for (i = 0; i < n; i++)
          {
            double pos = x * swelldir[i].coord[0] + z * swelldir[i].coord[2];
            curnormal = curnormal + Vector (swelldir[i].coord[0],
                                           swellamp[i] * cos (swellfreq[i] * time + pos),
                                           swelldir[i].coord[2]);
            res = res + swellamp[i] * sin (swellfreq[i] * time + pos);
          }
        curnormal.normalize ();
        return res;
      }

    // specify the number of waves to be used, and the size of the grid used.
    Water::Water (int swelllimit, int gridsize)
      {
        MaxSwell = swelllimit;
        SurfaceResolution = gridsize / 2;

        swelldir = new Vector [MaxSwell];
        swellamp = new double [MaxSwell];
        swellfreq = new double [MaxSwell];

        textSurface = 0;

        numswell = 0;
        swelldir[0] = Vector (1.0, 0.0, 1.0);
        swellamp[0] = 1.0;
        swellfreq[0] = 1.0;

        lasttime.mark ();
        
        textSurface = new TextureMap ("seatexture.png");
      }
    void Water::render ()
      {
        double time = lasttime.interval ();

//        if (DETAIL)
          {
            textSurface->activate ();
            glEnable (GL_TEXTURE_2D);
            glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
          }

        glEnable (GL_COLOR_MATERIAL);
        glEnable (GL_BLEND);
        glShadeModel(GL_SMOOTH);

        glColor4f (0.5f, 0.5f, 0.8f, 0.5f);

        glBegin (GL_TRIANGLES);

        int i;
        int j;
        for (i = -SurfaceResolution; i < SurfaceResolution; i++)
          {
            for (j = -SurfaceResolution; j < SurfaceResolution; j++)
              {
                float x = (float) i;
                float z = (float) j;
                float x1 = (float) i+1;
                float z1 = (float) j+1;
                Vector n;
                Vector n01;
                Vector n10;
                Vector n11;
                float y = (float) swellsize (x, z, n);
                float y01 = (float) swellsize (x, z1, n01);
                float y10 = (float) swellsize (x1, z, n10);
                float y11 = (float) swellsize (x1, z1, n11);

//                if (DETAIL)
                  glTexCoord2f ((float) (0.0 + (time / 10.0)),
                                 (float) (0.0 + (time / 10.0)));
                glNormal3f ((float) n.coord[0], (float) n.coord[1], (float) n.coord[2]);
                glVertex3f (x, y, z);
//                if (DETAIL)
                  glTexCoord2f ((float) (1.0 + (time / 10.0)),
                                 (float) (0.0 + (time / 10.0)));
                glNormal3f ((float) n10.coord[0], (float) n10.coord[1], (float) n10.coord[2]);
                glVertex3f (x1, y10, z);
//                if (DETAIL)
                  glTexCoord2f ((float) (1.0 + (time / 10.0)),
                                 (float) (1.0 + (time / 10.0)));
                glNormal3f ((float) n11.coord[0], (float) n11.coord[1], (float) n11.coord[2]);
                glVertex3f (x1, y11, z1);

//                if (DETAIL)
                  glTexCoord2f ((float) (0.0 + (time / 10.0)),
                                 (float) (0.0 + (time / 10.0)));
                glNormal3f ((float) n.coord[0], (float) n.coord[1], (float) n.coord[2]);
                glVertex3f (x, y, z);
//                if (DETAIL)
                  glTexCoord2f ((float) (1.0 + (time / 10.0)),
                                 (float) (1.0 + (time / 10.0)));
                glNormal3f ((float) n11.coord[0], (float) n11.coord[1], (float) n11.coord[2]);
                glVertex3f (x1, y11, z1);
//                if (DETAIL)
                  glTexCoord2f ((float) (0.0 + (time / 10.0)),
                                 (float) (1.0 + (time / 10.0)));
                glNormal3f ((float) n01.coord[0], (float) n01.coord[1], (float) n01.coord[2]);
                glVertex3f (x, y01, z1);
              }
          }
        glEnd ();
        glDisable (GL_TEXTURE_2D);
        glDisable (GL_BLEND);
        glShadeModel(GL_FLAT);
      }
void Water::renderWireframe ()
  
{  
}

void Water::renderNormals ()

{  
}

      
    void Water::AddSwell (Vector dir, double amp, double freq)
      {
        if (numswell < MaxSwell)
          {
            swelldir[numswell] = dir;
            swellamp[numswell] = amp;
            swellfreq[numswell] = freq;
            numswell++;
          }
      }
