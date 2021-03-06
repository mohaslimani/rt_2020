# include <rt.h>

t_vec rt_raytracer(t_thread *th, t_ray *r, int depth)
{
	t_vec		color;
	t_object	*o;

	th->rec.col = vec(0.0, 0.0, 0.0);
	color = vec(0.0, 0.0, 0.0);
	if (rt_hit(th->rt->scene, r, &th->rec))
	{
		o = th->rec.curr_obj;	
		if (o->txt)
			th->rec.col = rt_get_color_from_texture(o, &th->rec.u, &th->rec.v);
		else if (o->noi.is_noise == 1)
			th->rec.col =  rt_noise(o, th->rec);
		else
			th->rec.col = th->rec.curr_obj->col;
		rt_lighting(th, th->rt->scene->light);
		color = th->rec.col;

		t_ray rf;
		if (depth > 1 && o->mat.kr > 0)
		{
			rf.dir = ft_reflect(r->dir, th->rec.n);
			rf.origin = vec_add(th->rec.p, vec_pro_k(rf.dir, 0.001));
			color = vec_add(color, vec_pro_k(rt_raytracer(th, &rf, depth -1), o->mat.kr));
		}
	}
	// color = th->rec.col;
	rt_adjustment(&color);
	return (color);
}

t_vec rt_anti_aliasing(t_thread *t, int col, int row)
{
	// t_ray	r;
	// t_vec	color;
	// int		ss[2];
	// int		anti_a;

	// color = vec(0, 0, 0);
	// anti_a = t->rt->scene->anti_aliasing;

	// 		r = rt_get_ray(&t->rt->scene->cam, (double)((col + ((0 + 0.5)/ anti_a)) / IMG_WIDTH), (double)((row + ((0 + 0.5) / anti_a)) / IMG_HEIGHT));
	// 		color = vec_add(color, rt_raytracer(t, &r, 50));
	// return (vec_div_k(color, anti_a * anti_a));
	t_vec		color;

	color = vec(0, 0, 0);
	color = anti_aa(t, (double)col, (double)row, t->rt->scene->select);
	color = vec_div_k(color, t->rt->scene->select + 1);
	return (color);
}

void		*rt_run(t_thread *t)
{
	t_vec   c;
	t_ray	r;
	int			color;
	int			col;
	int			row;

	row = IMG_HEIGHT + 1;
	while (--row >= 0)
	{
		col = (int)(t->i * IMG_WIDTH / NBTHREAD) - 1;
		while (++col < (int)((t->i + 1) * IMG_WIDTH / NBTHREAD))
		{
			c = rt_anti_aliasing(t, col, row);
			color = rt_rgb_to_int(c);
			rt_mlx_putpixel(t->rt, col, IMG_HEIGHT - row, color);
		}
	}
	pthread_exit(NULL);
}

void			*rt_run_25(t_thread *t)
{
	t_vec   c;
	t_ray   r;
	int			color;
	int			col;
	int			row;

	row = IMG_HEIGHT + 1;
	while (--row >= 0)
	{
		col = (int)(t->i * IMG_WIDTH / NBTHREAD) - 1;
		while (++col < (int)((t->i + 1) * IMG_WIDTH / NBTHREAD))
		{
			c = rt_anti_aliasing(t, col, row);			
			color = rt_rgb_to_int(c);
			rt_mlx_putpixel(t->rt, col, IMG_HEIGHT - row, color);
			rt_mlx_putpixel(t->rt, col + 1, IMG_HEIGHT - row, color);
			rt_mlx_putpixel(t->rt, col , IMG_HEIGHT - row - 1, color);
			rt_mlx_putpixel(t->rt, col  + 1, IMG_HEIGHT - row - 1, color);
			col++;
		}
		row--;
	}
	pthread_exit(NULL);
}

void			*rt_run_50(t_thread *t)
{
	t_vec   c;
	t_ray r;
	int			color;
	int			col;
	int			row;

	row = IMG_HEIGHT + 1;
	while (--row >= 0)
	{
		col = (int)(t->i * IMG_WIDTH / NBTHREAD) - 1;
		while (++col < (int)((t->i + 1) * IMG_WIDTH / NBTHREAD))
		{
			c = rt_anti_aliasing(t, col, row);	
			color = rt_rgb_to_int(c);
			rt_mlx_putpixel(t->rt, col, IMG_HEIGHT - row, color);
			rt_mlx_putpixel(t->rt, col + 1, IMG_HEIGHT - row, color);
			col++;
		}
	}
	pthread_exit(NULL);
}


