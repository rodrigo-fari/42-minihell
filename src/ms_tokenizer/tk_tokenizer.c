/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tk_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 17:49:21 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/28 21:13:38 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	token_type(char *token)
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

static void	init_tokenizer_vars(t_token **after_heredoc, t_token **head,
		int *expect_cmd, int *expect_filename)
{
	*after_heredoc = NULL;
	*head = NULL;
	*expect_cmd = 1;
	*expect_filename = 0;
}

static t_token	*process_command(char *command, t_token **after_heredoc,
		int *expect_cmd, int *expect_filename)
{
	t_token	*current;

	current = ft_calloc(1, sizeof(t_token));
	if (!current)
		return (NULL);
	current->value = ft_strdup(command);
	if (!current->value)
	{
		free(current);
		return (NULL);
	}
	if (*after_heredoc)
		hd_atributes(current);
	current->type = token_type(command);
	if (current->type == TOKEN_HEREDOC)
		*after_heredoc = current;
	if (*expect_cmd && current->type == TOKEN_WORD)
		current->type = TOKEN_CMD;
	else if (*expect_filename && current->type == TOKEN_WORD)
		current->type = TOKEN_FILENAME;
	*expect_cmd = (current->type == TOKEN_PIPE);
	*expect_filename = return_filename(current);
	current->next = NULL;
	return (current);
}

static bool	add_token_to_list(t_token **head, t_token *current)
{
	if (!current)
		return (false);
	tk_listadd_back(head, current);
	return (true);
}

t_token	*token_to_struct(char **commands, int i)
{
	t_token	*head;
	t_token	*current;
	t_token	*after_heredoc;
	int		expect_cmd;
	int		expect_filename;

	init_tokenizer_vars(&after_heredoc, &head, &expect_cmd, &expect_filename);
	while (commands[i])
	{
		current = process_command(commands[i], &after_heredoc, &expect_cmd,
				&expect_filename);
		if (!add_token_to_list(&head, current))
			return (NULL);
		i++;
	}
	return (head);
}
