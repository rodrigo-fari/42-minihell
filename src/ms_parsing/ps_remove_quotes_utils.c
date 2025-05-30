/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ps_remove_quotes_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 18:14:52 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/30 16:55:16 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	bool_changer(bool key)
{
	return (!key);
}

char	*extract_var_name(char *input, int *i)
{
	int		start;
	char	*var_name;

	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	var_name = ft_substr(input, start, *i - start);
	return (var_name);
}

char	*get_env_value(t_env *env, char *var_name)
{
	while (env)
	{
		if (ft_strcmp(env->key, var_name) == 0)
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (ft_strdup(""));
}

char	*append_string_to_string(char *str1, const char *str2)
{
	char	*new_str;
	size_t	len1;
	size_t	len2;

	len1 = 0;
	if (str1)
		len1 = ft_strlen(str1);
	len2 = 0;
	if (str2)
		len2 = ft_strlen(str2);
	new_str = malloc(len1 + len2 + 1);
	if (!new_str)
	{
		if (str1)
			free(str1);
		return (NULL);
	}
	if (str1)
		ft_strcpy(new_str, str1);
	if (str2)
		ft_strcpy(new_str + len1, str2);
	new_str[len1 + len2] = '\0';
	return (new_str);
}
