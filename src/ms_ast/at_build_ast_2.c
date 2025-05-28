/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   at_build_ast_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:36:40 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/28 19:43:02 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_filename_redir(t_ast_node *redir, t_token **token)
{
	redir->args = ft_calloc(2, sizeof(char *));
	redir->args[0] = ft_strdup((*token)->value);
	*token = (*token)->next;
}

void	append_redir_node(t_ast_node **redirs,
		t_ast_node **last_redir, t_ast_node *redir)
{
	redir->right = NULL;
	if (!*redirs)
		*redirs = redir;
	else
		(*last_redir)->right = redir;
	*last_redir = redir;
}

void	parse_redir_loop(t_token **token, t_ast_node **redirs,
		t_ast_node **last_redir)
{
	t_ast_node	*redir;

	while (*token && (*token)->type != TOKEN_PIPE)
	{
		if (is_redir((*token)->type))
		{
			redir = create_node((*token)->type);
			*token = (*token)->next;
			if (redir->type == TOKEN_HEREDOC)
				handle_heredoc_redir(redir, token);
			if (*token && (*token)->type == TOKEN_FILENAME)
				handle_filename_redir(redir, token);
			append_redir_node(redirs, last_redir, redir);
		}
		else
			*token = (*token)->next;
	}
}

t_ast_node	*parse_command(t_token **token)
{
	t_ast_node	*redirs;
	t_ast_node	*last_redir;
	t_token		*start;
	int			argc;
	t_ast_node	*cmd;

	redirs = NULL;
	last_redir = NULL;
	start = *token;
	argc = count_cmd_args(*token);
	parse_redir_loop(token, &redirs, &last_redir);
	cmd = create_node(TOKEN_CMD);
	cmd->args = ft_calloc(argc + 1, sizeof(char *));
	fill_cmd_args(cmd, &start, argc);
	return (attach_redirs(cmd, redirs));
}

t_ast_node	*build_ast(t_token *tokens)
{
	t_token		*token;
	t_ast_node	*left;
	t_ast_node	*pipe;

	token = tokens;
	left = parse_command(&token);
	if (token && token->type == TOKEN_PIPE)
	{
		token = token->next;
		pipe = create_node(TOKEN_PIPE);
		pipe->left = left;
		pipe->right = build_ast(token);
		return (pipe);
	}
	return (left);
}
