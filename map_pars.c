#include "cub3d.h"

int check_map(char *line)
{
	int i = 0;
	while(line[i])
	{
		if (line[i] != ' ' && line[i] != '0' && line[i] != '1'
		&& line[i] != '2' && line[i] != 'N' && line[i] != 'S'
		&& line[i] != 'W' && line[i] != 'E')
			return (0);
		i++;
	}
	return (1);
}

int map_line_check(char *line)
{
	while(*line == ' ')
		line++;
	if (line[0] == '1')
		return (1);
	return(0);
}

int *char_to_int_line(char *line)
{
	int *int_line;
	int i = -1;
	if(!(int_line =(int*)malloc(sizeof(int) * (ft_strlen(line) + 1))))
		return (NULL);
	while(line[++i])
	{
		if(line[i] == ' ')
			int_line[i] = -1;
		else
			int_line[i] = line[i] - '0';
	}
	int_line[i] = -2;
	return (int_line);
}

int pars_map(int *line, t_pars *pars)
{
	int **tmp;
	if(!check_elem(pars))
		return(0);
	if(pars->map_idx == 0)
	{
		pars->map = (int**)malloc(sizeof(int *)* 2);
		pars->map[0] = line;
		pars->map[1] = NULL;
		pars->map_idx++;
	}
	else
	{
		tmp = pars->map;
		pars->map = (int**)malloc(sizeof(int *)* (pars->map_idx + 2));
		pars->map[pars->map_idx] = line;
		pars->map[pars->map_idx + 1] = NULL;
		int i = pars->map_idx;
		while(--i >= 0)
			pars->map[i] = tmp[i];
		free(tmp);
		pars->map_idx++;
	}
	return(1);
}

// NSWE의 좌표를 가져와서 pos로 변환해야됨.
int is_NSWE(int dir)
{
	if (dir == 'N' - '0' || dir == 'S' - '0' || dir == 'W' - '0'
	|| dir == 'E' - '0')
		return (1);
	return (0);
}

void count_sp(t_pars *pars)
{
	int i = -1;
	while(pars->map[++i])
	{
		int j = -1;
		while(pars->map[i][++j] > -2)
		{
			if (pars->map[i][j] == 2)
				(pars->sp_num)++;
		}
	}
}
void pars_dir(int dir, t_pars *pars)
{
	if (dir == 'N' - '0')
	{
		pars->dirX = 0;
		pars->dirY = -1;
		pars->planeX = 0.66;
		pars->planeY = 0;
	}
	else if(dir == 'S' - '0')
	{
		pars->dirX = 0;
		pars->dirY = 1;
		pars->planeX = -0.66;
		pars->planeY = 0;
	}
	else if(dir == 'W' - '0')
	{
		pars->dirX = -1;
		pars->dirY = 0;
		pars->planeX = 0.0;
		pars->planeY = -0.66;
	}
	else if(dir == 'E' - '0')
	{
		pars->dirX = 1;
		pars->dirY = 0;
		pars->planeX = 0;
		pars->planeY = 0.66;
	}
}

int valid_map(t_pars *pars)
{
	int n = 0;
	int flag = 0;
	int sp_num = pars->sp_num - 1;
	while(pars->map[n])
		n++;
	int i = -1;
	while(pars->map[++i])
	{
		int j = -1;
		while(pars->map[i][++j] > -2)
		{
			if(pars->map[i][j] == 0 || pars->map[i][j] == 2
			|| is_NSWE(pars->map[i][j]))
			{
				if(is_NSWE(pars->map[i][j]))   //방향 하나만 있어야함. 없어도 안됨
				{
					if (flag == 1)
						return (0);
					pars_dir(pars->map[i][j], pars);
					pars->posX = j;
					pars->posY = i;
					flag = 1;
				}
				if(pars->map[i][j] == 2)
				{
					pars->sprite[sp_num].x = j;
					pars->sprite[sp_num].y = i;
					sp_num--;
				}
				if(j == 0 || i == 0 || i == n
				|| pars->map[i - 1][j] == -1 || pars->map[i+1][j] == -1
				|| pars->map[i][j-1] == -1 || pars->map[i][j+1] == -2)
					return (0);
			}
		}
	}
	return (1);
}
