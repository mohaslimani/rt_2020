/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auto_anti.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoulaim <msoulaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/21 18:28:13 by msoulaim          #+#    #+#             */
/*   Updated: 2021/02/23 18:17:09 by msoulaim         ###   ########.fr       */
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

void		init_tab(t_vec *tab)
{
	tab[0] = vec(0, 0, 0);
	tab[1] = vec(0, 1, 0);
	tab[2] = vec(1, 0, 0);
	tab[3] = vec(1, 1, 0);
	tab[4] = vec(0, 2, 0);
	tab[5] = vec(2, 0, 0);
	tab[6] = vec(2, 2, 0);
	tab[7] = vec(1, 2, 0);
	tab[8] = vec(2, 1, 0);
}

t_vec		anti_aa(t_thread *t, double col, double row, int select)
{
	t_vec		tab[9];
	init_tab(tab);
	t_vec color;
	t_vec	colo = (t_vec){0, 0, 0};
	t_ray r;
	double anti_a = sqrt(t->rt->scene->max_anti_a);
	r = rt_get_ray(&t->rt->scene->cam, (double)((col + ((tab[select].x + 0.5)/ anti_a)) / IMG_WIDTH), (double)((row + ((tab[select].y + 0.5) / anti_a)) / IMG_HEIGHT));
	color = rt_raytracer(t, &r, 50);
	colo = infinite_pixels(t, (t_vec){col, row , 0}, select, color);
	my_mlx_putpixel(t->rt->scene->data1[select], col, IMG_HEIGHT - row, color);
	return(colo);
}