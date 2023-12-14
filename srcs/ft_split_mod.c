/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_mod.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 16:52:23 by lebarbos          #+#    #+#             */
/*   Updated: 2023/12/14 21:26:58 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_string(const char *s)
{
	int	k;
	int	wordlen;

	wordlen = 0;
	while (s[wordlen] != '\0' && s[wordlen] != ' ' && s[wordlen] != '\n')
	{
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
	return (ft_strndup(s, wordlen));
}

void	clean_words(int words, char **array)
{
	int	i;
	int	len;

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
}

char	**ft_split_mod(char *s)
{
	size_t	index;
	size_t	words;
	char	**array;
	char	**new_array;

	index = 0;
	words = ft_countword(s);
	array = malloc((words + 1) * sizeof(char *));
	if (!s || !array)
		return (NULL);
	while (index < words)
	{
		while (*s == ' ' || *s == '\n')
			s++;
		array[index] = get_string(s);
		s += ft_strlen(array[index]);
		index++;
	}
	array[index] = NULL;
	clean_words(words, array);
	new_array = remove_scape_symbol(array);
	ft_free_array(array);
	return (new_array);
}
