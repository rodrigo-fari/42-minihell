/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cc_cleaner.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 21:06:52 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/30 17:55:09 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_free(t_env *env, char *input, char **commands, t_token *tokens)
{
	if (tokens)
		free_tokens(tokens);
	if (env)
		free_env_list(env);
	if (input)
	{
		free(input);
		input = NULL;
	}
	if (commands)
		free_splits(commands);
	return ;
}

void	cleanup_ast(t_shell *shell, bool clean_hd, bool heredoc_child)
{
	if (shell->ast_root)
	{
		if (clean_hd && !heredoc_child)
			cleanup_heredocs(shell->ast_root);
		free_ast(shell->ast_root);
	}
}

void	cc_shell(t_shell *shell, bool clean_env, bool clean_hd,
			bool heredoc_child)
{
	if (shell->tokens)
		free_tokens(shell->tokens);
	cleanup_ast(shell, clean_hd, heredoc_child);
	if (shell->env_list && clean_env)
		free_env_list(shell->env_list);
	if (shell->envp)
		free_envp(shell->envp);
}

void	cleanup_heredocs(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->type == TOKEN_HEREDOC && node->heredoc_file)
	{
		unlink(node->heredoc_file);
		free(node->heredoc_file);
		node->heredoc_file = NULL;
	}
	cleanup_heredocs(node->left);
	cleanup_heredocs(node->right);
}

int	handle_new_filename(t_ast_node *node, char *filename)
{
	if (node->heredoc_file)
		free(node->heredoc_file);
	node->heredoc_file = ft_strdup(filename);
	if (!node->heredoc_file)
		return (-1);
	return (0);
}
