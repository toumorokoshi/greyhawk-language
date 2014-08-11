#include "fsm.hpp"


namespace lexer {

  // addChildren
  FSMNode FSMNode::addChildren(KeywordPairVector childrenPairs) {
    for (auto it = childrenPairs.begin(); it != childrenPairs.end(); ++it) {
      addChild(it->first, it->second);
    }
    return *this;
  }

  // adding children implementation
  void FSMNode::addChildInternal(StringIter it, StringIter end, L result) {
    if ((it + 1) == end) {

      // last element, we generate a new node for the child
      if (hasChild(*it)) {
        children[*it].value = result;
      } else {
        children.insert(std::pair<char, FSMNode>(*it, FSMNode(*it, result)));
      }

    } else {

      // if the element doesn't exist
      if (hasChild(*it)) {
        children.insert(std::pair<char, FSMNode>(*it, FSMNode(*it, BAD_TOKEN)));
      }
      children[*it].addChildInternal(++it, end, result);
    }
  }

  void FSMNode::addChild(std::string path, L result) {
    addChildInternal(path.begin(), path.end(), result);
  }

  // getting children
  L FSMNode::getValueInternal(StringIter it, StringIter end) {
    if (!hasChild(*it)) {
      return BAD_TOKEN;
    }

    FSMNode node = children[*it];

    if ((it + 1) == end) {
      return node.value;
    } else {
      return node.getValueInternal(++it, end);
    }
  }

  L FSMNode::getValue(std::string path) {
    return getValueInternal(path.begin(), path.end());
  }

}
