#include <iostream>

namespace topit {
  struct p_t {
    int x, y;
  };

  bool operator==(p_t, p_t);
  bool operator!=(p_t, p_t);

  struct f_t{
    p_t aa, bb;
  };

  struct IDraw{
    virtual p_t begin() const = 0;
    virtual p_t next(p_t) const = 0;
    virtual ~IDraw() = default;
  };

  struct Dot: IDraw
  {
    p_t begin() const override;
    p_t next(p_t) const override;
    Dot(int x, int y);
    explicit Dot(p_t dd);
    p_t d;
  };
  
  struct HorLine: IDraw
  {
    p_t begin() const override;
    p_t next(p_t) const override;
    HorLine(int, int, int);
    explicit HorLine(p_t, p_t);
    p_t beg, end;
  };

  size_t points(const IDraw& d, p_t** pts, size_t& s);//[1] достать все точки из фигур
  f_t frame(const p_t* pts, size_t& s);//[2] расчитать ограниченный прямоугольник
  char* canvas(f_t ff, char fill);//[3] подготовить полотно для рисования
  void paint(char *cnv, f_t fr, p_t p, char fill);//[4] нарисовать на полотне все точки
  void flush(std::ostream& os, const char* cnv, f_t hr);//[5] вывести полотно

}

int main() {
  using topit::IDraw;
  using topit::Dot;
  using topit::p_t;
  using topit::f_t;
  using topit::canvas;
  using topit::frame;
  using topit::paint;
  using topit::points;
  using topit::flush;
  using topit::HorLine;
  IDraw* shps[3] = {};
  int err = 0;
  p_t * pts = nullptr;
  size_t s = 0;
  try{
    shps[0] = new HorLine(5, 3, 6);
    shps[1] = new HorLine({1, 0}, {6, 0});
    shps[2] = new HorLine(3, 2, 2);

    for(size_t i = 0; i < 3; ++i){
      s += points(*(shps[i]), &pts, s);
    }

    f_t fr = frame(pts, s);

    char* cnv = canvas(fr, '.');

    for(size_t i = 0; i < s; ++i){
      paint(cnv, fr, pts[i], '#');
    }

    flush(std::cout, cnv, fr);
  }catch(...){
    std::cerr << "Bad drawing\n";
    err = 2;
  }

  delete[] shps[0];
  delete[] shps[1];
  delete[] shps[2];

  return err;
}

bool topit::operator==(p_t a, p_t b) { return a.x == b.x && a.y == b.y; }

bool topit::operator!=(p_t a, p_t b) { return !(a == b); }

topit::Dot::Dot(int x, int y):
  IDraw(),
  d{x, y}
{}

topit::Dot::Dot(p_t dd):
IDraw(), d(dd)
{}

topit::p_t topit::Dot::begin() const{
  return d;
}

topit::p_t topit::Dot::next(p_t prev) const{
  if(prev != begin()){
    throw std::logic_error("bad impl");
  }
  return d;
}

topit::p_t topit::HorLine::begin() const{
  return beg;
}

topit::p_t topit::HorLine::next(p_t prev) const{
  if(prev.x >= end.x){
    return begin();
  }
  
  return {prev.x + 1, beg.y};
}

topit::HorLine::HorLine(int y, int x1, int x2)
: IDraw(), beg{x1, y}, end{x2, y}
{
  if(x1 > x2){
    throw std::logic_error("Bad points");
  }
}

topit::HorLine::HorLine(p_t ss, p_t ee)
: IDraw(), beg{ss}, end(ee)
{
  if(ss.y != ee.y || ss.x > ee.x){
    throw std::logic_error("Bad points");
  }
}
