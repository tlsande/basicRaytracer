/*	----------------------------------------------------------------------------
	Tracy Sanders
	tlsande
	CPSC 1020-001
	11/04/2015
	Major Programming Assignment #4

	This file is the converted camera file from C to C++. It constructs the
  camera and calls the universal parser to get its attributes. It also
  prints the ppm image.
	----------------------------------------------------------------------------
*/

#include "ray.h"

// for the generalized parsher
static pparm_t cam_parse[] =
{
   {"pixeldim", 2, sizeof(int), "%d", 0},
   {"worlddim", 2, sizeof(double), "%lf", 0},
   {"viewpoint", 3, sizeof(double), "%lf", 0},
};

#define NUM_ATTRS (sizeof(cam_parse) / sizeof(pparm_t))


// Camera constructor
camera_t::camera_t(FILE *in) {
  char attrname[NAME_LEN];
  int mask;
  // Gets camera name
  assert (fscanf(in, "%s", name) == 1);

  // Eats first {
  fscanf(in, "%s", attrname);
  assert(attrname[0] == '{');

  // Create a new camera structure and initialize it
  cookie = CAM_COOKIE;

  // Parses camera attributes
  cam_parse[0].loc = &pixel_dim;
  cam_parse[1].loc = &world_dim;
  cam_parse[2].loc = &view_point;

  mask = parser(in, cam_parse, NUM_ATTRS, 0);
  assert(mask == 7);

  // Allocate a pixmap to hold the ppm image data
  pixmap = (irgb_t *)malloc(sizeof(irgb_t) * pixel_dim[0] * pixel_dim[1]);
}

void camera_t::camera_print(FILE *out) {
  assert(cookie == CAM_COOKIE);
  // Prints the camera and its name
  fprintf(out, "%-12s %-12s \n", "camera", name);
  // Prints the pixel dimensions
  fprintf(out, "%-12s %-6.01d %-6.01d\n", "pixeldim",
    pixel_dim[0], pixel_dim[1]);
  // Prints the world dimensions
  fprintf(out, "%-12s %-6.01lf %-6.01lf\n", "worlddim",
    world_dim[0], world_dim[1]);
  // Prints the viewpoint
  fprintf(out, "%-12s %-6.01lf %-6.01lf %-6.01lf\n", "viewpoint",
    view_point.x, view_point.y, view_point.z);
}

int camera_t::getxdim(void) {
  return(pixel_dim[0]);
}

int camera_t::getydim(void) {
  return(pixel_dim[1]);
}

void camera_t::getviewpt(vec_t *view) {
  vec_copy(&view_point, view);
}

void camera_t::getdir(int x, int y, vec_t *dir) {
  assert(cookie == CAM_COOKIE);

  vec_t world;
  double dx = x;
  double dy = y;

  if (AA_SAMPLES > 1) {
	  dx = randomize(dx);
	  dy = randomize(dy);
  }



  // Solves for the worldx, y, and z coordinates
  /*
  world.x = world_dim[0] * x / (pixel_dim[0] - 1);
  world.y = world_dim[1] * y / (pixel_dim[1] - 1);
  world.z = 0.0;
  */

  world.x = world_dim[0] * dx / (pixel_dim[0] - 1);
  world.y = world_dim[1] * dy / (pixel_dim[1] - 1);
  world.z = 0.0;

  // Gets the unit vector form the viewpoint to the world coordinates
  vec_diff(&view_point, &world, dir);
  vec_unit(dir, dir);
}

// Used to randomize the jitter needed for anti-aliasing
double randomize(double coord) {
	double i;

	i = (double)rand();
	i = i / 0x7fffffff;
	i = i - 0.5;
	return coord + i;
}

void camera_t::store_pixel(int x,int y, drgb_t *pix) {
  assert(cookie == CAM_COOKIE);

  int row, col;

  // Used to solve for the position of the pixel in the ppm file
  row = pixel_dim[1] - y - 1;
  col = x;

  irgb_t *ipix;
  //solves the pixel position
  ipix = pixmap + (row * pixel_dim[0]) + col;

  // converts the drgb_t values to irgb_t values
  ipix->r = get_rgb(pix->r);
  ipix->g = get_rgb(pix->g);
  ipix->b = get_rgb(pix->b);
}

double get_rgb(double color) {
  double result;
  result = color * 255 + .5;
  if (result >= 255) {
    result = 255;
  }
  else if (result <=0) {
    result = 0;
  }
  return result;
}

void camera_t::camera_write_image(FILE *out) {
  // Prints ppm header
  fprintf(out, "P6 %d %d 255\n", pixel_dim[0], pixel_dim[1]);

  fwrite(pixmap, sizeof(irgb_t), pixel_dim[0] * pixel_dim[1], out);
}
