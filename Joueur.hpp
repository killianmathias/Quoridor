// Joueur.h
#pragma once
#include "Position.hpp"
#include "Taille.hpp"
class Joueur {
private:
    Position position;
    Taille taille;
    int nbMurs;
    int id;
    static const int TAILLE_CASE = 75;
    static const float ESPACE;

public:
    Joueur(double x, double y,int id) : position(x, y), taille(TAILLE_CASE, TAILLE_CASE),nbMurs(10),id(id) {}

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
    int getId(){
        return id;
    }
    void setPosition(Position pos){
        position = pos;
    }
    void Droite(){
        position.setX(position.getX()+(ESPACE+TAILLE_CASE));
    }
    void Gauche(){
        position.setX(position.getX()-(ESPACE+TAILLE_CASE));
    }
    void Bas(){
        position.setY(position.getY()+(ESPACE+TAILLE_CASE));
    }
    void Haut(){
        position.setY(position.getY()-(ESPACE+TAILLE_CASE));
    }
    sf::String strNbMurs(){
        if (nbMurs==10){
            return "Nombre de murs restants : " + std::to_string(nbMurs);
        }else{
            return "Nombre de murs restants : " + std::to_string(nbMurs)+"";
        }
    }
};
const float Joueur::ESPACE = 12.5;