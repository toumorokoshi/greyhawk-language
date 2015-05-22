#ifndef GSTD_ARRAY_HPP
#define GSTD_ARRAY_HPP

namespace gstd {

  template <class T> class Array {
    T* values;
    int _length;
  public:
    T &operator[](int i) {
      return values[i];
    }

    int length() { return _length; }

    Array(int length) : _length(length) {
      values = new T[length];
    }

    Array(T* _values, int length):
      values(_values), _length(length) {}
  };
}

#endif
