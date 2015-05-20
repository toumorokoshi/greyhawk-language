#ifndef GSTD_ARRAY_HPP
#define GSTD_ARRAY_HPP

namespace gstd {

  template <class T> class Array {
    T* values;
    int _length;
  public:
    Array(T* _values, int length) :
      values(_values), _length(length) {}

    T &operator[](int i) {
      return values[i];
    }

    int length() { return _length; }
  };
}

#endif
