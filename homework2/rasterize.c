#include "bmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t bmp_calculate_size(bitmap_t *bmp);
void bmp_serialize(bitmap_t *bmp, uint8_t *data);

int main(void) {
  bitmap_t bmp = { 0 }; // initialize to zeros
  bmp.width = 640;
  bmp.height = 480;
  bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
  // for (int i = 0; i < 100; i++) {
  //   bmp.data[i].r = 255;
  //   bmp.data[i].g = 255;
  //   bmp.data[i].b = 255;
  // }
  size_t bmp_size = bmp_calculate_size(&bmp);
  uint8_t *serialized_bmp = malloc(bmp_size);
  bmp_serialize(&bmp, serialized_bmp); // serialized_bmp now has the full bmp formatted image

  // for (int i = 0; i < 32; i++) {
  //   printf("%x ", serialized_bmp[i]); // %x is for hexadecimal
  // }
  // printf("\n");

  FILE *f = fopen("my_image.bmp", "wb"); // write to a file so we can check if it is a valid image
  fwrite(serialized_bmp, bmp_size, 1, f);
  fclose(f);

  // free stuff we malloced- remember to later free the bmp.data when we are done using it
  free(serialized_bmp);
  free(bmp.data);
  // printf("hello world!");
  return 0;
}

//FUNCTIONS
size_t bmp_calculate_size(bitmap_t *bmp) {
  size_t size = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 921600;
  return size;
}
void bmp_serialize(bitmap_t *bmp, uint8_t *data) {
  BITMAPFILEHEADER file_header = { 0 }; // start out as all zero values
  file_header.bfType = 0x4d42;//0x424d
  file_header.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 921600;
  file_header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);;
  file_header.bfReserved1 = 0;
  file_header.bfReserved2 = 0;

  BITMAPINFOHEADER info_header = { 0 };
  info_header.biSize = sizeof(BITMAPINFOHEADER);
  info_header.biWidth = 640; //bmp.width;
  info_header.biHeight = 480; //bmp.height;
  info_header.biPlanes = 1;
  info_header.biBitCount = 24;
  info_header.biCompression = 0;
  info_header.biSizeImage = 0; //This can be set to 0 for uncompressed RGB bitmaps.?
  info_header.biXPelsPerMeter = 2835;
  info_header.biYPelsPerMeter = 2835;
  info_header.biClrUsed = 0;
  info_header.biClrImportant = 0;

  uint8_t *data_out = data; // keep track of the next place we should write in the data buffer
  memcpy(data_out, &file_header, sizeof(file_header)); // write X number of bytes
  data_out += sizeof(file_header); // and then move data_out forward X bytes
  memcpy(data_out, &info_header, sizeof(info_header));
  data_out += sizeof(info_header);

  for (int i = 480; i >= 1; i--) {
    memcpy(data_out, &bmp -> data[1], 640);
    data_out += 640;
  }
}

// void bresenham(int x0, int y0, int x1, int y1){
//   dx =  abs(x1-x0);
//   sx = x0 < x1 ? 1 : -1;
//   dy = -abs(y1-y0);
//   sy = y0 < y1 ? 1 : -1;
//   err = dx+dy;  /* error value e_xy */
//   while (true) { /* loop */
//     if (x0==x1 && y0==y1) break;
//     e2 = 2*err;
//     if (e2 >= dy){
//       err += dy; /* e_xy+e_x > 0 */
//       x0 += sx;
//     }
//     if (e2 <= dx){} /* e_xy+e_y < 0 */
//     err += dx;
//     y0 += sy;
//   }
// }
// }
