/*	----------------------------------------------------------------------------
	Tracy Sanders
	tlsande
	CPSC 1020-001
	11/04/2015
	Major Programming Assignment #4

	This file gets the rgb values of the pixel if the viewpoint hits
	and object. It then creates the image pixel by pixel, using
	image_create to start on the columb and then go to each row's
	pixel.
	----------------------------------------------------------------------------
*/

/*  image.c  */

/*  functions:
    	ray_trace() - code to add
		make_pixel() - complete function provided
		make_row() - code to add
		image_create() - code to add
*/

#include "ray.h"


//=============================================================================
// ******* ray_trace( ) *******
//
//
//=============================================================================
void ray_trace (
   model_t *model,
   vec_t *base,                // view point
   vec_t *dir,                 // unit direction vector
   drgb_t *pix,                // pixel return location
   double total_dist,          // distance ray has travelled so far
   object_t *last_hit)         // most recently hit object
{
   double dist;
   drgb_t thispix = {0.0, 0.0, 0.0};
   object_t *closest;

   /* Get the object (plane or sphere).        */
	/* Later we will use find_closest_object()  */
   /* to get the closest object                */
   //obj = (object_t *)list_get_entity(model->objs);

	/* get the distance and the color */
   //dist = obj->hits(obj, base, dir);
   //obj->ambient(obj, obj->mat, &thispix);

	closest = find_closest_object(model, base, dir, last_hit, &dist);

	if (closest != NULL) {
		#ifdef DBG_HIT
		fprintf(stderr, "%-12s HIT:(%5.1lf, %5.1lf, %5.1lf)",
				closest->obj_name, closest->last.h,
				closest->last_hit.y, closest->last_hit.z);
		#endif
   	// Old code from C
		//dist = closest->hits(closest, base, dir);
   	//closest->ambient(closest, closest->mat, &thispix);
   	// New code for C++
   	dist = closest->hits(base, dir);
   	closest->getambient(&thispix);
		add_illumination(model, base, closest, &thispix);
	}


   /**  check to see if object was hit.  if so
        1)  add this distance to the total distance travelled so far
        2)  scale thispix by 1/total_distance
        3)  add thispix to pix
   **/
	//  CODE HERE
	if (dist != -1) {
		total_dist += dist;
	/*
		thispix.r = (1 / dist) * thispix.r;
		thispix.g = (1 / dist) * thispix.g;
		thispix.b = (1 / dist) * thispix.b;

		pix->r += thispix.r;
		pix->g += thispix.g;
		pix->b += thispix.b;
	*/
		pix_scale(1 / total_dist, &thispix, &thispix);
		pix_sum(&thispix, pix, pix);
	}




}


//=============================================================================
// ******* make_pixel( ) *******
//
//
//=============================================================================
void make_pixel(model_t  *model, int  x, int  y) {
  vec_t raydir;
  drgb_t pix = {0.0, 0.0, 0.0};
  int i;
  camera_t *cam = model->cam;
  // New code for C++
  vec_t *viewpoint = (vec_t *) malloc(sizeof(vec_t));
  cam->getviewpt(viewpoint);
  // Old C code
  //camera_getdir(cam, x, y, &raydir);

  // New C++ code
  //cam->getdir(x, y, &raydir);


   /*  The ray_trace function determines the pixel color in */
   /*  d_rgb units.   The last two parameters are used ONLY */
   /*  in the case of specular (bouncing) rays              */
  // Old ray_trace for C
  //ray_trace(model, &cam->view_point, &raydir, &pix, 0.0, NULL);
  // New ray_trace for C++
  for (i = 0; i < AA_SAMPLES; i++) {
	  cam->getdir(x, y, &raydir);
	  ray_trace(model, viewpoint, &raydir, &pix, 0.0, NULL);
  }

  pix_scale(1.0 / AA_SAMPLES, &pix, &pix);

  // Old C code
  //camera_store_pixel(cam, x, y, &pix);

  // New C++ code
  cam->store_pixel(x, y, &pix);

   return;
}


//=============================================================================
// ******* make_row( ) *******
//
//
//=============================================================================
void make_row(model_t  *model, int  y)
{
   int x;
   camera_t *cam = model->cam;

   /*  for each pixel in the row, invoke make_pixel() */
   /*  to paint the pixel.                            */
	//  CODE HERE
  // Old for loop for C
	//for (x = 0; x < cam->pixel_dim[0]; x++) {
  // New for loop with C++
  for (x = 0; x < cam->getxdim(); x++) {
		make_pixel(model, x, y);
	}


}


//=============================================================================
// ******* image_create( ) *******
//
//
//=============================================================================
void image_create(model_t  *model, FILE  *out)
{
  int y;
  // Old C code
	//list_reset(model->objs);

  // New C++ code
  model->objs->reset();
   camera_t *cam = model->cam;

   /*  fire rays through each pixel in the window; */
   /*  for each row, invoke make_row() to paint    */
   /*  the row of pixels.                          */
   //  CODE HERE
   // Old for loop for C
	//for (y = 0; y < cam->pixel_dim[1]; y++) {
  // New for loop with c++
  for (y = 0; y < cam->getydim(); y++) {
		make_row(model, y);
	}



   /*  write the image                           */
  // Old C code
  //camera_write_image(model->cam, out);

  // New C++ code
  model->cam->camera_write_image(out);
}
