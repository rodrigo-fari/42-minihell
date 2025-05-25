/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   at_execute_ast.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 19:58:16 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/25 01:08:00 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int validate_cmd(char *cmd)
{
	struct stat buf;

	if (stat(cmd, &buf) == -1)
	{
		ft_putstr_fd("Minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		g_exit_status = 127;
		return (0);
	}
	if (S_ISDIR(buf.st_mode))
	{
		ft_putstr_fd("Minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		g_exit_status = 126;
		return (0);
	}
	if (access(cmd, X_OK) == -1)
	{
		perror("Minishell");
		g_exit_status = 126;
		return (0);
	}
	return (1);
}

void execute_forked_cmd(t_ast_node *node, t_env *env)
{
	pid_t pid;
	int status;
	int sig;
	struct	sigaction sa_old;
	struct	sigaction sa_ignore;


	if (!node->args || !node->args[0])
		return;

	if (ft_strchr(node->args[0], '/'))
		if (!validate_cmd(node->args[0]))
			return;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		g_exit_status = 1;
	}
	else if (pid == 0)
	{
		signal(SIGPIPE, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		bi_exec(node->args, env);
		cleanup_shell(get_shell(), true, true, false);
		exit(EXIT_SUCCESS);
	}
	else
	{
		sigaction(SIGINT, NULL, &sa_old);
		sa_ignore.sa_handler = SIG_IGN;
		sigemptyset(&sa_ignore.sa_mask);
		sa_ignore.sa_flags = 0;
		sigaction(SIGINT, &sa_ignore, NULL);
		waitpid(pid, &status, 0);
		sigaction(SIGINT, &sa_old, NULL);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			if (sig == SIGQUIT)
				write( 1, "Quit (core dumped)\n", 19);
			else if (sig == SIGINT)
				write(1, "\n", 1);
			g_exit_status = 128 + sig;
		}
	}
}

void execute_ast(t_ast_node *node, t_env *env, t_token *tokens, int flag)
{
	(void)flag;
	if (!node)
		return;
	if (node->type == TOKEN_PIPE)
		execute_pipe(node->left, node->right, env);
	else if (is_redir(node->type))
		execute_redirection(node, env);
	else if (node->type == TOKEN_CMD)
	{
		if (apply_redirections(node, 0))
		{
			if (is_builtin(node->args[0]))
				execute_builtin(node->args, env, tokens);
			else
				execute_forked_cmd(node, env);
		}
	}
}
