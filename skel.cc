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
#include "Button.hpp"



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

double modulo(double dividend, double divisor) {
    return dividend - std::floor(dividend / divisor) * divisor;
}

bool MursDispo(Mur mur, Mur murs[20]){
  // Déterminez les limites du mur que vous voulez placer
  double posXDroite = mur.getPosition().getX() + mur.getTaille().getWidth()/2-12.5;
  double posXGauche = mur.getPosition().getX() - mur.getTaille().getWidth()/2+12.5;
  double posYHaut = mur.getPosition().getY() + mur.getTaille().getHeight()/2-12.5;
  double posYBas = mur.getPosition().getY() - mur.getTaille().getHeight()/2+12.5;

  for (int i = 0; i < 20; ++i){
    // Déterminez les limites des murs déjà placés
    double posXDroiteExistant = murs[i].getPosition().getX() + murs[i].getTaille().getWidth()/2;
    double posXGaucheExistant = murs[i].getPosition().getX() - murs[i].getTaille().getWidth()/2;
    double posYHautExistant = murs[i].getPosition().getY() + murs[i].getTaille().getHeight()/2;
    double posYBasExistant = murs[i].getPosition().getY() - murs[i].getTaille().getHeight()/2;

    // Vérifiez si les limites se chevauchent
    if (!(posXDroite < posXGaucheExistant || posXGauche > posXDroiteExistant || posYHaut < posYBasExistant || posYBas > posYHautExistant)){
      return false; // Les limites se chevauchent et les murs sont de la même orientation, donc l'emplacement n'est pas disponible
    }
  }
  return true; // Aucun chevauchement trouvé avec un mur de la même orientation, l'emplacement est disponible
}



bool EstLibreADroite(Joueur j, Mur murs[20], int tailleCase) {
    // Calculez les coordonnées de la case à droite du joueur
    float xDroite = j.getPosition().getX() + j.getTaille().getWidth();
    float yDroite = j.getPosition().getY();
    if (xDroite-tailleCase>=1032.5){
      return false;
    }
    // Vérifiez s'il y a un mur à la position calculée
    for (int i = 0; i < 20; ++i) {
            // Si le mur est à droite du joueur
            if (murs[i].getPosition().getX()-murs[i].getTaille().getWidth()/2 == xDroite&&
                murs[i].getPosition().getX() + murs[i].getTaille().getWidth()/2 <= xDroite+12.5&&
                murs[i].getPosition().getY() - murs[i].getTaille().getHeight()/2 <= yDroite &&
                murs[i].getPosition().getY() + murs[i].getTaille().getHeight()/2 > yDroite){
                return false; // Il y a un mur à droite
            }
        }
    return true; // Aucun mur à droite
}
bool EstLibreAGauche(Joueur j, Mur murs[20], int tailleCase) {
// Calculez les coordonnées de la case à gauche du joueur
	float xGauche = j.getPosition().getX();
	float yGauche = j.getPosition().getY();
  if (xGauche==332.5){
    return false;
  }
  // Vérifiez s'il y a un mur à la position calculée
	for (int i = 0; i < 20; ++i) {
		if (murs[i].getPosition().getX()+ (murs[i].getTaille().getWidth())/2== xGauche &&
			murs[i].getPosition().getX()- (murs[i].getTaille().getWidth())/2 >= (xGauche-12.5)&&
			murs[i].getPosition().getY() - murs[i].getTaille().getHeight()/2 <= yGauche &&
      murs[i].getPosition().getY() + murs[i].getTaille().getHeight()/2 > yGauche){
			return false; // Il y a un mur à gauche
    }
  }
  return true;
}

bool EstLibreEnHaut(Joueur j, Mur murs[20], int tailleCase) {
// Calculez les coordonnées de la case en haut du joueur
	float xHaut = j.getPosition().getX() ;
	float yHaut = j.getPosition().getY();

  if (yHaut<=12.5){
    return false;
  }
  
  // Vérifiez s'il y a un mur à la position calculée
	for (int i = 0; i < 20; ++i) {
    float yMur = murs[i].getPosition().getY() + (murs[i].getTaille().getHeight()) / 2;

    if (murs[i].getPosition().getX()- murs[i].getTaille().getWidth()/2 <= xHaut &&
		murs[i].getPosition().getX() + murs[i].getTaille().getWidth()/2 >= xHaut &&
		yMur == yHaut &&
		murs[i].getPosition().getY()>= (yHaut-12.5))
		{
		return false; // Il y a un mur en haut
    }
  }
return true; // Aucun mur en haut
}

bool EstLibreEnBas(Joueur j, Mur murs[20], int tailleCase) {
// Calculez les coordonnées de la case en haut du joueur
	float xHaut = j.getPosition().getX() ;
	float yHaut = j.getPosition().getY()+j.getTaille().getHeight();

  if (yHaut>=712.5){
    return false;
  }
  // Vérifiez s'il y a un mur à la position calculée
	for (int i = 0; i < 20; ++i) {
		if (murs[i].getPosition().getX()- murs[i].getTaille().getWidth()/2 <= xHaut &&
		murs[i].getPosition().getX() + murs[i].getTaille().getWidth()/2 >= xHaut &&
		murs[i].getPosition().getY()-(murs[i].getTaille().getHeight())/2 == yHaut &&
		murs[i].getPosition().getY()<= (yHaut+12.5))
		{
		return false; // Il y a un mur en haut
    }
  }
return true; // Aucun mur en haut
}

