/*	----------------------------------------------------------------------------
	Tracy Sanders
	tlsande
	CPSC 1020-001
	11/04/2015
	Major Programming Assignment #4

	This file is the converted sphere file from C to C++. It constructs the
  sphere object and loads its attributes with the universal parser.
	----------------------------------------------------------------------------
*/

#include "ray.h"

// For the generalized parser
static pparm_t sphere_parse[] =
{
  {"center", 3, sizeof(double), "%lf", 0},
  {"radius", 1, sizeof(double), "%lf", 0},
};

#define NUM_ATTRS (sizeof(sphere_parse) / sizeof(pparm_t))

// Sphere Constructor
sphere_t::sphere_t(FILE *in, model_t *model, int attrmax) : object_t(in, model) {
  int mask;

  strcpy(obj_type, "sphere");

  // Parser
  sphere_parse[0].loc = &center;
  sphere_parse[1].loc = &radius;
  mask = parser(in, sphere_parse, NUM_ATTRS, attrmax);
  assert(mask == 3);
}

double sphere_t::hits(vec_t *base, vec_t *dir) {
  vec_t *newBase = (vec_t *) malloc(sizeof(vec_t));
  double a; // D dot D
  double b; // 2(V' dot D)
  double c; // V' dot V' - r^2
  double temp, dist, vecLen;
  vec_t *tempVec = (vec_t *) malloc(sizeof(vec_t));

  // Get new base V' = V - C
  vec_diff(&center, base, newBase);

  // Solve D dot D
  a = vec_dot(dir, dir);

  // Solve 2(V' dot D)
  b = 2 * (vec_dot(newBase, dir));

  // Solve V' dot V' - r^2
  c = (vec_dot(newBase, newBase)) - pow(radius, 2);

  // Solve b^2-4ac
  temp = pow(b, 2) - (4 * a * c);

  if (temp < 0) {
    return -1;
  }
  else if (temp == 0) {
    return -1;
  }
  else if (temp > 0) {
    dist = (-b - sqrt(pow(b, 2) - (4 * a * c))) / (2 * a);
  }

  // Solve for the hit point using H = V + tD
  vec_scale(dist, dir, &last_hitpt);
  vec_sum(&last_hitpt, base, &last_hitpt);

  // Continue only if hitpoint.z is negative
  if (last_hitpt.z > 0) {
    return -1;
  }

  // copy hitpoint to obj->last_hit
  //object_t::getlast_hitpt(hitpoint);
  
  //== Solve Normal ==
  // N = (H - C) / ||(H - C)|| where H is the hit point, and C is the Center
  vec_diff(&center, &last_hitpt, tempVec);
  vecLen = vec_len(tempVec);
  vec_scale(1 / vecLen, tempVec, &last_normal);

  //printf("%lf, %lf, %lf, :  %lf, %lf, %lf\n", center.x, center.y, center.z,
  //	last_normal.x, last_normal.y, last_normal.z);
  // return distance
  return dist;
}

void sphere_t::printer(FILE *out) {
  object_t::printer(out);

  fprintf(out, "%-12s %-6.01lf %-6.01lf %-6.01lf\n", "center",
    center.x, center.y, center.z);
  fprintf(out, "%-12s %-6.01lf\n\n", "radius", radius);
}
