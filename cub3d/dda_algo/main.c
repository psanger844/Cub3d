/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psanger <psanger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 19:24:38 by psanger           #+#    #+#             */
/*   Updated: 2024/06/25 23:01:50 by psanger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../MLX42/include/MLX42/MLX42.h"

# define BLUE "\033[0;34m"
# define RESET "\033[0m"
# define RED "\033[0;31m"
# define pixel_size 16
# define PI 3.141592


typedef struct s_dda
{
	char			**map;
	float			angle;

	int				height;
	int				color;

	float			start_x;
	float			start_y;

	float			delta_x;
	float			delta_y;

	float			end_x;
	float			end_y;

	float			len;

	int				step_direction_x;
	int				step_direction_y;

	char			wall_face;

}				t_dda;

typedef struct s_mlx
{
	mlx_t*			mlx;
	mlx_image_t*	img;

	int			width;
	int			height;

	t_dda*		dda;
}				t_mlx;


char *ft_strdup(char *src)
{
	int i = 0;
	char *res;

	while(src[i])
		i++;
	res = (char*)malloc(sizeof(*res) * i + 1);
	if (res == NULL)
		return (NULL);
	i = 0;
	while(src[i])
	{
		res[i]=src[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

float	to_rad(int angle)
{
	return (PI / 180 * angle);
}

void	get_dir(int angle, int *step_dir_x, int *step_dir_y)
{
	if (angle < 0)
		angle = angle * -1 + 180;
	if (angle % 360 >= 0 && angle % 360 <= 90)
	{
		*step_dir_x = 1;
		*step_dir_y = -1;
	}
	if (angle % 360 > 90 && angle % 360 <= 180)
	{
		*step_dir_x = -1;
		*step_dir_y = -1;
	}
	if (angle % 360 > 180 && angle % 360 <= 270)
	{
		*step_dir_x = -1;
		*step_dir_y = 1;
	}
	if (angle % 360 > 270 && angle % 360 <= 360)
	{
		*step_dir_x = 1;
		*step_dir_y = 1;
	}
}

int	is_wall_x(char **map, t_dda *dda)
{
	int		x_pos_int = (int)dda->end_x;
	int		y_pos_int = (int)dda->end_y;
	if (dda->step_direction_x == -1)
		x_pos_int += -1;
	if (map[y_pos_int][x_pos_int] == '1')
		return (1);
	return (0);
}

int	is_wall_y(char **map, t_dda *dda)
{
	int		x_pos_int = (int)dda->end_x;
	int		y_pos_int = (int)dda->end_y;
	if (dda->step_direction_y == -1)
		y_pos_int += -1;
	if (map[y_pos_int][x_pos_int] == '1')
		return (1);
	return (0);
}

float	abs_float(float num)
{
	if (num < 0)
		num = num * -1;
	return (num);
}

char	wall_face(int is_wall, t_dda *dda)
{
	if (is_wall == 1)
		is_wall *= dda->step_direction_x;
	if (is_wall == 2)
		is_wall *= dda->step_direction_y;
	if (is_wall == 1)
		return ('W');
	if (is_wall == -1)
		return ('O');
	if (is_wall == 2)
		return ('N');
	if (is_wall == -2)
		return ('S');
	return (0);
}

int	get_new_pos(t_dda *dda, char **map)
{
	float	hyplen_x = 0;
	float	hyplen_y = 0;

	if (dda->step_direction_x > 0)
		dda->delta_x = (int)dda->end_x + 1 - dda->end_x;
	else
		dda->delta_x = (int)dda->end_x - dda->end_x;
	if (dda->step_direction_y > 0)
		dda->delta_y = (int)dda->end_y + 1 - dda->end_y;
	else
		dda->delta_y = (int)dda->end_y - dda->end_y;

	if (dda->delta_x == 0)
		dda->delta_x = dda->step_direction_x;
	if (dda->delta_y == 0)
		dda->delta_y = dda->step_direction_y;

	// printf("Delta_x : |%f|\n", dda->delta_x);
	// printf("Delta_y : |%f|\n", dda->delta_y);

	hyplen_x = abs_float(dda->delta_x) / abs_float(cos(to_rad(dda->angle)));
	hyplen_y = abs_float(dda->delta_y) / abs_float(sin(to_rad(dda->angle)));

	// printf("test3\n");




	// printf("Hyp_x : |%f|\n", hyplen_x);
	// printf("Hyp_y : |%f|\n", hyplen_y);

	if (hyplen_x < hyplen_y)
	{
		// printf("%sSTEP in X%s\n", RED, RESET);
		dda->end_x = dda->end_x + dda->delta_x;
		dda->end_y = dda->end_y + (hyplen_x * abs_float(sin(to_rad(dda->angle))) * dda->step_direction_y);
		if (is_wall_x(map, dda) == 1)
			return (1);
	}
	else
	{
		// printf("%sSTEP in Y%s\n", RED, RESET);
		dda->end_x = dda->end_x + (hyplen_y * abs_float(cos(to_rad(dda->angle))) * dda->step_direction_x);
		dda->end_y = dda->end_y + dda->delta_y;
		if (is_wall_y(map, dda) == 1)
			return (2);
	}
	// printf("%sNew pos_x : |%f|%s\n", BLUE, dda->end_x, RESET);
	// printf("%sNew pos_y : |%f|%s\n", BLUE, dda->end_y, RESET);
	return (0);
}



void	dda_algo(t_dda *dda, char **map)
{
	int	is_wall = 0;

	get_dir(dda->angle, &dda->step_direction_x, &dda->step_direction_y);
	dda->end_x = dda->start_x;
	dda->end_y = dda->start_y;
	while (1)
	{
		is_wall = get_new_pos(dda, map);
		if (is_wall > 0)
			break;
	}
	dda->wall_face = wall_face(is_wall, dda);
	dda->delta_x = dda->end_x - dda->start_x;
	dda->delta_y = dda->end_y - dda->start_y;
	dda->len = pow(pow(dda->delta_x, 2) + pow(dda->delta_y, 2), 0.5);
	// printf("\n\n\n-----------------\n\n");
	// printf("END POS X :\t\t|%f|\n", dda->end_x);
	// printf("END POS Y :\t\t|%f|\n", dda->end_y);

	// printf("LEN :\t|%f|,angle |%f| \n", dda->len, dda->angle);

	// printf("WALL FACE :\t\t|%c|", dda->wall_face);
	// printf("\n---------\n");
}




void	init_dda(t_dda *dda, char **argv, float x_start, float y_start)
{
	if (!argv[1])
		dda->angle = 0;
	else
		dda->angle = atoi(argv[1]);
	dda->angle = 90;
	while (dda->angle < 360)
		dda->angle = dda->angle + 360;
	dda->height = 100;
	dda->map = malloc(sizeof(char *) * 15);

	dda->map[0] = ft_strdup ("1111111111111111111111111");
	dda->map[1] = ft_strdup ("1000000000110000000000001");
	dda->map[2] = ft_strdup ("1011000001110000000000001");
	dda->map[3] = ft_strdup ("1001000000000000000000001");
	dda->map[4] = ft_strdup ("111111111011000001110000000000001");
	dda->map[5] = ft_strdup ("100000000011000001110111111111111");
	dda->map[6] = ft_strdup ("11110111111111011100000010001");
	dda->map[7] = ft_strdup ("11110111111111011101010010001");
	dda->map[8] = ft_strdup ("11000000110101011100000010001");
	dda->map[9] = ft_strdup ("10000000000000001100000010001");
	dda->map[10] = ft_strdup ("10000000000000001101010010001");
	dda->map[11] = ft_strdup ("11000001110101011111011110N0111");
	dda->map[12] = ft_strdup ("11110111111101011101111010001");
	dda->map[13] = ft_strdup ("11111111111111111111111111111");
	dda->map[14] = NULL;

	// dda->map[0] = ft_strdup("11111");
	// dda->map[1] = ft_strdup("10001");
	// dda->map[2] = ft_strdup("10001");
	// dda->map[3] = ft_strdup("10001");
	// dda->map[4] = ft_strdup("11111");
	// dda->map[5] = NULL;


	dda->delta_x = 0;
	dda->delta_y = 0;
	dda->len = 0;
	dda->start_x = x_start;
	dda->start_y = y_start;
	dda->end_x = dda->start_x;
	dda->end_y = dda->start_y;
	get_dir(dda->angle, &dda->step_direction_x, &dda->step_direction_y);
}

int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

void	reset(t_mlx *mlx)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	while (y < mlx->height)
	{
		while (x < mlx->width)
		{
			mlx_put_pixel(mlx->img, x, y, ft_pixel(0, 0, 0, 255));
			x++;
		}
		x = 0;
		y++;
	}
}



void cast(t_dda *dda, t_mlx *mlx, int pixel)
{
	// getting rid of fish eye:
	float real_len = dda->len * abs_float(cos(to_rad(dda->angle)));
	// float real_len = dda->len;
	float height = (400 / real_len) / 2;

	if (height > mlx->height / 2)
		height = mlx->height / 2;

	printf("%f %f %f\n", height, dda->len, dda->angle);
	int mid = mlx->height / 2;
	// bottom
	for (int i = mid; i < mid + height; i++) {
		if (i < mlx->height)
			mlx_put_pixel(mlx->img, pixel, i, ft_pixel(123, 10, 255, 255));
	}
	// top
	for (int i = mid - height; i < mid; i++) {
		if (i > 0)
			mlx_put_pixel(mlx->img, pixel, i, ft_pixel(123, 10, 255, 255));
	}
}

void init_mlx(t_mlx *mlx) {
	mlx->width = 1024;
	mlx->height = 512;

	mlx->mlx = mlx_init(mlx->width, mlx->height, "Test", false);
	mlx->img = mlx_new_image(mlx->mlx, mlx->width, mlx->height);
	mlx_image_to_window(mlx->mlx, mlx->img, 0, 0);
}

void	loop_hook(void* param)
{
	t_mlx *mlx = (t_mlx *)param;
	t_dda *dda = mlx->dda;

	reset(mlx);

	float angle = dda->angle;
	dda->angle -= 60;
	double increment = (120.0f / 1024.0f);
	int pixel = 0;

	while (dda->angle < angle + 60)
	{
		pixel++;
		dda->angle += increment;
		// printf("%f\n", dda.angle);
		dda_algo(dda, dda->map);
		cast(dda, mlx, pixel);
	}
	dda->angle = angle;
	if (mlx_is_key_down(mlx->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(mlx->mlx);
	if (mlx_is_key_down(mlx->mlx, MLX_KEY_LEFT))
		dda->angle -= 2;
	if (mlx_is_key_down(mlx->mlx, MLX_KEY_RIGHT))
		dda->angle += 2;
	if (mlx_is_key_down(mlx->mlx, MLX_KEY_A))
		dda->start_x += 0.05;
	if (mlx_is_key_down(mlx->mlx, MLX_KEY_D))
		dda->start_x -= 0.05;
	if (mlx_is_key_down(mlx->mlx, MLX_KEY_S))
		dda->start_y += 0.05;
	if (mlx_is_key_down(mlx->mlx, MLX_KEY_W))
		dda->start_y -= 0.05;
	if (mlx_is_key_down(mlx->mlx, MLX_KEY_UP)) {
		dda->start_x += cos(to_rad(dda->angle)) * 0.075;
		dda->start_y -= sin(to_rad(dda->angle)) * 0.075;
	}

}

#include <stdbool.h>

float	to_deg(float angle)
{
	return (angle / PI * 180);
}

typedef struct s_pos
{
	float 		x;
	float 		y;
} 				t_pos;

bool isPointInTriangle(t_pos p, t_pos a, t_pos b, t_pos c)
{
	float denominator = ((b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y));
	float alpha = ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) / denominator;
	float beta = ((c.y - a.y) * (p.x - c.x) + (a.x - c.x) * (p.y - c.y)) / denominator;
	float gamma = 1.0 - alpha - beta;

	return (alpha > 0) && (beta > 0) && (gamma > 0);
}

void	get_pos_angle(float *x, float *y, int start_x, int start_y, float angle)
{
	float new_x = *x;
	float new_y = *y;
	float delta_x = new_x - (float)start_x;
	float delta_y = new_y - (float)start_y;
	float len = pow(pow(delta_x, 2) + pow(delta_y, 2), 0.5);
	float angle_norm = atan2(delta_x, delta_y);
	new_x = cos(angle + angle_norm) * len + (float)start_x;
	new_y = sin(angle + angle_norm) * len + (float)start_y;
	*x = (int)new_x;
	*y = (int)new_y;
}

void	p_draw_player(t_mlx *mlx, float alpha, int height, int color)
{
	int width = height / 2;
	int size = height;
	float angle = to_rad(alpha);

	if (height < width)
		size = width;
	size += 2;
	t_pos top;
	t_pos left;
	t_pos right;
	t_pos p;

	int	start_x = mlx->width / 2;
	int	start_y = mlx->height / 2;

	top.x = start_x;
	top.y = start_y - height / 2;
	get_pos_angle(&top.x, &top.y, start_x, start_y, angle);
	left.x = start_x - width / 2;
	left.y = start_y + height / 2;
	get_pos_angle(&left.x, &left.y, start_x, start_y, angle);
	right.x = start_x + width / 2;
	right.y = start_y + height / 2;
	get_pos_angle(&right.x, &right.y, start_x, start_y, angle);

	for (int i = start_y - size; i < start_y + size; i++)
	{
		for (int j = start_x - size; j < start_x + size; j++)
		{
			p.x = j;
			p.y = i;
			if (isPointInTriangle(p, top, left, right))
			{
				if (i < mlx->height && i > 0 && j < mlx->width && j > 0)
					mlx_put_pixel(mlx->img, j, i, color);
			}
		}
	}

}

void	loop_hook_player(void* param)
{
	t_mlx *mlx = (t_mlx *)param;
	t_dda *dda = mlx->dda;

	if (mlx_is_key_down(mlx->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(mlx->mlx);
	if (mlx_is_key_down(mlx->mlx, MLX_KEY_LEFT))
		dda->angle -= 2;
	if (mlx_is_key_down(mlx->mlx, MLX_KEY_RIGHT))
		dda->angle += 2;
	if (mlx_is_key_down(mlx->mlx, MLX_KEY_1))
		dda->height += 2;
	if (mlx_is_key_down(mlx->mlx, MLX_KEY_2))
		dda->height -= 2;
	if (mlx_is_key_down(mlx->mlx, MLX_KEY_UP) && dda->color < 0xFFFFFFFF - 512)
		dda->color += 512;
	if (mlx_is_key_down(mlx->mlx, MLX_KEY_DOWN)&& dda->color < 512)
		dda->color -= 512;
	reset(mlx);
	p_draw_player(mlx, dda->angle, dda->height, dda->color);
}

int main(int argc, char **argv)
{
	float	start[3] = {2.5, 2.5, 0};

	t_mlx *mlx;
	t_dda *dda;

	mlx = malloc(sizeof(t_mlx));
	dda = malloc(sizeof(t_dda));

	init_dda(dda, argv, 2.5, 2.5);
	init_mlx(mlx);
	mlx->dda = dda;
	dda->color = 0xFF0000FF;

	mlx_loop_hook(mlx->mlx, loop_hook_player, mlx);
	mlx_loop(mlx->mlx);
	mlx_terminate(mlx->mlx);
	return (0);
}
