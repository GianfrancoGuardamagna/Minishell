#ifndef MINISHELL_H
#   define MINISHELL_H

#include <stdio.h> //Printf
#include <stdlib.h> //Free Malloc
#include <readline/readline.h> //Readline
#include <unistd.h> //getcwd

char    *manage_route(char  *path);

#endif