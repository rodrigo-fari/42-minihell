/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 13:44:25 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/20 19:12:55 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*var_expand(char *input)
{
	t_env	*tmp;

	tmp = get_env(NULL);
	while (tmp)
	{
		if (ft_strcmp(input, tmp->key) == 0)
			return (ft_strdup(tmp->value));
		tmp = tmp->next;
	}
	return (ft_strdup(""));
}

static int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static char	*extract_name(char *str)
{
	int		len;

	len = 0;
	while (str[len] && is_valid_var_char(str[len]))
		len++;
	return (ft_substr(str, 0, len));
}

static char	*append_var(char *result, char *input, int *i)
{
	char	*var_name;
	char	*var_value;
	char	*temp;
	int		len;

	(*i)++;
	var_name = extract_name(&input[*i]);
	len = ft_strlen(var_name);
	var_value = var_expand(var_name);
	temp = ft_strjoin(result, var_value);
	free(var_name);
	free(var_value);
	free(result);
	*i += len;
	return (temp);
}

static char	*append_char(char *result, char c)
{
	char	buf[2];
	char	*temp;

	buf[0] = c;
	buf[1] = '\0';
	temp = ft_strjoin(result, buf);
	free(result);
	return (temp);
}

void	hd_atributes(t_token *current)
{
	if(ft_strchr(current->value, '\"') ||
	ft_strchr(current->value, '\''))
		current->eof_inquote = true;
	if (ft_strchr(current->value, '$'))
		current->eof_envvar = true;
}

char	*expand_vars(char *input)
{
	int		i;
	char	*result;

	i = 0;
	result = ft_strdup("");
	while (input[i])
	{
		if (input[i] == '$')
			result = append_var(result, input, &i);
		else
		{
			result = append_char(result, input[i]);
			i++;
		}
	}
	return (result);
}
