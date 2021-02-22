/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auto_anti.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoulaim <msoulaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/21 18:28:13 by msoulaim          #+#    #+#             */
/*   Updated: 2021/02/22 17:58:03 by msoulaim         ###   ########.fr       */
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

t_vec			infinite_pixels(t_thread *t, t_vec cr, int select, t_vec cl)
{
	t_vec color;
	t_vec	tmp;
	int i;
	
	i = -1;
	color = cl;
	while (++i < select)
	{
		tmp = my_mlx_getpixel(t->rt->scene->data1[i], cr.x , IMG_HEIGHT - cr.y);
		color = vec_add(color, tmp);
	}
	return (color);
}

t_vec		anti_aa(t_thread *t, double col, double row, int select)
{
	t_vec color;
	t_vec	colo = (t_vec){0, 0, 0};
	t_ray r;
	double anti_a = sqrt(t->rt->scene->anti_aliasing);

	if (select == 0)
	{
		r = rt_get_ray(&t->rt->scene->cam, (double)((col + ((0 + 0.5)/ anti_a)) / IMG_WIDTH), (double)((row + ((0 + 0.5) / anti_a)) / IMG_HEIGHT));
		color = rt_raytracer(t, &r, 50);
		colo = infinite_pixels(t, (t_vec){col, row , 0}, select, color);
		my_mlx_putpixel(t->rt->scene->data1[0], col, IMG_HEIGHT - row, color);
	}
	else if (select == 1)
	{
		r = rt_get_ray(&t->rt->scene->cam, (double)((col + ((0 + 0.5)/ anti_a)) / IMG_WIDTH), (double)((row + ((1 + 0.5) / anti_a)) / IMG_HEIGHT));
		color = rt_raytracer(t, &r, 50);
		colo = infinite_pixels(t, (t_vec){col, row , 0}, select, color);
		my_mlx_putpixel(t->rt->scene->data1[1], col, IMG_HEIGHT - row, color);
	}
	else if (select == 2)
	{
		r = rt_get_ray(&t->rt->scene->cam, (double)((col + ((1 + 0.5)/ anti_a)) / IMG_WIDTH), (double)((row + ((0 + 0.5) / anti_a)) / IMG_HEIGHT));
		color = rt_raytracer(t, &r, 50);
		colo = infinite_pixels(t, (t_vec){col, row , 0}, select, color);
		my_mlx_putpixel(t->rt->scene->data1[2], col, IMG_HEIGHT - row, color);
	}
	else if (select == 3)
	{
		r = rt_get_ray(&t->rt->scene->cam, (double)((col + ((1 + 0.5)/ anti_a)) / IMG_WIDTH), (double)((row + ((1 + 0.5) / anti_a)) / IMG_HEIGHT));
		color = rt_raytracer(t, &r, 50);
		colo = infinite_pixels(t, (t_vec){col, row , 0}, select, color);
		my_mlx_putpixel(t->rt->scene->data1[3], col, IMG_HEIGHT - row, color);
	}
	else if (select == 4)
	{
		r = rt_get_ray(&t->rt->scene->cam, (double)((col + ((0 + 0.5)/ anti_a)) / IMG_WIDTH), (double)((row + ((2 + 0.5) / anti_a)) / IMG_HEIGHT));
		color = rt_raytracer(t, &r, 50);
		colo = infinite_pixels(t, (t_vec){col, row , 0}, select, color);
		my_mlx_putpixel(t->rt->scene->data1[4], col, IMG_HEIGHT - row, color);
	}
	else if (select == 5)
	{
		r = rt_get_ray(&t->rt->scene->cam, (double)((col + ((2 + 0.5)/ anti_a)) / IMG_WIDTH), (double)((row + ((0 + 0.5) / anti_a)) / IMG_HEIGHT));
		color = rt_raytracer(t, &r, 50);
		colo = infinite_pixels(t, (t_vec){col, row , 0}, select, color);
		my_mlx_putpixel(t->rt->scene->data1[5], col, IMG_HEIGHT - row, color);
	}
	else if (select == 6)
	{
		r = rt_get_ray(&t->rt->scene->cam, (double)((col + ((2 + 0.5)/ anti_a)) / IMG_WIDTH), (double)((row + ((2 + 0.5) / anti_a)) / IMG_HEIGHT));
		color = rt_raytracer(t, &r, 50);
		colo = infinite_pixels(t, (t_vec){col, row , 0}, select, color);
		my_mlx_putpixel(t->rt->scene->data1[6], col, IMG_HEIGHT - row, color);
	}
	else if (select == 7)
	{
		r = rt_get_ray(&t->rt->scene->cam, (double)((col + ((1 + 0.5)/ anti_a)) / IMG_WIDTH), (double)((row + ((2 + 0.5) / anti_a)) / IMG_HEIGHT));
		color = rt_raytracer(t, &r, 50);
		colo = infinite_pixels(t, (t_vec){col, row , 0}, select, color);
		my_mlx_putpixel(t->rt->scene->data1[7], col, IMG_HEIGHT - row, color);
	}
		else if (select == 8)
	{
		r = rt_get_ray(&t->rt->scene->cam, (double)((col + ((2 + 0.5)/ anti_a)) / IMG_WIDTH), (double)((row + ((1 + 0.5) / anti_a)) / IMG_HEIGHT));
		color = rt_raytracer(t, &r, 50);
		colo = infinite_pixels(t, (t_vec){col, row , 0}, select, color);
		my_mlx_putpixel(t->rt->scene->data1[8], col, IMG_HEIGHT - row, color);
	}
	return(colo);
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
	// 		r = rt_get_ray(&t->rt->scene->cam, i + ((ss[0] + 0.5) / anti_a), j + ((ss[1] + 0.5) / anti_a));
	// 		col = vec_add(col, rt_raytracer(th, &r));, 50
	// 	}
	// }
	// col = ft_div_k(col, anti_a * anti_a);
	// ft_clamp(&col);
	 return (col);
}