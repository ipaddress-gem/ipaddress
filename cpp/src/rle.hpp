
#ifndef __RLE__
#define __RLE__

#include <cstdarg>
#include <vector>
#include <map>
#include <string>
#include <sstream>


class Rle {
public:
    size_t part;
    size_t pos;
    size_t cnt;
    bool max;

    Rle() : part(0), pos(0), cnt(0), max(false) {
    }

    Rle(size_t part, size_t pos, size_t cnt, bool max) :
      part(part), pos(pos), cnt(cnt), max(max) {
    }

    Rle *clonePtr() const {
      Rle *ret = new Rle();
      *ret = *this;
      return ret;
    }

    std::string toString() const {
      std::stringstream s2;
      s2 << "<Rle@part:" << this->part;
      s2 << ",pos:" << this->pos;
      s2 << ",cnt:" << this->cnt;
      s2 << ",max:" <<this->max << ">";
      return s2.str();
    }

    bool eq(const Rle &other) const {
        return this->part == other.part && this->pos == other.pos &&
            this->cnt == other.cnt && this->max == other.max;
    }
    bool ne(const Rle &other) const {
        return !this->eq(other);
    }

    static std::vector<Rle> code(const std::vector<size_t> &parts);
};

std::ostream& operator<<(std::ostream &o, const Rle &rle);

#endif
