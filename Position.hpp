#pragma once
class Position{
  private:
    float x;
    float y;

  public:
    Position(float x, float y) : x(x), y(y) {}
    Position(): x(0),y(0){}

    float getX() const{
      return x;
    }
    float getY() const{
      return y;
    }
    void setX(double newX) {
      x = newX; 
    }
    void setY(double newY) {
      y = newY;
    }
    bool operator<(const Position& p) const {
        if (x < p.x) {
            return true;
        }
        if (x == p.x && y < p.y) {
            return true;
        }
        return false;
    }

};
