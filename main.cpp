#include <iostream>

namespace topit {
  struct p_t {
    int x, y;
  };
  bool operator==(p_t, p_t);
  bool operator!=(p_t, p_t);
  struct f_t{p_t aa, bb;};
  struct IDraw{
    virtual p_t begin() const = 0;
    virtual p_t next(p_t) const = 0;//чисто виртуальные методы
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
  size_t points(const IDraw& d, p_t** pts, size_t& s);
  f_t frame(const p_t* pts, size_t& s);
  char* canvas(f_t ff, char fill);
  void paint(char *chv, f_t fr, p_t p, char fill);
  void flush(std::ostream& os, const char* chv, f_t hr);

} // namespace topit

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
  IDraw* shps[3] = {};
  int err = 0;
  p_t * pts = nullptr;
  size_t s = 0;
  try{
    shps[0] = new Dot(0, 0);
    shps[1] = new Dot(5, 7);
    shps[2] = new Dot(-5, -2);
    //TODO:
    //[1] достать все точки из фигур
    for(size_t i = 0; i < 3; ++i){
      s += topit::points(*(shps[i]), &pts, s);
    }
    f_t fr = topit::frame(pts, s);
    char* cnv = canvas(fr, '.');
    for(size_t i = 0; i < s; ++i){
      paint(cnv, fr, pts[i], '#');
    }
    flush(std::cout, cnv, fr);
    //[2] расчитать ограниченный прямоугольник
    //[3] подготовить полотно для рисования
    //[4] нарисовать на полотне все точки
    //[5] вывести полотно
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