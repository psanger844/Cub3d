/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psanger <psanger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 19:24:38 by psanger           #+#    #+#             */
/*   Updated: 2024/06/12 18:03:14 by psanger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
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
}				t_mlx;


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
	while (dda->angle < 360)
		dda->angle = dda->angle + 360;
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

void cast(t_dda *dda, t_mlx *mlx, int pixel) {

	// getting rid of fish eye:
	float real_len = dda->len * cos(to_rad(dda->angle));

	float height = (400 / real_len) / 2;

	printf("%f %f %f\n", height, dda->len, dda->angle);
	int mid = mlx->height / 2;
	// bottom
	for (int i = mid; i < mid + height; i++)
		mlx_put_pixel(mlx->img, pixel, i, ft_pixel(123, 10, 255, 255));
	// top
	for (int i = mid - height; i < mid; i++)
		mlx_put_pixel(mlx->img, pixel, i, ft_pixel(123, 10, 255, 255));
}

void init_mlx(t_mlx *mlx) {
	mlx->width = 1024;
	mlx->height = 512;

	mlx->mlx = mlx_init(mlx->width, mlx->height, "Test", false);
	mlx->img = mlx_new_image(mlx->mlx, mlx->width, mlx->height);
	mlx_image_to_window(mlx->mlx, mlx->img, 0, 0);
}

void	loop_hook()
{
	float angle = dda.angle;
	dda.angle -= 60;
	double increment = (120.0f / 1024.0f);
	int pixel = 0;

	while (dda.angle < angle + 60)
	{
		pixel++;
		dda.angle += increment;
		// printf("%f\n", dda.angle);
		dda_algo(&dda, map);
		cast(&dda, &mlx, pixel);
	}
}

int main(int argc, char **argv)
{
	float	start[3] = {2.5, 2.5, 0};

	char	*map[] = {
				"11111",
				"10001",
				"10001",
				"10001",
				"11111",
				NULL
	};

	t_dda dda;
	t_mlx mlx;
	init_mlx(&mlx);
	init_dda(&dda, argv, 2, 2.5);

	// mlx_loop_hook(&mlx, loop_hook, &mlx);
	mlx_loop(mlx.mlx);
	mlx_terminate(mlx.mlx);
	return (0);
}
