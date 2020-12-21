      #include <TextureMap.h>
      #include <Geometry.h>
      
      #include <iostream>
      #include <stdio.h>
      
      TextureMap::TextureMap ()
        {
          xsize = 0;
          ysize = 0;
        }
      TextureMap::~TextureMap ()
        {
          glDeleteTextures (1, &textureid);
          delete [] colours;
        }
      TextureMap::TextureMap (char * filename)
        {
          std::cout << "Loading texture: " << filename << "\n";

          FILE * file;
          if (strstr (filename, "ppm") == NULL)
            {
              char line [MAXSTRING];
              sprintf (line, "pngtopnm %s | pnmflip -tb >tmptexture.pnm", filename);
              system (line);

              file = fopen ("tmptexture.pnm", "r");
            }
          else
            file = fopen (filename, "r");

          if (file == NULL)
            {
              std::cerr << "Could not open pnm file " << filename << "\n";
              exit (1);
            }

          int c;
          if (fscanf (file, "P6\n%d %d\n%d\n", &xsize, &ysize, &c) != 3)
            {
              std::cerr << "Unable to read file header of converted " << filename << "\n";
              exit (1);
            }

          colours = new GLubyte [xsize * ysize * 3];
          fread (colours, xsize * 3, ysize, file);
          fclose (file);

          // set up OpenGL texturing.
          glGenTextures (1, &textureid);

          glBindTexture (GL_TEXTURE_2D, textureid);

          glTexImage2D (GL_TEXTURE_2D, 0, 3, xsize, ysize, 0, GL_RGB,
                        GL_UNSIGNED_BYTE, colours);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
          glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        }
      // make this texture active.
      void TextureMap::activate ()
        {
          glBindTexture (GL_TEXTURE_2D, textureid);
        }
      int TextureMap::textureID ()
        {
          return textureid;
        }
