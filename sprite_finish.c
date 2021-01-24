#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "mlx/mlx.h"


// 제출을 위해 수정
// 생각해야할 것.
// 제출용 맵은 직사각형으로 주어지지 않는다.
// 맵은 1 or 0 or 2(스프라이트)로 구성된다.
// sprite는 한개만 있으면 된다.
// 텍스쳐도 동서남북만 다르게 하면 된다.


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

# define numSprites 19

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

struct Sprite
{
	double x;
	double y;
	int		texture;
};

struct Sprite sprite[numSprites] =
{
	//mapX, mapY , 텍스쳐 number
	{20.5, 11.5, 10}, //green light in front of playerstart
	//green lights in every room
	{18.5,4.5, 10},
	{10.0,4.5, 10},
	{10.0,12.5,10},
	{3.5, 6.5, 10},
	{3.5, 20.5,10},
	{3.5, 14.5,10},
	{14.5,20.5,10},

	//row of pillars in front of wall: fisheye test
	{18.5, 10.5, 9},
	{18.5, 11.5, 9},
	{18.5, 12.5, 9},

	//some barrels around the map
	{21.5, 1.5, 8},
	{15.5, 1.5, 8},
	{16.0, 1.8, 8},
	{16.2, 1.2, 8},
	{3.5,  2.5, 8},
	{9.5, 15.5, 8},
	{10.0, 15.1,8},
	{10.5, 15.8,8},

};

int	spriteOrder[numSprites];
double spriteDistance[numSprites];

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

	double z_buffer[W_W];

}			t_game;

typedef struct s_pair
{
	double first;
	int	second;
}		t_pair;

void	key_update(t_game *game);

static int compare(const void *first, const void *second)
{
	if(*(int *)first > *(int *)second)
		return (1);
	else if(*(int *)first < *(int *)second)
		return (-1);
	else
		return (0);
}

