#ifndef TYPES_H
# define TYPES_H

# include "../lib/libft/libft.h"

/* ************************************************************************** */
/*                              Merged types.h                              */
/* ************************************************************************** */

/* --------------------------- */
/*    Basic type definitions   */
/* --------------------------- */

/* Couleur */
typedef struct s_color
{
    int r;
    int g;
    int b;
} t_color;

/* Couleur en double précision */
typedef struct s_color_d
{
    double r;
    double g;
    double b;
} t_color_d;

/* Rayon */
typedef struct s_ray
{
    t_vec origin;
    t_vec direction;
} t_ray;

/* Matériau */
typedef struct s_material
{
    t_color color;
    double reflectivity;
    double transparency;
    double refractive_index;
    double specular_exponent;
} t_material;

/* Déclaration anticipée de l'objet */
struct s_object;
typedef struct s_object t_object;

/* Intersection */
typedef struct s_hit
{
    double t;         /* Distance d'intersection */
    t_vec point;      /* Point d'intersection */
    t_vec normal;     /* Normale au point d'intersection */
    t_material material;  /* Propriétés matérielles de l'objet */
    t_vec uv;         /* Coordonnées UV pour la texture */
    t_object *object; /* Pointeur vers l'objet intersecté */
} t_hit;

/* Outils de calcul de l'intersection et de l'éclairage */
typedef struct s_tools
{
    t_ray ray;        /* Rayon primaire généré par la caméra */
    t_hit hit;        /* Informations sur l'intersection */
    int hit_found;    /* Intersection trouvée (1) ou non (0) */
    double ambient;   /* Contribution de l'éclairage ambiant */
    double diffuse;   /* Contribution de l'éclairage diffus */
    double specular;  /* Contribution spéculaire */
    double reflection;/* Contribution des réflexions/réfraction */
    t_color base_color;   /* Couleur de base de l'objet */
    t_color final_color;  /* Couleur finale calculée */
} t_tools;

/* Structure de trace pour la gestion des réflexions/réfraction */
typedef struct s_trace
{
    t_tools t;
    t_color reflect_color;
    t_color refract_color;
    t_ray reflect_ray;
    t_ray refract_ray;
    double eta;
} t_trace;

/* --------------------------- */
/*   Définitions supplémentaires   */
/* --------------------------- */

/* Paramètres d'éclairage */
typedef struct s_lighting_params
{
    double ambient;
    double diffuse;
    double specular;
    double reflection;
} t_lighting_params;

/* Paramètres de récursivité pour réflexions/réfraction */
typedef struct s_recursive_params
{
    double reflectivity;
    double transparency;
} t_recursive_params;

/* Paramètres pour le calcul du rayon (projection, coordonnées écran) */
typedef struct s_ray_calc
{
    float ndc_x;
    float ndc_y;
    float screen_x;
    float screen_y;
    float cam_x;
    float cam_y;
} t_ray_calc;

/* Image */
typedef struct s_image
{
    char *data;
    int size_line;
} t_image;

/* --------------------------- */
/*    Types spécifiques        */
/* --------------------------- */

/* Structure pour la rotation */
typedef struct s_rot
{
    t_vec axis;
    t_vec lo;
    t_vec ld;
    double angle;
} t_rot;

/* Coefficients pour équation quadratique */
typedef struct s_quad
{
    double a;
    double b;
    double c;
} t_quad;

/* Informations d'intersection (pour choix de candidat) */
typedef struct s_intinfo
{
    double t;
    int cand;
} t_intinfo;

/* Coefficients pour paraboloïde */
typedef struct s_parabcoeff
{
    double A;
    double B;
    double C;
} t_parabcoeff;

#endif
