#pragma once

#include <cassert>
#include <iostream>
#include <memory>
#include <tuple>

namespace splay {
template <typename T>
struct Node {
  std::weak_ptr<Node> parent;
  std::shared_ptr<Node> left, right;
  T value;

  template <typename U>
  Node(U&& u) : value(std::forward<U>(u)) {}

  template <typename... Args>
  Node(Args&&... args) : value(std::forward<Args>(args)...) {}
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
void RotateRight(std::shared_ptr<Node<T>> node) {
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
  if (left_right) {
    left_right->parent = node;
  }
}

template <typename T>
void RotateLeft(std::shared_ptr<Node<T>> node) {
  auto right = node->right;
  assert(right);

  auto left = node->left;
  auto node_parent = node->parent.lock();

  SetChildSafe(node_parent, node, right);
  right->parent = node_parent;
  node->parent = right;
  if (left) {
    left->parent = node;
  }

  auto right_left = right->left;
  right->left = node;
  node->right = right_left;
  if (right_left) {
    right_left->parent = node;
  }
}

template <typename T>
void Expose(std::shared_ptr<Node<T>> node) {
  while (node->parent.lock()) {
    auto parent = node->parent.lock();
    auto grand_parent = parent->parent.lock();
    if (!grand_parent) {
      if (node == parent->left) {
        RotateRight(parent);
      } else {
        RotateLeft(parent);
      }
    } else {
      if (grand_parent->left == parent) {
         if (parent->left == node) {
           // ZigZig
           RotateRight(grand_parent);
           RotateRight(parent);
         } else {
           // ZigZag
           RotateLeft(parent);
           RotateRight(grand_parent);
         }
      } else {
         if (parent->right == node) {
           // ZagZag
           RotateLeft(grand_parent);
           RotateLeft(parent);
         } else {
           // ZagZig
           RotateRight(parent);
           RotateLeft(grand_parent);
         }
      }
    }
  }
}

template <typename T>
std::shared_ptr<Node<T>> Find(std::shared_ptr<Node<T>> root, T key) {
  if (!root) {
    return nullptr;
  }
  std::shared_ptr<Node<T>> root_parent;
  while (root && root->value != key) {
    root_parent = root;
    root = (root->value < key ? root->right : root->left);
  }

  if (root) {
    assert(root->value == key);
    Expose(root);
    return root;
  } else {
    Expose(root_parent);
    return root_parent;
  }
}

template <typename T>
std::shared_ptr<Node<T>> Insert(std::shared_ptr<Node<T>> root, T key) {
  if (!root) {
    return std::make_shared<Node<T>>(key);
  }
  root = Find(root, key);

  auto inserted_root = std::make_shared<Node<T>>(key);
  inserted_root->left = root;
  root->parent = inserted_root;

  auto right = root->right;
  inserted_root->right = right;
  if (right) {
    right->parent = inserted_root;
  }

  return inserted_root;
}


}  // namespace splay