void sort_order(t_pair *orders, int amount)
{
	t_pair tmp;

	int i = -1;
	while (++i < amount)
	{
		int j = -1;
		while (++j < amount - 1)
		{
			if (orders[j].first > orders[j+i].first)
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



// int	map[MAP_H][MAP_W] = 	{
// 							{4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
// 							{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
// 							{4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
// 							{4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
// 							{4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
// 							{4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
// 							{4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
// 							{4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
// 							{4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
// 							{4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
// 							{4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
// 							{4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
// 							{6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
// 							{8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
// 							{6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
// 							{4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
// 							{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
// 							{4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
// 							{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
// 							{4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
// 							{4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
// 							{4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
// 							{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
// 							{4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
// 						};

int	map[MAP_H][MAP_W] =
									{
										{8,8,8,8,8,8,8,8,8,8,8,4,4,6,4,4,6,4,6,4,4,4,6,4},
										{8,0,0,0,0,0,0,0,0,0,8,4,0,0,0,0,0,0,0,0,0,0,0,4},
										{8,0,3,3,0,0,0,0,0,8,8,4,0,0,0,0,0,0,0,0,0,0,0,6},
										{8,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
										{8,0,3,3,0,0,0,0,0,8,8,4,0,0,0,0,0,0,0,0,0,0,0,4},
										{8,0,0,0,0,0,0,0,0,0,8,4,0,0,0,0,0,6,6,6,0,6,4,6},
										{8,8,8,8,0,8,8,8,8,8,8,4,4,4,4,4,4,6,0,0,0,0,0,6},
										{7,7,7,7,0,7,7,7,7,0,8,0,8,0,8,0,8,4,0,4,0,6,0,6},
										{7,7,0,0,0,0,0,0,7,8,0,8,0,8,0,8,8,6,0,0,0,0,0,6},
										{7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,6,0,0,0,0,0,4},
										{7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,6,0,6,0,6,0,6},
										{7,7,0,0,0,0,0,0,7,8,0,8,0,8,0,8,8,6,4,6,0,6,6,6},
										{7,7,7,7,0,7,7,7,7,8,8,4,0,6,8,4,8,3,3,3,0,3,3,3},
										{2,2,2,2,0,2,2,2,2,4,6,4,0,0,6,0,6,3,0,0,0,0,0,3},
										{2,2,0,0,0,0,0,2,2,4,0,0,0,0,0,0,4,3,0,0,0,0,0,3},
										{2,0,0,0,0,0,0,0,2,4,0,0,0,0,0,0,4,3,0,0,0,0,0,3},
										{1,0,0,0,0,0,0,0,1,4,4,4,4,4,6,0,6,3,3,0,0,0,3,3},
										{2,0,0,0,0,0,0,0,2,2,2,1,2,2,2,6,6,0,0,5,0,5,0,5},
										{2,2,0,0,0,0,0,2,2,2,0,0,0,2,2,0,5,0,5,0,0,0,5,5},
										{2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,5,0,5,0,5,0,5,0,5},
										{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5},
										{2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,5,0,5,0,5,0,5,0,5},
										{2,2,0,0,0,0,0,2,2,2,0,0,0,2,2,0,5,0,5,0,0,0,5,5},
										{2,2,2,2,1,2,2,2,2,2,2,1,2,2,2,5,5,5,5,5,5,5,5,5}
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
			// if (side == 1)
			// 	color = (color >> 1) & 8355711;
			if (side == 1)
				color /= 2;
			game->buf[y][x] = color;
		}
		//벽마다의 거리를 z버퍼에 저장한다.
		game->z_buffer[x] = raydist;
	}

	//스프라이트 캐스팅
	// 멀리있는 스프라이트부터 가까운 곳 까지 정렬함.
	int i = -1;
	while(++i < numSprites)
	{
		spriteOrder[i] = i;
		//거리구하는건데 제곱근 필요없음. 실제 거리가 필요x. 어안효과때문에 수직거리사용함.
		spriteDistance[i] = ((game->posX - sprite[i].x) * (game->posX - sprite[i].x) + (game->posY - sprite[i].y) * (game->posY - sprite[i].y));
	}
	sortSprites(spriteOrder, spriteDistance, numSprites);

	//스프라이트 그리기

	i = -1;
	while (++i < numSprites)
	{
		//플레이어의 위치에서 스프라이트 위치를 뺀 상대적인 거리를 구한다.
		//sprite중에 i번째에 해당하는 스프라이트의 좌표를 가져온다.
		double spriteX = sprite[spriteOrder[i]].x - game->posX;
		double spriteY = sprite[spriteOrder[i]].y - game->posY;

		//카메라행렬의 역행렬로 벡터를 변형해줘야한다. (스프라이트의 방향 fisheye제거)
		double invDet = 1.0 / (game->planeX * game->dirY - game->dirX * game->planeY);

		//spriteX,Y에 카메라 역행렬을 곱해서 변형시킨 X,Y값
		double transformX = invDet * (game->dirY * spriteX - game->dirX * spriteY);
		double transformY = invDet * (-game->planeY * spriteX + game->planeX * spriteY);

		//transformY를 스프라이트에 대한 거리로 생각해도 된다.

		//스크린 상에서 sprite의 x좌표를 의미한다. 너비의 가운데에서 tranform
		//sprite에서 dir에 대한 수직거리는 transformX / transformY이다. 1을 더해주는 것은 plane에 대한 거리이다. 벡터연산
		int spriteScreenX = (int)((W_W / 2) * (1 + transformX / transformY));

		//스프라이트의 크기나 움직임을 따로 지정할 수 있음.
		#define uDiv 1
		#define vDiv 1
		#define vMove 10
		int vMoveScreen = (int)(vMove / transformY);

		int spriteHeight = (int)fabs((W_H / transformY) / vDiv);

		//그려야할 시작 높이와 끝 높이를 구함.
		int drawStartY = -spriteHeight / 2 + W_H / 2 + vMoveScreen;
		if(drawStartY < 0) drawStartY = 0;
		int drawEndY = spriteHeight / 2 + W_H / 2 + vMoveScreen;
		if (drawEndY >= W_H) drawEndY = W_H - 1;

		//sprite의 너비 계산   q. 왜 height로 구하는가?
		int spriteWidth = (int)fabs((W_H / transformY) / uDiv);
		int drawStartX = -spriteWidth / 2 + spriteScreenX;
		if(drawStartX < 0) drawStartX = 0;
		int drawEndX = spriteWidth / 2 + spriteScreenX;
		if(drawEndX >= W_W) drawEndX = W_W - 1;

		int stripe = drawStartX - 1;
		while (++stripe < drawEndX)
		{
			int texX = (int)((256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * T_W / spriteWidth) / 256);
			//조건 1 카메라평면 앞에있으면 안보임.
			//조건 2,3 스크린의 좌측이나 우측(범위안)에 있어야함.
			//조건 4 transform Y가 zbuffer보다 작아야함 (크면 벽뒤에 있는 것)
			if (transformY > 0 && stripe > 0 && stripe < W_W && transformY < game->z_buffer[stripe])
			{
				int y = drawStartY - 1;
				while(++y < drawEndY)
				{
					//floats를 피하기위해서 256한다는데 더 찾아봐야겠다.
					int d = (y - vMoveScreen) * 256 - W_H * 128 + spriteHeight * 128;
					int texY = ((d * T_H) / spriteHeight) / 256;
					int color = game->texture[sprite[spriteOrder[i]].texture][T_W * texY + texX];
					if((color & 0x00FFFFFF) != 0) game->buf[y][stripe] = color;
				}
			}

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
	load_image(game, game->texture[8], "textures/barrel.xpm", &img);
	load_image(game, game->texture[9], "textures/pillar.xpm", &img);
	load_image(game, game->texture[10], "textures/greenlight.xpm", &img);

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
	if(!(game.texture = (int **)malloc(sizeof(int*) * 11)))
		return(-1);
	i = -1;
	while (++i < 11)
	{
		if(!(game.texture[i] = (int *)malloc(sizeof(int) * T_H * T_W)))
			return (-1);
	}

	//텍스쳐 buf초기화
	i = -1;
	while(++i < 11)
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




