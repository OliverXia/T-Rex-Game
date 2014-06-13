#include "TextureHelper.h"
#include "GLSL_helper.h"
#include "png_texture.h"

/*data structure for the image used for  texture mapping */
typedef struct Image {
  unsigned long sizeX;
  unsigned long sizeY;
  char *data;
} Image;

typedef struct RGB {
  GLubyte r;
  GLubyte g; 
  GLubyte b;
} RGB;

static unsigned int getint(FILE *fp);
int ImageLoad(char *filename, Image *image, int flip_colors);

//routines to load in a bmp files - must be 2^nx2^m and a 24bit bmp
GLvoid LoadTexture(char* image_file, int texID, int flip_colors) { 
  Image TextureImage;
  
  memset(&TextureImage, 0, sizeof(Image));
  
  std::cout << "trying to load " << image_file << std::endl;
  if (!ImageLoad(image_file, &TextureImage, flip_colors)) {
    exit(1);
  }  
  /*  2d texture, level of detail 0 (normal), 3 components (red, green, blue),            */
  /*  x size from image, y size from image,                                              */    
  /*  border 0 (normal), rgb color data, unsigned byte data, data  */ 
  glTexImage2D(GL_TEXTURE_2D, 0, 3,
    TextureImage.sizeX, TextureImage.sizeY,
    0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage.data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef __APPLE__
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#else
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);
#endif

  free(TextureImage.data);
}

/* BMP file loader loads a 24-bit bmp file only */

/*
* getint and getshort are help functions to load the bitmap byte by byte
*/
static unsigned int getint(FILE *fp) {
  int c, c1, c2, c3;
  
  /*  get 4 bytes */ 
  c = getc(fp);  
  c1 = getc(fp);  
  c2 = getc(fp);  
  c3 = getc(fp);
  
  return ((unsigned int) c) +   
    (((unsigned int) c1) << 8) + 
    (((unsigned int) c2) << 16) +
    (((unsigned int) c3) << 24);
}

static unsigned int getshort(FILE *fp){
  int c, c1;
  
  /* get 2 bytes*/
  c = getc(fp);  
  c1 = getc(fp);
  
  return ((unsigned int) c) + (((unsigned int) c1) << 8);
}

/*  quick and dirty bitmap loader...for 24 bit bitmaps with 1 plane only.  */

int ImageLoad(char *filename, Image *image, int flip_colors) {
  FILE *file;
  unsigned long size;                 /*  size of the image in bytes. */
  unsigned long i;                    /*  standard counter. */
  unsigned short int planes;          /*  number of planes in image (must be 1)  */
  unsigned short int bpp;             /*  number of bits per pixel (must be 24) */
  char temp;                          /*  used to convert bgr to rgb color. */
  
  /*  make sure the file is there. */
  if ((file = fopen(filename, "rb"))==NULL) {
    printf("File Not Found : %s\n",filename);
    return 0;
  }
  
  /*  seek through the bmp header, up to the width height: */
  fseek(file, 18, SEEK_CUR);
  
  /*  No 100% errorchecking anymore!!! */
  
  /*  read the width */    
  image->sizeX = getint (file);
  
  /*  read the height */ 
  image->sizeY = getint (file);
  
  /*  calculate the size (assuming 24 bits or 3 bytes per pixel). */
  size = image->sizeX * image->sizeY * 3;
  
  /*  read the planes */    
  planes = getshort(file);
  if (planes != 1) {
    printf("Planes from %s is not 1: %u\n", filename, planes);
    return 0;
  }
  
  /*  read the bpp */    
  bpp = getshort(file);
  if (bpp != 24) {
    printf("Bpp from %s is not 24: %u\n", filename, bpp);
    return 0;
  }
  
  /*  seek past the rest of the bitmap header. */
  fseek(file, 24, SEEK_CUR);
  
  /*  read the data.  */
  image->data = (char *) malloc(size);
  if (image->data == NULL) {
    printf("Error allocating memory for color-corrected image data");
    return 0; 
  }
  
  if ((i = fread(image->data, size, 1, file)) != 1) {
    printf("Error reading image data from %s.\n", filename);
    return 0;
  }
  
  if (flip_colors) 
    for (i=0;i<size;i+=3) { /*  reverse all of the colors. (bgr -> rgb) */
      temp = image->data[i];
      image->data[i] = image->data[i+2];
      image->data[i+2] = temp;
    }
  
  fclose(file); /* Close the file and release the filedes */
  
  /*  we're done. */
  return 1;
}

