/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 21:08:08 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/28 18:17:05 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define RED "\001\033[1;31m\002"
# define YELLOW "\001\033[1;33m\002"
# define GREEN "\001\033[1;32m\002"
# define CYAN "\001\033[1;36m\002"
# define BLUE "\001\033[1;34m\002"
# define MAGENTA "\001\033[1;35m\002"
# define RESET "\001\033[0m\002"

# include <stddef.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <dirent.h>
# include <string.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <curses.h>
# include <term.h>
# include <errno.h>
# include "../src/ms_libft/libft/libft.h"

# define LONG_MAX 9223372036854775807
# define LONG_MIN -9223372036854775808

extern int	g_exit_status;

typedef enum e_type
{
	TOKEN_WORD = 0,				
	TOKEN_PIPE,					
	TOKEN_REDIR_IN,				
	TOKEN_REDIR_OUT,			
	TOKEN_REDIR_OUT_APPEND,		
	TOKEN_REDIR_ERR,			
	TOKEN_REDIR_ERR_APPEND,		
	TOKEN_ENV_VAR,				
	TOKEN_CMD,					
	TOKEN_FILENAME,				
	TOKEN_HEREDOC				
}	t_type;

typedef struct s_token
{
	t_type				type;
	bool				eof_inquote;
	bool				eof_envvar;
	char				*value;
	struct s_token		*next;
}	t_token;

typedef struct s_ast_node
{
	t_type				type;
	bool				eof_inquote;
	bool				eof_envvar;
	char				**args;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	char				*heredoc_file;
}	t_ast_node;

typedef struct s_env
{
	char				*key;
	char				*value;
	bool				has_equal;
	struct s_env		*next;
}	t_env;

typedef struct s_shell
{
	t_token				*tokens;
	t_ast_node			*ast_root;
	t_env				*env_list;
	char				**envp;
	int					heredoc_fd;
}	t_shell;

//In the code:
t_ast_node	*create_node(t_type type);
void		handle_pipe(t_ast_node **root, t_ast_node *new_node,
				t_token **token);
void		handle_redir(t_ast_node **root, t_token **token);
t_ast_node	*build_ast(t_token *tokens);
void		hd_flag_definer(t_ast_node *node, t_token *token);
int			validate_cmd(char *cmd);
void		execute_forked_cmd(t_ast_node *node, t_env *env);
void		execute_ast(t_ast_node *node, t_env *env, t_token *tokens,
				int flag);
void		free_ast(t_ast_node *node);
int			count_tokens(t_token *token);
void		fill_args(t_ast_node *node, t_token **token, int count);
void		handle_command(t_ast_node **root, t_ast_node **current,
				t_token **token);
void		handle_cd_error(void);
void		change_directory(const char *path, t_env *env, char *old_pwd);
void		bi_cd(char **user_input, t_env *env);
bool		check_too_many_arguments(char **user_input);
char		*find_path_home_in_env(t_env *env);
char		*find_oldpwd_in_env(t_env *env);
void		update_pwd(t_env *env, char *old_pwd);
void		bi_echo(t_token *tmp);
bool		flag_verify(char *str);
void		bi_error(char *str);
void		handle_directory_error(char *command);
void		check_command_path(char *command_path, char **commands,
				t_shell *shell);
void		bi_exec(char **commands, t_env *env);
void		execute_builtin(char **commands, t_env *env, t_token *tokens);
void		execute_builtin_child(char **commands, t_env *env, t_token *tokens);
bool		ps_parsing(char **commands);
char		*check_direct_path(const char *cmd);
char		*find_executable(char **paths, const char *cmd);
char		*resolve_command_path(const char *cmd, t_env *env);
void		handle_builtin_or_empty(char **commands, t_env *env);
void		handle_command_not_found(char *command, t_shell *shell);
void		bi_exit(t_token *token);
void		define_exit_status(char *exit_status);
bool		check_exit_arguments(t_token *token);
bool		check_exit_signals(t_token *token);
bool		check_signal_quantity(int qnt);
void		org_env_alpha(t_env *env);
void		print_org_env(t_env *env);
void		env_update(t_env *env, char *key, char *value, bool has_equal);
void		handle_invalid_key(char *key, char *value);
void		parse_key_value(char *input, char **key, char **value,
				bool *has_equal);
void		process_key_value(t_env *env, char *key, char *value,
				bool has_equal);
