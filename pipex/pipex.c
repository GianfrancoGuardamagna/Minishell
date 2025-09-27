#include "pipex.h"

static char	**get_paths_from_env(char **envp)
{
	char	**paths;
	int		i;

	i = 0;
	while (envp[i])
	{
		if (envp[i][0] == 'P' && envp[i][1] == 'A' && \
envp[i][2] == 'T' && envp[i][3] == 'H' && envp[i][4] == '=')
		{
			paths = ft_split(envp[i] + 5, ':');
			return (paths);
		}
		i++;
	}
	return (NULL);
}

static void	error_executing\
(int site_of_error, int *fd, char **env, char **cmd_params)
{
	int	i;

	i = 0;
	if (fd[0] != -1)
		close(fd[0]);
	if (fd[1] != -1)
		close(fd[1]);
	freeing_env(env);
	while (cmd_params[i])
	{
		free (cmd_params[i]);
		i++;
	}
	free (cmd_params);
	if (site_of_error == 0)
		exit((perror("command not found"), 127));
	else if (site_of_error == 1)
		exit((perror("bin not found"), 127));
	else
		exit((perror("execve"), 127));
}

static void	execute_process(char *cmd, int *fd, int child, char **envp)
{
	char	**env;
	char	**cmd_params;
	char	*bin_path;

	env = get_paths_from_env(envp);
	cmd_params = ft_split(cmd, ' ');
	if (!cmd_params || !cmd_params[0])
		error_executing(0, fd, env, cmd_params);
	bin_path = find_binary(cmd_params[0], env);
	if (!bin_path)
		error_executing(1, fd, env, cmd_params);
	if (child == 1)
		dup2(fd[1], STDOUT_FILENO);
	else
		dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	freeing_env(env);
	if (execve(bin_path, cmd_params, envp) == -1)
		error_executing(2, fd, env, cmd_params);
}

static void	process_loader(char **argv, char **envp, int *fd, int child)
{
	char	*cmd;
	int		file;

	cmd = NULL;
	if (child == 1)
	{
		cmd = argv[2];
		file = open(argv[1], O_RDONLY);
		if (file < 0)
			exit(failed_fd());
		dup2(file, STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
	}
	else
	{
		cmd = argv[3];
		file = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (file < 0)
			exit(failed_fd());
		dup2(fd[0], STDIN_FILENO);
		dup2(file, STDOUT_FILENO);
	}
	execute_process(cmd, fd, child, envp);
}

int	main(int argc, char **argv, char **envp)
{
	int			fd[2];
	pid_t		pid1;
	pid_t		pid2;

	if (argc != 5)
		return (write(2, "Invalid arguments, please try again:\n\
./pipex infile cmd1 cmd2 outfile\n", 70));
	if (pipe(fd) == -1)
		return (1);
	pid1 = fork();
	if (pid1 == -1)
		return (1);
	if (pid1 == 0)
		process_loader(argv, envp, fd, 1);
	pid2 = fork();
	if (pid2 == -1)
	{
		kill(pid1, SIGTERM);
		waitpid(pid1, NULL, 0);
		return (1);
	}
	if (pid2 == 0)
		process_loader(argv, envp, fd, 2);
	freeing_memory(fd, pid1, pid2);
	return (0);
}
