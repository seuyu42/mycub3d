/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seuyu <seuyu@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/29 14:28:47 by seuyu             #+#    #+#             */
/*   Updated: 2021/02/04 01:58:07 by seuyu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"


int		ft_endl(char *buff)
{
	int i;

	i = 0;
	if (!buff)
		return (-1);
	while (buff[i])
	{
		if (buff[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

int		gnl_split(char **line, char **rem)
{
	int		idx;
	char	*tmp;

	idx = ft_endl(*rem);
	if (*rem && idx != -1)
	{
		(*rem)[idx] = '\0';
		*line = ft_strdup(*rem);
		tmp = *rem;
		*rem = ft_strdup(&(*rem)[idx + 1]);
		free(tmp);
		return (1);
	}
	else if (*rem)
	{
		*line = ft_strdup(*rem);
		free(*rem);
		*rem = 0;
		return (0);
	}
	*line = ft_strdup("");
	return (0);
}

int		get_next_line(int fd, char **line)
{
	int			rdsize;
	static char *rem[OPEN_MAX];
	char		buff[BUFFER_SIZE + 1];
	char		*tmp;

	if (fd < 0 || BUFFER_SIZE <= 0 || !line)
		return (-1);
	while (0 < (rdsize = read(fd, buff, BUFFER_SIZE)))
	{
		buff[rdsize] = '\0';
		tmp = rem[fd];
		if (!(rem[fd] = ft_strjoin(rem[fd], buff)))
			return (-1);
		free(tmp);
		if (ft_endl(rem[fd]) != -1)
			break ;
	}
	if (rdsize == -1)
		return (-1);
	return (gnl_split(line, &rem[fd]));
}
