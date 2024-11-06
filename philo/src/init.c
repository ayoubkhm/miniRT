#include "../include/philo.h"

void    philo_day(t_philo *p)
{
        pthread_mutex_lock(&p->l_f);            // Left fork has been locked
        display_status(p, FORK);
        if (!p->r_f)
                return (ft_usleep(p->param->die * 2));
        pthread_mutex_lock(p->r_f);             // Right fork has been locked
        display_status(p, FORK);
        display_status(p, EAT);
        pthread_mutex_lock(&p->param->time_eat);
        p->ms_eat = actual_time();
        pthread_mutex_unlock(&p->param->time_eat);
        ft_usleep(p->param->eat);
        pthread_mutex_unlock(p->r_f);
        pthread_mutex_unlock(&p->l_f);
        display_status(p, SLEEP);
        ft_usleep(p->param->sleep);
        display_status(p, THINK);
}

int     init(t_env *env)
{
        int     i;

        i = 0;
        env->arg.start_t = actual_time();
        env->arg.stop = 0;
        env->arg.nb_ph_finish = 0;
        init_mutex(env);
        while (i < env->arg.nb_ph)
        {
                env->p[i].id = i + 1;
                env->p[i].ms_eat = env->arg.start_t;
                env->p[i].nb_eat = 0;
                env->p[i].finish = 0;
                env->p[i].r_f = NULL;
                pthread_mutex_init(&env->p[i].l_f, NULL);
                if (env->arg.nb_ph == 1)
                        return (1);
                else if (i == env->arg.nb_ph - 1)
                        env->p[i].r_f = &env->p[0].l_f;
                else
                        env->p[i].r_f = &env->p[i + 1].l_f;
                i++;
        }
        return (1);
}

void    init_mutex(t_env *env)
{
        pthread_mutex_init(&env->arg.write_mutex, NULL);
        pthread_mutex_init(&env->arg.time_eat, NULL);
        pthread_mutex_init(&env->arg.dead, NULL);
        pthread_mutex_init(&env->arg.finish, NULL);
}


