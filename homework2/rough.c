if (p1x >= 0 && p1y >= 0) {
  pg_append(rect_vec,p1x, p1y);
} else {
  p1x = 0;
  p1y = 0;
  pg_append(rect_vec,p1x, p1y);
}
double p2x = xc + width / 2.0;
double p2y = yc - height / 2.0;
if (p2x >= 0 && p2y >= 0) {
  pg_append(rect_vec,p2x, p2y);
} else {
  p2x = 0;
  p2y = 0;
  pg_append(rect_vec,p2x, p2y);
}
double p3x = xc - width / 2.0;
double p3y = yc - height / 2.0;
if (p3x >= 0 && p3y >= 0) {
  pg_append(rect_vec,p3x, p3y);
} else {
  p3x = 0;
  p3y = 0;
  pg_append(rect_vec,p3x, p3y);
}
double p4x = xc - width / 2.0;
double p4y = yc + height / 2.0;
if (p4x >= 0 && p4y >= 0) {
  pg_append(rect_vec,p4x, p4y);
} else {
  p4x = 0;
  p4y = 0;
  pg_append(rect_vec,p4x, p4y);
}


void pg_draw(bitmap_t *bmp, color_bgr_t color, pg_vector_t *rect_vec) {
  //pg_vector_t *pixelpts = cd2pixel(points);
  // for(int i = 0; i < pixelpts->size; i++){
  //   bresenham(pixelpts->data[i].x, pixelpts->data[i].y, pixelpts->data[i+1].x, pixelpts->data[i+1].y, bmp, color);
  // }
  // bresenham(pixelpts->data[pixelpts->size - 1].x, pixelpts->data[pixelpts->size - 1].y, pixelpts->data[0].x, pixelpts->data[0].y, bmp, color);
  // pg_free(pixelpts);
  bresenham(rect_vec->data[0].x, rect_vec->data[0].y, rect_vec->data[1].x, rect_vec->data[1].y, bmp, color);
  bresenham(rect_vec->data[1].x, rect_vec->data[1].y, rect_vec->data[2].x, rect_vec->data[2].y, bmp, color);
  bresenham(rect_vec->data[2].x, rect_vec->data[2].y, rect_vec->data[3].x, rect_vec->data[3].y, bmp, color);
  printf("%d ", rect_vec->data[1].x);
  printf("%d ", rect_vec->data[1].y);
  printf("%d ", rect_vec->data[2].x);
  printf("%d ", rect_vec->data[2].y);
  bresenham(rect_vec->data[0].x, rect_vec->data[0].y, rect_vec->data[1].x, rect_vec->data[1].y, bmp, color);
  bresenham(rect_vec->data[1].x, rect_vec->data[1].y, rect_vec->data[2].x, rect_vec->data[2].y, bmp, color);
  bresenham(rect_vec->data[2].x, rect_vec->data[2].y, rect_vec->data[3].x, rect_vec->data[3].y, bmp, color);

double cd2pixel(double value) {
  pg_vector_t pixels;
  pg_create(&pixels);
  double epsilon = 1e-6;
  double tempx = points->data[0].x; //find a better way to do this perhaps??
  double tempy = points->data[0].y;
  for(int i = 0; i <points->size; i++) { //find min value and store in tempx,tempy
    if (tempx > points->data[i].x) {
      tempx = points->data[i].x;
    }
    if (tempy > points->data[i].y) {
      tempy = points->data[i].y;
    }
  }
  for(int i = 0; i <points->size; i++) { //check for min value and round off
    if (points->data[i].x == tempx) {
      pixels.data[i].x = ceil(points->data[i].x);
    } else {
      pixels.data[i].x = floor(points->data[i].x - epsilon);
    }
    if (points->data[i].y == tempy) {
      pixels.data[i].y = ceil(points->data[i].y);
    } else {
      pixels.data[i].y = floor(points->data[i].y - epsilon);
    }
  }
  pg_free(points);
  return &pixels;
} //MUST FREE THIS AS WELL!!!