#include <queue>

bool cheminLibre(int TAILLE_CASE, double ESPACE, int WIDTH, int HEIGHT, Mur murs[20],Mur nvMur, Joueur joueur1) {
  // Créez une copie de la liste des murs et ajoutez le nouveau mur
  Mur copieMurs[20];
  double id = joueur1.getId();
  for(int i=0;i<20;i++){
      copieMurs[i]=murs[i];
  }
  bool ajoute = false;
  int i=0;
  do{
    if (copieMurs[i].getPosition().getX()==0 && copieMurs[i].getPosition().getY()==0){
      copieMurs[i]= nvMur;
      ajoute=true;
    }else{
      i++;
    };
  }while (ajoute==false && i<20);

  std::queue<Position> q;
  std::map<Position, bool> visited;

  // Ajoutez la position initiale du joueur à la file d'attente
  Position startPos = Position(joueur1.getPosition().getX(), joueur1.getPosition().getY());
  q.push(startPos);
  visited[startPos] = true;

  while (!q.empty()) {
    Position pos = q.front();
    q.pop();

    // Vérifiez la condition de victoire ou de sortie ici
    if (pos.getY()<25 && id==1){
      return true;
    }else if(pos.getY()>700 && id==2){
      return true;
    }

    // Ajoutez toutes les positions non visitées et accessibles à la file d'attente
    Position newPos;
    Joueur j = joueur1;
    j.setPosition(pos);
    if (EstLibreEnHaut(j, copieMurs, TAILLE_CASE) && !visited[Position(pos.getX(), pos.getY() - (TAILLE_CASE + ESPACE))]) {
        newPos = Position(pos.getX(), pos.getY() - (TAILLE_CASE + ESPACE));
        q.push(newPos);
        visited[newPos] = true; // Marquez la position comme visitée ici
    }
    if (EstLibreADroite(j, copieMurs, TAILLE_CASE) && !visited[Position(pos.getX() + (TAILLE_CASE + ESPACE), pos.getY())]) {
        newPos = Position(pos.getX() + (TAILLE_CASE + ESPACE), pos.getY());
        q.push(newPos);
        visited[newPos] = true; // Marquez la position comme visitée ici
    }
    if (EstLibreAGauche(j, copieMurs, TAILLE_CASE) && !visited[Position(pos.getX() - (TAILLE_CASE + ESPACE), pos.getY())]) {
        newPos = Position(pos.getX() - (TAILLE_CASE + ESPACE), pos.getY());
        q.push(newPos);
        visited[newPos] = true; // Marquez la position comme visitée ici
    }
    if (EstLibreEnBas(j, copieMurs, TAILLE_CASE) && !visited[Position(pos.getX(), pos.getY() + (TAILLE_CASE + ESPACE))]) {
        newPos = Position(pos.getX(), pos.getY() + (TAILLE_CASE + ESPACE));
        q.push(newPos);
        visited[newPos] = true; // Marquez la position comme visitée ici
    }
  }

  // Si nous avons épuisé toutes les positions possibles sans atteindre la ligne opposée, retournez faux
  return false;
}


bool JoueurADroite(Joueur j, Joueur joueur2, int tailleCase) {
    // Calculez les coordonnées de la case à droite du joueur
    float xDroite = j.getPosition().getX() + j.getTaille().getWidth()+12.5;
    float yDroite = j.getPosition().getY();
    if (xDroite>=1045){
      return false;
    }
    if (xDroite==joueur2.getPosition().getX() && xDroite && j.getPosition().getY()==joueur2.getPosition().getY()){
        return true; // Il y a un mur à droite
    }
    
    return false; // Aucun mur à droite
}
bool JoueurAGauche(Joueur j,Joueur joueur2, int tailleCase) {
// Calculez les coordonnées de la case à gauche du joueur
	float xGauche = j.getPosition().getX()-tailleCase-12.5;
	float yGauche = j.getPosition().getY();
  if (xGauche<=332.5){
    return false;
  }
  // Vérifiez s'il y a un mur à la position calculée
		if (xGauche==joueur2.getPosition().getX() && j.getPosition().getY()==joueur2.getPosition().getY()){
			return true; // Il y a un mur à gauche
    }
  return false;
}

bool JoueurEnHaut(Joueur j, Joueur joueur2, int tailleCase) {
// Calculez les coordonnées de la case en haut du joueur
	float xHaut = j.getPosition().getX();
	float yHaut = j.getPosition().getY()-j.getTaille().getHeight()-12.5;
    if (j.getPosition().getX()==joueur2.getPosition().getX() && yHaut==joueur2.getPosition().getY())
		{
		return true; // Il y a un mur en haut
    }
return false; // Aucun mur en haut
}

bool JoueurEnBas(Joueur j, Joueur joueur2, int tailleCase) {
// Calculez les coordonnées de la case en haut du joueur
	float xHaut = j.getPosition().getX() ;
	float yHaut = j.getPosition().getY()+j.getTaille().getHeight()+12.5;
  // Vérifiez s'il y a un mur à la position calculée
		if (j.getPosition().getX()==joueur2.getPosition().getX() && yHaut==joueur2.getPosition().getY())
		{
		return true; // Il y a un mur en haut
    }
return false; // Aucun mur en haut
}

bool Victoire(Joueur j1, Joueur j2){
  if (j1.getPosition().getY()==12.5){
    return true;
  }else if (j2.getPosition().getY()==712.5){
    return true;
  }else{
    return false;
  }
}

