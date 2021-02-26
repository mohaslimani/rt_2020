/* ************************************************************************** */

#include <rt.h>

t_object rt_sl_plan(t_object *o, t_vec ax)
{
  t_object plan;

	plan.pos = o->sl_pnt;
	plan.rot = vec_pro_k(ax, -1);
	plan.col = o->col;
	plan.noi = o->noi;
	plan.txt = o->txt;
	plan.mat = o->mat;
  return (plan);
}

int		in_sphere(t_object *o)
{
	if ( vec_length(vec_sub(o->sl_pnt, o->pos)) >= o->size)
		return (0);
	return (1);
}

int		in_cylindr(t_object *o)
{
	t_vec a;
	t_vec b;
	double c;

	a = vec_sub(o->pos, o->sl_pnt);
	b = vec_cross(a, o->rot);
	c = vec_length(b) / vec_length(o->rot);
	if (c >= o->size)
		return 0;
	return (1);
}

int		in_cone(t_object *o)
{
	t_vec a;
	t_vec b;
	double c;

	a = vec_sub(o->pos, o->sl_pnt);
	b = vec_cross(a, o->rot);
	c = vec_length(b) / vec_length(o->rot);
	if (c >= tan(o->size / 2))
		return 0;
	return (1);
}

int			rt_slicing(t_object *o, t_ray *r, t_hit *rec)
{
	t_vec     ax;
	t_vec     my;
  	t_object  plan;
	t_vec	p;
	t_hit	recp;
	int		ret;

  	o->sl_vec = vec(0, 1, 0);//sl_vec
	  //check if sl_pnt is inside the object before put o->is_sliced = 1;
  	o->sl_pnt = vec(1,5,1);//sl_pnt
	//if (in_sphere(o) == 0)return 1;
	//if (in_cylindr(o) == 0)return 1;
	//if (in_cone(o) == 0)return 1;
	p = vec_ray(r, rec->t0);
  	ax = vec_unit(o->sl_vec);
  	my = vec_unit(vec_sub(p, o->sl_pnt));
	if (vec_dot(my, ax) <= 0)
	{
		p = vec_ray(r, rec->t1);
		my = vec_unit(vec_sub(p, o->sl_pnt));
		if (vec_dot(my, ax) <= 0)
			return(0);
		ft_memcpy(&recp, rec, sizeof(t_hit));
		plan = rt_sl_plan(o, ax);
		recp.negative[0] = 0;
		recp.negative[1] = 0;
		recp.negative_normal = (t_vec){0, 0, 0};
		ret = rt_hit_plan(&plan, r, &recp);
		if (ret == 1 && recp.t < rec->t1)
		{
			rec->tx = rec->t0;
			rec->t0 = recp.t;
			o->sl_sl = recp.t;
			return (1);
		}
		else
			return 0;
	}
	return 1;
}

int			rt_negative_cylinder(t_object *o, t_ray *r, t_hit *rec)
{
	rec->or = vec_sub(r->origin, o->pos);
	rec->coef[0] = vec_dot(r->dir, r->dir) - pow(vec_dot(r->dir, o->rot), 2);
	rec->coef[1] = 2 * (vec_dot(r->dir, rec->or) - (vec_dot(r->dir, o->rot)
				* vec_dot(rec->or, o->rot)));
	rec->coef[2] = vec_dot(rec->or, rec->or) - pow(vec_dot(rec->or, o->rot), 2)
		- o->size * o->size;
	rec->delta = rec->coef[1] * rec->coef[1] - 4 * rec->coef[0] * rec->coef[2];
	if (rec->delta >= 0)
	{
		rec->t0 = (-rec->coef[1] - sqrt(rec->delta)) / (2 * rec->coef[0]);
		rec->t1 = (-rec->coef[1] + sqrt(rec->delta)) / (2 * rec->coef[0]);
		(rec->t0 > rec->t1) ? ft_float_swap(&rec->t0, &rec->t1) : 0;
		rec->negative[0] = rec->t0;
		rec->negative[1] = rec->t1;
		rec->p = vec_ray(r, rec->negative[1]);
		rec->negative_normal = vec_unit(vec_sub(vec_sub(rec->p, o->pos), vec_pro_k(o->rot, vec_dot(r->dir, o->rot) * rec->t + vec_dot(rec->or, o->rot))));
		rec->negative_normal = vec_pro_k(rec->negative_normal, -1);
	}
	else
	{
		rec->negative[0] = 0;
		rec->negative[1] = 0;
		rec->negative_normal = (t_vec){0, 0, 0};
	}
	return (0);
}

