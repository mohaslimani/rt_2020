
#include<rt.h>

void    cone_uv(t_object *o, t_hit *rec)
{
    t_vec d;
    double scale;

    scale = 1.0;
	d = vec_pro_k(vec_sub(rec->p, o->pos), scale);
	d = vec(vec_dot(d, o->vec1), vec_dot(d, o->rot), vec_dot(d, o->vec2));
	rec->u = (atan2(d.x, d.z) + M_PI / (2.0 * M_PI));
	rec->v = d.y;
    rec->u = rec->u - floor(rec->u);
    rec->v = rec->v - floor(rec->v);
}

t_vec   normale_cone(t_object *o, t_ray *r, t_hit *rec)
{
    t_vec normale;

	rec->tmp = vec_pro_k(o->rot, (vec_dot(r->dir, o->rot) * rec->t
				+ vec_dot(rec->or, o->rot)));
	rec->tmp = vec_pro_k(rec->tmp, (1 + pow(tan(o->size), 2)));
    normale = vec_unit(vec_sub(vec_sub(rec->p, o->pos), rec->tmp));
    normale = vec_unit(normale); 
   return (normale);
}

/*
int			ft_hit_cone(t_object *o, t_ray *r, t_record *rec)
{
	rec->oc = ft_minus(r->origin, o->pos);
	rec->coef[0] = ft_dot(r->dir, r->dir) - (1 + pow(tan(o->size), 2))
		* pow(ft_dot(r->dir, o->rot), 2);
	rec->coef[1] = 2 * (ft_dot(r->dir, rec->oc) - (1 + pow(tan(o->size), 2))
			* ft_dot(r->dir, o->rot) * ft_dot(rec->oc, o->rot));
	rec->coef[2] = ft_dot(rec->oc, rec->oc) - (1 + pow(tan(o->size), 2))
		* pow(ft_dot(rec->oc, o->rot), 2);
	rec->delta = rec->coef[1] * rec->coef[1] - 4 * rec->coef[0] * rec->coef[2];
	if (rec->delta < 0)
		return (0);
	rec->sol[0] = (-rec->coef[1] - sqrt(rec->delta)) / (2 * rec->coef[0]);
	rec->sol[1] = (-rec->coef[1] + sqrt(rec->delta)) / (2 * rec->coef[0]);
	(rec->sol[0] < rec->sol[1]) ? 0 : ft_int_swap(&rec->sol[0], &rec->sol[1]);
	double m = ft_dot(r->dir, o->rot) * rec->sol[0] + ft_dot(rec->oc, o->rot);
	if (m <= 2 || m >= 4)// 5 howa hight dial cylindre dialk
	{
		m = ft_dot(r->dir, o->rot) * rec->sol[1] + ft_dot(rec->oc, o->rot);
		if (m <= 2 || m >= 4)
			return (0);
		ft_int_swap(&rec->sol[0], &rec->sol[1]);
	}
	if (rec->sol[0] < rec->closest && rec->sol[0] > TMIN)
	{
		rec->t = rec->sol[0];
		rec->p = ray_fctn(r, rec->t);
		rec->normal = ft_unit_vec(ft_minus(ft_minus(rec->p, o->pos), rec->tmp));
		rec->normal = (rec->sol[0] > rec->sol[1]) ? ft_pro_k(rec->normal, -1) : rec->normal;
		return (1);
	}
	return (0);
}
*/
int			rt_hit_cone(t_object *o, t_ray *r, t_hit *rec)//hit_limited cone
{
	double m;
	o->height = 0;//the min_cap
	o->height1 = 3;//the max cap //o->height ]-oo,+oo[ //o->height < height1
	rec->or = vec_sub(r->origin, o->pos);
	rec->coef[0] = vec_dot(r->dir, r->dir) - ((1 + pow(tan(o->size), 2))
		* pow(vec_dot(r->dir, o->rot), 2));
	rec->coef[1] = 2 * (vec_dot(r->dir, rec->or) - ((1 + pow(tan(o->size), 2))
			* vec_dot(rec->or, o->rot) * vec_dot(r->dir, o->rot)));
	rec->coef[2] = vec_dot(rec->or, rec->or) - ((1 + pow(tan(o->size), 2)) // * rec->or.z * rec->or.z);
		* pow(vec_dot(rec->or, o->rot), 2));
	rec->delta = rec->coef[1] * rec->coef[1] - 4 * rec->coef[0] * rec->coef[2];
	if (rec->delta < 0)
		return (0);
	rec->t0 = (-rec->coef[1] - sqrt(rec->delta)) / (2 * rec->coef[0]);
	rec->t1 = (-rec->coef[1] + sqrt(rec->delta)) / (2 * rec->coef[0]);
	(rec->t0 < rec->t1) ? 0 : ft_float_swap(&rec->t0, &rec->t1);
	m = vec_dot(r->dir, o->rot) * rec->t0 + vec_dot(rec->or, o->rot);
	if (m <= o->height || m >= o->height1)
	{
		m = vec_dot(r->dir, o->rot) * rec->t1 + vec_dot(rec->or, o->rot);
		if (m <= o->height || m >= o->height1)
			return (0);
		ft_float_swap(&rec->t0, &rec->t1);
	}
	rec->t = rec->t0;
	if (rec->t < rec->closest && rec->t > MIN)
	{
		rec->p = vec_ray(r, rec->t);
        rec->n = normale_cone(o, r, rec);
		rec->n = (rec->t0 > rec->t1) ? vec_pro_k(rec->n, -1) : rec->n;
		cone_uv(o, rec);
        return (1);
	}
	return (0);
}


