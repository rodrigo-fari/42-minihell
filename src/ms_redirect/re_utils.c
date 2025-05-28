/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   re_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:36:57 by aeberius          #+#    #+#             */
/*   Updated: 2025/05/28 02:43:01 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_redir_node(t_ast_node *node)
{
	if (!node->right || !node->right->args || !node->right->args[0])
	{
		ft_putstr_fd("Minishell: missing redirection target\n", STDERR_FILENO);
		return (0);
	}
	return (1);
}

int	process_redirection(t_ast_node *node, char *filename, int is_pipe)
{
	int	fd;

	fd = get_redir_fd(node, filename);
	if (fd == -1)
	{
		ft_putstr_fd("Minishell: ", STDERR_FILENO);
		perror(filename);
		return (0);
	}
	handle_redir_fd(node, fd, is_pipe);
	return (1);
}

int	apply_redirections(t_ast_node *node, int is_pipe)
{
	char	*filename;
	int		fd;

	filename = NULL;
	if (!node || !is_redir(node->type))
		return (1);
	if (node->right && is_redir(node->right->type))
		if (!apply_redirections(node->right, is_pipe))
			return (0);
	if (node->type == TOKEN_HEREDOC)
	{
		fd = open(node->heredoc_file, O_RDONLY);
		if (fd < 0)
			return (0);
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			close(fd);
			return (0);
		}
		close(fd);
	}
	else if (node_type(node))
		validate_and_process(node, filename, is_pipe);
	return (1);
}

bool	node_type(t_ast_node *node)
{
	if (node->type == TOKEN_REDIR_IN
		|| node->type == TOKEN_REDIR_OUT
		|| node->type == TOKEN_REDIR_OUT_APPEND
		|| node->type == TOKEN_REDIR_ERR
		|| node->type == TOKEN_REDIR_ERR_APPEND)
		return (true);
	return (false);
}

int	validate_and_process(t_ast_node *node, char *filename, int is_pipe)
{
	if (!validate_redir_node(node))
		return (0);
	filename = node->args[0];
	return (process_redirection(node, filename, is_pipe));
}
