
#include "cub3d.h"




void	key_update(t_info *info);


// static int compare(const void *first, const void *second)
// {
// 	if(*(int *)first > *(int *)second)
// 		return (1);
// 	else if(*(int *)first < *(int *)second)
// 		return (-1);
// 	else
// 		return (0);
// }

//스프라이트 정렬
void sort_order(t_pair *orders, int amount)
{
	t_pair tmp;

	int i = -1;
	while (++i < amount)
	{
		int j = -1;
		while (++j < amount - 1)
		{
			if (orders[j].first > orders[j + 1].first)
			{
				tmp.first = orders[j].first;
				tmp.second = orders[j].second;
				orders[j].first = orders[j+1].first;
				orders[j].second = orders[j+1].second;
				orders[j+1].first = tmp.first;
				orders[j+1].second = tmp.second;
			}
		}
	}
}


//정렬방식
//sprite구조체에 first인자로 거리 , second로 순서를 두고, 정렬을 한다.
//그럼 크기에 first(거리)에 따라서 정렬된다. 0인덱스가 가장 작고 끝 인덱스가 가장큼
// 이걸 다시 dist와 order에 반대로 넣는다. 인덱스0에 제일 큰값
void sortSprites(int *order, double *dist, int amount)
{
	t_pair *sprites;

	sprites = (t_pair*)malloc(sizeof(t_pair) * amount);
	int i = -1;
	while (++i < amount)
	{
		sprites[i].first = dist[i];
		sprites[i].second = order[i];
	}
	sort_order(sprites, amount);
	i = -1;
	while(++i < amount)
	{
		dist[i] = sprites[amount - i - 1].first;
		order[i] = sprites[amount - i - 1].second;
	}
	free(sprites);
}

