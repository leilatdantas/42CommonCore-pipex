/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 22:07:27 by lebarbos          #+#    #+#             */
/*   Updated: 2023/12/16 18:01:13 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int	ft_countword(const char *input)
{
	int			word_count;
	int			inside_quotes;
	const char	*ch;

	word_count = 0;
	inside_quotes = 0;
	ch = input;
	while (*ch != '\0')
	{
		if (*ch == '\\' && *(ch + 1) != '\0')
			ch += 2;
		if (*ch == '"' || *ch == '\'')
			inside_quotes = !inside_quotes;
		else if (*ch == ' ' && !inside_quotes)
			word_count++;
		ch++;
	}
	if (!inside_quotes)
		word_count++;
	return (word_count);
}

char	*ft_strndup(const char *s, size_t len)
{
	char	*dup;
	size_t	i;

	i = 0;
	dup = malloc(sizeof(char) * (len + 1));
	if (dup == NULL)
		return (NULL);
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

int	array_size(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

char	*remove_backslashes(char *s)
{
	size_t	len;
	size_t	j;
	size_t	k;
	char	*result;

	len = 0;
	j = 0;
	k = 0;
	while (s[j])
	{
		if (s[j + k] == '\\' && s[j + k + 1] != '\0')
			k++;
		j++;
	}
	result = malloc(sizeof(char) * (ft_strlen(s) - k + 1));
	j = 0;
	len = 0;
	while (s[j])
	{
		if (s[j] == '\\' && s[j + 1] != '\0')
			j++;
		result[len++] = s[j++];
	}
	result[len] = '\0';
	return (result);
}

char	**remove_scape_symbol(char **s)
{
	char	**new_array;
	int		i;

	remove_spaces(s);
	if (!s[0])
		return (NULL);
	new_array = malloc(sizeof(char *) * (array_size(s) + 1));
	i = 0;
	while (i < (array_size(s)))
	{
		new_array[i] = remove_backslashes(s[i]);
		i++;
	}
	new_array[i] = NULL;
	return (new_array);
}
