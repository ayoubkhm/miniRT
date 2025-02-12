#ifndef TYPES_H
# define TYPES_H
# include "../lib/libft/libft.h"

// Définition de la couleur
typedef struct s_color
{
    int r;
    int g;
    int b;
} t_color;

// Définition du rayon
typedef struct s_ray
{
    t_vec origin;
    t_vec direction;
} t_ray;

typedef struct s_material
{
    t_color     color;              // Couleur de base de l'objet
    double  reflectivity;       // Coefficient de réflexion (0.0 à 1.0)
    double  transparency;       // Coefficient de transparence (0.0 à 1.0)
    double  refractive_index;   // Indice de réfraction (ex. 1.0 pour l'air, 1.5 pour le verre)
    double  specular_exponent;  // Exposant spéculaire (pour le calcul des reflets brillants, modèle Phong)
} t_material;

/*
 * Structure t_hit
 * ----------------
 * t_hit regroupe les informations relatives à une intersection entre un rayon et un objet.
 *
 * t         : La distance (le paramètre de l'équation du rayon) jusqu'au point d'intersection.
 * point     : Les coordonnées 3D du point d'intersection.
 * normal    : La normale à la surface de l'objet au point d'intersection.
 * material  : Le matériau de l'objet intersecté (couleur, brillance, réflectivité, etc.).
 */

    struct s_object;
    typedef struct s_object t_object;

typedef struct s_hit 
{
    double t;         // Distance d'intersection
    t_vec point;      // Point d'intersection
    t_vec normal;     // Normale au point
    t_material material; // Propriétés matérielles
    t_vec uv;         // Coordonnées UV calculées (pour la texture)
    t_object *object; // Pointeur vers l'objet intersecté
} t_hit;


typedef struct s_tools
{
    /* Rayon primaire généré à partir de la caméra pour ce pixel */
    t_ray   ray;

    /* Informations sur l'intersection avec un objet de la scène */
    t_hit   hit;          // Structure contenant le point d'intersection, la normale, etc.
    int     hit_found;    // Indique si une intersection a été trouvée (1) ou non (0)

    /* Contributions d'éclairage intermédiaires */
    double  ambient;      // Intensité de l'éclairage ambiant
    double  diffuse;      // Contribution de l'éclairage diffus (Lambert)
    double  specular;     // Contribution de l'éclairage spéculaire (brillance)
    double  reflection;   // Contribution issue des rebonds (reflets, réfractions), le cas échéant

    /* Couleurs */
    t_color     base_color;   // Couleur de base de l'objet (ex. 0xRRGGBB)
    t_color     final_color;  // Couleur finale calculée pour ce pixel (après combinaison des contributions)
} t_tools;


#endif
