/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tk_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:47:09 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/28 21:47:38 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tk_listadd_back(t_token **lst, t_token *new)
{
	t_token	*last;

	if (*lst == NULL)
		*lst = new;
	else
	{
		last = *lst;
		while (last->next)
			last = last->next;
		last->next = new;
	}
}

void	free_token_struct(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		if (token->value)
			free(token->value);
		tmp = token->next;
		free(token);
		token = tmp;
	}
	if (tmp)
	{
		free(tmp->value);
		free(tmp);
		tmp = NULL;
	}
	token = NULL;
}

int	tk_listsize(t_token *token)
{
	size_t	size;

	if (token == NULL)
		return (0);
	size = 1;
	while (token->next != NULL)
	{
		size++;
		token = token->next;
	}
	return (size);
}

int	skip_whitespace(char *input, int i)
{
	while (input[i] && ft_isspace(input[i]))
		i++;
	return (i);
}

bool	return_filename(t_token	*current)
{
	return ((current->type == TOKEN_REDIR_IN
			|| current->type == TOKEN_REDIR_OUT
			|| current->type == TOKEN_REDIR_OUT_APPEND
			|| current->type == TOKEN_REDIR_ERR
			|| current->type == TOKEN_REDIR_ERR_APPEND));
}
