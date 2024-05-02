#pragma once
class Position{
  private:
    double x;
    double y;

  public:
    Position(double x, double y) : x(x), y(y) {}
    Position(): x(0),y(0){}

    double getX() {
      return x;
    }
    double getY() {
      return y;
    }
    void setX(double newX) {
      x = newX; 
    }
    void setY(double newY) {
      y = newY;
    }

};
