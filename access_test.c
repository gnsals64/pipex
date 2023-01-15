#include <unistd.h>
#include <stdio.h>

int main(int ac, char **av, char **env)
{
	char *cmd[3];

	cmd[0] = "ls";
	cmd[1] = "-l";
	cmd[2] = NULL;

	execve("/bin/ls", cmd, env);
}
