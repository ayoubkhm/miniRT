#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>

# define THINK  "is thinking\n"
# define EAT    "is eating\n"
# define SLEEP  "is sleeping\n"
# define FORK   "has taken a fork\n"
# define DIED    "died\n"

typedef struct s_param
{
        int                     nb_ph;
        int                     die;
        int                     eat;
        int                     sleep;
        int                     m_eat;
        long int                start_t;
        pthread_mutex_t         write_mutex;
        pthread_mutex_t         dead;
        pthread_mutex_t         time_eat;
        pthread_mutex_t         finish;
        int                     nb_ph_finish;
        int                     stop;
}                               t_param;

typedef struct s_philo
{
        int             id;
        pthread_t       thread_id;
        pthread_t       thread_death_id;
        pthread_mutex_t *r_f;
        pthread_mutex_t l_f;
        t_param         *param;
        long int        ms_eat;
        unsigned int    nb_eat;
        int             finish;
}                       t_philo;

typedef struct s_env
{
        t_philo         *p;
        t_param         arg;
}                       t_env;


int             ft_error(char *str);
long int        actual_time(void);

int             thread_creator(t_env *env);
int             init(t_env *env);
void            init_mutex(t_env *env);
void            *cycling(void *data);
void            display_status(t_philo *p, char *str);
void            philo_day(t_philo *p);
void            creating_new_theory(t_philo *p);
int             parse_arg(int ac, char **av, t_env *env);
void    philo_day(t_philo *p);
int	check_death(t_philo *p, int i);
int	check_death2(t_env *env);
void	ft_usleep(long int time_in_ms);
int     ft_strlen(char *str);
int     ft_atoi(char *str);
int     check_input(char **av);


















#endif