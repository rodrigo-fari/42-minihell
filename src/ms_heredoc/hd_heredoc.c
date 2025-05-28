/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 19:43:29 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/28 02:07:58 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	handle_heredoc_input(const char *delimiter, int fd, bool doiexpand)
{
	char *line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_cstrcmp(line, delimiter) == 0 ||
			ft_cstrcmp(line, "") == 0)
		{
			free(line);
			break;
		}
		if (ft_strchr(line, '$') && doiexpand)
			line = expand_vars(line);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

int	execute_heredoc(t_ast_node *node)
{
	int		fd;
	pid_t	pid;
	int		status;
	struct	sigaction sa_old;
	struct	sigaction sa_ignore;

	status = 0;
	if (!node || node->type != TOKEN_HEREDOC)
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		fd = open(node->heredoc_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd < 0)
			return (-1);
		if (node->eof_inquote)
			handle_heredoc_input(node->args[0], fd, false);
		else
			handle_heredoc_input(node->args[0], fd, true);
		close(fd);
		free(node->heredoc_file);
		cleanup_shell(get_shell(), true, false, true);
		exit(0);
	}
	else if (pid > 0)
	{
		sigaction(SIGINT, NULL, &sa_old);
		sa_ignore.sa_handler = SIG_IGN;
		sigemptyset(&sa_ignore.sa_mask);
		sa_ignore.sa_flags = 0;
		sigaction(SIGINT, &sa_ignore, NULL);
		waitpid(pid, &status, 0);
		sigaction(SIGINT, &sa_old, NULL);
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			write(1, "\n", 1);
			g_exit_status = 130;
			return (-1);
		}
	}
	return (0);
}

int	collect_all_heredocs(t_ast_node *node)
{
	static int	heredoc_count;
	char		filename[64];

	if (!node)
		return (0);
	if (node->type == TOKEN_HEREDOC)
	{
		snprintf(filename, sizeof(filename), ".heredoc_%d", ++heredoc_count);
		if (node->heredoc_file)
			free(node->heredoc_file); // Libera a memória previamente alocada
		node->heredoc_file = ft_strdup(filename);
		if (!node->heredoc_file)
			return (-1);
		if (execute_heredoc(node) == -1)
			return (-1);
	}
	if (collect_all_heredocs(node->left) == -1)
		return (-1);
	if (collect_all_heredocs(node->right) == -1)
		return (-1);
	return (0);
}

bool	collect_hd_verify(t_ast_node *ast_root)
{
	if (collect_all_heredocs(ast_root) == -1)
	{
		cleanup_heredocs(ast_root);
		return (false);
	}
	return (true);
}