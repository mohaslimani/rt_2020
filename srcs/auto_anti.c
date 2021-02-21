/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auto_anti.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoulaim <msoulaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/21 18:28:13 by msoulaim          #+#    #+#             */
/*   Updated: 2021/02/21 19:30:57 by msoulaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rt.h>

void			my_mlx_putpixel(t_vec *data, int x, int y, t_vec color)
{
	if (x >= 0 && x < IMG_WIDTH && y >= 0 && y < IMG_HEIGHT)
		data[y * (int)IMG_WIDTH + x] = color;
}

t_vec			my_mlx_getpixel(t_vec *data, int x, int y)
{
	if (x >= 0 && x < IMG_WIDTH && y >= 0 && y < IMG_HEIGHT)
		return (data[y * (int)IMG_WIDTH + x]);
	return ((t_vec){0, 0, 0});
}


t_vec		anti_aa(t_thread *th, double i, double j, int select)
{
	t_vec color[9];
	t_vec	col = (t_vec){0, 0, 0};
	t_ray r[9];

	if (select == 0)
	{
		r[0] = rt_get_ray(&th->rt->scene->cam, i + ((0 + 0.5) / sqrt(th->rt->scene->anti_aliasing)), j + ((0 + 0.5) / sqrt(th->rt->scene->anti_aliasing)));
		color[0] = rt_raytracer(th, &r[0], 50);
		col = color[0];
		my_mlx_putpixel(th->rt->scene->data1[0], i, IMG_HEIGHT - j, color[0]);
	}
	else if (select == 1)
	{
		r[1] = rt_get_ray(&th->rt->scene->cam, i + ((0 + 0.5) / sqrt(th->rt->scene->anti_aliasing)), j + ((1 + 0.5) / sqrt(th->rt->scene->anti_aliasing)));
		color[1] = rt_raytracer(th, &r[1], 50);
		color[0] = my_mlx_getpixel(th->rt->scene->data1[0], i, IMG_HEIGHT - j);
		col = vec_add(color[1], color[0]);
		my_mlx_putpixel(th->rt->scene->data1[1], i, IMG_HEIGHT - j, color[1]);
	}
	else if (select == 2)
	{
		r[2] = rt_get_ray(&th->rt->scene->cam, i + ((1 + 0.5) / sqrt(th->rt->scene->anti_aliasing)), j + ((0 + 0.5) / sqrt(th->rt->scene->anti_aliasing)));
		color[2] = rt_raytracer(th, &r[2], 50);
		color[0] = my_mlx_getpixel(th->rt->scene->data1[0], i, IMG_HEIGHT - j);
		color[1] = my_mlx_getpixel(th->rt->scene->data1[1], i, IMG_HEIGHT - j);
		col = vec_add(color[1], color[0]);
		col = vec_add(color[2], col);
		my_mlx_putpixel(th->rt->scene->data1[2], i, IMG_HEIGHT - j, color[2]);
	}
	else if (select == 3)
	{
		r[3] = rt_get_ray(&th->rt->scene->cam, i + ((1 + 0.5) / sqrt(th->rt->scene->anti_aliasing)), j + ((1 + 0.5) / sqrt(th->rt->scene->anti_aliasing)));
		color[3] = rt_raytracer(th, &r[3], 50);
		color[0] = my_mlx_getpixel(th->rt->scene->data1[0], i, IMG_HEIGHT - j);
		color[1] = my_mlx_getpixel(th->rt->scene->data1[1], i, IMG_HEIGHT - j);
		color[2] = my_mlx_getpixel(th->rt->scene->data1[2], i, IMG_HEIGHT - j);
		col = vec_add(color[1], color[0]);
		col = vec_add(color[2], col);
		col = vec_add(color[3], col);
		my_mlx_putpixel(th->rt->scene->data1[3], i, IMG_HEIGHT - j, color[3]);
	}
	else if (select == 4)
	{
		r[4] = rt_get_ray(&th->rt->scene->cam, i + ((0 + 0.5) / sqrt(th->rt->scene->anti_aliasing)), j + ((2 + 0.5) / sqrt(th->rt->scene->anti_aliasing)));
		color[4] = rt_raytracer(th, &r[4], 50);
		color[0] = my_mlx_getpixel(th->rt->scene->data1[0], i, IMG_HEIGHT - j);
		color[1] = my_mlx_getpixel(th->rt->scene->data1[1], i, IMG_HEIGHT - j);
		color[2] = my_mlx_getpixel(th->rt->scene->data1[2], i, IMG_HEIGHT - j);
		color[3] = my_mlx_getpixel(th->rt->scene->data1[3], i, IMG_HEIGHT - j);
		col = vec_add(color[1], color[0]);
		col = vec_add(color[2], col);
		col = vec_add(color[3], col);
		col = vec_add(color[4], col);
		my_mlx_putpixel(th->rt->scene->data1[4], i, IMG_HEIGHT - j, color[4]);
	}
	else if (select == 5)
	{
		r[5] = rt_get_ray(&th->rt->scene->cam, i + ((2 + 0.5) / sqrt(th->rt->scene->anti_aliasing)), j + ((0 + 0.5) / sqrt(th->rt->scene->anti_aliasing)));
		color[5] = rt_raytracer(th, &r[5], 50);
		color[0] = my_mlx_getpixel(th->rt->scene->data1[0], i, IMG_HEIGHT - j);
		color[1] = my_mlx_getpixel(th->rt->scene->data1[1], i, IMG_HEIGHT - j);
		color[2] = my_mlx_getpixel(th->rt->scene->data1[2], i, IMG_HEIGHT - j);
		color[3] = my_mlx_getpixel(th->rt->scene->data1[3], i, IMG_HEIGHT - j);
		color[4] = my_mlx_getpixel(th->rt->scene->data1[4], i, IMG_HEIGHT - j);
		col = vec_add(color[1], color[0]);
		col = vec_add(color[2], col);
		col = vec_add(color[3], col);
		col = vec_add(color[4], col);
		col = vec_add(color[5], col);
		my_mlx_putpixel(th->rt->scene->data1[5], i, IMG_HEIGHT - j, color[5]);
	}
	else if (select == 6)
	{
		r[6] = rt_get_ray(&th->rt->scene->cam, i + ((2 + 0.5) / sqrt(th->rt->scene->anti_aliasing)), j + ((2 + 0.5) / sqrt(th->rt->scene->anti_aliasing)));
		color[6] = rt_raytracer(th, &r[6], 50);
		color[0] = my_mlx_getpixel(th->rt->scene->data1[0], i, IMG_HEIGHT - j);
		color[1] = my_mlx_getpixel(th->rt->scene->data1[1], i, IMG_HEIGHT - j);
		color[2] = my_mlx_getpixel(th->rt->scene->data1[2], i, IMG_HEIGHT - j);
		color[3] = my_mlx_getpixel(th->rt->scene->data1[3], i, IMG_HEIGHT - j);
		color[4] = my_mlx_getpixel(th->rt->scene->data1[4], i, IMG_HEIGHT - j);
		color[5] = my_mlx_getpixel(th->rt->scene->data1[5], i, IMG_HEIGHT - j);
		col = vec_add(color[1], color[0]);
		col = vec_add(color[2], col);
		col = vec_add(color[3], col);
		col = vec_add(color[4], col);
		col = vec_add(color[5], col);
		col = vec_add(color[6], col);
		my_mlx_putpixel(th->rt->scene->data1[6], i, IMG_HEIGHT - j, color[6]);
	}
	else if (select == 7)
	{
		r[7] = rt_get_ray(&th->rt->scene->cam, i + ((1 + 0.5) / sqrt(th->rt->scene->anti_aliasing)), j + ((2 + 0.5) / sqrt(th->rt->scene->anti_aliasing)));
		color[7] = rt_raytracer(th, &r[7], 50);
		color[0] = my_mlx_getpixel(th->rt->scene->data1[0], i, IMG_HEIGHT - j);
		color[1] = my_mlx_getpixel(th->rt->scene->data1[1], i, IMG_HEIGHT - j);
		color[2] = my_mlx_getpixel(th->rt->scene->data1[2], i, IMG_HEIGHT - j);
		color[3] = my_mlx_getpixel(th->rt->scene->data1[3], i, IMG_HEIGHT - j);
		color[4] = my_mlx_getpixel(th->rt->scene->data1[4], i, IMG_HEIGHT - j);
		color[5] = my_mlx_getpixel(th->rt->scene->data1[5], i, IMG_HEIGHT - j);
		color[6] = my_mlx_getpixel(th->rt->scene->data1[6], i, IMG_HEIGHT - j);
		col = vec_add(color[1], color[0]);
		col = vec_add(color[2], col);
		col = vec_add(color[3], col);
		col = vec_add(color[4], col);
		col = vec_add(color[5], col);
		col = vec_add(color[6], col);
		col = vec_add(color[7], col);
		my_mlx_putpixel(th->rt->scene->data1[7], i, IMG_HEIGHT - j, color[7]);
	}
		else if (select == 8)
	{
		r[8] = rt_get_ray(&th->rt->scene->cam, i + ((2 + 0.5) / sqrt(th->rt->scene->anti_aliasing)), j + ((1 + 0.5) / sqrt(th->rt->scene->anti_aliasing)));
		color[8] = rt_raytracer(th, &r[8], 50);
		color[0] = my_mlx_getpixel(th->rt->scene->data1[0], i, IMG_HEIGHT - j);
		color[1] = my_mlx_getpixel(th->rt->scene->data1[1], i, IMG_HEIGHT - j);
		color[2] = my_mlx_getpixel(th->rt->scene->data1[2], i, IMG_HEIGHT - j);
		color[3] = my_mlx_getpixel(th->rt->scene->data1[3], i, IMG_HEIGHT - j);
		color[4] = my_mlx_getpixel(th->rt->scene->data1[4], i, IMG_HEIGHT - j);
		color[5] = my_mlx_getpixel(th->rt->scene->data1[5], i, IMG_HEIGHT - j);
		color[6] = my_mlx_getpixel(th->rt->scene->data1[6], i, IMG_HEIGHT - j);
		color[7] = my_mlx_getpixel(th->rt->scene->data1[7], i, IMG_HEIGHT - j);
		col = vec_add(color[1], color[0]);
		col = vec_add(color[2], col);
		col = vec_add(color[3], col);
		col = vec_add(color[4], col);
		col = vec_add(color[5], col);
		col = vec_add(color[6], col);
		col = vec_add(color[7], col);
		col = vec_add(color[8], col);
		my_mlx_putpixel(th->rt->scene->data1[8], i, IMG_HEIGHT - j, color[8]);
	}
	return(col);
}

t_vec				rt_anti_a(t_thread *th, double i, double j)
{
	t_vec		col;

	col = vec(0, 0, 0);
	col = anti_aa(th, i, j, th->rt->scene->select);
	col = vec_div_k(col, th->rt->scene->select + 1);

	// t_vec		col;
	// t_ray		r;
	// double		ss[2];
	// int			anti_a;

	// col = ft_vec(0, 0, 0);
	// ss[0] = -1;
	// anti_a = 1;
	// while (++ss[0] < anti_a)
	// {
	// 	ss[1] = -1;
	// 	while (++ss[1] < anti_a)
	// 	{
	// 		r = rt_get_ray(&th->rt->scene->cam, i + ((ss[0] + 0.5) / anti_a), j + ((ss[1] + 0.5) / anti_a));
	// 		col = vec_add(col, rt_raytracer(th, &r));, 50
	// 	}
	// }
	// col = ft_div_k(col, anti_a * anti_a);
	// ft_clamp(&col);
	 return (col);
}