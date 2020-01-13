#pragma once

#include <memory>
#include <tuple>
#include <cassert>

namespace splay {
template <typename T>
struct Node {
  std::weak_ptr<Node> parent;
  std::shared_ptr<Node> left, right;
  T value;

  template <typename U>
  Node(U&& u) : value(std::forward<U>(u)){}

  template <typename... Args>
  Node(Args&&... args) : value(std::forward<Args>(args)...){}

};


template <typename T>
void SetChildSafe(std::shared_ptr<Node<T>> node, std::shared_ptr<Node<T>> from,
                  std::shared_ptr<Node<T>> to) {
  if (!node) {
    return;
  }
  if (node->left == from) {
    node->left = to;
  }
  if (node->right == from) {
    node->right = to;
  }
}

template <typename T>
void RotateRight(std::shared_ptr<Node<T>> node)
{
  auto left = node->left;
  assert(left);

  auto right = node->right;
  auto node_parent = node->parent.lock();

  SetChildSafe(node_parent, node, left);
  left->parent = node_parent;
  node->parent = left;
  if (right) {
    right->parent = node;
  }

  auto left_right = left->right;
  left->right = node;
  node->left = left_right;
}

template <typename T>
std::tuple<bool, T, T> IsSearchTree(std::shared_ptr<Node<T>> node) {
  if (!node) {
    return true;
  }

  const auto [is_left_serch_tree, left_min_element, left_max_element] = IsSearchTree(node->left);

  const auto [is_right_serch_tree, right_min_element, right_max_element] = IsSearchTree(node->right);

  if (node->value < left_max_element || node->value > right_min_element) {
    return false;
  }

  return {true, left_min_element, right_max_element};
}

}  // namespace splay
