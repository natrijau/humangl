# HumanGL

Scène OpenGL 4.0+ avec un personnage articulé, une pile de matrices maison et
trois animations : immobilité, marche et saut.

## Prérequis

- Un compilateur C++17 (`c++`)
- GLFW 3
- Les headers OpenGL / X11

Sur Debian/Ubuntu :

```sh
sudo apt install build-essential libglfw3-dev libgl1-mesa-dev xorg-dev
```

GLAD est fourni dans `external/glad`. Aucune autre dépendance n'est nécessaire :
les matrices et les transformations sont implémentées dans le projet.

## Compilation et exécution

```sh
make
./HumanGL
```

Cibles disponibles : `make`, `make run`, `make clean`, `make fclean`, `make re`.

## Commandes

| Touche | Action |
|---|---|
| `W` | marche |
| `S` | immobilité |
| `Espace` | saut |
| `1` / `2` | raccourcir / allonger les bras |
| `3` / `4` | raccourcir / allonger les jambes |
| `5` / `6` | réduire / augmenter la hauteur du torse |
| `Échap` | quitter |

Les changements de taille sont appliqués à l'exécution : les articulations et
les parties enfants se repositionnent automatiquement.

## Architecture

```text
include/
├── graphics/
│   ├── cube.hpp      # cube 1x1x1, un seul appel de dessin
│   ├── opengl.hpp    # GLAD + GLFW
│   ├── shader.hpp
│   └── window.hpp
├── human/
│   └── human.hpp     # hiérarchie du personnage
└── math/
    ├── mat4.hpp      # matrices 4x4 maison (colonne-major)
    ├── matrix_stack.hpp
    └── vec3.hpp
src/                  # implémentations correspondantes
```

### Pile de matrices

`MatrixStack` est la pile du projet (`include/math/matrix_stack.hpp`) :

- `push()` duplique la matrice courante ;
- `translate`, `rotateX/Y/Z`, `scale` composent la transformation locale ;
- `pop()` restaure le repère du parent.

Aucune fonction OpenGL obsolète (`glPushMatrix`, `glRotate`, ...) n'est utilisée.

### Hiérarchie

Le personnage est un arbre de 10 parties :

```text
torse
├── tête
├── bras gauche → avant-bras gauche
├── bras droit  → avant-bras droit
├── cuisse gauche → jambe gauche
└── cuisse droite → jambe droite
```

`Human::rebuild()` calcule chaque pivot à partir des dimensions du parent.
Modifier la longueur d'un bras déplace donc automatiquement l'avant-bras ; modifier
la longueur des jambes remonte le torse et la tête pour garder les pieds au sol.

`Human::drawPart()` parcourt l'arbre : le repère de l'articulation est empilé,
la partie est dessinée, puis les enfants sont parcourus avant de dépiler.

### Dessin

`Cube::draw()` dessine un cube de taille `1 × 1 × 1` centré sur l'origine de la
matrice courante. Chaque partie appelle cette fonction **exactement une fois** ;
sa taille et son orientation viennent uniquement des transformations de la pile.

## Vérifications

- `make re` compile sans warning avec `-Wall -Wextra -Werror`.
- Le contexte demandé est OpenGL 4.0 core (GLAD est généré pour 4.6 et sert de
  loader ; le programme vérifie `GLAD_GL_VERSION_4_0`).
- `S` fige complètement la pose (aucune animation résiduelle).
- `W` fait marcher le personnage, `Espace` déclenche un saut avec montée et
  descente identifiables.
- Les touches `1` à `6` démontrent le repositionnement automatique des
  articulations.
