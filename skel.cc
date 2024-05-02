/*
 * Pour compiler ce code, il suffit de taper `make` dans un terminal.
 * Si tout ce passe bien, un exécutable du nom `skel` apparaît dans
 * le répertoire et vous pouvez l'exécuter en tapant `./skel`.
 *
 * Ne faites pas attention au warning qui vous indique que la variable
 * `dt` n'est pas utilisée. C'est normal pour l'instant.
 */

#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cassert>
#include "Mur.hpp"
#include "Joueur.hpp"
#include "Taille.hpp"
#include "Position.hpp"
#include <array>



using namespace std;
using namespace sf;

/*
 * Ce morceau de code pour permet de tirer un nombre flottant au hasard
 * entre 0.0 et 1.0. Pour cela, il suffit de l'appeler de la manière
 * suivante:
 *
 *     float r = Math::random();
 */
struct Math {
  static float random() {
    static mt19937 engine(time(nullptr));
    uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(engine);
  }
};

/*
 * La structure Rectangle contient des champs pour manipuler un rectangle
 * avec sa position (x,y) qui correspond au coin en haut à gauche, sa
 * taille width x height, et sa couleur.
 *
 * Le type Color correspond à une structure de SFML:
 * http://www.sfml-dev.org/documentation/2.1/classsf_1_1Color.php
 *
 * Il existe des constantes pour les principales couleurs.
 *
 * Pour pouvoir dessiner des formes, vous pouvez lire le tutoriel suivant:
 * http://www.sfml-dev.org/tutorials/2.1/graphics-shape.php
 */

// struct Position{
//   double x;
//   double y;
// };
// struct CaseDisponible{
//   bool haut;
//   bool bas;
//   bool gauche;
//   bool droite;
// };

// struct Taille{
//   double width;
//   double height;
// };

// struct Case{
//   Position position;
//   Taille taille;
//   Texture texture;
//   Texture textureS;
//   CaseDisponible caseD;
// };



// struct Mur{
//   Position position;
//   Taille taille;
//   Texture texture;
//   int angle; 
// };
double modulo(double dividend, double divisor) {
    return dividend - std::floor(dividend / divisor) * divisor;
}

