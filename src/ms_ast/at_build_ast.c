/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   at_build_ast.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 19:57:47 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/20 19:08:10 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
static t_ast_node *attach_redirs(t_ast_node *cmd, t_ast_node *redirs);
static int count_cmd_args(t_token *token);
static void fill_cmd_args(t_ast_node *node, t_token **token, int count);
static t_ast_node *parse_command(t_token **token);


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
// Helper: Attach redirections as left-deep children to a command node
static t_ast_node	*attach_redirs(t_ast_node *cmd, t_ast_node *redirs)
{
	t_ast_node *last = cmd;
	if (!redirs)
		return cmd;
	while (redirs)
	{
		t_ast_node *next = redirs->right;
		redirs->right = last;
		last = redirs;
		redirs = next;
	}
	return last;
}

// Only count CMD/WORD/ENV_VAR tokens for args
static int count_cmd_args(t_token *token)
{
	int count = 0;
	while (token && token->type != TOKEN_PIPE)
	{
		if (token->type == TOKEN_CMD || token->type == TOKEN_WORD ||
			token->type == TOKEN_ENV_VAR)
			count++;
		token = token->next;
	}
	return count;
}

// Fill args for command node
static void	fill_cmd_args(t_ast_node *node, t_token **token, int count)
{
	int i = 0;
	t_token *curr = *token;
	while (curr && curr->type != TOKEN_PIPE && i < count)
	{
		if (is_redir(curr->type))
		{
			if (curr->next)
				curr = curr->next;
		}
		else if (curr->type == TOKEN_CMD || curr->type == TOKEN_WORD || curr->type == TOKEN_ENV_VAR)
			node->args[i++] = ft_strdup(curr->value);
		curr = curr->next;
	}
	*token = curr;
}

// Parse a command segment (until pipe or end), collect redirs, then build command node
static t_ast_node	*parse_command(t_token **token)
{
	t_ast_node *redirs = NULL, *last_redir = NULL;
	t_token *start = *token;
	int argc = count_cmd_args(*token);

	// Collect redirections and advance token
	while (*token && (*token)->type != TOKEN_PIPE)
	{
		if (is_redir((*token)->type))
		{
			t_ast_node *redir = create_node((*token)->type);
			*token = (*token)->next;
			if (redir->type == TOKEN_HEREDOC)
			{
				if (*token)
				{
					redir->args = ft_calloc(2, sizeof(char *));
					redir->args[0] = ft_strdup((*token)->value);
					
					*token = (*token)->next;
				}
			}
			if (*token && (*token)->type == TOKEN_FILENAME)
			{
				redir->args = ft_calloc(2, sizeof(char *));
				redir->args[0] = ft_strdup((*token)->value);
				*token = (*token)->next;
			}
			redir->right = NULL;
			if (!redirs)
				redirs = redir;
			else
				last_redir->right = redir;
			last_redir = redir;
		}
		else
			*token = (*token)->next;
	}

	// Now build the command node and fill its args
	t_ast_node *cmd = create_node(TOKEN_CMD);
	cmd->args = ft_calloc(argc + 1, sizeof(char *));
	fill_cmd_args(cmd, &start, argc);

	// Attach redirections as left-deep children
	return attach_redirs(cmd, redirs);
}

// Recursively build the AST for the whole input
t_ast_node	*build_ast(t_token *tokens)
{
	t_token *token;
	t_ast_node *left;

	token = tokens;
	left = parse_command(&token);
	if (token && token->type == TOKEN_PIPE)
	{
		token = token->next;
		t_ast_node *pipe = create_node(TOKEN_PIPE);
		pipe->left = left;
		pipe->right = build_ast(token);
		return pipe;
	}
	return left;
}
