/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   at_build_ast.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 19:57:47 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/28 19:38:10 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*create_node(t_type type)
{
	t_ast_node	*node;

	node = ft_calloc(1, sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->args = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_ast_node	*attach_redirs(t_ast_node *cmd, t_ast_node *redirs)
{
	t_ast_node	*last;
	t_ast_node	*next;

	last = cmd;
	if (!redirs)
		return (cmd);
	while (redirs)
	{
		next = redirs->right;
		redirs->right = last;
		last = redirs;
		redirs = next;
	}
	return (last);
}

int	count_cmd_args(t_token *token)
{
	int	count;

	count = 0;
	while (token && token->type != TOKEN_PIPE)
	{
		if (token->type == TOKEN_CMD || token->type == TOKEN_WORD
			|| token->type == TOKEN_ENV_VAR)
			count++;
		token = token->next;
	}
	return (count);
}

void	fill_cmd_args(t_ast_node *node, t_token **token, int count)
{
	t_token	*curr;
	int		i;

	i = 0;
	curr = *token;
	while (curr && curr->type != TOKEN_PIPE && i < count)
	{
		if (is_redir(curr->type))
		{
			if (curr->next)
				curr = curr->next;
		}
		else if (curr->type == TOKEN_CMD
			|| curr->type == TOKEN_WORD
			|| curr->type == TOKEN_ENV_VAR)
			node->args[i++] = ft_strdup(curr->value);
		curr = curr->next;
	}
	*token = curr;
}

void	handle_heredoc_redir(t_ast_node *redir, t_token **token)
{
	hd_flag_definer(redir, (*token));
	if (*token)
	{
		redir->args = ft_calloc(2, sizeof(char *));
		redir->args[0] = ft_strdup((*token)->value);
		*token = (*token)->next;
	}
}