void		bi_export(t_env *env, char **user_input);
bool		is_valid_key(char *key);
void		env_add(t_env *env, char *key, char *value, bool has_equal);
void		bi_pwd(void);
void		bi_unset(char **user_input, t_env *env);
void		env_remove(t_env *env, char *key);
t_env		*get_env(t_env *env);
void		update_shell_level(void);
t_env		*env_to_struct(char **environ);
char		**split_once(char *input, char c);
void		print_env(t_env *env);
void		listadd_back(t_env **lst, t_env *new);
t_env		*list_last(t_env *lst);
void		free_env_struct(t_env *env);
int			listsize(t_env *env);
char		**array_envs(t_env *envs);
void		handle_heredoc_input(const char *delimiter, int fd, bool doiexpand);
int			execute_heredoc(t_ast_node *node);
int			collect_all_heredocs(t_ast_node *node);
void		hd_atributes(t_token *current);
char		*var_expand(char *input);
char		*expand_vars(char *input);
int			ft_snprintf(char *str, size_t size, const char *format, ...);
char		*hd_remove_quotes(char *input);
void		ms_exec(char *input, t_env *env);
void		print_ast(t_ast_node *tmp);
void		ms_free(t_env *env, char *input, char **commands, t_token *tokens);
void		ms_print_fd(char *str, int fd);
int			count_args(char **commands);
void		shlvl_warning(void);
void		ps_error(char *str);
bool		quote_verifier(char *input);
bool		parse_pipes(char **commands);
bool		parse_quotes(char *input, int i, int validation);
bool		parse_redin(char **commands);
bool		parse_redout(char **commands);
bool		bool_changer(bool key);
char		*extract_var_name(char *input, int *i);
char		*get_env_value(t_env *env, char *var_name);
char		*append_string_to_string(char *str1, const char *str2);
void		quote_fix(t_token *tokens);
char		*verify_quotes(t_token *tmp);
char		*replace_values(char *input, char quote, bool key, t_token *tmp);
char		*remove_quotes_and_expand(char *input, t_env *env);
char		*remove_quotes(char *input);
void		eof_quote_remove(t_token *tokens);
void		pipe_child1(int *pipefd, t_ast_node *left, t_env *env);
void		pipe_child2(int *pipefd, t_ast_node *right, t_env *env);
void		execute_pipe(t_ast_node *left, t_ast_node *right, t_env *env);
int			node_has_out_redir(t_ast_node *node);
int			get_redir_fd(t_ast_node *node, char *filename);
void		restore_std_fds(int in, int out, int err);
void		child_process(t_ast_node *node, t_env *env);
void		execute_redirection(t_ast_node *node, t_env *env);
t_ast_node	*find_command_node(t_ast_node *node);
int			is_builtin(char *cmd);
int			node_has_in_redir(t_ast_node *node);
int			validate_redir_node(t_ast_node *node);
int			apply_redirections(t_ast_node *node, int is_pipe);
int			process_redirection(t_ast_node *node, char *filename, int is_pipe);
void		handle_redir_fd(t_ast_node *node, int fd, int is_pipe);
int			is_redir(t_type type);
void		sig_ctrl_c(int sig);
char		*get_token_type_str(t_type type);
void		print_tokens(t_token *token);
int			skip_quoted_segment(char *input, int cursor);
char		*extract_word(char *input, int *i);
int			skip_quotes(char *input, int i);
int			tk_count_words(char *input, int i, int count);
char		**tk_splitter(char *input, int i, int j);
int			is_special_char(char c);
char		*extract_special_token(char *input, int *i);
int			token_type(char *token);
t_token		*token_to_struct(char **commands);
void		tk_listadd_back(t_token **lst, t_token *new);
void		free_token_struct(t_token *token);
int			tk_listsize(t_token *token);
int			skip_whitespace(char *input, int i);

//In test:
void		free_tokens(t_token *token);
void		free_ast(t_ast_node *node);
void		free_env_list(t_env *env);
void		free_envp(char **envp);
void		cleanup_ast(t_shell *shell, bool clean_hd, bool heredoc_child);
void		cc_shell(t_shell *shell, bool clean_env, bool clean_hd,
				bool heredoc_child);
int			handle_new_filename(t_ast_node *node, char *filename);
void		cleanup_heredocs(t_ast_node *node);
t_shell		*get_shell(void);
int			execute_heredoc(t_ast_node *node);
int			collect_all_heredocs(t_ast_node *node);
char		*get_current_directory(char *current_wd);
char		*get_own_env(char *env_name);
void		free_static_pwd(void);
t_shell		*shell_atributes(t_env *env, t_token *tokens, t_ast_node *ast_root);
bool		pasring_verify(char **cmds, char *input);

//build ast
t_ast_node	*attach_redirs(t_ast_node *cmd, t_ast_node *redirs);
int			count_cmd_args(t_token *token);
void		fill_cmd_args(t_ast_node *node, t_token **token, int count);
t_ast_node	*parse_command(t_token **token);

//execute ast
void		exec_child_process(t_ast_node *node, t_env *env);
void		exec_parent_process(int pid);

#endif
