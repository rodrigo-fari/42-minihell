/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tk_print_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:51:45 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/20 16:05:53 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_token_type_str(t_type type)
{
	static char *type_str[] = {
		"TOKEN_WORD",				// 0
		"TOKEN_PIPE",				// 1
		"TOKEN_REDIR_IN",			// 2
		"TOKEN_REDIR_OUT",			// 3
		"TOKEN_REDIR_OUT_APPEND",	// 4
		"TOKEN_REDIR_ERR",			// 5
		"TOKEN_REDIR_ERR_APPEND",	// 6
		"TOKEN_ENV_VAR",			// 7
		"TOKEN_CMD",				// 8
		"TOKEN_FILENAME",			// 9
		"TOKEN_HEREDOC"				// 10
	};
	if (type >= 0 && type <= TOKEN_HEREDOC)
		return (type_str[type]);
	return ("TOKEN_UNKNOWN");
}

void print_tokens(t_token *token)
{
	t_token *token_tmp;
	int i;

	token_tmp = token;
	i = 1;
	printf("==================== TOKENS ====================\n");
	while (token_tmp)
	{
		printf("[#%02d] ARG: %-15s | TYPE: %s\n       EOF_ENVVAR: %s\n       EOF_INQUOTES: %s\n",
				i, token_tmp->value, get_token_type_str(token_tmp->type),
				token_tmp->eof_envvar ? "true" : "false",
				token_tmp->eof_inquote ? "true" : "false");
		token_tmp = token_tmp->next;
		i++;
	}
	printf("================================================\n");
}
