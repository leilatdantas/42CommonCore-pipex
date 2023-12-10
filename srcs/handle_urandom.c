/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_urandom.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 15:18:52 by lebarbos          #+#    #+#             */
/*   Updated: 2023/12/10 16:51:28 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_read(char **line, int fd, char limiter)
{
	char	*buffer;
	char	c;
	int		i;

	i = 0;
	if (fd < 0)
		return (-1);
	buffer = ft_calloc(sizeof(char), 10000000);
	if (!buffer)
		return (-1);
	while (read(fd, &c, 1) > 0 && c && c != limiter && i < 10000000 - 2)
		buffer[i++] = c;
	buffer[i] = '\n';
	buffer[i + 1] = '\0';
	*line = buffer;
	return (i + 1);
}

void	handle_urandom(void)
{
	int		tmp_fd;
	int		urandom_fd;
	char	*buffer;

	urandom_fd = open("/dev/urandom", O_RDONLY);
	if (urandom_fd < 0)
		return ;
	tmp_fd = open(URANDOM_PATH, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (tmp_fd < 0)
		return ;
	if (ft_read(&buffer, urandom_fd, '\0') == -1)
		return ;
	write(tmp_fd, buffer, ft_strlen(buffer));
	free(buffer);
	close(tmp_fd);
}
