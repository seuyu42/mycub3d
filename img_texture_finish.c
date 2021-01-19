#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "mlx/mlx.h"

# define W_W 640
# define W_H 480
# define MAP_W 24
# define MAP_H 24
# define T_W 64
# define T_H 64
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
	int img_W;
	int img_H;

	void *ptr;
	int *data;
}	t_img;

typedef struct s_game
{
	// int		**buf;
	int		buf[W_H][W_W];
	int		**texture;
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


// buff에 저장해둔 이미지데이터를 넣어준다.
// 이렇게하면 랜더링이 되는 중간에 이미지데이터가 바뀌더라도 괜찮다.
// 울펜슈타인에서는 더블 버퍼링도 사용했음.
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
// int	map[MAP_H][MAP_W] = {
// 							{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
// 							{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 							{1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 							{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 							{1,0,0,0,0,0,1,1,1,1,2,0,0,0,0,1,0,1,0,1},
// 							{1,0,0,0,0,0,1,0,0,0,2,0,0,0,0,0,0,0,0,1},
// 							{1,0,0,0,0,0,1,0,0,0,2,0,0,0,0,1,0,0,0,1},
// 							{1,0,0,0,0,0,1,0,0,0,2,0,0,0,0,0,0,0,0,1},
// 							{1,0,0,0,0,0,1,1,1,1,2,0,0,0,0,1,0,1,0,1},
// 							{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 							{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 							{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 							{1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1},
// 							{1,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
// 							{1,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1},
// 							{1,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
// 							{1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1},
// 							{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 							{1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1},
// 							{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
// 						};

int	map[MAP_H][MAP_W] = 	{
							{4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
							{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
							{4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
							{4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
							{4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
							{4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
							{4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
							{4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
							{4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
							{4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
							{4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
							{4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
							{6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
							{8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
							{6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
							{4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
							{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
							{4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
							{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
							{4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
							{4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
							{4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
							{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
							{4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
						};



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
		double raydist;


		int	side;
		int stepX;
		int stepY;


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
			if (map[mapX][mapY] != 0)
				break;
		}

		if (side == 1)
		{
			// double yDist = mapY - game->posY + (1 - stepY)/2;
			// raydist = yDist / raydirY;
			raydist = (mapY - game->posY + (1 - stepY) / 2) / raydirY;
		}
		else
		{
			// double xDist = mapX - game->posX + (1 - stepX)/2;
			// raydist = xDist / raydirX;
			raydist = (mapX - game->posX + (1 - stepX) / 2) / raydirX;
		}

		int lineHeight = (int)(W_H / raydist);


		int drawStart = -lineHeight / 2 + W_H / 2;
		if(drawStart < 0)
			drawStart = 0;
		int drawEnd = lineHeight / 2 + W_H / 2;
		if(drawEnd >= W_H)
			drawEnd = W_H - 1;

		int t_num = map[mapX][mapY] - 1;

		//벽의 어디에 부딪혔는지 파악하기 위해 구함.
		double wallX;
		// y값에 부딪친 경우 (side = 1) 수직인 posX와 raydirX를 활용한다.
		// x값에 부딪친 경우는 수직인 y값을 활용.
		if (side == 1)
			wallX = game->posX + raydist * raydirX;
		else
			wallX = game->posY + raydist * raydirY;
		wallX -= floor(wallX);

		//텍스쳐 상의 x좌표  0 ~ 32
		//벽이 훨씬크니까 거기에 텍스쳐가 여러개 들어감 그 텍스쳐 중의 x좌표
		int texX = (int)(wallX * (double)T_W);
		//x값에 부딪치고 ray의 기울기가 양수이거나
		//y값에 부딪치고 ray가 아래로 향하면 x좌표를 반대로 계산해주어야한다.
		if (side == 0 && raydirX > 0)
			texX = T_W - texX - 1;
		if (side == 1 && raydirY < 0)
			texX = T_W - texX - 1;
		//texstep은 실제 벽의 픽셀에 텍스쳐 좌표를 얼마나 증가시킬지. (얼마나 늘릴지)

		double texstep = 1.0 * T_H / lineHeight;


		// drawStart가 0보다 큰 경우에는 texpos는 0이된다. (관호안이 drawstart의 반대값이기 때문)
		// 하지만 drawStart가 0보다 작은 경우 0으로 고정되기 때문에 texpos는 0이 아니게된다.
		// 이것을 쓰는 용도를 생각해보니 만약 엄청 벽에 가까워서 카메라평면에 벽이 잘리는 부분이 생기는 경우,
		// 픽셀상에서는 0부터 그려주어야 하지만 텍스쳐는 화면 바깥으로 잘리는 부분을 생각해주어서 잘린부분 다음부터 그려주어야한다.
		double texPos = (drawStart - W_H / 2 + lineHeight / 2) * texstep;
		int y = drawStart - 1;
		while (++y < drawEnd)
		{
			// 비트마스크를 사용해서 2진수의 자릿수 -1 에 &연산을 해주면 그 값 이상의 값이 나올 수 없게만든다.
			// 그렇다면 텍스쳐의 높이가 2진수의 자릿수가 아니면 ??
			int texY = (int)texPos & (T_H - 1);
			texPos += texstep;
			// 2차원배열을 활용해서 행에는 텍스쳐넘버로 어떤 텍스쳐를 쓸 지를 결정하는 데이터를 담고
			// 열에는 x,y 좌표를 2차원배열을 1차원배열처럼 사용하는 방법으로 표현한다.

			int color = game->texture[t_num][texY * T_H + texX];
			// 비트연산, 비트마스크으로 y축 면의 색을 더 어둡게 만든다.
			if (side == 1)
				color = (color >> 1) & 8355711;
			game->buf[y][x] = color;
		}
	}
}

int	draw_loop(t_game *game)
{
	draw_bground(game);
	cal_dist(game);
	draw(game);
	return (0);
}

//key적용
//plane도 적용해줘야댐
//이부분에서 틀린이유 가이드랑 다르게 나는 xy값을 다르게 받아서 처리해서 그부분에서 헷갈렸다.
int	key_press(int key, t_game *game)
{
	// 좌표가 뺄수록 위로감.
	if (key ==  KEY_UP)
	{
		// if(!map[(int)game->posY][(int)(game->posX + game->dirX * game->moveSpeed)])
		// 	game->posX += game->dirX * game->moveSpeed;
		// if(!map[(int)(game->posY + game->dirY * game->moveSpeed)][(int)(game->posX)])
		// 	game->posY += game->dirY * game->moveSpeed;
		if(!map[(int)(game->posX + game->dirX * game->moveSpeed)][(int)game->posY])
			game->posX += game->dirX * game->moveSpeed;
		if(!map[(int)(game->posX)][(int)(game->posY + game->dirY * game->moveSpeed)])
			game->posY += game->dirY * game->moveSpeed;
	}
	else if(key == KEY_DOWN)  //좌표더할수록 아래로감
	{
		if(!map[(int)(game->posX - game->dirX * game->moveSpeed)][(int)game->posY])
			game->posX -= game->dirX * game->moveSpeed;
		if(!map[(int)(game->posX)][(int)(game->posY - game->dirY * game->moveSpeed)])
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



/* 1. mlx_xpm_file_to함수로 xpm파일을 이미지 포인터로 가져온다. */
/* 2. mlx_get_data_addr로 이미지에 대한 정보를 가져온다. */
/* 3. 이미지에 대한 정보를 텍스쳐에다가 넣어준다. */

void	load_image(t_game *game, int *texture, char *path, t_img *img)
{
	img->ptr = mlx_xpm_file_to_image(game->mlx, path, &img->img_W, &img->img_H);

	img->data = (int *)mlx_get_data_addr(img->ptr, &img->bpp, &img->size_l, &img->endian);

	int y = -1;
	while (++y < img->img_H)
	{
		int x = -1;
		while(++x < img->img_W)
			texture[img->img_W * y + x] = img->data[img->img_W * y + x];
	}
	mlx_destroy_image(game->mlx, img->ptr);

}

void	load_tex(t_game *game)
{
	t_img img;
	load_image(game, game->texture[0], "textures/eagle.xpm", &img);
	load_image(game, game->texture[1], "textures/redbrick.xpm", &img);
	load_image(game, game->texture[2], "textures/purplestone.xpm", &img);
	load_image(game, game->texture[3], "textures/greystone.xpm", &img);
	load_image(game, game->texture[4], "textures/bluestone.xpm", &img);
	load_image(game, game->texture[5], "textures/mossy.xpm", &img);
	load_image(game, game->texture[6], "textures/wood.xpm", &img);
	load_image(game, game->texture[7], "textures/colorstone.xpm", &img);

}

int main()
{

	t_game game;
	game.mlx = mlx_init();


	game.rotationSpeed = 0.05;
	game.moveSpeed = 0.5;

	game.posX = 12;
	game.posY = 5;
	game.dirX = -1.0;
	game.dirY = 0.0;
	game.planeX = 0.0;
	game.planeY = 0.66;

	//buff할당
	// game.buf = (int **)malloc(sizeof(int *) * W_H);
	// int i = -1;
	// while (++i < W_H)
	// 	game.buf[i] = (int *)malloc(sizeof(int) * W_W);


	//buf 초기화
	int i = -1;
	while (++i < W_H)
	{
		int j = -1;
		while (++j < W_W)
			game.buf[i][j] = 0;
	}


	// 텍스쳐 할당
	if(!(game.texture = (int **)malloc(sizeof(int*) * 8)))
		return(-1);
	i = -1;
	while (++i < 8)
	{
		if(!(game.texture[i] = (int *)malloc(sizeof(int) * T_H * T_W)))
			return (-1);
	}

	//텍스쳐 buf초기화
	i = -1;
	while(++i < 8)
	{
		int j = -1;
		while (++j < T_H * T_W)
			game.texture[i][j] = 0;
	}

	load_tex(&game);

	game.win = mlx_new_window(game.mlx, W_W, W_H, "test");
	game.img.ptr = mlx_new_image(game.mlx, W_W, W_H);
	game.img.data = (int *)mlx_get_data_addr(game.img.ptr, &game.img.bpp, &game.img.size_l, &game.img.endian);

	mlx_loop_hook(game.mlx, &draw_loop, &game);
	mlx_hook(game.win, KEY_EVENT_PRESS, 0, &key_press, &game);
	mlx_loop(game.mlx);
}




