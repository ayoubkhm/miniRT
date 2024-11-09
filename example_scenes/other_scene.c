# Lumière ambiante
A 0.2 255,255,255             # Lumière ambiante avec un ratio de 0.2 et couleur blanche

# Caméra
C 0,2,-15.1 0,0,1 70            # Caméra positionnée à (0, 1, -10) regardant vers lavant avec un FOV de 70

# Lumières ponctuelles
L 0,10,-5 0.7 255,255,255     # Lumière ponctuelle à (0, 10, -5) avec une intensité de 0.7 et couleur blanche
L 5,5,-10 0.5 255,255,255     # Lumière ponctuelle à (5, 5, -10) avec une intensité de 0.5 et couleur blanche



# Sol et Plafond
pl 0,0,25 0,0,1 0,0,255      # Plan bleu simulant larrière-plan derrière la scène

pl 0,-2,0 0,1,0 100,100,100   # Plan gris au sol (0, -2, 0) orienté vers le haut
pl 0,5,0 0,-1,0 50,50,50      # Plan gris sombre pour le plafond (0, 5, 0) orienté vers le bas


# Colonnes (cylindres)
cy 3,-5,0 0,1,0 2 9 200,200,200      # Cylindre gris avec base à (3, 0, 0), diamètre 2, hauteur 5
cy -3,-5,0 0,1,0 2 9 200,200,200     # Cylindre gris avec base à (-3, 0, 0), diamètre 2, hauteur 5
cy 3,-5,-8 0,1,0 2 9 200,200,200     # Cylindre gris avec base à (3, 0, -8), diamètre 2, hauteur 5
cy -3,-5,-8 0,1,0 2 9 200,200,200    # Cylindre gris avec base à (-3, 0, -8), diamètre 2, hauteur 5

# Sphères sur les colonnes (décorations en haut)
sp 3,5,0 3 200,200,200       # Sphère grise de rayon 1 centrée au-dessus de la colonne
sp -3,5,0 3 200,200,200      # Sphère grise de rayon 1 centrée au-dessus de la colonne
sp 3,5,-8 3 200,200,200      # Sphère grise de rayon 1 centrée au-dessus de la colonne
sp -3,5,-8 3 200,200,200     # Sphère grise de rayon 1 centrée au-dessus de la colonne

sp 3,-2,0 3 200,200,200       # Sphère grise de rayon 1 centrée en-dessous de la colonne
sp -3,-2,0 3 200,200,200      # Sphère grise de rayon 1 centrée en-dessous de la colonne
sp 3,-2,-8 3 200,200,200      # Sphère grise de rayon 1 centrée en-dessous de la colonne
sp -3,-2,-8 3 200,200,200     # Sphère grise de rayon 1 centrée en-dessous de la colonne

# Sphère rouge au centre
sp 0,-1,-6 1 255,0,0           # Sphère rouge de rayon 1 centrée à (0, -1, -4)