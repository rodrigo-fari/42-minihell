/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cc_cleaner.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 21:06:52 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/21 17:49:05 by rde-fari         ###   ########.fr       */
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

void	cleanup_shell(t_shell *shell, int flag)
{
	if (shell->tokens)
		free_tokens(shell->tokens);
	if (shell->ast_root)
	{
		cleanup_heredocs(shell->ast_root);
		free_ast(shell->ast_root);
	}
	if (shell->env_list && flag == 1)
		free_env_list(shell->env_list);
	if (shell->envp)
		free_envp(shell->envp);
}

void	cleanup_heredocs(t_ast_node *node)
{
	if (!node)
		return;
	if (node->type == TOKEN_HEREDOC && node->heredoc_file)
	{
		unlink(node->heredoc_file);
		free(node->heredoc_file);
		node->heredoc_file = NULL;
	}
	cleanup_heredocs(node->left);
	cleanup_heredocs(node->right);
}