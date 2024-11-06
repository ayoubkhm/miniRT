#include "../include/philo.h"

void	ft_usleep(long int time_in_ms)
{
	long int	start_t;

	start_t = actual_time();
	while ((actual_time() - start_t) < time_in_ms)
		usleep(500);
}

long int   actual_time(void)
{
        struct timeval  curr_t;

        if (gettimeofday(&curr_t, NULL) == -1)
                ft_error("Gettimofday failed !");
        return ((curr_t.tv_sec * 1000) + (curr_t.tv_usec / 1000));
}

void    display_status(t_philo *p, char *str)
{
        long int        time;

        pthread_mutex_lock(&p->param->write_mutex);
        time = actual_time() - p->param->start_t;
        if (time >= 0 && time <= 2147483647 && !check_death(p, 0))
                printf("%ld %d %s" ,time, p->id, str);
        pthread_mutex_unlock(&p->param->write_mutex);


}


int	check_death2(t_env *env)
{
	pthread_mutex_lock(&env->arg.dead);
	if (env->arg.stop)
	{
		pthread_mutex_unlock(&env->arg.dead);
		return (1);
	}
	pthread_mutex_unlock(&env->arg.dead);
	return (0);
}


int	check_death(t_philo *p, int i)
{
	pthread_mutex_lock(&p->param->dead);
	if (i)
		p->param->stop = i;
	if (p->param->stop)
	{
		pthread_mutex_unlock(&p->param->dead);
		return (1);
	}
	pthread_mutex_unlock(&p->param->dead);
	return (0);
}