/*  raytrace.c  */

/*  find_closest_object() function
    arguments:
       model_t *model - the model structure
       vect_t *base - base of the ray (viewpoint)
       vect_t *dir - unit direction vector of the ray
       object_t *last_hit - the last hit object
       double *retdist - store distance to the hit point here

     	 if no object is hit, then retdist will have the value -1, 
	       and NULL will be returned 
		 if an object is hit, retdist will end up with the mindist
		    and minobj will be a non-NULL object that is returned 
*/


#include "ray.h"


object_t *find_closest_object(model_t  *model, vec_t  *base, vec_t  *dir,
                              object_t  *last_hit, double  *retdist)     {
	object_t *minobj = NULL;
	double mindist = -1.0;

	// other declarations
	double tempdist = -1;
	object_t *obj = (object_t *) malloc(sizeof(object_t));

	// process entire list of objects to see if any are hit by the ray
	// how do you process a list??
	model->objs->reset();
	while(model->objs->not_end()) {
		// 	if current object from the list is not the last_hit,
		//			get the distance
		// Gets distance of hit object
		obj = (object_t *)model->objs->get_entity();
		if (obj  != last_hit) {
			tempdist = obj->hits(base, dir);
		}
		
		// 		and if that distance is smaller than the last mindist
		//			i.e. the ray hits that object at a smaller distance
		//				- - then remember that distance in mindist 
		// 			- - and remember that object in minobj
		if (((tempdist < mindist) || (mindist == -1))  && (tempdist > 0.0)) {
			mindist = tempdist;
			minobj = obj;
		}
		model->objs->next_link();
	}




	// save the mindist to retdist 
	*retdist = mindist;

	// and return the minobj
	return minobj;
}

void add_illumination(model_t *model, vec_t *base, object_t *hitobj,
							drgb_t *pixel) {
	light_t *light;
	list_t *list;

	list = model->lgts;
	list->reset();

	// Get each light
	while(list->not_end()) {
		// Get light
		light = (light_t *)list->get_entity();
		light->illuminate(model, base, hitobj, pixel);
		list->next_link();
	}
}


