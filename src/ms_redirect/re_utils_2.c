/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   re_utils_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:39:05 by aeberius          #+#    #+#             */
/*   Updated: 2025/05/30 14:39:25 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	char	**tokens;
	int		result;

	tokens = NULL;
	if (!cmd)
		return (0);
	tokens = ft_split(cmd, ' ');
	if (!tokens || !tokens[0])
	{
		free_splits(tokens);
		return (0);
	}
	result = (ft_strcmp(tokens[0], "echo") == 0
			|| ft_strcmp(tokens[0], "cd") == 0
			|| ft_strcmp(tokens[0], "pwd") == 0
			|| ft_strcmp(tokens[0], "export") == 0
			|| ft_strcmp(tokens[0], "unset") == 0
			|| ft_strcmp(tokens[0], "exit") == 0
			|| ft_strcmp(tokens[0], "env") == 0);
	free_splits(tokens);
	return (result);
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

int	is_redir_no_hd(t_ast_node *node)
{
	return (node->type == TOKEN_REDIR_IN
		|| node->type == TOKEN_REDIR_OUT
		|| node->type == TOKEN_REDIR_OUT_APPEND
		|| node->type == TOKEN_REDIR_ERR
		|| node->type == TOKEN_REDIR_ERR_APPEND);
}

bool	handle_hd_fd(t_ast_node *node)
{
	int	fd;

	fd = open(node->heredoc_file, O_RDONLY);
	if (fd < 0)
		return (false);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (false);
	}
	close(fd);
	return (true);
}
