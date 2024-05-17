// Mur.h
#pragma once
#include "Position.hpp"
#include "Taille.hpp"

class Mur {
private:
    Position position;
    Taille taille;
    int angle;

public:
    Mur(float x, float y, float h, float w, int a) : position(x, y), taille(h, w), angle(a) {}
    Mur() : position(),taille(),angle(0){}
    Mur(float x, float y, int a) : position(x,y),taille(0,0),angle(a){}
    Taille getTaille(){
        return taille;
    }
    Position getPosition(){
        return position;
    }
    void setTaille(Taille newTaille){
        taille = newTaille;
    }
    void setPosition(Position newPos){
        position = newPos;
    }
};
