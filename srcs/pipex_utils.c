/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 22:07:27 by lebarbos          #+#    #+#             */
/*   Updated: 2023/12/03 17:16:35 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int ft_countword(const char *input) {
    int wordCount = 0;
    int insideQuotes = 0;

    const char *ch = input;
    while (*ch != '\0') {
        if (*ch == '"' || *ch == '\'') {
            insideQuotes = !insideQuotes;
        } else if (*ch == ' ' && !insideQuotes) {
            ++wordCount;
        }

        ++ch;
    }

    if (!insideQuotes) {
        ++wordCount;
    }

    return wordCount;
}

static char *ft_strndup(const char *s, size_t len) {
    char *dup;
    size_t i;

    i = 0;
    dup = malloc(sizeof(char) * (len + 1));
    if (dup == NULL)
        return NULL;
    while (i < len) {
        dup[i] = s[i];
        i++;
    }
    dup[i] = '\0';
    return dup;
}

char **ft_split_mod(const char *s) {
    size_t index;
    size_t words;
    size_t wordlen;
    char **array;

    index = 0;
    words = ft_countword(s);
    array = malloc((words + 1) * sizeof(char *));
    if (!s || !array)
        return NULL;

    while (index < words) {
        while (*s == ' ' || *s == '\t' || *s == '\n')
            s++;

        wordlen = 0;
        int insideQuotes = 0;
        while (s[wordlen] != '\0' && ((s[wordlen] != ' ' && s[wordlen] != '\t' && s[wordlen] != '\n') || insideQuotes)) {
            if (s[wordlen] == '"' || s[wordlen] == '\'') {
                insideQuotes = !insideQuotes;
            }
            wordlen++;
        }

        array[index++] = ft_strndup(s, wordlen);
        s += wordlen;
    }

    array[index] = NULL;

    for (size_t i = 0; i < words; ++i) {
        size_t len = strlen(array[i]);
        if (len >= 2 && (array[i][0] == '"' || array[i][0] == '\'' || array[i][0] == ' ') &&
            ((array[i][len - 1] == array[i][0] && array[i][len - 2] != '\\') || array[i][0] == '\0')) {
            memmove(array[i], array[i] + 1, len - 2);
            array[i][len - 2] = '\0';
        }
    }

    return array;
}
