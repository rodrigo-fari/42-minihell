/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ps_remove_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   */
/*   Created: 2025/03/12 14:29:07 by rde-fari          #+#    #+#             */
/*   Updated: 2025/04/21 22:27:16 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	quote_fix(t_token *tokens)
{
	t_token	*tmp;
	char	*new_value;

	tmp = tokens;
	while (tmp)
	{
		eof_quote_remove(tmp);
		new_value = verify_quotes(tmp);
		tmp->value = ft_strdup(new_value);
		tmp = tmp->next;
		free (new_value);
	}
}

char	*verify_quotes(t_token *tmp)
{
	int		i;
	char	current_quote;
	bool	key;
	char	*input;

	i = 0;
	key = false;
	current_quote = '\0';
	input = tmp->value;
	if (input[i] && (input[i] == '\"' || input[i] == '\''))
	{
		current_quote = input[i];
		if (tmp->type == TOKEN_HEREDOC)
			tmp->eof_inquote = true;
	}
	if (input[i] == '$' && tmp->type == TOKEN_HEREDOC)
		tmp->eof_envvar = true;
	while (input[i] == current_quote)
	{
		key = bool_changer(key);
		i++;
	}
	return (replace_values(input, current_quote, key, tmp));
}

char	*replace_values(char *input, char quote, bool key, t_token *tmp)
{
	char	*ret_str;
	t_env	*env;

	(void)quote;
	(void)key;
	env = get_env(NULL);
	if (tmp->eof_envvar)
		return (ft_strdup(tmp->value));
	ret_str = remove_quotes_and_expand(input, env);
	free(input);
	return (ret_str);
}

void	append_exit_status(char **ret_str, int *i)
{
	char	*tmp;
	char	*exit_status_str;

	exit_status_str = ft_itoa(g_exit_status);
	tmp = append_string_to_string(*ret_str, exit_status_str);
	if (*ret_str)
		free(*ret_str);
	*ret_str = tmp;
	free(exit_status_str);
	*i += 2;
}

void	append_variable(char **ret_str, char *input, int *i, t_env *env)
{
	char	*tmp;
	char	*var_name;
	char	*var_value;

	(*i)++;
	var_name = extract_var_name(input, i);
	var_value = get_env_value(env, var_name);
	free(var_name);
	if (var_value)
	{
		tmp = append_string_to_string(*ret_str, var_value);
		if (*ret_str)
			free(*ret_str);
		*ret_str = tmp;
		free(var_value);
	}
	else
	{
		tmp = append_string_to_string(*ret_str, "");
		if (*ret_str)
			free(*ret_str);
		*ret_str = tmp;
	}
}
