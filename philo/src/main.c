#include "../include/philo.h"

void    *is_dead(void *data)
{
        t_philo *p;

        p = (t_philo *)data;
        ft_usleep(p->param->die + 1);
        pthread_mutex_lock(&p->param->time_eat);
        pthread_mutex_lock(&p->param->finish);
        if (!check_death(p, 0) && !p->finish && (actual_time() - p->ms_eat) >= (long)p->param->die)
        {
                pthread_mutex_unlock(&p->param->time_eat);
                pthread_mutex_unlock(&p->param->finish);
                display_status(p, DIED);
                check_death(p, 1);
        }
        pthread_mutex_unlock(&p->param->time_eat);
        pthread_mutex_unlock(&p->param->finish);  
        return (NULL);
}

void    *cycling(void *data)
{
        t_philo         *p;

        p = (t_philo *)data;
        if (p->id % 2 == 0)
                ft_usleep(50);
        while (!check_death(p, 0))
        {
                pthread_create(&p->thread_death_id, NULL, is_dead, data);
                philo_day(p);
                pthread_detach(p->thread_death_id);
                if ((int)++p->nb_eat == p->param->m_eat)
                {
                        pthread_mutex_lock(&p->param->finish);
                        p->finish = 1;
                        p->param->nb_ph_finish++;
                        if (p->param->nb_ph_finish == p->param->nb_ph)
                        {
                                pthread_mutex_unlock(&p->param->finish);
                                check_death(p, 2);
                        }
                        pthread_mutex_unlock(&p->param->finish);
                        return (NULL);
                }
        }
        return (NULL);
}

int	thread_creator(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->arg.nb_ph)
	{
		env->p[i].param = &env->arg;
		if (pthread_create(&env->p[i].thread_id, NULL, cycling, &env->p[i]) != 0)
			return (ft_error("Pthread did not return 0\n"));
		i++;
	}
	return (1);
}

void	stop(t_env *env)
{
	int	i;

	i = -1;
	while (!check_death2(env))
		ft_usleep(1);
	while (++i < env->arg.nb_ph)
		pthread_join(env->p[i].thread_id, NULL);
        pthread_mutex_destroy(&env->arg.dead);
        pthread_mutex_destroy(&env->arg.time_eat);
        pthread_mutex_destroy(&env->arg.finish);
	pthread_mutex_destroy(&env->arg.write_mutex);
	i = -1;
	while (++i < env->arg.nb_ph)
		pthread_mutex_destroy(&env->p[i].l_f);
	if (env->arg.stop == 2)
		printf("Each philosopher ate %d time(s)\n", env->arg.m_eat);
	free(env->p);
}

int     main(int ac, char **av)
{
        t_env   env;

        if (!parse_arg(ac, av, &env))
                return(ft_error("Invalid Arguments\n"));
        env.p = malloc(sizeof(t_philo) * env.arg.nb_ph);
        if (!env.p)
                return (ft_error("Malloc Failed\n"));
        if (!init(&env) || !thread_creator(&env))
        {
                free(env.p);
                return (0);
        }
        stop(&env);
        // return (0);
}
