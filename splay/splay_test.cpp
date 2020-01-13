#include<gtest/gtest.h>
#include "splay.h"

using splay::Node;

TEST(Splay, RotateRight) {
  using splay::RotateRight;

  auto node = std::make_shared<Node<int>>(2);
  auto left = std::make_shared<Node<int>>(1);
  auto right = std::make_shared<Node<int>>(3);
  left->parent = node;
  right->parent = node;
  node->left = left;
  node->right = right;
  RotateRight(node);

  EXPECT_EQ(node->parent.lock(), left);
  EXPECT_EQ(node->right, right);
  EXPECT_EQ(node->left, nullptr);

  EXPECT_EQ(left->parent.lock(), nullptr);
  EXPECT_EQ(left->left, nullptr);
  EXPECT_EQ(left->right, node);

  EXPECT_EQ(right->parent.lock(), node);
  EXPECT_EQ(right->left, nullptr);
  EXPECT_EQ(right->right, nullptr);
}
