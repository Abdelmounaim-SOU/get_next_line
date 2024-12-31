/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asou <asou@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 04:10:15 by asou              #+#    #+#             */
/*   Updated: 2024/12/28 10:34:13 by asou             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*extract_line(char	*bucket)
{
	size_t	i;
	char	*result;

	i = 0;
	while (bucket[i] && bucket[i] != '\n')
		i++;
	result = ft_substr(bucket, 0, i + (bucket[i] == '\n'));
	return (result);
}

static char	*update_bucket(char *bucket)
{
	size_t	i;
	size_t	bucket_len;
	char	*result;

	i = 0;
	bucket_len = ft_strlen(bucket);
	while (bucket[i] && bucket[i] != '\n')
		i++;
	result = ft_substr(bucket, i + (bucket[i] == '\n'), bucket_len - i);
	return (result);
}

static char	*gnl_internals(int fd, char *cup, char *bucket)
{
	ssize_t		bytes_read;
	char		*temp;

	while (!ft_strchr(bucket, '\n'))
	{
		bytes_read = read(fd, cup, BUFFER_SIZE);
		if (bytes_read < 0)
			return (free(bucket), bucket = NULL, NULL);
		if (bytes_read == 0)
			return (bucket);
		cup[bytes_read] = '\0';
		temp = ft_strjoin(bucket, cup);
		if (!temp)
			return (free(bucket), bucket = NULL, NULL);
		free(bucket);
		bucket = temp;
	}
	return (bucket);
}

char	*get_next_line(int fd)
{
	static char	*bucket[OPEN_MAX];
	char		*temp;
	char		*line;
	char		*cup;

	if (fd < 0 || fd >= OPEN_MAX || BUFFER_SIZE < 0)
		return (NULL);
	cup = (char *)malloc(sizeof(char) * ((size_t)BUFFER_SIZE + 1));
	if (!cup)
		return (NULL);
	bucket[fd] = gnl_internals(fd, cup, bucket[fd]);
	free(cup);
	if (!bucket[fd] || !*bucket[fd])
		return (free(bucket[fd]), bucket[fd] = NULL, NULL);
	line = extract_line(bucket[fd]);
	temp = update_bucket(bucket[fd]);
	if (!temp)
		return (free(bucket[fd]), free(line), bucket[fd] = NULL, NULL);
	free(bucket[fd]);
	bucket[fd] = temp;
	return (line);
}
