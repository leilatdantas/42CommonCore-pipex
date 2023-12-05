/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 22:07:27 by lebarbos          #+#    #+#             */
/*   Updated: 2023/12/05 09:51:34 by lebarbos         ###   ########.fr       */
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
			ch+=2;
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

static char	*ft_strndup(const char *s, size_t len)
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

char **remove_

char	**ft_split_mod(const char *s)
{
	size_t	index;
	size_t	words;
	size_t	wordlen;
	size_t	k;
	size_t	i;
	char	**array;
	size_t	len;

	index = 0;
	words = ft_countword(s);
	array = malloc((words + 1) * sizeof(char *));
	if (!s || !array)
		return (NULL);
	while (index < words)
	{
		while (*s == ' ' || *s == '\t' || *s == '\n')
			s++;
		wordlen = 0;
		while (s[wordlen] != '\0' && s[wordlen] != ' '
			&& s[wordlen] != '\t' && s[wordlen] != '\n')
		{
			// if (s[wordlen] == '\\' && s[wordlen + 1] != '\0')
			// 	wordlen+=2;
			if (s[wordlen] == '\"' || s[wordlen] == '\'')
			{
				k = 1;
				while (s[wordlen + k] != '\0' && s[wordlen + k] != s[wordlen])
				{
					if (s[wordlen + k] == '\\' && s[wordlen + k + 1] != '\0')
						k++;
					k++;
				}
				wordlen += k + 1;
			}
			else
				wordlen++;
		}
		array[index++] = ft_strndup(s, wordlen);
		s += wordlen;
	}
	array[index] = NULL;
	i = 0;
	while (i < words)
	{
		len = ft_strlen(array[i]);
		if (len >= 2 && (array[i][0] == '\"' || array[i][0] == '\''
			|| array[i][0] == ' ') && ((array[i][len - 1] == array[i][0])
			|| array[i][0] == '\0'))
		{
			ft_memmove(array[i], array[i] + 1, len - 2);
			array[i][len - 2] = '\0';
		}
		i++;
	}
	remove_scape_symbol(array);
	return (array);
}
