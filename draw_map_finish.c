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

typedef struct s_img
{
	int bpp;
	int size_l;
	int endian;

	void *ptr;
	int *data;
}	t_img;

//세로축 선 그리는 함수
void draw_col_line(void *mlx, void *win, t_img *img, int x)
{
	int i;
	x *= TILE_SIZE;
	i = -1;
	while (++i < w_h)
		mlx_pixel_put(mlx, win, x, w_h, 0xffffff);
		//img->data[(i * w_w) + x] = 0xffffff;
}

// 가로축 선 그리는 함수
void draw_row_line(t_img *img, int y)
{
	int i;
	y *= TILE_SIZE;
	i = -1;
	while (++i < w_w)
	{
		img->data[(y * w_w) + i] = 0xffffff;
	}
}

void draw_rec(t_img *img, int x, int y)
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
			img->data[(y + i) * w_w + x + j] = 0x18af1b;
		}
	}
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

	t_img img;
	void *mlx;
	void *win;
	mlx = mlx_init();
	// 창크기
	win = mlx_new_window(mlx, w_w, w_h, "test");
	//이미지 크기
	img.ptr = mlx_new_image(mlx, w_w, w_h);
	img.data = (int *)mlx_get_data_addr(img.ptr, &img.bpp, &img.size_l, &img.endian);

	int i = -1;
	while(++i < ROWS)
	{
		int j = -1;
		while(++j < COLS)
		{
			if(map[i][j] == 1)
			{
				draw_rec(&img, j, i);
				draw_col_line(mlx, win, &img, j);
			}
			draw_row_line(&img, i);
		}
	}
	mlx_put_image_to_window(mlx, win, img.ptr, 0, 0);
	mlx_loop(mlx);
}




