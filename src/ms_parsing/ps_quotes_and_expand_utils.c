/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ps_quotes_and_expand_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 18:17:01 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/31 13:45:58 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_quote_open(char c, char quote)
{
	return ((c == '\'' || c == '\"') && quote == '\0');
}

static bool	is_invalid_var_char(char c)
{
	return (!c || (!ft_isalnum(c) && c != '_'));
}

static void	handle_dollar(char *input, int *i, char **ret, t_env *env)
{
	char	*tmp;

	if (input[*i + 1] == '?')
		append_exit_status(ret, i);
	else if (is_invalid_var_char(input[*i + 1]))
	{
		tmp = append_char_to_string(*ret, '$');
		if (*ret)
			free(*ret);
		*ret = tmp;
		(*i)++;
	}
	else
		append_variable(ret, input, i, env);
}

static void	append_normal_char(char *input, int *i, char **ret)
{
	char	*tmp;

	tmp = append_char_to_string(*ret, input[*i]);
	if (*ret)
		free(*ret);
	*ret = tmp;
	(*i)++;
}

char	*remove_quotes_and_expand(char *input, t_env *env)
{
	int		i;
	char	quote;
	char	*ret_str;

	i = 0;
	quote = '\0';
	ret_str = NULL;
	env = get_env(NULL);
	if ((input[0] == '$') && (input[1] == '\'' || input[1] == '\"'))
		input++;
	while (input[i])
	{
		if (is_quote_open(input[i], quote))
			quote = input[i++];
		else if (input[i] == quote)
		{
			quote = '\0';
			i++;
		}
		else if (input[i] == '$' && quote != '\'')
			handle_dollar(input, &i, &ret_str, env);
		else
			append_normal_char(input, &i, &ret_str);
	}
	return (ret_str);
}
