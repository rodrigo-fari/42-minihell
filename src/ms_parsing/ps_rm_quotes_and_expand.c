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
	char	*old_value;

	tmp = tokens;
	while (tmp)
	{
		eof_quote_remove(tmp);
		new_value = verify_quotes(tmp);
		old_value = tmp->value;
		tmp->value = new_value;
		free(old_value);
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
	{
		key = bool_changer(key);
		i++;
	}
	i -= 1;
	return (remove_and_expand(tmp, current_quote, &i));
}

char	*remove_and_expand(t_token *tmp, char quote, int *i)
{
	char	*new_tmp_value;

	if (quote == '\"' || ft_strchr(tmp->value, '$'))
		tmp->expand = true;
	else
		tmp->expand = false;
	new_tmp_value = ft_substr(tmp->value, (*i + 1),
	ft_strlen(tmp->value) - ((*i * 2) + 2));
	if (tmp->expand == true)
		return (expander(new_tmp_value));
	return (new_tmp_value);
}

char	*expander(char *new_tmp_value)
{
	char	**split;
	
	split = ft_split(new_tmp_value, '$');
	ft_print_array(split);
	return (NULL);
}
