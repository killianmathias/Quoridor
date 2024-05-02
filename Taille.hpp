#ifndef POSITION_HPP
#define POSITION_HPP
class Taille{
  private:
    double width;
    double height;

  public:
    Taille(int h, int w) : height(h), width(w) {}
    Taille(): height(0),width(0){}

    double getWidth() const {
      return width;
    }
    double getHeight() const {
      return height;
    }

    void setWidth(double newWidth) {
      width = newWidth; 
    }
    void setHeight(double newHeight) {
      height= newHeight;
    }

};
#endif