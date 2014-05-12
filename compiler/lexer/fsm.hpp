/*
   A start to a finite state machine. Eventually this may replace the standard lexer
 */
#include <string>
#include <map>
#include <vector>

#ifndef FSM_HPP

#define FSM_HPP

namespace lexer {

  typedef std::string::iterator StringIter;

  template <typename T> class FSMNode {
    typedef typename std::pair<std::string, T*> ChildrenPair;
    typedef typename std::vector<ChildrenPair> ChildrenPairVector;
    typedef typename ChildrenPairVector::iterator ChildrenPairIterator;

  private:
    void addChildInternal(StringIter it, StringIter end, T* result);
    T* getValueInternal(StringIter it, StringIter end);

  public:
    const char token;
    T* value;
    std::map<char, FSMNode<T>> children;
    FSMNode() : FSMNode(' ', NULL) {}

    FSMNode(char _token, T* _value) :
      token(_token),
      value(_value) {}

    FSMNode<T> addChildren(ChildrenPairVector childrenPairs);
    bool hasChild(char childToken) { return children.find(childToken) != children.end(); }
    void addChild(std::string path, T* result);
    T* getValue(std::string path);
  };

  // addChildren
  template <class T> FSMNode<T> FSMNode<T>::addChildren(ChildrenPairVector childrenPairs) {
    for (ChildrenPairIterator it = childrenPairs.begin(); it != childrenPairs.end(); ++it) {
      addChild(it->first, it->second);
    }
    return *this;
  }

  // adding children implementation
  template <class T> void FSMNode<T>::addChildInternal(StringIter it, StringIter end, T* result) {
    if ((it + 1) == end) {

      // last element, we generate a new node for the child
      if (hasChild(*it)) {
        children[*it].value = result;
      } else {
        children.insert(std::pair<char, FSMNode<T>>(*it, FSMNode<T>(*it, result)));
      }

    } else {

      // if the element doesn't exist
      if (hasChild(*it)) {
        children.insert(std::pair<char, FSMNode<T>>(*it, FSMNode<T>(*it, NULL)));
      }
      children[*it].addChildInternal(++it, end, result);
    }
  }

  template <class T> void FSMNode<T>::addChild(std::string path, T* result) {
    addChildInternal(path.begin(), path.end(), result);
  }

  // getting children
  template <class T> T* FSMNode<T>::getValueInternal(StringIter it, StringIter end) {
    if (!hasChild(*it)) {
      return NULL;
    }

    FSMNode<T> node = children[*it];

    if ((it + 1) == end) {
      return node.value;
    } else {
      return node.getValueInternal(++it, end);
    }
  }

  template <class T> T* FSMNode<T>::getValue(std::string path) {
    return getValueInternal(path.begin(), path.end());
  }

}

#endif
