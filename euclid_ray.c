#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "mlx/mlx.h"

# define W_W 640
# define W_H 480
# define MAP_W 20
# define MAP_H 20

# define KEY_ESC 53
# define KEY_EVENT_PRESS 2
# define KEY_EVENT_EXIT 17
# define KEY_UP 126
# define KEY_DOWN 125
# define KEY_LEFT 123
# define KEY_RIGHT 124

# define PI 3.141592
# define RAD PI/180

typedef struct s_img
{
	int bpp;
	int size_l;
	int endian;

	void *ptr;
	int *data;
}	t_img;

typedef struct s_game
{
	int		**buf;
	void	*mlx;
	void	*win;
	t_img	img;
	double posX;
	double posY;
	double dirX;
	double dirY;
	double planeX;
	double planeY;
	double moveSpeed;
	double rotationSpeed;

}			t_game;


// buff에 저장해둔 이미지데이터를 넣어줌.
void	draw(t_game *game)
{
	int y = 0;
	while (y < W_H)
	{
		int x = 0;
		while(x < W_W)
		{
			game->img.data[y * W_W + x] = game->buf[y][x];
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(game->mlx, game->win, game->img.ptr, 0, 0);
}

//맵적용
int	map[MAP_H][MAP_W] = {
							{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
							{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1,0,1,0,1},
							{1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,1},
							{1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,1,0,1,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							{1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1},
							{1,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
							{1,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1},
							{1,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
							{1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1},
							{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							{1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1},
							{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
						};



//key적용
//plane도 적용해줘야댐
int	key_press(int key, t_game *game)
{
	// 좌표가 뺄수록 위로감.
	if (key ==  KEY_UP)
	{
		if(!map[(int)game->posY][(int)(game->posX + game->dirX * game->moveSpeed)])
			game->posX += game->dirX * game->moveSpeed;
		if(!map[(int)(game->posY + game->dirY * game->moveSpeed)][(int)(game->posX)])
			game->posY += game->dirY * game->moveSpeed;
	}
	else if(key == KEY_DOWN)  //좌표더할수록 아래로감
	{
		if(!map[(int)game->posY][(int)(game->posX - game->dirX * game->moveSpeed)])
			game->posX -= game->dirX * game->moveSpeed;
		if(!map[(int)(game->posY - game->dirY * game->moveSpeed)][(int)(game->posX)])
			game->posY -= game->dirY * game->moveSpeed;
	}
	else if(key == KEY_RIGHT)
	{
		double oldx = game->dirX;
		game->dirX = game->dirX * cos(-game->rotationSpeed) - game->dirY * sin(-game->rotationSpeed);
		game->dirY = oldx * sin(-game->rotationSpeed) + game->dirY * cos(-game->rotationSpeed);
		double oldplanex = game->planeX;
		game->planeX = game->planeX * cos(-game->rotationSpeed) - game->planeY * sin(-game->rotationSpeed);
		game->planeY = oldplanex * sin(-game->rotationSpeed) + game->planeY * cos(-game->rotationSpeed);

	}
	else if(key == KEY_LEFT)
	{
		double oldx = game->dirX;
		game->dirX = game->dirX * cos(game->rotationSpeed) - game->dirY * sin(game->rotationSpeed);
		game->dirY = oldx * sin(game->rotationSpeed) + game->dirY * cos(game->rotationSpeed);
		double oldplanex = game->planeX;
		game->planeX = game->planeX * cos(game->rotationSpeed) - game->planeY * sin(game->rotationSpeed);
		game->planeY = oldplanex * sin(game->rotationSpeed) + game->planeY * cos(game->rotationSpeed);
	}
	if (key == KEY_ESC)
		exit(0);

	return(0);
}

int	exit_button(void)
{
	exit(0);
}

void draw_bground(t_game *game)
{
	int vert = W_H / 2;
	int i = -1;
	while(++i < W_H)
	{
		int j = -1;
		while(++j < W_W)
		{
			if(i < vert)
				game->buf[i][j] = 0x00a3d2;
			else
				game->buf[i][j] = 0x9c6136;
		}
	}
}

void ver_line(t_game *game, int x, int start, int end, int color)
{
	int i = start - 1;
	while (++i < end)
	{
		// mlx_pixel_put(game->mlx, game->win, x, i, color);
		game->buf[i][x] = color;
	}
}

void	cal_dist(t_game *game)
{
	int x = -1;
	while (++x < W_W)
	{
		double cameraX = 2 * x / (double)W_W - 1;
		double raydirX = game->dirX + game->planeX * cameraX;
		double raydirY = game->dirY + game->planeY * cameraX;

		int mapX = (int)game->posX;
		int mapY = (int)game->posY;
		double deltadistX = fabs(1/ raydirX);
		double deltadistY = fabs(1/ raydirY);
		double firstdistX;
		double firstdistY;
		int	side;
		int stepX;
		int stepY;
		double raydist;
		double euclidist;


		if (raydirX < 0)
		{
			stepX = -1;
			firstdistX = (game->posX - mapX) * deltadistX;
		}
		else
		{
			stepX = 1;
			firstdistX = (mapX + 1.0 - game->posX) * deltadistX;
		}
		if (raydirY < 0)
		{
			stepY = -1;
			firstdistY = (game->posY - mapY) * deltadistY;
		}
		else
		{
			stepY = 1;
			firstdistY = (mapY + 1.0 - game->posY) * deltadistY;
		}


		while (1)
		{
			if (firstdistX < firstdistY)
			{
				side = 0;   //side가 1인건 x축값에 부딪혔다는 소리
				mapX += stepX;
				firstdistX += deltadistX;
			}
			else
			{
				side = 1;
				mapY += stepY;
				firstdistY += deltadistY;
			}
			if (map[mapY][mapX] != 0)
				break;
		}

		if (side == 1)
		{
			double yDist = mapY - game->posY + (1 - stepY)/2;
			euclidist = sqrt(raydirX * raydirX + raydirY * raydirY) * yDist/ raydirY;
			// raydist = (mapY - game->posY + (1 - stepY) / 2) / raydirY;
		}
		else
		{
			double xDist = mapX - game->posX + (1 - stepX)/2;
			euclidist = sqrt(raydirX * raydirX + raydirY * raydirY) * xDist/ raydirX;
			// raydist = (mapX - game->posX + (1 - stepX) / 2) / raydirX;
		}

		// int lineHeight = (int)(W_H / raydist);
		int lineHeight = (int)(W_H / euclidist);


		int drawStart = -lineHeight / 2 + W_H / 2;
		if(drawStart < 0)
			drawStart = 0;
		int drawEnd = lineHeight / 2 + W_H / 2;
		if(drawEnd >= W_H)
			drawEnd = W_H - 1;

		int	color;
		if (map[mapY][mapX] == 1)
			color = 0x666666;

		if (side == 1)
			color = 0x5e7e9b;


		ver_line(game , x , drawStart,drawEnd,color);
		//구해야하는 것
		//deltadist, sidedist, perpwalldist;
	}

}

int	draw_loop(t_game *game)
{
	draw_bground(game);
	cal_dist(game);
	draw(game);
	return (0);
}

int main()
{

	t_game game;
	game.mlx = mlx_init();
	game.win = mlx_new_window(game.mlx, W_W, W_H, "test");
	game.img.ptr = mlx_new_image(game.mlx, W_W, W_H);
	game.img.data = (int *)mlx_get_data_addr(game.img.ptr, &game.img.bpp, &game.img.size_l, &game.img.endian);

	game.rotationSpeed = 0.05;
	game.moveSpeed = 0.5;

	game.posX = 12;
	game.posY = 5;
	game.dirX = -1;
	game.dirY = 0;
	game.planeX = 0;
	game.planeY = 0.66;




	game.buf = (int **)malloc(sizeof(int *) * W_H);
	int i = 0;
	while (i < W_H)
	{
		game.buf[i] = (int *)malloc(sizeof(int) * W_W);
		i++;
	}
	// 초기화
	i = 0;
	while (i < W_H)
	{
		int j = 0;
		while (j < W_W)
		{
			game.buf[i][j] = 0;
			j++;
		}
		i++;
	}

	mlx_loop_hook(game.mlx, &draw_loop, &game);
	mlx_hook(game.win, KEY_EVENT_PRESS, 0, &key_press, &game);
	mlx_loop(game.mlx);
}




