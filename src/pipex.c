#include "../inc/pipex.h"

int	error_handle(int a)
{
	if (a == 0)
	{
		write(1, "invalid argument\n", 17);
		write(1, "ex)./pipex <infile> <cmd1> <cmd2> <outfile>\n", 44);
		exit(0);

	}
	if (a == 1)
	{
		write(1, "error\n", 6);
		exit(0);
	}
	return (0);
}

char	**ft_get_path(char **env)
{
	int		i;
	char	**split_path;
	char	*path;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH", 4) == 0)
		{
			path = ft_strdup(env[i] + 5);
			break ;
		}
		i++;
	}
	if (!path)
		return (NULL);
	split_path = ft_split(path, ':');
	free(path);
	return (split_path);
}

char	*ft_get_cmd(char **cmd, char **path)
{
	int		i;
	char	*tmp;
	char	*cmd_path;

	i = 0;
	while (path[i] != NULL)
	{
		tmp = ft_strjoin(path[i], "/");
		cmd_path = ft_strjoin(tmp, cmd[0]);
		free(tmp);
		if (access(cmd_path, X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		cmd_path = NULL;
		i++;
	}
	return (NULL);
}

void	ft_free_data(t_data *data, char **path)
{
	int	i;

	if (data->cmd1.data != NULL && data->cmd2.data == NULL)
	{
		i = 0;
		while (data->cmd1.data[i] != NULL)
		{
			free(data->cmd1.data[i]);
			i++;
		}
		free(data->cmd1.data);
	}
	if (data->cmd2.data != NULL && data->cmd2.data == NULL)
	{
		i = 0;
		while (data->cmd2.data[i] != NULL)
		{
			free(data->cmd2.data[i]);
			i++;
		}
		free(data->cmd2.data);
	}
	i = -1;
	while (path[++i] != NULL)
		free(path[i]);
	free(path);
}

void	ft_free_path(t_data *data, char **path)
{
	int	i;

	if (data->cmd1.path == NULL && data->cmd2.path != NULL)
		free(data->cmd2.path);
	else if (data->cmd2.path == NULL && data->cmd1.path != NULL)
		free(data->cmd1.path);
	else if (data->cmd1.path != NULL && data->cmd2.path != NULL)
	{
		free(data->cmd1.path);
		free(data->cmd1.path);
	}
	i = -1;
	while (data->cmd1.data[++i])
	{
		free(data->cmd1.data[i]);
		data->cmd1.data[i] = NULL;
	}
	i = -1;
	while (data->cmd2.data[++i])
	{
		free(data->cmd2.data[i]);
		data->cmd2.data[i] = NULL;
	}
	i = -1;
	while (path[++i])
		free(path[i]);
	free(data->cmd1.data);
	free(data->cmd2.data);
	free(path);
}

void	check_av(char **av, char **env, t_data *data)
{
	char	**path;
	int		i;

	i = -1;
	path = ft_get_path(env);
	if (!path)
		error_handle(1);
	data->cmd1.data = ft_split(av[2], ' ');
	data->cmd2.data = ft_split(av[3], ' ');
	if (!data->cmd1.data || !data->cmd2.data)
	{
		ft_free_data(data, path);
		error_handle(1);
	}
	data->cmd1.path = ft_get_cmd(data->cmd1.data, path);
	data->cmd2.path = ft_get_cmd(data->cmd2.data, path);
	if (!data->cmd1.path || !data->cmd2.path)
	{
		ft_free_path(data, path);
		error_handle(1);
	}
	while (path[++i] != NULL)
		free(path[i]);
	free(path);
	return ;
}

void	ft_child_process(t_data *data, char **env)
{
	int	i;

	i = -1;
	dup2(data->fd[1], 1);
	dup2(data->input_fd, 0);
	close(data->fd[0]);
	if (execve(data->cmd1.path, data->cmd1.data, env) == -1)
	{
		free(data->cmd1.path);
		free(data->cmd2.path);
		while (data->cmd1.data[++i])
			free(data->cmd1.data[i]);
		i = -1;
		while (data->cmd2.data[++i])
			free(data->cmd2.data[i]);
		free(data->cmd2.data);
		free(data->cmd1.data);
		error_handle(1);
	}

}

void	ft_parents_process(t_data *data, char **env)
{
	int i;

	i = -1;
	dup2(data->fd[0], 0);
	dup2(data->output_fd, 1);
	close(data->fd[1]);
	if (execve(data->cmd2.path, data->cmd2.data, env) == -1)
	{
		free(data->cmd1.path);
		free(data->cmd2.path);
		while (data->cmd1.data[++i])
			free(data->cmd1.data[i]);
		i = -1;
		while (data->cmd2.data[++i])
			free(data->cmd2.data[i]);
		free(data->cmd2.data);
		free(data->cmd1.data);
		error_handle(1);
	}
	waitpid(data->pid, NULL, 0);

}

int main(int ac, char **av, char **env)
{
	t_data	data;

	if (ac == 5)
	{
		data.input_fd = open(av[1], O_RDONLY);
		data.output_fd = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (data.input_fd == -1 || data.output_fd == -1)
			return (error_handle(1));
		check_av(av, env, &data);
		if (pipe(data.fd) == -1)
			return (error_handle(1));
		data.pid = fork();
		if (data.pid == -1)
			return (error_handle(1));
		else if (data.pid == 0)
			ft_child_process(&data, env);
		else
			ft_parents_process(&data, env);
	}
	else
		return (error_handle(0));
	return (0);
}
