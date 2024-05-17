#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
class Button {
public:
    Button(float x, float y, float width, float height, sf::Font& font, std::string text){
        if (!textureNormal.loadFromFile("Textures/button_dark.png")) {
            // Gérer l'erreur
        }
        if (!textureSound.loadFromFile("Textures/soundButton.png")) {
            // Gérer l'erreur
        }
        if (!textureSoundMute.loadFromFile("Textures/soundButtonMute.png")) {
            // Gérer l'erreur
        }
        textureSound.setSmooth(true);
        textureNormal.setSmooth(true);
        // Définir la forme du bouton avec des bords arrondis
        shape.setPosition(sf::Vector2f(x, y));
        shape.setSize(sf::Vector2f(width, height));
        shape.setOrigin(width/2,height/2);
        // Définir le texte du bouton
        buttonText.setFont(font);
        buttonText.setString(text);
        buttonText.setCharacterSize(32);
        buttonText.setFillColor(sf::Color(44, 24, 15));
        buttonText.setPosition(
            x,
            y
        );
        buttonText.setOrigin(buttonText.getLocalBounds().width/2,buttonText.getLocalBounds().height/2);
    }

    // Fonction pour dessiner le bouton
    void drawTo(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(buttonText);
    }

    // Fonction pour vérifier si la souris est sur le bouton
    bool isMouseOver(sf::RenderWindow& window) {
        float mouseX = sf::Mouse::getPosition(window).x;
        float mouseY = sf::Mouse::getPosition(window).y;

        float btnPosX = shape.getPosition().x-shape.getLocalBounds().width/2;
        float btnPosY = shape.getPosition().y-shape.getLocalBounds().height/2;

        float btnxPosWidth = shape.getPosition().x + shape.getLocalBounds().width/2;
        float btnyPosHeight = shape.getPosition().y + shape.getLocalBounds().height/2;

        if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY) {
            return true;
        }
        return false;
    }

    // Fonction pour mettre à jour l'état du bouton
    void update(sf::RenderWindow& window, bool& variable) {
    float hoverIncrease = 1.05f; // Facteur d'agrandissement au survol
    if (isMouseOver(window)) {
        shape.setScale(hoverIncrease, hoverIncrease);
        buttonText.setScale(hoverIncrease, hoverIncrease);
        if(variable==true && buttonText.getString()==""){
            shape.setTexture(&textureSound);
        }else if (variable ==false && buttonText.getString()==""){
            shape.setTexture(&textureSoundMute);
        }
        
    } else {
        if (buttonText.getString()==""){
            if(variable){
                shape.setTexture(&textureSound);
            }else {
                shape.setTexture(&textureSoundMute);
            }
        }else{
            shape.setTexture(&textureNormal);
        }
        shape.setScale(1.f, 1.f);
        buttonText.setScale(1.f, 1.f); // Taille normale
        // Réinitialiser l'origine et la position à la normale
    }
}
    // Fonction pour gérer les clics sur le bouton
    void handleMouseClick(bool& variable) {
        variable = !variable;
        std::cout << "La variable est maintenant " << (variable ? "true" : "false") << std::endl;
    }

private:
    sf::RectangleShape shape; // Utiliser RoundedRectangleShape pour les bords arrondis
    sf::Text buttonText;
    sf::Texture textureNormal;
    sf::Texture textureSound;
    sf::Texture textureSoundMute;

    
};