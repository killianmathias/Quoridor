// Joueur.h
#pragma once
#include "Position.hpp"
#include "Taille.hpp"
class Joueur {
private:
    Position position;
    Taille taille;
    int nbMurs;
    static const int TAILLE_CASE = 75;
    static const float ESPACE;

public:
    Joueur(int x, int y) : position(x, y), taille(TAILLE_CASE, TAILLE_CASE),nbMurs(10) {}

    void retirerMurs(){
        if (nbMurs>0){
            nbMurs-=1;
        }
    }
    Taille getTaille(){
        return taille;
    }
    Position getPosition(){
        return position;
    }
    int getNbMurs(){
        return nbMurs;
    }
    void Droite(){
        position.setX(position.getX()+(ESPACE+TAILLE_CASE));
    }
    void Gauche(){
        position.setX(position.getX()-(ESPACE+TAILLE_CASE));
    }
    void Haut(){
        position.setY(position.getY()+(ESPACE+TAILLE_CASE));
    }
    void Bas(){
        position.setY(position.getY()-(ESPACE+TAILLE_CASE));
    }
};
const float Joueur::ESPACE = 12.5;