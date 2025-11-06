#ifndef MINISHELL_H
#	define MINISHELL_H

#define _POSIX_C_SOURCE 200809L //?

# include <sys/stat.h> //Ax
# include <dirent.h> //Ax
# include <termios.h> //Ax
# include <fcntl.h> //Ax
# include <errno.h> //Ax
#include <stdio.h> //Printf
#include <stdlib.h> //Free Malloc
#include <readline/readline.h> //Readline
#include <readline/history.h> //AddHistory
#include <unistd.h> //getcwd
#include <sys/types.h> //PID type
#include <sys/wait.h> //wait functions
#include <signal.h> //Sigaction
#include "libft.h/libft.h"

// Token types
typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_APPEND,
	T_HEREDOC,
	T_EOF
}	t_token_type;

// Token structure
typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

// Command structure
typedef struct s_cmd
{
	char			**av;
	int				ac;
	int				in_fd;
	int				out_fd;
	int				pipe[2];
	int				is_builtin;
	struct s_cmd	*next;
}	t_cmd;

// Main shell structure
typedef struct s_shell
{
	t_token		*tokens;
	t_cmd		*commands;
	char		**env;
	int			exit_status;
	int			stdin_copy;
	int			stdout_copy;
}	t_shell;


// Variable global para el código de salida
extern int g_exit_status;

//dir_manager
int		change_directory(char *path);
char	*format_cwd(char *cwd);

//commands
void	ft_echo(char **argv);
void	ft_env(void);

//utils
char	**manage_env(char **envp, int type, char **arg);
void	error_executing\
(int site_of_error, char **env, char **cmd_params);

//signals
void	sigint_handler(int sig);

//pipex
//char	**ft_split(const char *s, char c);
//int		failed_fd(void);
void	freeing_env(char **env);
//void	freeing_memory(int *fd, pid_t pid1, pid_t pid2);
//char	*ft_strjoin(char const *s1, char const *s2);
//size_t	ft_strlcpy(char *dst, const char *src, size_t size);
char	*find_binary(char *command, char **paths);
//char	**get_args(int type, char **argv);

//Struct Utils
void	init_shell(t_shell *shell);
void	cleanup_shell(t_shell *shell);
void	check_struct(t_shell *shell);

// Parser Functions prototypes
// Main
void	init_shell(t_shell *shell);
void	cleanup_shell(t_shell *shell);

// Parser tokenizer
int	is_metachar(char c);
t_token_type	get_token_type(char *str);
t_token	*tokenize(char *input);
t_cmd	*parse_tokens(t_token **tokens);
void	expand_variables(t_shell *shell, t_token *tokens);
char	*expand_string(t_shell *shell, char *str);
char	*handle_single_quotes(char *str, int *i);
char	*handle_double_quotes(t_shell *shell, char *str, int *i);
void	handle_quotes_in_token(char *input, int *i, char *quote);
char	*extract_word(char *input, int *i);
char	*extract_metachar(char *input, int *i);
int		handle_redirection(t_token **tokens, t_cmd *cmd);
void	set_builtin_flag(t_cmd *cmd);
t_cmd	*create_command(void);
void	add_arg_to_command(t_cmd *cmd, char *arg);
void	init_first_command(t_cmd **head, t_cmd **current_cmd);
void	handle_pipe_in_parse(t_token **token, t_cmd **cmd);
void	handle_redir_in_parse(t_token **token, t_cmd **cmd);
char	*expand_dollar(t_shell *shell, char *str, int *i);
int		is_dollar_terminator(char c);
int		handle_append_redirection(char *filename);
int		handle_output_redirection(char *filename);
int		handle_input_redirection(char *filename);
int		handle_heredoc(char *delimiter);

// Utils
char	*find_executable(char *cmd, char **env);
void	free_tokens(t_token **tokens);
void	free_commands(t_cmd **commands);
char	**copy_env(char **env);
void	print_error(char *cmd, char *msg);

#endif