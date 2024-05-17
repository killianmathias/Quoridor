#ifndef POSITION_HPP
#define POSITION_HPP
class Taille{
  private:
    float width;
    float height;

  public:
    Taille(float h, float w) : height(h), width(w) {}
    Taille(): height(0),width(0){}

    float getWidth() const {
      return width;
    }
    float getHeight() const {
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