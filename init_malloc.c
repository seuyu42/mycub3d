#include "cub3d.h"


void info_init(t_info *info)
{
	info->rotSpeed = 0.05;
	info->moveSpeed = 0.05;

	info->dirX = info->pars->dirX;
	info->dirY = info->pars->dirY;
	info->planeX = info->pars->planeX;
	info->planeY = info->pars->planeY;
	info->key_up = 0;
	info->key_down = 0;
	info->key_left = 0;
	info->key_right = 0;
	info->key_esc = 0;
}

void buff_init(t_info *info)
{
	if(!(info->buf = (int **)malloc(sizeof(int *) * info->pars->win_height + 1)))
		return ;
	int i = -1;
	while (++i < info->pars->win_height)
	{
		if(!(info->buf[i] = (int *)malloc(sizeof(int) * info->pars->win_wid + 1)))
			return ;
	}
	i = -1;
	while (++i < info->pars->win_height)
	{
		int j = -1;
		while (++j < info->pars->win_wid)
			info->buf[i][j] = 0;
	}
}

void tex_init(t_info *info)
{
	if(!(info->texture = (int **)malloc(sizeof(int*) * 5)))
		return;
	int i = -1;
	while (++i < 5)
	{
		if(!(info->texture[i] = (int *)malloc(sizeof(int) * T_H * T_W)))
			return ;
	}

	//텍스쳐 buf초기화
	i = -1;
	while(++i < 5)
	{
		int j = -1;
		while (++j < T_H * T_W)
			info->texture[i][j] = 0;
	}
}

// void zbuffer_mal(t_info *info)
// {
// 	info->z_buffer = (double *)malloc(sizeof(double) * info->pars->win_wid);
// 	int i = 0;
// 	while (i < info->pars->win_wid)
// 	{
// 		info->z_buffer[i] = 0;
// 		i++;
// 	}
// }
