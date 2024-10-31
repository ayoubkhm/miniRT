// scene.c

#include "scene.h"

void init_scene(t_scene *scene) {
    scene->spheres = NULL;
    scene->planes = NULL;
    scene->cylinders = NULL;
    // ... initialiser d'autres éléments si nécessaire
}

void load_scene(t_scene *scene, const char *filename) {
    // Ouvrir le fichier et lire ligne par ligne
    // En fonction du type d'objet, appeler les fonctions d'ajout appropriées
}

void add_sphere(t_scene *scene, t_sphere *sphere) {
    // Ajouter la sphère à la liste des sphères dans la scène
}

// ... fonctions pour ajouter d'autres objets
