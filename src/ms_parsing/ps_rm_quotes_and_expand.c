/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ps_remove_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   */
/*   Created: 2025/03/12 14:29:07 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/26 22:00:00 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	quote_fix(t_token *tokens)
{
	t_token	*tmp;
	char	*new_value;

	tmp = tokens;
	while (tmp)
	{
		eof_quote_remove(tmp);
		new_value = verify_quotes(tmp);
		free(tmp->value);
		tmp->value = new_value;
		tmp = tmp->next;
	}
}

char	*verify_quotes(t_token *tmp)
{
	int		i;
	char	current_quote;
	bool	key;
	char	*input;

	i = 0;
	key = false;
	current_quote = '\0';
	input = tmp->value;
	if (input[i] && (input[i] == '\"' || input[i] == '\''))
		current_quote = input[i];
	while (input[i] == current_quote)
	{
		key = bool_changer(key);
		i++;
	}
	if (!key && (input[i] == '\"' || input[i] == '\''))
		current_quote = input[i];
	while (input[i] == current_quote)
		i++;
	i -= 1;
	return (remove_and_expand(tmp, current_quote, &i));
}

char	*remove_and_expand(t_token *tmp, char quote, int *i)
{
	char	*new_tmp_value;

	if (quote == '\'')
		tmp->expand = false;
	if (ft_strchr(tmp->value, '$') && quote != '\'')
		tmp->expand = true;
	new_tmp_value = ft_substr(tmp->value, (*i + 1),
	ft_strlen(tmp->value) - ((*i * 2) + 2));
	if (tmp->expand == true)
		return (expander(new_tmp_value));
	return (new_tmp_value);
}


char	*expander(char *new_tmp_value)
{
	char	**split;
	char	**tmp;
	char	*ret_str;
	int		i;
	
	split = expander_split(new_tmp_value, '$');
	ft_print_array(split);
	tmp = split;
	i = 0;
	while (split[i])
	{
		tmp[i] = get_own_env(split[i]);
		i++;
	}
	ret_str = expander_strjoin(tmp);
	return (ret_str);
}

char	*expander_strjoin(char **arr)
{
	char	*ret_str;
	int		cursor;
	int		i;
	int		j;

	ret_str = ft_calloc(sizeof(char), (array_strlen(arr) + 1));
	if (!ret_str)
		return (NULL);
	cursor = 0;
	i = 0;
	while (arr[i])
	{
		j = 0;
		while (arr[i][j])
		{
			ret_str[cursor++] = arr[i][j++];
		}
		i++;
	}
	return (ret_str);
}

int	array_strlen(char **arr)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	while (arr[i])
	{
		len += ft_strlen(arr[i]);
		i++;
	}
	return (len);
}