// buff에 저장해둔 이미지데이터를 넣어준다.
// 이렇게하면 랜더링이 되는 중간에 이미지데이터가 바뀌더라도 괜찮다.
// 울펜슈타인에서는 더블 버퍼링도 사용했음.
void	draw(t_info *info)
{
	int y = 0;
	while (y < info->pars->win_height)
	{
		int x = 0;
		while(x < info->pars->win_wid)
		{
			info->img.data[y * info->pars->win_wid + x] = info->buf[y][x];
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(info->mlx, info->win, info->img.ptr, 0, 0);
}


int	exit_button(void)
{
	exit(0);
}


void draw_bground(t_info *info)
{
	int vert = info->pars->win_height / 2;
	int i = -1;
	while(++i < info->pars->win_height)
	{
		int j = -1;
		while(++j < info->pars->win_wid)
		{
			if(i < vert)
				info->buf[i][j] = info->pars->ceil_clr;
			else
				info->buf[i][j] = info->pars->floor_clr;

		}
	}
}


int map_tex_num(int side, int stepX , int stepY)
{
	if (side == 0 && stepX == 1)
		return (0);
	else if (side == 0 && stepX == -1)
		return (1);
	else if (side == 1 && stepY == -1)
		return (2);
	else if (side == 1 && stepY == 1)
		return (3);
	return(-1);
}

void	cal_dist(t_info *info)
{
	int x = -1;
	while (++x < info->pars->win_wid)
	{

		double cameraX = 2 * x / (double)info->pars->win_wid - 1;
		double raydirX = info->dirX + info->planeX * cameraX;
		double raydirY = info->dirY + info->planeY * cameraX;

		int mapX = (int)info->pars->posX;
		int mapY = (int)info->pars->posY;

		double deltadistX = fabs(1/ raydirX);
		double deltadistY = fabs(1/ raydirY);
		double firstdistX;
		double firstdistY;
		double raydist;


		int	side;
		int stepX;
		int stepY;


// ray의 방향에따라서 스텝과 계산 다른 거 구하기 구조체활용?
//
		if (raydirX < 0)
		{
			stepX = -1;
			firstdistX = (info->pars->posX - mapX) * deltadistX;
		}
		else
		{
			stepX = 1;
			firstdistX = (mapX + 1.0 - info->pars->posX) * deltadistX;
		}
		if (raydirY < 0)
		{
			stepY = -1;
			firstdistY = (info->pars->posY - mapY) * deltadistY;
		}
		else
		{
			stepY = 1;
			firstdistY = (mapY + 1.0 - info->pars->posY) * deltadistY;
		}

		//dda알고리즘 활용해서 부딪힌 부분 찾는 과정
		//is_Wall_hit함수로
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

			if (info->pars->map[mapY][mapX] == 1)
				break;
		}
		//부딪힌 면에 따라 다르게 구하기
		if (side == 1)
		{
			// double yDist = mapY - info->posY + (1 - stepY)/2;
			// raydist = yDist / raydirY;
			raydist = (mapY - info->pars->posY + (1 - stepY) / 2) / raydirY;
		}
		else
		{
			// double xDist = mapX - info->posX + (1 - stepX)/2;
			// raydist = xDist / raydirX;
			raydist = (mapX - info->pars->posX + (1 - stepX) / 2) / raydirX;
		}

		// 구한 거리를 통해 그려야할 높이 구하기.
		int lineHeight = (int)(info->pars->win_height / raydist);


		int drawStart = -lineHeight / 2 + info->pars->win_height / 2;
		if(drawStart < 0)
			drawStart = 0;
		int drawEnd = lineHeight / 2 + info->pars->win_height / 2;
		if(drawEnd >= info->pars->win_height)
			drawEnd = info->pars->win_height - 1;

		//텍스쳐 부분
		int t_num = map_tex_num(side, stepX, stepY);

		//벽의 어디에 부딪혔는지 파악하기 위해 구함.
		double wallX;
		// y값에 부딪친 경우 (side = 1) 수직인 posX와 raydirX를 활용한다.
		// x값에 부딪친 경우는 수직인 y값을 활용.
		if (side == 1)
			wallX = info->pars->posX + raydist * raydirX;
		else
			wallX = info->pars->posY + raydist * raydirY;
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
		double texPos = (drawStart - info->pars->win_height / 2 + lineHeight / 2) * texstep;
		int y = drawStart - 1;
		while (++y < drawEnd)
		{
			// 비트마스크를 사용해서 2진수의 자릿수 -1 에 &연산을 해주면 그 값 이상의 값이 나올 수 없게만든다.
			// 그렇다면 텍스쳐의 높이가 2진수의 자릿수가 아니면 ??
			int texY = (int)texPos & (T_H - 1);
			texPos += texstep;
			// 2차원배열을 활용해서 행에는 텍스쳐넘버로 어떤 텍스쳐를 쓸 지를 결정하는 데이터를 담고
			// 열에는 x,y 좌표를 2차원배열을 1차원배열처럼 사용하는 방법으로 표현한다.

			int color = info->texture[t_num][texY * T_H + texX];
			// 비트연산, 비트마스크으로 y축 면의 색을 더 어둡게 만든다.
			// if (side == 1)
			// 	color = (color >> 1) & 8355711;
			if (side == 1)
				color /= 2;
			info->buf[y][x] = color;
		}
		//벽마다의 거리를 z버퍼에 저장한다.
		info->z_buffer[x] = raydist;
	}
	int	spriteOrder[info->pars->sp_num];
	double spriteDistance[info->pars->sp_num];
	//스프라이트 캐스팅
	// 멀리있는 스프라이트부터 가까운 곳 까지 정렬함.
	int i = -1;
	while(++i < info->pars->sp_num)
	{
		spriteOrder[i] = i;
		//거리구하는건데 제곱근 필요없음. 실제 거리가 필요x. 어안효과때문에 수직거리사용함.
		spriteDistance[i] = ((info->pars->posX - info->pars->sprite[i].x) * (info->pars->posX - info->pars->sprite[i].x) + (info->pars->posY - info->pars->sprite[i].y) * (info->pars->posY - info->pars->sprite[i].y));
	}
	sortSprites(spriteOrder, spriteDistance, info->pars->sp_num);

	//스프라이트 그리기
	//calc sprite
	i = -1;
	while (++i < info->pars->sp_num)
	{
		//플레이어의 위치에서 스프라이트 위치를 뺀 상대적인 거리를 구한다.
		//sprite중에 i번째에 해당하는 스프라이트의 좌표를 가져온다.
		double spriteX = info->pars->sprite[spriteOrder[i]].x - info->pars->posX;
		double spriteY = info->pars->sprite[spriteOrder[i]].y - info->pars->posY;

		//카메라행렬의 역행렬로 벡터를 변형해줘야한다. (스프라이트의 방향 fisheye제거)
		double invDet = 1.0 / (info->planeX * info->dirY - info->dirX * info->planeY);

		//spriteX,Y에 카메라 역행렬을 곱해서 변형시킨 X,Y값
		double transformX = invDet * (info->dirY * spriteX - info->dirX * spriteY);
		double transformY = invDet * (-info->planeY * spriteX + info->planeX * spriteY);

		//transformY를 스프라이트에 대한 거리로 생각해도 된다.

		//스크린 상에서 sprite의 x좌표를 의미한다. 너비의 가운데에서 tranform
		//sprite에서 dir에 대한 수직거리는 transformX / transformY이다. 1을 더해주는 것은 plane에 대한 거리이다. 벡터연산
		int spriteScreenX = (int)((info->pars->win_wid / 2) * (1 + transformX / transformY));

		//스프라이트의 크기나 움직임을 따로 지정할 수 있음.
		#define uDiv 1
		#define vDiv 1
		#define vMove 1
		int vMoveScreen = (int)(vMove / transformY);

		int spriteHeight = (int)fabs((info->pars->win_height / transformY) / vDiv);

		//그려야할 시작 높이와 끝 높이를 구함.
		int drawStartY = -spriteHeight / 2 + info->pars->win_height / 2 + vMoveScreen;
		if(drawStartY < 0) drawStartY = 0;
		int drawEndY = spriteHeight / 2 + info->pars->win_height / 2 + vMoveScreen;
		if (drawEndY >= info->pars->win_height) drawEndY = info->pars->win_height - 1;

		//sprite의 너비 계산   q. 왜 height로 구하는가?
		// int spriteWidth = (int)fabs((info->pars->win_height / transformY) / uDiv);
		int spriteWidth = (int)fabs((info->pars->win_wid / transformY) / uDiv);
		int drawStartX = -spriteWidth / 2 + spriteScreenX;
		if(drawStartX < 0) drawStartX = 0;
		int drawEndX = spriteWidth / 2 + spriteScreenX;
		if(drawEndX >= info->pars->win_wid) drawEndX = info->pars->win_wid - 1;

		int stripe = drawStartX - 1;
		while (++stripe < drawEndX)
		{
			// int texX = (int)((256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * T_W / spriteWidth) / 256);
			int texX = (int)(((stripe - (-spriteWidth / 2 + spriteScreenX)) * T_W / spriteWidth));
			//조건 1 카메라평면 앞에있으면 안보임.
			//조건 2,3 스크린의 좌측이나 우측(범위안)에 있어야함.
			//조건 4 transform Y가 zbuffer보다 작아야함 (크면 벽뒤에 있는 것)
			if (transformY > 0 && stripe > 0 && stripe < info->pars->win_wid && transformY < info->z_buffer[stripe])
			{
				// int drawStartY = -spriteHeight / 2 + info->pars->win_height / 2 + vMoveScreen;
				// if(drawStartY < 0) drawStartY = 0;
				int y = drawStartY - 1;
				while(++y < drawEndY)
				{
					//floats를 피하기위해서 256한다는데 더 찾아봐야겠다. float끼리 계산할때 오류방지 // 형변환이랑 다른점은??
					// int d = (y - vMoveScreen) * 256 - info->pars->win_height * 128 + spriteHeight * 128;
					int d = (y - vMoveScreen) - (info->pars->win_height / 2) + (spriteHeight / 2);

					//스프라이트 픽셀이 그려지는 곳 vMoveScreen이 클수록 스프라이트는 위로 올라감.
					// printf("d = %d\n", d);

					// int texY = ((d * T_H) / spriteHeight) / 256;
					int texY = ((d * T_H) / spriteHeight);
					// int color = info->texture[pars->sprite[spriteOrder[i]].texture][T_W * texY + texX];
					int color = info->texture[4][T_W * texY + texX];
					if((color & 0x00FFFFFF) != 0) info->buf[y][stripe] = color;
				}
			}
		}
	}
}





//key적용
//plane도 적용해줘야댐
//이부분에서 틀린이유 가이드랑 다르게 나는 xy값을 다르게 받아서 처리해서 그부분에서 헷갈렸다.
int	main_loop(t_info *info)
{
	draw_bground(info);

	cal_dist(info);

	draw(info);

	key_update(info);

	return (0);
}

void	key_update(t_info *info)
{
	if (info->key_up)
	{
		if (info->pars->map[(int)(info->pars->posY)][(int)(info->pars->posX + info->dirX * info->moveSpeed)] != 1)
			info->pars->posX += info->dirX * info->moveSpeed;
		if (info->pars->map[(int)(info->pars->posY + info->dirY * info->moveSpeed)][(int)(info->pars->posX)] != 1)
			info->pars->posY += info->dirY * info->moveSpeed;
	}
	//move backwards if no wall behind you
	if (info->key_down)
	{
		if (info->pars->map[(int)(info->pars->posY)][(int)(info->pars->posX - info->dirX * info->moveSpeed)] != 1)
			info->pars->posX -= info->dirX * info->moveSpeed;
		if (info->pars->map[(int)(info->pars->posY - info->dirY * info->moveSpeed)][(int)(info->pars->posX)] != 1)
			info->pars->posY -= info->dirY * info->moveSpeed;
	}
	//rotate to the right
	if (info->key_right)
	{
		//both camera direction and camera plane must be rotated
		double oldDirX = info->dirX;
		info->dirX = info->dirX * cos(-info->rotSpeed) - info->dirY * sin(-info->rotSpeed);
		info->dirY = oldDirX * sin(-info->rotSpeed) + info->dirY * cos(-info->rotSpeed);
		double oldPlaneX = info->planeX;
		info->planeX = info->planeX * cos(-info->rotSpeed) - info->planeY * sin(-info->rotSpeed);
		info->planeY = oldPlaneX * sin(-info->rotSpeed) + info->planeY * cos(-info->rotSpeed);
	}
	//rotate to the left
	if (info->key_left)
	{
		//both camera direction and camera plane must be rotated
		double oldDirX = info->dirX;
		info->dirX = info->dirX * cos(info->rotSpeed) - info->dirY * sin(info->rotSpeed);
		info->dirY = oldDirX * sin(info->rotSpeed) + info->dirY * cos(info->rotSpeed);
		double oldPlaneX = info->planeX;
		info->planeX = info->planeX * cos(info->rotSpeed) - info->planeY * sin(info->rotSpeed);
		info->planeY = oldPlaneX * sin(info->rotSpeed) + info->planeY * cos(info->rotSpeed);
	}
	if (info->key_esc)
		exit(0);
}

int		key_press(int key, t_info *info)
{
	if (key == K_ESC)
		exit(0);
	else if (key == KEY_UP)
		info->key_up = 1;
	else if (key == KEY_DOWN)
		info->key_down = 1;
	else if (key == KEY_LEFT)
		info->key_left = 1;
	else if (key == KEY_RIGHT)
		info->key_right = 1;
	return (0);
}

int		key_release(int key, t_info *info)
{
	if (key == K_ESC)
		exit(0);
	else if (key == KEY_UP)
		info->key_up = 0;
	else if (key == KEY_DOWN)
		info->key_down = 0;
	else if (key == KEY_LEFT)
		info->key_left = 0;
	else if (key == KEY_RIGHT)
		info->key_right = 0;
	return (0);
}


/* 1. mlx_xpm_file_to함수로 xpm파일을 이미지 포인터로 가져온다. */
/* 2. mlx_get_data_addr로 이미지에 대한 정보를 가져온다. */
/* 3. 이미지에 대한 정보를 텍스쳐에다가 넣어준다. */

void	load_image(t_info *info, int *texture, char *path, t_img *img)
{
	img->ptr = mlx_xpm_file_to_image(info->mlx, path, &img->img_W, &img->img_H);

	img->data = (int *)mlx_get_data_addr(img->ptr, &img->bpp, &img->size_l, &img->endian);

	int y = -1;
	while (++y < img->img_H)
	{
		int x = -1;
		while(++x < img->img_W)
			texture[img->img_W * y + x] = img->data[img->img_W * y + x];
	}
	mlx_destroy_image(info->mlx, img->ptr);

}

// 경로를 파싱해서 가져와야함.
void	load_tex(t_info *info)
{
	t_img img;
	load_image(info, info->texture[0], info->pars->EA_path, &img);
	load_image(info, info->texture[1], info->pars->WE_path, &img);
	load_image(info, info->texture[2], info->pars->SO_path, &img);
	load_image(info, info->texture[3], info->pars->NO_path, &img);
	load_image(info, info->texture[4], info->pars->SP_path, &img);
}


// void map_malloc(t_info *info)
// {
// 	int i = 0;
// 	while(info->pars->map[i])
// 		i++;
// 	info->map = (int **)malloc(sizeof(int *) * i + 1);
// 	info->map[i] = NULL;
// 	i = 0;
// 	while(info->pars->map[i])
// 	{
// 		int j = 0;
// 		while(info->pars->map[i][j] > -2)
// 			j++;
// 		info->map[i] = (int *)malloc(sizeof(int) * j);
// 		i++;
// 	}
// }

// void map_dup(t_info *info)
// {
// 	int i = 0;
// 	while (info->pars->map[i])
// 	{
// 		int j = 0;
// 		while(info->pars->map[i][j] > -2)
// 		{
// 			info->map[i][j] = info->pars->map[i][j];
// 			j++;
// 		}
// 		info->map[i][j] = info->pars->map[i][j];
// 		i++;
// 	}
// }