int main() {
  bool boolSon = true;
  Music music;
  if (!music.openFromFile("cave.mp3")){
    return -1;
  }
  music.play();
  music.setLoop(true);
  const double WIDTH=1440;
  const double HEIGHT = 800;
  const double TAILLE_CASE = 75;
  const double ESPACE = 12.5;
  const int TAILLE_TAB =20;
  bool seDeplace = false;
  bool tourJoueur1 = true;
  bool affichageMur=true;
  bool enJeu=false;
  Mur murs[TAILLE_TAB];

  Joueur joueur1(WIDTH/2 - TAILLE_CASE/2, HEIGHT - TAILLE_CASE - ESPACE,1);
  Joueur joueur2(WIDTH/2 - TAILLE_CASE/2,ESPACE,2);

  Texture pion1;
  pion1.setSmooth(true);
  if (!pion1.loadFromFile("Textures/pion2.png")){

  }
  Texture pion2;
  if (!pion2.loadFromFile("Textures/pion.png")){

  }
  pion2.setSmooth(true);
  Texture plateau;
  if (!plateau.loadFromFile("Textures/plateau.png")){

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
  Texture apercuTexture;
  if(!apercuTexture.loadFromFile("Textures/apercu.png")){
  }
  caseDispo.setSmooth(true);
  Font font;
  if (!font.loadFromFile("./Fonts/Freeman-Regular.ttf"))
    {
    }
  textureMur.setSmooth(true);
  textureMurVertical.setSmooth(true);
  Texture backgrounds;
  if(!backgrounds.loadFromFile("Textures/Background/background.jpg")){
  }
  Texture background_home;
  if(!background_home.loadFromFile("Textures/Background/background_home.jpg")){
  }

  Font title;
  if (!title.loadFromFile("./Fonts/LibreBaskerville-Regular.ttf"))
    {
    }
  Text tour2;
  tour2.setFont(font);
  tour2.setCharacterSize(20);
  tour2.setFillColor(sf::Color(255, 255, 255, 122));
  tour2.setString("C'est le tour du joueur adverse !");
  Text tour;
  tour.setFont(font);
  tour.setCharacterSize(20);
  tour.setFillColor(sf::Color(255, 255, 255, 122));
  tour.setString("C'est votre tour !");
  Text j1Text;
  j1Text.setFont(font);
  j1Text.setStyle(sf::Text::Bold);
  j1Text.setCharacterSize(30);
  j1Text.setFillColor(sf::Color::White);
  j1Text.setString("Joueur 1");
  Text j2Text;
  j2Text.setFont(font);
  j2Text.setStyle(sf::Text::Bold);
  j2Text.setCharacterSize(30);
  j2Text.setFillColor(sf::Color::White);
  j2Text.setString("Joueur 2");
  Text j1Murs;
  j1Murs.setFont(font);
  j1Murs.setCharacterSize(20);
  j1Murs.setFillColor(sf::Color(255, 255, 255, 122));
  Text j2Murs;
  j2Murs.setFont(font);
  j2Murs.setCharacterSize(20);
  j2Murs.setFillColor(sf::Color(255, 255, 255, 122));
  Text Title;
  Title.setFont(title);
  Title.setCharacterSize(120);
  Title.setStyle(sf::Text::Bold);
  Title.setFillColor(sf::Color(0, 0, 0, 122));
  Title.setString("QUORIDOR");
  Title.setPosition(Vector2f((WIDTH/2-Title.getGlobalBounds().width/2),HEIGHT/4-Title.getGlobalBounds().height/2));

  
  Button button(WIDTH/2, HEIGHT/2, 300, 200, font, "Jouer");
  Button soundButton(WIDTH/2, HEIGHT/2 + 150, 100, 100, font, "");
  
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
  Mur mur;
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
        if (boolSon){
          music.setVolume(100);
        }else{
          music.setVolume(0);
        }
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
        if(event.key.code == sf::Keyboard::Escape){
          seDeplace=false;
        }
      }

      if (event.type == Event::KeyReleased) {
      }
      if (event.type == Event::MouseMoved){
        double mouseX = event.mouseMove.x;
        double mouseY = event.mouseMove.y;
        if(enJeu){
          if (mouseX<320 || mouseX>1120){
            affichageMur=false;
          }else{
            if (!seDeplace){
              if (tourJoueur1){
                if (mouseX>=joueur1.getPosition().getX() && mouseX<=joueur1.getPosition().getX()+TAILLE_CASE && mouseY>=joueur1.getPosition().getY()&& mouseY<=joueur1.getPosition().getY()+TAILLE_CASE){
                  affichageMur =false;
                }else {
                  affichageMur = true;
                }
              }
              else{
                if (mouseX>=joueur2.getPosition().getX() && mouseX<=joueur2.getPosition().getX()+TAILLE_CASE && mouseY>=joueur2.getPosition().getY()&& mouseY<=joueur2.getPosition().getY()+TAILLE_CASE){
                  affichageMur =false;
                }else {
                  affichageMur = true;
                }
              }
            }
          }
        }
          if(affichageMur == true && !seDeplace){
            murLargeur = 2*(TAILLE_CASE+ESPACE);
            murHauteur = ESPACE;
            double mouseX = event.mouseMove.x-murHauteur/2.0f;
            double mouseY = event.mouseMove.y - murLargeur/2.0f;
            if(mouseX>320 && mouseX<1120-TAILLE_CASE-2*(ESPACE) && mouseY > 0 && mouseY < HEIGHT-TAILLE_CASE-2*(ESPACE) )
            {
              mousey = mouseY + (TAILLE_CASE+ESPACE+ESPACE/2-modulo(mouseY,TAILLE_CASE+ESPACE));
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
              previewWall.setTexture(&textureMur);
              previewWall.setPosition(mousex,mousey);
              previewWall.setSize(Vector2f(murLargeur,murHauteur));
              Taille t(murHauteur,murLargeur);
              mur.setTaille(t);
              Position pos(mousex,mousey);
              mur.setPosition(pos);
            }
          }
        }
        if (event.type == sf::Event::MouseButtonPressed) {
          double mouseX = event.mouseButton.x;
          double mouseY = event.mouseButton.y;
          if(enJeu){
          if (tourJoueur1){
            if (mouseX>=joueur1.getPosition().getX() && mouseX<=joueur1.getPosition().getX()+TAILLE_CASE && mouseY>=joueur1.getPosition().getY()&& mouseY<=joueur1.getPosition().getY()+TAILLE_CASE){
              affichageMur =false;
              seDeplace =true;
            }else {
              affichageMur = true;
              
            }
          }
          else{
            if (mouseX>=joueur2.getPosition().getX() && mouseX<=joueur2.getPosition().getX()+TAILLE_CASE && mouseY>=joueur2.getPosition().getY()&& mouseY<=joueur2.getPosition().getY()+TAILLE_CASE){
              affichageMur =false;
              seDeplace= true;
            }else {
              affichageMur = true;
              
            }
          }
          if (seDeplace){
            if (tourJoueur1){
              if (EstLibreEnHaut(joueur1,murs,TAILLE_CASE)&& JoueurEnHaut(joueur1,joueur2,TAILLE_CASE)&& !EstLibreEnHaut(joueur2,murs,TAILLE_CASE)){
                if (mouseX >= joueur1.getPosition().getX()+TAILLE_CASE+ESPACE && mouseX <= joueur1.getPosition().getX()+TAILLE_CASE*2+ESPACE && mouseY >= joueur2.getPosition().getY()&&mouseY <= joueur2.getPosition().getY()+(TAILLE_CASE)){
                  joueur1.Droite();
                  joueur1.Haut();
                  tourJoueur1=false;
                  seDeplace=false;
                }else if (mouseX >= joueur1.getPosition().getX()-(TAILLE_CASE+ESPACE) && mouseX <= joueur1.getPosition().getX()-ESPACE && mouseY >= joueur2.getPosition().getY()&&mouseY <= joueur2.getPosition().getY()+(TAILLE_CASE)){
                  joueur1.Gauche();
                  joueur1.Haut();
                  tourJoueur1=false;
                  seDeplace=false;
                }
              }else{
                if(EstLibreADroite(joueur1,murs,TAILLE_CASE)){
                  if (JoueurADroite(joueur1,joueur2,TAILLE_CASE)){
                    if (mouseX>=joueur1.getPosition().getX()+(TAILLE_CASE+ESPACE)*2 && mouseX<=joueur1.getPosition().getX()+TAILLE_CASE+(TAILLE_CASE+ESPACE)*2 && mouseY>=joueur1.getPosition().getY()&& mouseY<=joueur1.getPosition().getY()+TAILLE_CASE){
                      joueur1.Droite();
                      joueur1.Droite();
                      tourJoueur1=false;
                      seDeplace=false;
                    }

                  }else{
                    if (mouseX>=joueur1.getPosition().getX()+(TAILLE_CASE+ESPACE) && mouseX<=joueur1.getPosition().getX()+TAILLE_CASE+(TAILLE_CASE+ESPACE) && mouseY>=joueur1.getPosition().getY()&& mouseY<=joueur1.getPosition().getY()+TAILLE_CASE){
                    joueur1.Droite();
                    tourJoueur1=false;
                    seDeplace=false;
                    }
                  }
                }
                if (EstLibreAGauche(joueur1,murs,TAILLE_CASE)){
                  if (JoueurAGauche(joueur1,joueur2,TAILLE_CASE)){
                    if (mouseX>=joueur1.getPosition().getX()-TAILLE_CASE*2-ESPACE*2 && mouseX<=joueur1.getPosition().getX()-ESPACE && mouseY>=joueur1.getPosition().getY()&& mouseY<=joueur1.getPosition().getY()+TAILLE_CASE){
                      joueur1.Gauche();
                      joueur1.Gauche();
                      tourJoueur1=false;
                      seDeplace=false;
                    }
                  }else{
                    if (mouseX>=joueur1.getPosition().getX()-(TAILLE_CASE+ESPACE) && mouseX<=joueur1.getPosition().getX()+ESPACE && mouseY>=joueur1.getPosition().getY()&& mouseY<=joueur1.getPosition().getY()+TAILLE_CASE) {
                      joueur1.Gauche();
                      tourJoueur1=false;
                      seDeplace=false;
                    }
                  }
                }
                if (EstLibreEnBas(joueur1,murs,TAILLE_CASE)){
                  if (JoueurEnBas(joueur1,joueur2,TAILLE_CASE)){
                    if (mouseX>=joueur1.getPosition().getX() && mouseX<=joueur1.getPosition().getX()+TAILLE_CASE && mouseY>=joueur1.getPosition().getY()+(TAILLE_CASE+ESPACE)*2&& mouseY<=joueur1.getPosition().getY()+TAILLE_CASE+(TAILLE_CASE+ESPACE)*2){
                      joueur1.Bas();
                      joueur1.Bas();
                      tourJoueur1=false;
                      seDeplace=false;
                    }
                  }else{
                    if (mouseX>=joueur1.getPosition().getX() && mouseX<=joueur1.getPosition().getX()+TAILLE_CASE && mouseY>=joueur1.getPosition().getY()+(TAILLE_CASE+ESPACE)&& mouseY<=joueur1.getPosition().getY()+TAILLE_CASE+(TAILLE_CASE+ESPACE)){
                        joueur1.Bas();
                        tourJoueur1=false;
                        seDeplace=false;
                    }
                  }
                }
                if (EstLibreEnHaut(joueur1,murs,TAILLE_CASE)){
                  if (JoueurEnHaut(joueur1,joueur2,TAILLE_CASE)){
                    if (mouseX>=joueur1.getPosition().getX() && mouseX<=joueur1.getPosition().getX()+TAILLE_CASE && mouseY>=joueur1.getPosition().getY()-(TAILLE_CASE+ESPACE)*2&& mouseY<=joueur1.getPosition().getY()-ESPACE -(TAILLE_CASE+ESPACE)){
                      joueur1.Haut();
                      joueur1.Haut();
                      tourJoueur1=false;
                      seDeplace=false;
                    }
                  }else{
                    if (mouseX>=joueur1.getPosition().getX() && mouseX<=joueur1.getPosition().getX()+TAILLE_CASE && mouseY>=joueur1.getPosition().getY()-(TAILLE_CASE+ESPACE)&& mouseY<=joueur1.getPosition().getY()-ESPACE){
                        joueur1.Haut();
                        tourJoueur1=false;
                        seDeplace=false;
                      }
                    }
                }
              }
            }else{
              if (!(EstLibreEnBas(joueur2,murs,TAILLE_CASE) && JoueurEnBas(joueur2,joueur1,TAILLE_CASE)&& !EstLibreEnBas(joueur1,murs,TAILLE_CASE))){
                if(EstLibreADroite(joueur2,murs,TAILLE_CASE)){
                  if (JoueurADroite(joueur2,joueur1,TAILLE_CASE)){
                    if (mouseX>=joueur2.getPosition().getX()+(TAILLE_CASE+ESPACE)*2 && mouseX<=joueur2.getPosition().getX()+TAILLE_CASE+(TAILLE_CASE+ESPACE)*2 && mouseY>=joueur2.getPosition().getY()&& mouseY<=joueur2.getPosition().getY()+TAILLE_CASE){
                      joueur2.Droite();
                      joueur2.Droite();
                      tourJoueur1=true;
                      seDeplace=false;
                    }

                  }else{
                    if (mouseX>=joueur2.getPosition().getX()+(TAILLE_CASE+ESPACE) && mouseX<=joueur2.getPosition().getX()+TAILLE_CASE+(TAILLE_CASE+ESPACE) && mouseY>=joueur2.getPosition().getY()&& mouseY<=joueur2.getPosition().getY()+TAILLE_CASE){
                    joueur2.Droite();
                    tourJoueur1=true;
                    seDeplace=false;
                    }
                  }
                }
                if (EstLibreAGauche(joueur2,murs,TAILLE_CASE)){
                  if (JoueurAGauche(joueur2,joueur1,TAILLE_CASE)){
                    if (mouseX>=joueur2.getPosition().getX()-TAILLE_CASE*2-ESPACE*2 && mouseX<=joueur2.getPosition().getX()-ESPACE && mouseY>=joueur2.getPosition().getY()&& mouseY<=joueur2.getPosition().getY()+TAILLE_CASE){
                      joueur2.Gauche();
                      joueur2.Gauche();
                      tourJoueur1=true;
                      seDeplace=false;
                    }
                  }else{
                    if (mouseX>=joueur2.getPosition().getX()-(TAILLE_CASE+ESPACE) && mouseX<=joueur2.getPosition().getX()+ESPACE && mouseY>=joueur2.getPosition().getY()&& mouseY<=joueur2.getPosition().getY()+TAILLE_CASE) {
                      joueur2.Gauche();
                      tourJoueur1=true;
                      seDeplace=false;
                    }
                  }
                }
                if (EstLibreEnBas(joueur2,murs,TAILLE_CASE)){
                  if (JoueurEnBas(joueur2,joueur1,TAILLE_CASE)){
                    if (mouseX>=joueur2.getPosition().getX() && mouseX<=joueur2.getPosition().getX()+TAILLE_CASE && mouseY>=joueur2.getPosition().getY()+(TAILLE_CASE+ESPACE)*2&& mouseY<=joueur2.getPosition().getY()+TAILLE_CASE+(TAILLE_CASE+ESPACE)*2){
                      joueur2.Bas();
                      joueur2.Bas();
                      tourJoueur1=true;
                      seDeplace=false;
                    }
                  }else{
                    if (mouseX>=joueur2.getPosition().getX() && mouseX<=joueur2.getPosition().getX()+TAILLE_CASE && mouseY>=joueur2.getPosition().getY()+(TAILLE_CASE+ESPACE)&& mouseY<=joueur2.getPosition().getY()+TAILLE_CASE+(TAILLE_CASE+ESPACE)){
                        joueur2.Bas();
                        tourJoueur1=true;
                        seDeplace=false;
                    }
                  }
                }
                if (EstLibreEnHaut(joueur2,murs,TAILLE_CASE)){
                  if (JoueurEnHaut(joueur2,joueur1,TAILLE_CASE)){
                    if (mouseX>=joueur2.getPosition().getX() && mouseX<=joueur2.getPosition().getX()+TAILLE_CASE && mouseY>=joueur2.getPosition().getY()-(TAILLE_CASE+ESPACE)*2&& mouseY<=joueur2.getPosition().getY()-ESPACE -(TAILLE_CASE+ESPACE)){
                      joueur2.Haut();
                      joueur2.Haut();
                      tourJoueur1=true;
                      seDeplace=false;
                    }
                  }else{
                    if (mouseX>=joueur2.getPosition().getX() && mouseX<=joueur2.getPosition().getX()+TAILLE_CASE && mouseY>=joueur2.getPosition().getY()-(TAILLE_CASE+ESPACE)&& mouseY<=joueur2.getPosition().getY()-ESPACE){
                        joueur2.Haut();
                        tourJoueur1=true;
                        seDeplace=false;
                      }
                    }
                }
              }else{
                if (mouseX >= joueur2.getPosition().getX()+TAILLE_CASE+ESPACE && mouseX <= joueur2.getPosition().getX()+TAILLE_CASE*2+ESPACE && mouseY >= joueur1.getPosition().getY()&&mouseY <= joueur1.getPosition().getY()+(TAILLE_CASE)){
                  joueur2.Droite();
                  joueur2.Bas();
                  tourJoueur1=true;
                  seDeplace=false;
                }
                if (mouseX >= joueur2.getPosition().getX()-(TAILLE_CASE+ESPACE) && mouseX <= joueur2.getPosition().getX()-ESPACE && mouseY >= joueur1.getPosition().getY()&&mouseY <= joueur1.getPosition().getY()+(TAILLE_CASE)){
                  joueur2.Gauche();
                  joueur2.Bas();
                  tourJoueur1=true;
                  seDeplace=false;
                }
              }
            }
          }else{
            if (MursDispo(mur,murs) && cheminLibre(TAILLE_CASE,ESPACE,WIDTH,HEIGHT,murs,mur,joueur1)&& cheminLibre(TAILLE_CASE,ESPACE,WIDTH,HEIGHT,murs,mur,joueur2)){
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
                if (joueur1.getNbMurs()>0){
                  joueur1.retirerMurs();
                  tourJoueur1=false;
                }
              }else{
                if (joueur2.getNbMurs()>0){
                  joueur2.retirerMurs();
                  tourJoueur1=true;
                }
              }
            }
  }
        }else {
          if (button.isMouseOver(window)) {
              button.handleMouseClick(enJeu);
            }
          if (soundButton.isMouseOver(window)){
            button.handleMouseClick(boolSon);
          }
        }
            
            
          
        
      };
            
        

    /*
     * Mise à jour de l'état du jeu.
     * On calcule le temps (en secondes) depuis la dernière frame qu'on place
     * dans la variable `dt`. Ensuite, il faut compléter suivant ce qui est
     * demandé.
     */
    float dt = elapsedTime.asSeconds();