// int     rt_hit_cone(t_object *obj, t_ray *ray, t_hit *record)
// {
//     // double aux;

//     // t_vec tmp;
// 	obj->size *=  M_PI / 180;
// 	obj->dir = vec(0, 1, 10);
//     record->or = vec_sub(ray->origin, obj->pos);
//     // aux = vec_dot(ray->dir, vec_unit(obj->rot));
//     record->a = vec_dot(ray->dir, obj->rot) * vec_dot(ray->dir, obj->rot) - (1 + pow(obj->size, 2));
// 	record->b = 2 * vec_dot(ray->dir, obj->rot) * vec_dot(record->or, obj->rot) - vec_dot(ray->dir, record->or) * obj->size *obj->size;
// 	record->c = vec_dot(record->or, obj->rot) * vec_dot(record->or, obj->rot) - vec_dot(record->or, record->or) * obj->size * obj->size;
//     record->delta = pow(record->b, 2) - 4.0 * record->a * record->c;
//      if(record->delta < 0)
//         return (0);
//     record->t0 = (-record->b - sqrt(record->delta)) / (2 * record->a);
// 	record->t1 = (-record->b + sqrt(record->delta)) / (2 * record->a);

// 	record->t = record->t0;
// 	if ((record->t < 0 || record->t1 > 0) && record->t1 < record->t)
// 		record->t = record->t1;
// 	if (record->t < 0)
// 		return (0);
//      if (record->t < record->closest)
//     {
//         record->p = vec_ray(ray, record->t);
// 		record->n = vec_unit(vec_sub(vec_pro_k(record->p,\
// 				vec_dot(obj->rot, record->p) / vec_dot(record->p, record->p)), obj->rot));
//         cone_uv(obj, record);
// 	    return (1);
//     }
// 	return (0);
// }


// int			rt_hit_cone(t_object *o, t_ray *r, t_hit *rec)
// {
// 	rec->or = vec_sub(r->origin, o->pos);
// 	rec->coef[0] = vec_dot(r->dir, r->dir) - ((1 + pow(tan(o->size), 2))
// 		* pow(vec_dot(r->dir, o->rot), 2));
// 	// rec->coef[0] = vec_dot(r->dir, r->dir) - (1 + pow(tan(o->size), 2))
// 	// 	* pow(r->dir.z, 2);
// 	rec->coef[1] = 2 * (vec_dot(r->dir, rec->or) - ((1 + pow(tan(o->size), 2))
// 			* vec_dot(rec->or, o->rot) * vec_dot(r->dir, o->rot)));
// 	rec->coef[2] = vec_dot(rec->or, rec->or) - ((1 + pow(tan(o->size), 2)) // * rec->or.z * rec->or.z);
// 		* pow(vec_dot(rec->or, o->rot), 2));
// 	rec->delta = rec->coef[1] * rec->coef[1] - 4 * rec->coef[0] * rec->coef[2];
// 	if (rec->delta < 0)
// 		return (0);
// 	rec->t0 = (-rec->coef[1] - sqrt(rec->delta)) / (2 * rec->coef[0]);
// 	rec->t1 = (-rec->coef[1] + sqrt(rec->delta)) / (2 * rec->coef[0]);
// 	rec->t0 = (rec->t0 > MIN && rec->t0 < rec->t1) ?
// 					rec->t0 : rec->t1;
// 	if (rec->t0 < rec->closest && rec->t0 > MIN)
// 	{
// 		rec->t = rec->t0;
// 		rec->p = vec_ray(r, rec->t);
// 		rec->tmp = vec_pro_k(o->rot, (vec_dot(r->dir, o->rot) * rec->t
// 					+ vec_dot(rec->or, o->rot)));
// 		rec->tmp = vec_pro_k(rec->tmp, (1 + pow(tan(o->size), 2)));
//         rec->n = normale_cone(o, r, rec);
// 		cone_uv(o, rec);
//         return (1);
// 	}
// 	return (0);
// }

