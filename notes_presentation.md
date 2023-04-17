# Slide 1

Axel pas Alex

# Slide 4

Ajout d'une slide de transition ?

## Programme minimal

```sh
./morse.exe -m MESSAGE [-b] [-n REPEAT] [-s] [-t TIMER_STRING] [-h]
```

`m MESSAGE` : le message envoyé (sa taille doit être inférieur à 249 caractères)
`b` : spécifier que le message doit être affiché en boucle
`n REPEAT` : le nombre de répétition de l'affichage du message
`t TIMER_STRING` : les valeurs de trois temps utilisé pour le morse (court, moyen et long)
`h` : affichage du message de "usage"

# Slide 5

## Exigence PC_morse :

- Lecture des arguments de lancement
    - `-b` et `-n` sont exclusifs
    - `MESSAGE` ne peut pas être vide sans `-s`
    - La trame entière ne peut excédé 256 bytes
- Envoi de trame par UART

Ajout slide : choix d'implémentation

- Si `-b` puis `-n x`, `-b` prends le dessus et `-n` est ignoré
- Si `-n x` puis `-b`, `-n` prends le dessus et `-b` est ignoré
- Un message trop long produit un message d'erreur et un non-envoie UART
- Des valeurs numériques négatives sont spécifié, la valeur absolue est prise en compte
- `-s` fait envoie une "trame de stop" avec 7 bytes à 0 et le '\n' de fin de trame.
- `-T` pour ne pouvoir précisé que le temps court car c'est ce temps multiplé par 3 et 7 qui est défini dans le standard du Morse.

## Fonctionnement final

```sh
./morse.exe -m MESSAGE [-b] [-n REPEAT] [-s] [-t TIMER_STRING] [-T TIMER] [-c COM_PORT] [-h]
```

`m MESSAGE` : le message envoyé (sa taille doit être inférieur à 249 caractères)
`b` : spécifier que le message doit être affiché en boucle
`n REPEAT` : le nombre de répétition de l'affichage du message
`t TIMER_STRING` : les valeurs de trois temps utilisé pour le morse (court, moyen et long)
`T TIMER` : la seule valeur du temps court
`c COM_PORT` : Port COM utilisé par la carte STM32F103RB via ST-Link
`h` : affichage du message de "usage"

# Slide 6

## Exigence microcontroller_morse

- Reception de trame via UART
- Traitement de la trame reçu
    - Affichage du message (en lettre) contenu dans la trame en morse via la LED intégré à la carte
    - Suivant les éléments de la trame, soit afficher le message en boucle soit un nombre déterminé de fois
    - Possibilité d'arrêter l'affichage de message

Ajout de slide: choix d'implémentation

- Réception de bytes via UART
- Réencapsulation de bytes reçu en trame
- Mise en place d'une file d'attente
- Traitement du la plus vieille trame reçu
- Lorsque la trame reçu est une trame d'arrêt (les deux premiers bytes à 0) arrêt du programme
    - Pour denouveau recevoir et afficher des messages, il faut redémarrer la carte
- Possibilité d'afficher des chiffres ainsi que des caractères spéciaux (.,?'!/()&:;=+-_"$@)
    - *Un charactère inconnu est traité comme si il n'existait pas* **TODO**
- Mise en place d'une file d'attente d'un message reçu
    - Lorsqu'un message est en court d'affichage, seul le dernier message reçu est mis dans la file **TO CHECK**

# Slide 7

Tests pas testes

## PC_morse

- Respect des paramètres de lancement
- Vérification de la validé des paramètres de lancement
    - Valeurs par défaut si non données
    - Gestion d'un message trop long
    - Gestion d'un message vide
    - Gestion du domaines de valeurs données

# Slide 8

## microcontroller_morse

- Relecture par les pairs du code
- Rythme du code morse non respecté
- Utilisation des TIMERS de la carte
- Vérification des données reçu **TODO**

# Slide 9

- Gestion des horloges
- Configuration du lien UART
- Bytes fantôme entre la réception d'octet et leur transformation trame

# Slide 10

- Inversion de la communication :
    - utilisation du bouton integré pour saisir un message en morse
    - l'envoyé vers un PC
    - afficher le message en toute lettre
- Extension des caractères pris en charge (par exemple les caractères internationaux comme les lettres accentuées)
- Utilisation d'un potentiomètre pour définir les temps utilisés
