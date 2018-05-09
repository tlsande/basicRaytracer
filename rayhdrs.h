/*  == material.c  ==  */

material_t *material_getbyname(model_t  *, char  *);

void material_list_print(model_t  *, FILE  *);

/*  == object.c ==  */
void object_list_print(model_t  *, FILE  *);


/*  == image.c == */
void ray_trace (
   model_t *model,
   vec_t *base,             //  view point
   vec_t *dir,              //  unit direction vector
   drgb_t *pix,             //  pixel return location
   double total_dist,       //  distance ray has travelled so far
   object_t *last_hitobj);  //  most recently hit object

void make_pixel(model_t  *model, int  x, int  y);

void make_row(model_t  *, int );

void image_create(model_t  *, FILE  *);

//== raytrace.c ==
/* Find the closest object intersection on the ray based at "base" and  */
/* having direction "dir"                                               */
/* list is the object list										                          */
/* base is the base of the ray							  	                        */
/* dir is the direction of the ray							                        */
/* last_hit is the object's last hit point				                      */
object_t *find_closest_object(model_t  *model, vec_t *base, vec_t  *dir,
										object_t  *last_hit, double  *retdist);

void add_illumination(
	model_t *model,	// Object list
	vec_t *base,		// base of ray
	object_t *hitobj,	// object that was hit by the ray
	drgb_t *pixel);	// where to add intensity


/*==  parser.c ==  */
int parser(
FILE    *in,
pparm_t *pct,         /* parser control table                */
int      numattrs,    /* number of legal attributes          */
int      attrmax);    /* Quit after this many attrs if not 0 */

// get_rgb for camera.cpp     scales and clamps
double get_rgb(double color);
double randomize(double coord);
//==  light.c  ==
void light_list_print(model_t *, FILE *);
