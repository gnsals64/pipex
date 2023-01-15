#include "../inc/pipex.h"

int	error_handle(int a)
{
	if (a == 0)
	{
		write(1, "invalid argument\n", 17);
		write(1, "ex)./pipex <infile> <cmd1> <cmd2> <outfile>\n", 44);
		exit(1);
	}
	if (a == 1)
	{
		write(1, "error\n", 6);
		exit(1);
	}
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

void	check_av(int ac, char **av, char **env, t_data *data)
{
	char	**path;

	path = ft_get_path(env);
	if (!path)
		error_handle(1);
	data->cmd1.data = ft_split(av[2], ' ');
	data->cmd2.data = ft_split(av[3], ' ');
	if (!data->cmd1.data || !data->cmd2.data)
		error_handle(1);
	data->cmd1.path = ft_get_cmd(data->cmd1.data, path);
	data->cmd2.path = ft_get_cmd(data->cmd2.data, path);
	if (!data->cmd1.path || data->cmd2.path)
		error_handle(1);
	return ;
}

int	ft_child_process(int ac, char **av)
{

}

int	ft_parents_process(int ac, char **av)
{

}

int main(int ac, char **av, char **env)
{
	t_data	data;

	if (ac == 5)
	{
		check_av(ac, av, env, &data);
		if (pipe(data.fd) == -1)
			return (error_handle(1));
		data.pid = fork();
		if (data.pid == -1)
			return (error_handle(1));
		else if (data.pid == 0)
			ft_child_process(ac, av);
		else if (data.pid > 0)
			ft_parents_process(ac, av);
	}
	else
		return (error_handle(0));
	return (0);
}