sf::RectangleShape entiteD(sf::Vector2f(TAILLE_CASE, TAILLE_CASE));
sf::RectangleShape entiteG(sf::Vector2f(TAILLE_CASE, TAILLE_CASE));
sf::RectangleShape entiteH(sf::Vector2f(TAILLE_CASE, TAILLE_CASE));
sf::RectangleShape entiteB(sf::Vector2f(TAILLE_CASE, TAILLE_CASE));
if (seDeplace){
  if (tourJoueur1){
    if (EstLibreEnHaut(joueur1,murs,TAILLE_CASE)&& JoueurEnHaut(joueur1,joueur2,TAILLE_CASE)&& !EstLibreEnHaut(joueur2,murs,TAILLE_CASE)){
      entiteD.setPosition(joueur1.getPosition().getX()+TAILLE_CASE+ESPACE, joueur1.getPosition().getY()-(TAILLE_CASE+ESPACE));
      entiteD.setTexture(&caseDispo);
      entiteG.setPosition(joueur1.getPosition().getX()-(TAILLE_CASE+ESPACE), joueur1.getPosition().getY()-(TAILLE_CASE+ESPACE));
      entiteG.setTexture(&caseDispo);

    }else{
      if (EstLibreADroite(joueur1,murs,TAILLE_CASE)){
        if (JoueurADroite(joueur1,joueur2,TAILLE_CASE)){
          entiteD.setPosition(joueur1.getPosition().getX() + (joueur1.getTaille().getWidth()+ESPACE)*2, joueur1.getPosition().getY());
        }else{
          entiteD.setPosition(joueur1.getPosition().getX() + joueur1.getTaille().getWidth()+ESPACE, joueur1.getPosition().getY());
        }
        entiteD.setTexture(&caseDispo); // Choisissez la couleur souhaitée
        // entite.setFillColor(Color::Red);
      }
      if (EstLibreAGauche(joueur1,murs,TAILLE_CASE)){
        if (JoueurAGauche(joueur1,joueur2,TAILLE_CASE)){
          entiteG.setPosition(joueur1.getPosition().getX() - (joueur1.getTaille().getWidth()+ESPACE)*2, joueur1.getPosition().getY());
        }else{
          entiteG.setPosition(joueur1.getPosition().getX() - joueur1.getTaille().getWidth()-ESPACE, joueur1.getPosition().getY());
        }
        entiteG.setTexture(&caseDispo); // Choisissez la couleur souhaitée
        // entite.setFillColor(Color::Red);
      }
      if (EstLibreEnHaut(joueur1,murs,TAILLE_CASE)){
        if (JoueurEnHaut(joueur1,joueur2,TAILLE_CASE)){  
        }else{
          entiteH.setPosition(joueur1.getPosition().getX(), joueur1.getPosition().getY()-TAILLE_CASE-ESPACE);
        }
        entiteH.setTexture(&caseDispo); // Choisissez la couleur souhaitée
        // entite.setFillColor(Color::Red);
      }
      if (EstLibreEnBas(joueur1,murs,TAILLE_CASE)){
        if (JoueurEnBas(joueur1,joueur2,TAILLE_CASE)){
            entiteB.setPosition(joueur1.getPosition().getX(), joueur1.getPosition().getY()+(TAILLE_CASE+ESPACE)*2);
          
        }else{
          entiteB.setPosition(joueur1.getPosition().getX(), joueur1.getPosition().getY()+TAILLE_CASE+ESPACE);
        }
        entiteB.setTexture(&caseDispo); // Choisissez la couleur souhaitée
        // entite.setFillColor(Color::Red);
      }
    }
    }else{
      if (EstLibreEnBas(joueur2,murs,TAILLE_CASE)&& JoueurEnBas(joueur2,joueur1,TAILLE_CASE)&& !EstLibreEnBas(joueur1,murs,TAILLE_CASE)){
            entiteD.setPosition(joueur2.getPosition().getX()+(TAILLE_CASE+ESPACE), joueur2.getPosition().getY()+(TAILLE_CASE+ESPACE));
            entiteG.setPosition(joueur2.getPosition().getX()-(TAILLE_CASE+ESPACE), joueur2.getPosition().getY()+(TAILLE_CASE+ESPACE));
            entiteD.setTexture(&caseDispo);
            entiteG.setTexture(&caseDispo);
      }else{
        if (EstLibreADroite(joueur2,murs,TAILLE_CASE)){
          if (JoueurADroite(joueur2,joueur1,TAILLE_CASE)){
            entiteD.setPosition(joueur2.getPosition().getX() + (joueur2.getTaille().getWidth()+ESPACE)*2, joueur2.getPosition().getY());

          }else{
            entiteD.setPosition(joueur2.getPosition().getX() + joueur2.getTaille().getWidth()+ESPACE, joueur2.getPosition().getY());
          }
          entiteD.setTexture(&caseDispo); // Choisissez la couleur souhaitée
          // entite.setFillColor(Color::Red);
        }
        if (EstLibreAGauche(joueur2,murs,TAILLE_CASE)){
          if (JoueurAGauche(joueur2,joueur1,TAILLE_CASE)){
            entiteG.setPosition(joueur2.getPosition().getX() - (joueur2.getTaille().getWidth()+ESPACE)*2, joueur2.getPosition().getY());
          }else{
            entiteG.setPosition(joueur2.getPosition().getX() - joueur2.getTaille().getWidth()-ESPACE, joueur2.getPosition().getY());
          }
          entiteG.setTexture(&caseDispo); // Choisissez la couleur souhaitée
          // entite.setFillColor(Color::Red);
        }
        if (EstLibreEnHaut(joueur2,murs,TAILLE_CASE)){
          if (JoueurEnHaut(joueur2,joueur1,TAILLE_CASE)){
            entiteH.setPosition(joueur2.getPosition().getX(), joueur2.getPosition().getY()-(TAILLE_CASE+ESPACE)*2);
          }else{
            entiteH.setPosition(joueur2.getPosition().getX(), joueur2.getPosition().getY()-TAILLE_CASE-ESPACE);
          }
          entiteH.setTexture(&caseDispo); // Choisissez la couleur souhaitée
          // entite.setFillColor(Color::Red);
        }
        if (EstLibreEnBas(joueur2,murs,TAILLE_CASE)){
          if (JoueurEnBas(joueur2,joueur1,TAILLE_CASE)){
            entiteB.setPosition(joueur2.getPosition().getX(), joueur2.getPosition().getY()+(TAILLE_CASE+ESPACE)*2);
          }else{
            entiteB.setPosition(joueur2.getPosition().getX(), joueur2.getPosition().getY()+TAILLE_CASE+ESPACE);
          }
          entiteB.setTexture(&caseDispo); // Choisissez la couleur souhaitée
          // entite.setFillColor(Color::Red);
        }
      }
    }
  }
  

    /*
     * Affichage de l'état du jeu.
     * À chaque tour de boucle, on efface tout grâce à `clear` (qui prend
     * en paramètre la couleur de fond), puis on dessine tous les éléments,
     * puis on affiche la nouvelle image grâce à `display`.
     */
    window.setFramerateLimit(60);
    
  if(enJeu && !Victoire(joueur1,joueur2)){
    window.clear(Color::White);
    RectangleShape rect;
    rect.setSize(Vector2f(HEIGHT,HEIGHT));
    rect.setPosition(Vector2f(WIDTH/2 - HEIGHT/2,0));
    rect.setTexture(&plateau);
    window.draw(rect);
    RectangleShape background;
    background.setSize(Vector2f(WIDTH,HEIGHT));
    background.setPosition(Vector2f(0,0));
    background.setTexture(&backgrounds);
    window.draw(background);
    
    RectangleShape apercu;
    apercu.setSize(Vector2f(TAILLE_CASE,TAILLE_CASE));
    if (tourJoueur1){
      apercu.setPosition(Vector2f(joueur1.getPosition().getX(),joueur1.getPosition().getY()));
    }else{
      apercu.setPosition(Vector2f(joueur2.getPosition().getX(),joueur2.getPosition().getY()));
    }
    
    apercu.setTexture(&apercuTexture);
    window.draw(apercu);
      for (int i = 0; i<TAILLE_TAB; i++){
        RectangleShape mur;
        CircleShape c(5);
        float y = murs[i].getPosition().getY()-murs[i].getTaille().getHeight()/2;
        if (murs[i].getTaille().getHeight()>0 && murs[i].getTaille().getWidth()>0){
          if (murs[i].getTaille().getHeight()>murs[i].getTaille().getWidth()){
            mur.setOrigin(ESPACE/2,TAILLE_CASE+ESPACE);
            mur.setTexture(&textureMurVertical);

          }else{
            mur.setOrigin(TAILLE_CASE+ESPACE,ESPACE/2);
            mur.setTexture(&textureMur);
            
          }
          mur.setSize(Vector2f(murs[i].getTaille().getWidth(),murs[i].getTaille().getHeight()));
          mur.setPosition(Vector2f(murs[i].getPosition().getX(),murs[i].getPosition().getY()));
          window.draw(mur);
        }
      }
      if (tourJoueur1){
        tour.setPosition(Vector2f(160-tour.getGlobalBounds().width/2,HEIGHT/2-TAILLE_CASE*2));
        tour2.setPosition(Vector2f(WIDTH-160-tour2.getGlobalBounds().width/2,HEIGHT/2-TAILLE_CASE*2));
      }else{
        tour2.setPosition(Vector2f(160-tour2.getGlobalBounds().width/2,HEIGHT/2-TAILLE_CASE*2));
        tour.setPosition(Vector2f(WIDTH-160-tour.getGlobalBounds().width/2,HEIGHT/2-TAILLE_CASE*2));
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
      window.draw(tour);
      window.draw(tour2);
      RectangleShape p1 = pion;
      p1.setSize(Vector2f(1.5*pion.getSize().x,1.5*pion.getSize().y));
      p1.setPosition(Vector2f(160-p1.getGlobalBounds().width/2,HEIGHT/2-TAILLE_CASE*4));
      j1Text.setPosition(Vector2f(160-j1Text.getGlobalBounds().width/2,HEIGHT/2-TAILLE_CASE*2.5));
      window.draw(j1Text);
      RectangleShape p2 = pionj2;
      p2.setSize(Vector2f(1.5*pionj2.getSize().x,1.5*pionj2.getSize().y));
      p2.setPosition(Vector2f(WIDTH-160-p2.getGlobalBounds().width/2,HEIGHT/2-TAILLE_CASE*4));
      j2Text.setPosition(Vector2f(WIDTH-160-j2Text.getGlobalBounds().width/2,HEIGHT/2-TAILLE_CASE*2.5));
      j1Murs.setPosition(Vector2f(43.5,HEIGHT/2-TAILLE_CASE));
      j1Murs.setString(joueur1.strNbMurs());
      j2Murs.setPosition(Vector2f(1163,HEIGHT/2-TAILLE_CASE));
      j2Murs.setString(joueur2.strNbMurs());
      window.draw(j2Text);
      window.draw(j2Murs);
      window.draw(j1Murs);
      if (affichageMur){
        window.draw(previewWall);
      }
      window.draw(p1);
      window.draw(p2);
      if (entiteD.getPosition().x != 0 && entiteD.getPosition().y !=0){
        window.draw(entiteD);
      }
      if (entiteG.getPosition().x != 0 && entiteG.getPosition().y !=0){
        window.draw(entiteG);
      }
      if (entiteH.getPosition().x != 0 && entiteH.getPosition().y !=0){
        window.draw(entiteH);
      }
      if (entiteB.getPosition().x != 0 && entiteB.getPosition().y !=0){
        window.draw(entiteB);
      }
  }else if (!enJeu){
    window.clear(Color::White);
    RectangleShape bg;
    bg.setSize(Vector2f(WIDTH,HEIGHT));
    bg.setTexture(&background_home);
    bg.setPosition(Vector2f(0,0));
    window.draw(bg);
    soundButton.update(window,boolSon);
    soundButton.drawTo(window);
    button.update(window,enJeu);
    button.drawTo(window);
    window.draw(Title);
  }else if (enJeu && Victoire(joueur1,joueur2)){
    window.clear(Color::White);
    RectangleShape bg;
    bg.setSize(Vector2f(WIDTH,HEIGHT));
    bg.setTexture(&background_home);
    bg.setPosition(Vector2f(0,0));
    window.draw(bg);
    Text text;
    text.setFillColor(Color::Green);
    text.setFont(font);
    String str = " Victoire du joueur ";
    if (joueur1.getPosition().getY()<13){
      str += "1 !";
    }else{
      str += "2 !";
    }
    text.setString(str);
    text.setCharacterSize(40);
    text.setPosition(WIDTH/2-text.getGlobalBounds().width/2, HEIGHT/2 - text.getGlobalBounds().height/2);
    window.draw(text);
  }
  window.display();
      sf::sleep(frameTime - elapsedTime);
    }
  }
  return 0;
}