int			rt_negative_sphere(t_object *sphere, t_ray *r, t_hit *rec)
{
	t_vec tmp;

	rec->or = vec_sub(r->origin, sphere->pos);
	rec->coef[0] = vec_dot(r->dir, r->dir);
	rec->coef[1] = 2 * vec_dot(rec->or, r->dir);
	rec->coef[2] = vec_dot(rec->or, rec->or) - pow(sphere->size, 2);
	rec->delta = rec->coef[1] * rec->coef[1] - 4 * rec->coef[0] * rec->coef[2];
	if (rec->delta > 0)
	{
		rec->negative[0] = (-rec->coef[1] - sqrt(rec->delta)) / (2 * rec->coef[0]);
		rec->negative[1] = (-rec->coef[1] + sqrt(rec->delta)) / (2 * rec->coef[0]);
		(rec->negative[0] > rec->negative[1]) ? ft_float_swap(&rec->negative[0], &rec->negative[1]) : 0;
		tmp = vec_ray(r, rec->negative[1]);
		rec->negative_normal = vec_div_k(vec_sub(tmp, sphere->pos), -sphere->size);
	}
	else
	{
		rec->negative[0] = 0;
		rec->negative[1] = 0;
		rec->negative_normal = (t_vec){0, 0, 0};
	}
	return (0);
}

int			rt_negative_cone(t_object *o, t_ray *r, t_hit *rec)
{
	t_vec tmp;

	rec->or = vec_sub(r->origin, o->pos);
	rec->coef[0] = vec_dot(r->dir, r->dir) - ((1 + pow(tan(o->size), 2))
		* pow(vec_dot(r->dir, o->rot), 2));
	rec->coef[1] = 2 * (vec_dot(r->dir, rec->or) - ((1 + pow(tan(o->size), 2))
			* vec_dot(rec->or, o->rot) * vec_dot(r->dir, o->rot)));
	rec->coef[2] = vec_dot(rec->or, rec->or) - ((1 + pow(tan(o->size), 2)) // * rec->or.z * rec->or.z);
		* pow(vec_dot(rec->or, o->rot), 2));
	rec->delta = rec->coef[1] * rec->coef[1] - 4 * rec->coef[0] * rec->coef[2];
	if (rec->delta > 0)
	{
		rec->negative[0] = (-rec->coef[1] - sqrt(rec->delta)) / (2 * rec->coef[0]);
		rec->negative[1] = (-rec->coef[1] + sqrt(rec->delta)) / (2 * rec->coef[0]);
		(rec->negative[0] > rec->negative[1]) ? ft_float_swap(&rec->negative[0], &rec->negative[1]) : 0;
		tmp = vec_ray(r, rec->negative[1]);
		rec->t = rec->negative[1];
		rec->negative_normal = normale_cone(o, r, rec);
		rec->negative_normal = vec_pro_k(rec->negative_normal, -1);
	}
	else
	{
		rec->negative[0] = 0;
		rec->negative[1] = 0;
		rec->negative_normal = (t_vec){0, 0, 0};
	}
	return (0);
}

// float power;

// t_vec		sceneinfo(t_vec position) {
//     t_vec z = position;
// 	float dr = 1.0;
// 	float r = 0.0;
//     int iterations = 0;

// 	for (int i = 0; i < 15 ; i++) {
//         iterations = i;
// 		r = length(z);

// 		if (r>2) {
//             break;
//         }
        
// 		// convert to polar coordinates
// 		float theta = acos(z.z/r);
// 		float phi = atan2(z.y,z.x);
// 		dr =  pow( r, power-1.0)*power*dr + 1.0;

// 		// scale and rotate the point
// 		float zr = pow( r,power);
// 		theta = theta*power;
// 		phi = phi*power;
		
// 		// convert back to cartesian coordinates
// 		z = vec_pro_k((t_vec){sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta)}, zr);
// 		z = vec_add(z, position);
// 	}
//     float dst = 0.5*log(r)*r/dr;
// 	return ((t_vec){iterations,dst*1, 0});
// }
// static const float epsilon = 0.001f;
// int		ray_march(t_ray *r, t_hit *rec)
// {
//     int width = IMG_WIDTH;
// 	int height = IMG_HEIGHT;

//     float rayDst = 0;
//     int marchSteps = 0;

//     while (rayDst < MAX && marchSteps < 5) {
//         marchSteps ++;
//         t_vec sceneInfo = sceneinfo(r->origin);
//         float dst = sceneInfo.y;
// 		t_vec
        
