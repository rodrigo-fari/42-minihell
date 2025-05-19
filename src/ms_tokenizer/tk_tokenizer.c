/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tk_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 17:49:21 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/17 19:24:18 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int token_type(char *token)
{
	if (ft_strncmp(token, "2>>", 3) == 0)
		return (TOKEN_REDIR_ERR_APPEND);
	else if (ft_strncmp(token, "2>", 2) == 0)
		return (TOKEN_REDIR_ERR);
	else if (ft_strncmp(token, ">>", 2) == 0)
		return (TOKEN_REDIR_OUT_APPEND);
	else if (ft_strncmp(token, "<<", 2) == 0)
		return (TOKEN_HEREDOC);
	else if (ft_strcmp(token, "|") == 0)
		return (TOKEN_PIPE);
	else if (ft_strcmp(token, "<") == 0)
		return (TOKEN_REDIR_IN);
	else if (ft_strcmp(token, ">") == 0)
		return (TOKEN_REDIR_OUT);
	else if (ft_strchr(token, '$'))
		return (TOKEN_ENV_VAR);
	else
		return (TOKEN_WORD);
}

t_token *token_to_struct(char **commands)
{
	t_token *head;
	t_token *current;
	int i;
	int expect_cmd;
	int expect_filename;

	head = NULL;
	i = 0;
	expect_cmd = 1; // At the start, expect a command
	expect_filename = 0;
	while (commands[i])
	{
		current = ft_calloc(1, sizeof(t_token));
		if (!current)
			return (NULL);
		current->value = ft_strdup(commands[i]);
		if (!current->value)
		{
			free(current);
			return (NULL);
		}
		current->type = token_type(commands[i]);
		if (expect_cmd && current->type == TOKEN_WORD)
			current->type = TOKEN_CMD;
		else if (expect_filename && current->type == TOKEN_WORD)
			current->type = TOKEN_FILENAME;
		expect_cmd = (current->type == TOKEN_PIPE);
		expect_filename = (current->type == TOKEN_REDIR_IN ||
						   current->type == TOKEN_REDIR_OUT ||
						   current->type == TOKEN_REDIR_OUT_APPEND ||
						   current->type == TOKEN_REDIR_ERR ||
						   current->type == TOKEN_REDIR_ERR_APPEND);
		current->next = NULL;
		tk_listadd_back(&head, current);
		i++;
	}
	return (head);
}
