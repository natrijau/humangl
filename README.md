# HumanGL

## Objet

HumanGL est une scène 3D réalisée avec **OpenGL 4.0 ou version ultérieure**.
Le projet doit présenter un personnage articulé, animé par une hiérarchie de transformations implémentée par le projet lui-même.

## Exigences obligatoires

- Utiliser OpenGL **4.0+**.
- Implémenter les matrices et transformations nécessaires au projet, sans dépendre d’une pile de matrices OpenGL obsolète : translations, rotations, mises à l’échelle et composition hiérarchique.
- Fournir une pile de matrices propre au projet (`push`/`pop`, ou équivalent) afin de préserver et restaurer le repère de chaque partie.
- Construire un personnage articulé comprenant au minimum :
  - une tête et un torse ;
  - deux bras, chacun composé séparément d’un bras supérieur et d’un avant-bras ;
  - deux jambes, chacune composée séparément d’une cuisse et d’une partie inférieure.
- Permettre les poses ou animations suivantes : marche, saut et immobilité.
- Respecter la hiérarchie : une articulation enfant doit suivre les transformations de ses parents, sans modifier les autres branches du personnage.
- Repositionner automatiquement les articulations et les enfants lorsqu’une longueur ou une taille de membre change. Les positions ne doivent donc pas être codées à nouveau manuellement pour chaque dimension.
- Dessiner chaque partie du personnage par **exactement un appel** à une fonction qui dessine une forme de taille `1 × 1 × 1` autour de l’origine de la matrice courante. La taille et l’orientation d’une partie doivent être obtenues par les transformations de la matrice.
- Garder distinctes les parties supérieures et inférieures des membres : un bras ou une jambe ne doit pas être représenté par un seul bloc lorsque le cahier des charges demande deux segments.
- Fournir un système de compilation reproductible avec un `Makefile`.

## Compilation et exécution

Depuis la racine du projet :

```sh
make
./HumanGL
```

Le `Makefile` doit au minimum proposer une cible de compilation par défaut et une cible de nettoyage (`clean`). Il doit déclarer les dépendances nécessaires et transmettre les options appropriées pour OpenGL 4.0+.

## Démonstration attendue pour l’évaluation

La démonstration doit permettre de vérifier visuellement et rapidement :

1. le personnage complet et toutes ses parties articulées ;
2. la marche, avec un mouvement cohérent des bras et des jambes ;
3. le saut, avec une phase identifiable de montée et de descente ;
4. l’immobilité, sans animation résiduelle ;
5. la continuité de la hiérarchie lors des rotations et translations ;
6. le repositionnement correct des articulations après modification de la taille d’un membre ;
7. le fonctionnement de la compilation via `make`.

Le code doit rendre clairement identifiable la pile de matrices, la construction hiérarchique et l’unique appel de dessin utilisé pour chaque partie.
