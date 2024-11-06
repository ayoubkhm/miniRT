#include "../include/philo.h"

int     parse_arg(int ac, char **av, t_env *env)
{
        if ((ac == 5 || ac == 6) && check_input(av))
        {
                env->arg.nb_ph = ft_atoi(av[1]);
                env->arg.die = ft_atoi(av[2]);
                env->arg.eat = ft_atoi(av[3]);
                env->arg.sleep = ft_atoi(av[4]);
                env->arg.m_eat = -1;
                if (ac == 6)
                        env->arg.m_eat = ft_atoi(av[5]);
                if (env->arg.nb_ph <= 0 || env->arg.die <= 0\
                        || env->arg.eat <= 0 || env->arg.sleep <= 0)
                        return (0);
                return (1);
        }
        return (0);
}

int     check_input(char **av)
{
        int     i;
        int     j;

        i = 1;
        while (av[i])
        {
                if (ft_strlen(av[i]) > 10)
                        return (0);
                while (av[i][j])
                {
                        if (av[i][j] >= '0' && av[i][j] <= '9')
                                j++;
                        else
                                return (0);
                }
                j = 0;
                i++;
        }
        return (1);
}

int     ft_strlen(char *str)
{
        int     i;

        i = 0;
        while (str[i++])
                ;
        return (i);
}

int        ft_error(char *str)
{
        write(2, "Error : ", 8);
        write(2, str, ft_strlen(str));
        return (0);
}

int     ft_atoi(char *str)
{
        int     n;
        int     sign;

        n = 0;
        sign = 1;
        while ((*str >= 9 && *str <= 13) || *str == 32)
                str++;
        if (*str == '-' || *str == '+')
        {
                if (*str == '-')
                        sign *= -1;
                str++;
        }
        while (*str >= '0' && *str <= '9')
        {
                n = n * 10 + *str - '0';
                str++;
        }
        return (sign * n);
}