bool Collision(Joueur j, Mur mur){
    return !(j.getPosition().getX() + j.getTaille().getWidth() <= mur.getPosition().getX() ||
           j.getPosition().getX() >= mur.getPosition().getX() + mur.getTaille().getWidth() ||
           j.getPosition().getY() + j.getTaille().getHeight() <= mur.getPosition().getY() ||
           j.getPosition().getY() >= mur.getPosition().getY() + mur.getTaille().getHeight());
    
};
// void changerTexture (Case tab[9][9], Texture texture1, Texture texture2,Texture texture3){
//   for (int i=0; i<9; i++){
//     for (int j=0;j<9;j++){
//       tab[i][j].texture = (i == 0) ? texture3 : (i == 8) ? texture2 : texture1;
//     }
//   }
// }
bool EstLibreADroite(Joueur j, Mur murs[20], int tailleCase) {
    // Calculez les coordonnées de la case à droite du joueur
    int xDroite = j.getPosition().getX() + j.getTaille().getWidth()+12.5;
    int yDroite = j.getPosition().getY();
  cout<<"joueur ("+to_string(xDroite)+","+to_string(yDroite)+")";
    // Vérifiez s'il y a un mur à la position calculée
    for (int i = 0; i < 20; ++i) {

            // Si le mur est à droite du joueur
            if (murs[i].getPosition().getX() <= xDroite &&
                murs[i].getPosition().getX() >= (xDroite-12.5)&&
                murs[i].getPosition().getY() <= yDroite &&
                murs[i].getPosition().getY()- murs[i].getTaille().getHeight()/2 <= yDroite){
                return false; // Il y a un mur à droite
            }
        }
    
    return true; // Aucun mur à droite
}
bool CollisionTab(Joueur j, Mur murs[20]){
  for (int i = 0; i < 5; ++i) {
    if (Collision(j, murs[i])) {
      return true;
    }
  }
  return false;
}
int main() {
  Music music;
  if (!music.openFromFile("cave.mp3"))
    return -1; // error
  music.play();
  music.setLoop(true);
  const int WIDTH=1440;
  const int HEIGHT = 800;
  const int TAILLE_CASE = 75;
  const double ESPACE = 12.5;
  const int TAILLE_TAB =20;
  bool seDeplace = false;
  bool tourJoueur1 = true;
  Mur murs[TAILLE_TAB];

  Joueur joueur1(WIDTH/2 - TAILLE_CASE/2, HEIGHT - TAILLE_CASE - ESPACE);
  Joueur joueur2(WIDTH/2 - TAILLE_CASE/2,ESPACE);
  Texture pion1;
  pion1.setSmooth(true);
  if (!pion1.loadFromFile("Textures/pion2.png")){

  }
  Texture pion2;
  if (!pion2.loadFromFile("Textures/pion.png")){

  }
  pion2.setSmooth(true);
  Texture plateau;
  if (!plateau.loadFromFile("Textures/plateau.jpg")){

  }
  Texture textureMur;
  if(!textureMur.loadFromFile("Textures/mur.png")){
  }
  Texture textureMurVertical;
  if(!textureMurVertical.loadFromFile("Textures/mur_vertical.png")){
  }
  Texture caseDispo;
  if(!caseDispo.loadFromFile("Textures/caseDispo.png")){
  }
  caseDispo.setSmooth(true);
  Font font;
    if (!font.loadFromFile("./Fonts/SyneMono-Regular.ttf"))
    {
        // error...
    }
  Text text;
  text.setFont(font);
  
  text.setCharacterSize(20);
  text.setFillColor(sf::Color::Black);
  text.setStyle(Text::Bold);
  text.setPosition(Vector2f(ESPACE, HEIGHT/2 ));

  /*
   * Une RenderWindow est une fenêtre qui permet de récupérer des événements
   * d'entrée (comme le clavier et la souris) et d'afficher des entités.
   *
   * La documentation se trouve ici:
   * http://www.sfml-dev.org/documentation/2.1/classsf_1_1RenderWindow.php
   */
  RenderWindow window(VideoMode(WIDTH, HEIGHT), "Quoridor");
  sf::RectangleShape previewWall; // Aperçu du mur
  previewWall.setFillColor(sf::Color(200, 200, 200, 100));
  int mouseX;
  int mouseY;
  double murHauteur;
  double murLargeur;
  const sf::Time frameTime = sf::seconds(1.f / 120.f);
  /*
   * Une Clock permet de compter le temps. Vous en aurez besoin pour savoir
   * le temps entre deux frames.
   */
  sf::Clock frameClock;
  sf::Clock timer;

  /*
   * La boucle de jeu principale. La condition de fin est la fermeture de la
   * fenêtre qu'on provoque à l'aide d'un appel `window.close()`.
   */
  while (window.isOpen()) {
    sf::Time elapsedTime = frameClock.restart();
    

    
    /*
     * Un Event est un événement d'entrée. Il contient toutes les informations
     * nécessaires pour traiter tous les événements.
     *
     * Vous pouvez lire ce tutoriel pour comprendre comment récupérer les
     * informations relatives à chaque événement:
     * http://www.sfml-dev.org/tutorials/2.1/window-events.php
     */
    Event event;
    bool gauche = false;
    bool droite = false;
    bool haut = false;
    bool bas = false;
    /*
     * Cette boucle permet de traiter tous les événements en attente.
     */
    while (window.pollEvent(event)) {
      double mousex;
      double mousey;
      /*
       * L'événement Event::Closed est reçu quand on clique sur la croix
       * dans la barre de la fenêtre. À ce moment là, il faut fermer la
       * fenêtre explicitement.
       */
      if (event.type == Event::Closed) {
        window.close();
      }

      /*
       * Les événements dont vous aurez besoin sont Event::KeyPressed,
       * Event::KeyReleased et Event::MouseButtonPressed.
       */

      if (event.type == Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Left){
          gauche=true;
        }else if (event.key.code == sf::Keyboard::Right){
          droite=true;
        }else if (event.key.code == sf::Keyboard::Down){
          haut=true;
        }else if (event.key.code == sf::Keyboard::Up){
          bas=true;
        }
      }

      if (event.type == Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Left){
          gauche=false;
        }else if (event.key.code == sf::Keyboard::Right){
          droite=false;
        }else if (event.key.code == sf::Keyboard::Down){
          haut=false;
        }else if (event.key.code == sf::Keyboard::Up){
          bas=false;
        }
      }
      if (event.type == Event::MouseMoved){
        murLargeur = ESPACE;
        murHauteur = 2*(TAILLE_CASE+ESPACE);
        double mouseX = event.mouseMove.x-murHauteur/2.0f;
        double mouseY = event.mouseMove.y - murLargeur/2.0f;
        if(mouseX>320 && mouseX<1100-TAILLE_CASE && mouseY > 0 && mouseY < HEIGHT - ESPACE )
        {
          mousey = mouseY + (TAILLE_CASE+ESPACE/2-modulo(mouseY-ESPACE,TAILLE_CASE+ESPACE));
          mousex = mouseX + (TAILLE_CASE+ESPACE+ESPACE/2-modulo(mouseX-((WIDTH-HEIGHT)/2+TAILLE_CASE+ESPACE),TAILLE_CASE+ESPACE));
          if ((modulo(mouseY-ESPACE,TAILLE_CASE+ESPACE)>TAILLE_CASE) && (modulo(mouseX-((WIDTH-HEIGHT)/2+TAILLE_CASE+ESPACE),TAILLE_CASE+ESPACE)<TAILLE_CASE)){
            previewWall.setRotation(0);
            previewWall.setOrigin(TAILLE_CASE+ESPACE,ESPACE/2);
          }else{
            int tmp = murLargeur;
            murLargeur=murHauteur;
            murHauteur=tmp;
            previewWall.setOrigin(ESPACE/2,TAILLE_CASE+ESPACE);
          }
          previewWall.setPosition(mousex,mousey);
          previewWall.setSize(Vector2f(murHauteur,murLargeur));
        }
        
      }
      if (event.type == sf::Event::MouseButtonPressed) {
        Mur mur(mousex, mousey,0);
        // if ((modulo(mouseY-ESPACE,TAILLE_CASE+ESPACE)>TAILLE_CASE+ESPACE/2) && (modulo(mouseX-((WIDTH-HEIGHT)/2+TAILLE_CASE+ESPACE),TAILLE_CASE+ESPACE)<TAILLE_CASE)){
        //   Taille t(ESPACE,(TAILLE_CASE+ESPACE)*2);
        //   mur.setTaille(t);
        // }else{
        //   Taille t((TAILLE_CASE+ESPACE)*2,ESPACE);
        //   mur.setTaille(t);
        // }
        Taille t(murHauteur,murLargeur);
        mur.setTaille(t);
        cout<<"("+to_string(mur.getPosition().getX())+","+to_string(mur.getPosition().getY())+")"<<endl;
        bool ajoute = false;
        int i=0;
        do{
          if (murs[i].getPosition().getX()==0 && murs[i].getPosition().getY()==0){
            murs[i]= mur;
            ajoute=true;
          }else{
            i++;
          };
        }while (ajoute==false && i<TAILLE_TAB);
        if (tourJoueur1){
          joueur1.retirerMurs();
          tourJoueur1=false;
        }else{
          joueur1.retirerMurs();
          tourJoueur1=true;
        }
          // if (sf::Mouse::getPosition(window).x >= joueur1.getPosition().x && sf::Mouse::getPosition(window).y >= joueur1.getPosition().y && sf::Mouse::getPosition(window).x <= joueur1.getPosition().x+TAILLE_CASE && sf::Mouse::getPosition(window).y <= joueur1.getPosition().y+TAILLE_CASE){
          //   cout<<"Se déplace"<<endl;
          //   if (seDeplace){
          //     seDeplace = false;
          //   }else{
          //     seDeplace = true;
          //   }
          // }
            
            
          
        
      };
            
        

    /*
     * Mise à jour de l'état du jeu.
     * On calcule le temps (en secondes) depuis la dernière frame qu'on place
     * dans la variable `dt`. Ensuite, il faut compléter suivant ce qui est
     * demandé.
     */
    float dt = elapsedTime.asSeconds();
if (tourJoueur1){
    text.setString("C'est le tour du joueur 1");
  }else{
    text.setString("C'est le tour du joueur 2");
}
sf::RectangleShape entite(sf::Vector2f(TAILLE_CASE, TAILLE_CASE));
if (tourJoueur1){
  if (EstLibreADroite(joueur1,murs,TAILLE_CASE)){
    entite.setPosition(joueur1.getPosition().getX() + joueur1.getTaille().getWidth()+ESPACE, joueur1.getPosition().getY());
    entite.setTexture(&caseDispo); // Choisissez la couleur souhaitée
    // entite.setFillColor(Color::Red);
  }
}
  if (tourJoueur1){
    if (gauche){
      joueur1.Gauche();
      tourJoueur1 = false;
    }else if (droite){
      if(EstLibreADroite(joueur1,murs,TAILLE_CASE)){
        joueur1.Droite();
        tourJoueur1 = false;
      }
    }else if (haut){
      joueur1.Haut();
      tourJoueur1 = false;
    }else if (bas){
      joueur1.Bas();
      tourJoueur1 = false;
    }
  
  }else{
    if (gauche){
    joueur2.Gauche();
    tourJoueur1 = true;
  }else if (droite){
    joueur2.Droite();
    tourJoueur1 = true;
  }else if (haut){
    joueur2.Haut();
    tourJoueur1 = true;
  }else if (bas){
    joueur2.Bas();
    tourJoueur1 = true;
  }
  }

    /*
     * Affichage de l'état du jeu.
     * À chaque tour de boucle, on efface tout grâce à `clear` (qui prend
     * en paramètre la couleur de fond), puis on dessine tous les éléments,
     * puis on affiche la nouvelle image grâce à `display`.
     */
    

    window.clear(Color::White);
    RectangleShape rect;
    rect.setSize(Vector2f(HEIGHT,HEIGHT));
    rect.setPosition(Vector2f(WIDTH/2 - HEIGHT/2,0));
    rect.setTexture(&plateau);
    window.draw(rect);
    // if (seDeplace){
    //           if (joueur1.caseActuelle.caseD.haut){
    //               joueur1.position.y -= TAILLE_CASE+ESPACE;
    //               seDeplace = false;
    //           }else if (joueur1.caseActuelle.caseD.bas){
    //               joueur1.position.y += TAILLE_CASE+ESPACE;
    //               seDeplace = false;
    //           }else if (joueur1.caseActuelle.caseD.droite){
    //               joueur1.position.x += TAILLE_CASE+ESPACE;
    //               seDeplace = false;
    //           }else {
    //               joueur1.position.x -= TAILLE_CASE+ESPACE;
    //               seDeplace = false;
    //           }
    //         }
    
    for (int i = 0; i<TAILLE_TAB; i++){
      RectangleShape mur;
      if (murs[i].getTaille().getHeight()>0 && murs[i].getTaille().getWidth()>0){
        if (murs[i].getTaille().getHeight()>murs[i].getTaille().getWidth()){
          mur.setOrigin(TAILLE_CASE+ESPACE,ESPACE/2);
          mur.setTexture(&textureMur);
        }else{
          mur.setOrigin(ESPACE/2,TAILLE_CASE+ESPACE);
          mur.setTexture(&textureMurVertical);
        }
        mur.setSize(Vector2f(murs[i].getTaille().getHeight(),murs[i].getTaille().getWidth()));
        mur.setPosition(Vector2f(murs[i].getPosition().getX(),murs[i].getPosition().getY()));
        window.draw(mur);
      }
    }
    RectangleShape pion;
    RectangleShape pionj2;
    pion.setSize(Vector2f(joueur1.getTaille().getWidth(),joueur1.getTaille().getHeight()));
    pion.setPosition(Vector2f(joueur1.getPosition().getX(),joueur1.getPosition().getY()));
    pionj2.setSize(Vector2f(joueur2.getTaille().getWidth(),joueur2.getTaille().getHeight()));
    pionj2.setPosition(Vector2f(joueur2.getPosition().getX(),joueur2.getPosition().getY()));
    pion.setTexture(&pion1);
    window.draw(pion);
    pionj2.setTexture(&pion2);
    window.draw(pionj2);
    window.draw(text);
    window.draw(previewWall);
    window.draw(entite);
    window.display();
    sf::sleep(frameTime - elapsedTime);

  }
  }
  return 0;
}

