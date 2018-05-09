/*	----------------------------------------------------------------------------
	Tracy Sanders
	tlsande
	CPSC 1020-001
	11/04/2015
	Major Programming Assignment #4

	This file is the converted plane file from C to C++. It constructs the plane
  objects and calls the universal parser to load its attributes.
	----------------------------------------------------------------------------
*/

#include "ray.h"

// For the generalized parser
static pparm_t plane_parse[] =
{
  {"point", 3, sizeof(double), "%lf", 0},
  {"normal", 3, sizeof(double), "%lf", 0},
};

#define NUM_ATTRS (sizeof(plane_parse) / sizeof(pparm_t))

// Plane constructor
plane_t::plane_t(FILE *in, model_t *model,
    int attrmax) : object_t(in, model) {
  int mask;

  strcpy(obj_type, "plane");

  // Parser
  plane_parse[0].loc = &point;
  plane_parse[1].loc = &normal;
  mask = parser(in, plane_parse, NUM_ATTRS, attrmax);
  assert(mask == 3);

  // Unit vector of the normal vector
  vec_unit(&normal, &normal);
  vec_copy(&normal, &last_normal);

  ndotq = vec_dot(&point, &normal);
}

void plane_t::printer(FILE *out) {
  // Calls object printer
  object_t::printer(out);

  // Prints unique plane attributes
  fprintf(out, "%-12s %-6.01lf %-6.01lf %-6.01lf\n", "normal",
    normal.x, normal.y, normal.z);
  fprintf(out, "%-12s %-6.01lf %-6.01lf %-6.01lf\n", "point",
    point.x, point.y, point.z);
}

double plane_t::hits(vec_t *base, vec_t *dir) {
  double ndotd;
  double t;
  double ndotb;
  ndotq = vec_dot(&normal, &point);
  ndotd = vec_dot(dir, &normal);

  // If ndotd = 0 then ray is parallel to the plane
  if (ndotd == 0) {
    return -1;
  }
  // Computes ndotq
  ndotb = vec_dot(&normal, base);

  // Compute distance
  t = (ndotq - ndotb) / ndotd;

  // Only continue if the distance is positive
  if (t <= 0) {
    return -1;
  }

  // Compute location of hit point by scaling dir by distance (t) and adding
  // to base
  //object_t::getlast_hitpt(templast_hit);
  vec_scale(t, dir, &last_hitpt);
  vec_sum(&last_hitpt, base, &last_hitpt);

  if ((last_hitpt.z > 0.01) && (strcmp(obj_type, "projector"))) {
    return -1;
  }

  return t;

}
