#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <stdbool.h>
# include "../libft/libft.h"

typedef struct s_cmd_data
{
	char	*path;
	char	**data;
}	t_cmd_data;

typedef struct s_data
{
	t_cmd_data	cmd1;
	t_cmd_data	cmd2;
	int			fd[2];
	int			input_fd;
	int			output_fd;
	pid_t		pid;
}	t_data;

int		error_handle(int a);
char	**ft_get_path(char **env);
char	*ft_get_cmd(char **cmd, char **path);
void	ft_free_data(t_data *data, char **path);
void	ft_free_path(t_data *data, char **path);
void	check_av(char **av, char **env, t_data *data);
void	ft_child_process(t_data *data, char **env);
void	ft_parents_process(t_data *data, char **env);

#endif
