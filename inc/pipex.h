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
	pid_t		pid;
}	t_data;

#endif
