#ifndef PNG_WRITE_H
#define PNG_WRITE_H

#include <png.h>
#include "types.h"

/* Given "bitmap", this returns the pixel of bitmap at the point 
   ("x", "y"). */
static color_t* color_at (bitmap_t* bitmap, int x, int y)
{
    return bitmap->pixels + bitmap->width * y + x;
}

static int save_png_to_file(bitmap_t* bitmap, const char* path) {
  FILE* fp;
  png_structp png_ptr = NULL;
  png_infop info_ptr = NULL;
  size_t x, y;
  png_byte** row_pointers = NULL;

  // `status` contains return value of this function.  
  // default is failure.  it gets set to success on 
  // valid write.
  int status = -1;

  int pixel_size = 3;
  int depth = 8;

  fp = fopen(path, "wb");
  if (!fp) {
    goto fopen_failed;
  }

  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (NULL == png_ptr) {
    goto png_create_write_struct_failed;
  }
    
  info_ptr = png_create_info_struct (png_ptr);
  if (NULL == info_ptr) {
      goto png_create_info_struct_failed;
  }

  // Set up error handling
  if (setjmp (png_jmpbuf (png_ptr))) {
    goto png_failure;
  }
    
  png_set_IHDR (png_ptr,
                info_ptr,
                bitmap->width,
                bitmap->height,
                depth,
                PNG_COLOR_TYPE_RGB,
                PNG_INTERLACE_NONE,
                PNG_COMPRESSION_TYPE_DEFAULT,
                PNG_FILTER_TYPE_DEFAULT);
    
  /* Initialize rows of PNG. */

  row_pointers = png_malloc (png_ptr, bitmap->height * sizeof (png_byte *));
  for (y = 0; y < bitmap->height; y++) {
    png_byte *row = 
        png_malloc (png_ptr, sizeof (uint8_t) * bitmap->width * pixel_size);
    row_pointers[y] = row;
    for (x = 0; x < bitmap->width; x++) {
      color_t* pixel = color_at(bitmap, x, y);
      *row++ = pixel->r;
      *row++ = pixel->g;
      *row++ = pixel->b;
    }
  }
    
  /* Write the image data to "fp". */

  png_init_io (png_ptr, fp);
  png_set_rows (png_ptr, info_ptr, row_pointers);
  png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

  /* The routine has successfully written the file, so we set
     "status" to a value which indicates success. */

  status = 0;
  
  for (y = 0; y < bitmap->height; y++) {
      png_free (png_ptr, row_pointers[y]);
  }
  png_free (png_ptr, row_pointers);
    
png_failure:
png_create_info_struct_failed:
  png_destroy_write_struct (&png_ptr, &info_ptr);
png_create_write_struct_failed:
  fclose (fp);
fopen_failed:
  return status;
}


#endif
