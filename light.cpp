/*	----------------------------------------------------------------------------
	Tracy Sanders
	tlsande
	CPSC 1020-001
	11-24-2015
	Major Programming Assignement #3

	This file is responsible for diffuse lighting and shadows. It parses
	the light object's attributes and saves them to the lights list.
	It also solves for the pixel illumination.
	----------------------------------------------------------------------------
*/


#include "ray.h"
#include <iostream>

// For the generalized parser
static pparm_t light_parse[] =
{
	{"location", 3, sizeof(double), "%lf", 0},
	{"emissivity", 3, sizeof(double), "%lf", 0},
};

#define NUM_ATTRS (sizeof(light_parse) / sizeof(pparm_t))
//#define DBG_OCCLUDE
// Light Constructor
light_t::light_t(FILE *in, model_t *model, int attrmax) {
	int mask;
	int count;
	char temp[NAME_LEN];

	// Get name of light
	count = fscanf(in, "%s", name);
	assert(count == 1);

	// Consume the delimiter
	count = fscanf(in, "%s", temp);
	assert(*temp == '{');

	// Parser
	light_parse[0].loc = &location;
	light_parse[1].loc = &emissivity;
	mask = parser(in, light_parse, NUM_ATTRS, attrmax);
	assert(mask == 3);

	// Add the object to the light list
	model->lgts->add((void *)this);
}

void light_t::getemiss(drgb_t *newEmissLoc) {
	pix_copy(&emissivity, newEmissLoc);
}

void light_t::getloc(vec_t *newLoc) {
	vec_copy(&location, newLoc);
}

void light_t::printer(FILE *out) {
	fprintf(out, "%-12s %s \n", "light", name);
	fprintf(out, "%-12s %-6.01lf %-6.01lf %-6.01lf\n", "location",
		location.x, location.y, location.z);
	fprintf(out, "%-12s %-6.01lf %-6.01lf %-6.01lf\n", "emissivity",
		emissivity.r, emissivity.g, emissivity.b);
}

void light_list_print(model_t *model, FILE *out) {
	light_t *lgt;
	list_t *list = model->lgts;

	list->reset();
	while (list->not_end()) {
		lgt = (light_t *)list->get_entity();
		lgt->printer(out);
		fprintf(out, "\n");
		list->next_link();
	}
}

void light_t::illuminate(model_t *model, vec_t *base, object_t *hitobj, 
								drgb_t *pixel) {

	vec_t dir;			// unit direction to light from hitpt
	object_t *obj;		// closest object in dir to light
	double close;		// dist to closest object in dir to light
	double cos;			// of angle between normal and dir to light
	double dist;		// distance to the light from hitpoint
	vec_t last_hitpt;	// from hitobj argument
	drgb_t diffuse = {0.0, 0.0, 0.0};
	vec_t last_norm;


	// Get last_hitpt from hitobj
	hitobj->getlast_hitpt(&last_hitpt);

	// Get the vector from the last_hitpt to the light's location
	vec_diff(&last_hitpt, &location , &dir);
	dist = vec_len(&dir);
	vec_unit(&dir, &dir);

	//== Test for self-occulation ==

	// get last normal
	hitobj->getlast_normal(&last_norm);


	// Solve dot product of hit point and surface normal
	cos = vec_dot(&dir, &last_norm);
	if (cos <= 0) {
		return;
	}

	// See if there is another object between the light and the current object
	obj = find_closest_object(model, &last_hitpt, &dir, hitobj, &close);

	// If there is another object, return
	if ((close < dist) && (obj != NULL)) {
		return;
	}

	// Get diffuse of object
	hitobj->getdiffuse(&diffuse);

	// Compute componentwise the diffuse reflectivity by the emmisivity
	pix_prod(&diffuse, &emissivity, &diffuse);

	// Scale the resulting diffuse reflectivity by cos/dist
	pix_scale(cos / dist, &diffuse, &diffuse);

	// Add scaled value to pixel
	pix_sum(&diffuse, pixel, pixel);

	return;
}