//         // Ray has hit a surface
//         if (dst <= epsilon) {
//             float escapeIterations = sceneInfo.x;
//             t_vec normal = EstimateNormal(ray.origin-ray.direction*epsilon*2);

//             //float colourA = saturate(dot(normal*.5+.5,-_LightDirection));
//             //float colourB = saturate(escapeIterations/16.0);
//             //t_vec colourMix = saturate(colourA * colourAMix + colourB * colourBMix);

//             result = float4(colourMix.xyz,1);
//             break;
//         }
//         ray.origin += ray.direction * dst;
//         rayDst += dst;
//     }

//     float rim = marchSteps/darkness;
//     Destination[id.xy] = lerp(result, 1, blackAndWhite) * rim;
// }
// }

// float distance_from_sphere(t_vec p, t_vec c, float r)
// {
//     return vec_length(vec_sub(vec_sub(p, c), (t_vec){r, r, r}));
// }

// float map_the_world(t_vec p)
// {
//     float sphere_0 = distance_from_sphere(p, (t_vec){0, 0, 0} , 1.0);

//     return sphere_0;
// }

// t_vec	calculate_normal(t_vec p)
// {
//     const float small_step = 0.001;

//     float gradient_x = map_the_world((t_vec){p.x + small_step, p.y, p.z}) - map_the_world((t_vec){p.x - small_step, p.y, p.z});
//     float gradient_y = map_the_world((t_vec){p.x, p.y + small_step, p.z}) - map_the_world((t_vec){p.x, p.y - small_step, p.z});
//     float gradient_z = map_the_world((t_vec){p.x, p.y, p.z + small_step}) - map_the_world((t_vec){p.x, p.y, p.z - small_step});

//     t_vec normal = (t_vec){gradient_x, gradient_y, gradient_z};

//     return vec_unit(normal);
// }

// float	ft_max(double a, double b)
// {
// 	return ((a > b) ? a : b);
// }

// t_vec	ray_march(t_ray *r, t_hit *rec)//t_ray *r
// {
//     float total_distance_traveled = 0.0;
//     const int NUMBER_OF_STEPS = 32;
//     const float MINIMUM_HIT_DISTANCE = 0.001;
//     const float MAXIMUM_TRACE_DISTANCE = 1000.0;

//     for (int i = 0; i < NUMBER_OF_STEPS; ++i)
//     {
//         t_vec current_position = vec_add(r->origin, vec_pro_k(r->dir, total_distance_traveled));

//         float distance_to_closest = map_the_world(current_position);

//         if (distance_to_closest < MINIMUM_HIT_DISTANCE) 
//         {
//             t_vec normal = calculate_normal(current_position);
//             t_vec light_position = (t_vec){2.0, -5.0, 3.0};
//             t_vec direction_to_light = vec_unit(vec_sub(current_position, light_position));

//             float diffuse_intensity = ft_max(0.0, vec_dot(normal, direction_to_light));
// 			rec->n = normal;
			
//             return vec_pro_k((t_vec){1.0, 0.0, 0.0}, diffuse_intensity);
//         }

//         if (total_distance_traveled > MAXIMUM_TRACE_DISTANCE)
//         {
//             break;
//         }
//         total_distance_traveled += distance_to_closest;
//     }
//     return (t_vec){0, 0, 0};
// }

int rt_hit(t_scene *scene, t_ray *r, t_hit *record)
{
  t_object  *obj;
  int      check_hit;
  
 t_object nega;
 // negative sphere
//   nega.pos = vec(1, 5, 1);
//   nega.size = 1;
//   nega.col = vec(0, 0, 0);
//   rt_negative_sphere(&nega, r, record);
  /* if (there is no negative object)*/
		// record->negative[0] = 0;
		// record->negative[1] = 0;
		// record->negative_normal = (t_vec){0, 0, 0};
	//negative cylindre
	// nega.pos = vec(0, 0, 0);
	// nega.rot = vec_unit(vec(0, 1, 0));
	// nega.size = 2;
	// rt_negative_cylinder(&nega, r, record);

	//negative cone
	nega.pos = vec(1, 1, -3);
	nega.rot = vec_unit(vec(0, 1, 0));
	nega.size = tan(3.14/6);
	rt_negative_cone(&nega, r, record);
  check_hit = 0;
  record->closest = MAX;
  obj = scene->object;
  while (obj)
  {
    if (obj->hit(obj, r, record))
    {
      record->closest = record->t;
      record->curr_obj = obj;
      record->ray = r;
      check_hit = 1;
    }
    obj = obj->next;
  }
  return (check_hit);
}