void			*rt_run_12(t_thread *t)
{
	t_vec   c;
	t_ray r;
	int			color;
	int			col;
	int			row;

	row = IMG_HEIGHT + 1;
	while (--row >= 0)
	{
		col = (int)(t->i * IMG_WIDTH / NBTHREAD) - 1;
		while (++col < (int)((t->i + 1) * IMG_WIDTH / NBTHREAD))
		{
			c = rt_anti_aliasing(t, col, row);	
			color = rt_rgb_to_int(c);
			rt_mlx_putpixel(t->rt, col, IMG_HEIGHT - row, color);
			rt_mlx_putpixel(t->rt, col + 1, IMG_HEIGHT - row, color);
			rt_mlx_putpixel(t->rt, col + 2, IMG_HEIGHT - row, color);
 
			rt_mlx_putpixel(t->rt, col , IMG_HEIGHT - row - 1, color);
			rt_mlx_putpixel(t->rt, col  + 1, IMG_HEIGHT - row - 1, color);
			rt_mlx_putpixel(t->rt, col  + 2, IMG_HEIGHT - row - 1, color);

			rt_mlx_putpixel(t->rt, col , IMG_HEIGHT - row - 2, color);
			rt_mlx_putpixel(t->rt, col  + 1, IMG_HEIGHT - row - 2, color);
			rt_mlx_putpixel(t->rt, col  + 2, IMG_HEIGHT - row - 2, color);

			col += 2;;
		}
		row -= 2;
	}
	pthread_exit(NULL);
}

void		rt_start(t_rt *rt, void* (*rt_runner)(t_thread *t)) 
{
	pthread_t	thread[NBTHREAD];
	t_thread	div[NBTHREAD];
	int i;

	i = -1;
	while (++i < NBTHREAD)
	{
		div[i].rt = rt;
		div[i].i = i;
		pthread_create(&thread[i], NULL, (void*)rt_runner, &div[i]);
	}
	while (--i >= 0)
		pthread_join(thread[i], NULL);
}


void	progress_fill(t_rt *rt)
{
	int i;
	int j;

	(rt->scene->select == rt->scene->anti_aliasing) ? (rt->scene->progress = 12) : 0;
	i = -1;
	while (++i < ((double)IMG_WIDTH / (double)12) * (double)(rt->scene->progress))
	{
		j = -1;
		while (++j < 8)
			rt_mlx_putpixel(rt, i, IMG_HEIGHT + j, 0xFF0000);
	}
}

int		progress_bar(t_rt *rt)
{
	ft_bzero(rt->data, IMG_WIDTH * IMG_HEIGHT * 4);
	if (rt->scene->progress == 1)
		rt_start(rt, rt_run_12);
	//usleep(2000);
	//ft_putendl("pp");
	if (rt->scene->progress == 2)
		rt_start(rt, rt_run_25);
	else if (rt->scene->progress == 3)
		rt_start(rt, rt_run_50);
	else if (rt->scene->progress >= 4 && rt->scene->progress <= 12 && rt->scene->select <= rt->scene->anti_aliasing + 1)
	{
		rt_start(rt, rt_run);
		rt->scene->select++;
	}
	if (rt->scene->progress <= 12 && rt->scene->select <= rt->scene->anti_aliasing + 1)
	{
		progress_fill(rt);
		rt->scene->progress++;
		mlx_put_image_to_window(rt->mlx, rt->win, rt->img, 40, 180);
	}
	return 0;
}

void		rt_auto_draw(t_rt *rt)
{
	ft_bzero(rt->data, IMG_WIDTH * IMG_HEIGHT * 4);
	if (rt->scene->progress == 1)
		rt_start(rt, rt_run_25);
	else if (rt->scene->progress == 2)
		rt_start(rt, rt_run_50);
	else
		rt_start(rt, rt_run);
}

int		rt_draw(t_rt *rt)
{
	ft_bzero(rt->data, IMG_WIDTH * IMG_HEIGHT * 4);
	rt_start(rt, rt_run_12);
	mlx_put_image_to_window(rt->mlx, rt->win, rt->img, 40, 180);
	return (EXIT_SUCCESS);
}

// int			rt_draw(t_rt *rt)
// {
// 	ft_bzero(rt->data, IMG_WIDTH * IMG_HEIGHT * 4);
// 	rt_start(rt);
// 	mlx_put_image_to_window(rt->mlx, rt->win, rt->img, 40, 180);
// 	return (EXIT_SUCCESS);
// }
