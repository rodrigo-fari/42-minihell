/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   re_utils_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:39:05 by aeberius          #+#    #+#             */
/*   Updated: 2025/05/28 02:44:03 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	return (ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

int	node_has_in_redir(t_ast_node *node)
{
	while (node)
	{
		if (node->type == TOKEN_REDIR_IN || node->type == TOKEN_HEREDOC)
			return (1);
		node = node->right;
	}
	return (0);
}

int	node_has_out_redir(t_ast_node *node)
{
	while (node)
	{
		if (node->type == TOKEN_REDIR_OUT
			|| node->type == TOKEN_REDIR_OUT_APPEND
			|| node->type == TOKEN_REDIR_ERR
			|| node->type == TOKEN_REDIR_ERR_APPEND)
			return (1);
		node = node->right;
	}
	return (0);
}

void	handle_redir_fd(t_ast_node *node, int fd, int is_pipe)
{
	(void) is_pipe;
	if (node->type == TOKEN_REDIR_IN)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			bi_error("Invalid input.\n");
	}
	else if (node->type == TOKEN_REDIR_OUT
		|| node->type == TOKEN_REDIR_OUT_APPEND)
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
			bi_error("Invalid input.\n");
	}
	else if (node->type == TOKEN_REDIR_ERR
		|| node->type == TOKEN_REDIR_ERR_APPEND)
	{
		if (dup2(fd, STDERR_FILENO) == -1)
			bi_error("Invalid input.\n");
	}
	close(fd);
}

int	is_redir(t_type type)
{
	return (type == TOKEN_REDIR_IN
		|| type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_OUT_APPEND
		|| type == TOKEN_REDIR_ERR
		|| type == TOKEN_REDIR_ERR_APPEND
		|| type == TOKEN_HEREDOC
	);
}
