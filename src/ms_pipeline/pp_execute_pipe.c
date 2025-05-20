/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 21:11:14 by aeberius          #+#    #+#             */
/*   Updated: 2025/05/14 21:13:27 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipe_child1(int *pipefd, t_ast_node *left, t_env *env)
{
	int has_out_redir;

	has_out_redir = node_has_out_redir(left);
	if (!apply_redirections(left, 1))
		exit(1);
	close(pipefd[0]);
    if (!has_out_redir)
    {
        dup2(pipefd[1], STDOUT_FILENO);
    }
	close(pipefd[1]);
    signal(SIGPIPE, SIG_DFL);
	execute_ast(left, env, NULL);
	exit(g_exit_status);
}

int apply_output_redirections(t_ast_node *node, int is_pipe)
{
    char *filename;
    while (node && is_redir(node->type))
    {
        if (node->type == TOKEN_REDIR_OUT || node->type == TOKEN_REDIR_OUT_APPEND ||
            node->type == TOKEN_REDIR_ERR || node->type == TOKEN_REDIR_ERR_APPEND)
        {
            if (!validate_redir_node(node))
                return (0);
            filename = node->args[0];
            if (!process_redirection(node, filename, is_pipe))
                return (0);
        }
        node = node->right;
    }
    return 1;
}

void	pipe_child2(int *pipefd, t_ast_node *right, t_env *env)
{
    if (!apply_output_redirections(right, 1))
        exit(1);
    close(pipefd[1]);
    if (!node_has_in_redir(right))
    {
        dup2(pipefd[0], STDIN_FILENO);
    }
    close(pipefd[0]);
    signal(SIGPIPE, SIG_DFL);
    execute_ast(right, env, NULL);
    exit(g_exit_status);
}

void	execute_pipe(t_ast_node *left, t_ast_node *right, t_env *env)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status1;
	int		status2;

	if (pipe(pipefd) == -1)
	{
		g_exit_status = 1;
		return ;
	}
	pid1 = fork();
	if (pid1 == 0)
		pipe_child1(pipefd, left, env);
	pid2 = fork();
	if (pid2 == 0)
		pipe_child2(pipefd, right, env);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
		g_exit_status = WEXITSTATUS(status2);
	else
		g_exit_status = 1;
}

