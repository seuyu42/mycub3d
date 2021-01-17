#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "mlx/mlx.h"

# define w_w COLS * TILE_SIZE
# define w_h ROWS * TILE_SIZE
# define ROWS 11
# define COLS 15
# define TILE_SIZE 32

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
	int		map[ROWS][COLS];
	t_img	img;
	double posX;
	double posY;
	double dirX;
	double dirY;
	int radius;
	int turnDirection;
	int walkDirection;
	double rotationAngle;
	double moveSpeed;
	double rotationSpeed;

}			t_game;

//buff에 저장해둔 이미지데이터를 넣어줌.
void	draw(t_game *game)
{
	int y = 0;
	while (y < w_h)
	{
		int x = 0;
		while(x < w_w)
		{
			game->img.data[y * w_w + x] = game->buf[y][x];
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(game->mlx, game->win, game->img.ptr, 0, 0);
}

//key적용
int	key_press(int key, t_game *game)
{
	// 좌표가 뺄수록 위로감.
	if (key ==  KEY_UP)
	{
		if(!game->map[(int)game->posY / TILE_SIZE][(int)(game->posX + game->dirX * game->moveSpeed) / TILE_SIZE])
			game->posX += game->dirX * game->moveSpeed;
		if(!game->map[(int)(game->posY + game->dirY * game->moveSpeed)/TILE_SIZE][(int)(game->posX)/TILE_SIZE])
			game->posY += game->dirY * game->moveSpeed;
	}
	else if(key == KEY_DOWN)  //좌표더할수록 아래로감
	{
		if(!game->map[(int)game->posY / TILE_SIZE][(int)(game->posX - game->dirX * game->moveSpeed) / TILE_SIZE])
			game->posX -= game->dirX * game->moveSpeed;
		if(!game->map[(int)(game->posY - game->dirY * game->moveSpeed)/TILE_SIZE][(int)(game->posX)/TILE_SIZE])
			game->posY -= game->dirY * game->moveSpeed;
	}
	else if(key == KEY_LEFT)
	{
		double oldx = game->dirX;
		game->dirX = game->dirX * cos(-game->rotationSpeed) - game->dirY * sin(-game->rotationSpeed);
		game->dirY = oldx * sin(-game->rotationSpeed) + game->dirY * cos(-game->rotationSpeed);

	}
	else if(key == KEY_RIGHT)
	{
		double oldx = game->dirX;
		game->dirX = game->dirX * cos(game->rotationSpeed) - game->dirY * sin(game->rotationSpeed);
		game->dirY = oldx * sin(game->rotationSpeed) + game->dirY * cos(game->rotationSpeed);
	}
	if (key == KEY_ESC)
		exit(0);

	return(0);
}

int	exit_button(void)
{
	exit(0);
}


//세로축 선 그리는 함수
void draw_col_line(t_game *game, int x)
{
	int i;
	x *= TILE_SIZE;
	i = -1;
	while (++i < w_h)
		game->buf[i][x] = 0xffffff;
		// game->img.data[(i * w_w) + x] = 0xffffff;
}

// 가로축 선 그리는 함수
void draw_row_line(t_game *game, int y)
{
	int i;
	y *= TILE_SIZE;
	i = -1;
	while (++i < w_w)
	{
		game->buf[y][i] = 0xffffff;
		// game->img.data[(y * w_w) + i] = 0xffffff;
	}
}

//DDA 알고리즘 적용해서 광선 쏘기
//인자로 들어온 angle만큼 회전행렬을 적용해서 회전시킨다.
void	draw_ray(t_game *game, double angle)
{
	double	deltaX;
	double	deltaY;
	double	step;
	double x1 = game->posX;
	double y1 = game->posY;

	deltaX = cos(angle) * game->dirX - sin(angle) * game->dirY;
	deltaY = sin(angle) * game->dirX + cos(angle) * game->dirY;


	step = (fabs(deltaX) > fabs(deltaY)) ? fabs(deltaX) : fabs(deltaY);
	deltaX /= step;
	deltaY /= step;

	while (1)
	{
		if (game->map[(int)floor(y1) / TILE_SIZE][(int)floor(x1) / TILE_SIZE] == 0)
			game->buf[(int)floor(y1)][(int)floor(x1)] = 0xb3b3b3;
		else
			break;
		x1 += deltaX;
		y1 += deltaY;
	}
}

void	draw_rays(t_game *game)
{
	double angle;

	angle = 0;
	// 각도를 가운데에서 약 +- 30도씩을 광선의 범위로 한다.
	while (angle < PI / 6)
	{
		draw_ray(game, angle);
		draw_ray(game, -angle);
		angle += PI/72;
	}
}

void draw_rec(t_game *game, int x, int y, int color)
{
	int i;
	int j;
	//x와 y는 배열상의 좌표라고 생각하면 편하다.
	x *= TILE_SIZE;
	y *= TILE_SIZE;

	i = -1;
	while (++i < TILE_SIZE)
	{
		j = -1;
		while(++j < TILE_SIZE)
		{
			game->buf[y + i][x + j] = color;
		}
	}
}

void	draw_map(t_game *game)
{
	int i = -1;
	while(++i < ROWS)
	{
		int j = -1;
		while(++j < COLS)
		{
			if(game->map[i][j] == 1)
			{
				draw_rec(game, j, i, 0x18af1b);
				draw_col_line(game, j);
			}
			else if(game->map[i][j] == 0)
			{
				draw_rec(game, j, i, 0x808080);
			}
			draw_row_line(game, i);
		}
	}
}

void	draw_user(t_game *game)
{
	int x = game->posX;
	int y = game->posY;

	int angle = -1;
	while(++angle <= 360)
	{
		int i = (cos(RAD * angle) * game->radius);
		int j = (sin(RAD * angle) * game->radius);
		game->buf[j + y][i + x] = 0xff3399;
		// game->img.data[((j + y) * w_w) + i + x] = 0xff3399;
	}
}

int	draw_loop(t_game *game)
{
	draw_map(game);
	draw_user(game);
	draw_rays(game);
	draw(game);
	// mlx_put_image_to_window(game->mlx, game->win, game->img.ptr, 0, 0);
	return (0);
}

int main()
{
	int map[ROWS][COLS] = {
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
		{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1},
		{1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	};

	t_game game;
	game.mlx = mlx_init();
	// 창크기
	game.win = mlx_new_window(game.mlx, w_w, w_h, "test");
	//이미지 크기


	memcpy(game.map, map, sizeof(int) * ROWS * COLS);
	game.img.ptr = mlx_new_image(game.mlx, w_w, w_h);
	game.img.data = (int *)mlx_get_data_addr(game.img.ptr, &game.img.bpp, &game.img.size_l, &game.img.endian);
	game.posX = w_w/2;
	game.posY = w_h/2;
	game.dirX = 0;
	game.dirY = 1;
	game.radius = 3;
	game.walkDirection = 0;
	game.turnDirection = 0;
	game.rotationAngle = PI / 2;
	game.rotationSpeed = 0.05;
	game.moveSpeed = 2.0;
	//이미지 데이터 저장할 버프 생성 (끊킴방지)
	game.buf = (int **)malloc(sizeof(int *) * w_h);
	int i = 0;
	while (i < w_h)
	{
		game.buf[i] = (int *)malloc(sizeof(int) * w_w);
		i++;
	}
	// 초기화
	i = 0;
	while (i < w_h)
	{
		int j = 0;
		while (j < w_w)
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