#define MAX_TEXTURE_UNITS 16
void TextureHelper::createTexture(std::string tag, std::string path) {
  createTexture(tag, path, 1, 2);
}

void TextureHelper::createTexturePNG(std::string tag, std::string path) {
   int width = 0, height = 0;
   if (texTable.find(tag) != texTable.end())
   {
      printf("Let's not load a frame texture to an image texture...\n");
      return;
   }

   Texture t;
   // image textures go in texture slot 2
   t.activeTex = 2;
   printf("HEY\n");
   glActiveTexture(GL_TEXTURE0 + t.activeTex);
    // glGenTextures(1, &texture);
    // glBindTexture(GL_TEXTURE_2D, texture);
    // glTexImage2D(GL_TEXTURE_2D, 0, format, temp_width, temp_height, 0, format, GL_UNSIGNED_BYTE, image_data);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   png_texture_load((char*)path.c_str(), &width, &height, &(t.texId));

   printf("Created PNG texture on tag \"%s\"=>%d (w:%d, h:%d)\n",
         tag.c_str(), t.texId, width, height);

   texTable.insert(TextureMap::value_type(tag, t));
}

void TextureHelper::createTexture(std::string tag, std::string path, int flip_colors, int slot) {
   int width = 0, height = 0;
   if (texTable.find(tag) != texTable.end())
   {
      auto tex = texTable.find(tag);
      glActiveTexture(GL_TEXTURE0 + texTable.find(tag)->second.activeTex);
      glBindTexture(GL_TEXTURE_2D, tex->second.texId);
      LoadTexture((char*)path.c_str(), tex->second.texId, flip_colors);
      return;
   }

   Texture t;
   // image textures go in texture slot 2
   t.activeTex = slot;

   /* GLuint t = png_texture_load(path.c_str(), &width, &height); */
   glActiveTexture(GL_TEXTURE0 + t.activeTex);
   glGenTextures(1, &(t.texId));
   glBindTexture(GL_TEXTURE_2D, t.texId);
   LoadTexture((char *)path.c_str(), t.texId, flip_colors);

   printf("Loaded texture \"%s\" to tag \"%s\"=>%d (w:%d, h:%d)\n",
         path.c_str(), tag.c_str(), t.texId, width, height);

   texTable.insert(TextureMap::value_type(tag, t));
}

void TextureHelper::createTexture(std::string tag, int width, int height) {
   if (texTable.find(tag) != texTable.end())
   {
      printf("Let's not load a frame texture to an image texture...\n");
      return;
   }
   
   Texture t;
   // this is used for framebuffer and other speshul stuff,
   // so it goes in texture slot 3
   t.activeTex = 3;

   glActiveTexture(GL_TEXTURE0 + t.activeTex);
   glGenTextures(1, &(t.texId));
   glBindTexture(GL_TEXTURE_2D, t.texId);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
   GL_RGBA, GL_INT, NULL);

   printf("Created blank texture on tag \"%s\"=>%d (w:%d, h:%d)\n",
         tag.c_str(), t.texId, width, height);

   texTable.insert(TextureMap::value_type(tag, t));
}

void TextureHelper::createDepthTexture(std::string tag, int size) {
   if (texTable.find(tag) != texTable.end())
   {
      printf("Let's not load a frame texture to an image texture...\n");
      return;
   }

   Texture t;
   // depth textures go in slot 4
   t.activeTex = 4;

   glActiveTexture(GL_TEXTURE0 + t.activeTex);
   glGenTextures(1, &(t.texId));
   glBindTexture(GL_TEXTURE_2D, t.texId);

   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
   float bColor[4] = {1.0, 1.0, 1.0, 1.0};
   glTexParameterfv(
      GL_TEXTURE_2D,
      GL_TEXTURE_BORDER_COLOR,
      bColor
   );

   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, size, size, 0,
   GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

   printf("Created depth texture on tag \"%s\"=>%d (w:%d, h:%d)\n",
         tag.c_str(), t.texId, size, size);

   texTable.insert(TextureMap::value_type(tag, t));
}

Texture TextureHelper::getTexture(std::string tag) {
   return texTable.find(tag)->second;
}
