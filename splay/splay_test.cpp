#include "splay.h"

#include <gtest/gtest.h>

#include <limits>
#include <sstream>

using splay::Node;

constexpr int32_t kTestsSize = 7;

namespace {

template <typename T>
[[maybe_unused]] void CerrTree(std::shared_ptr<splay::Node<T>> root,
                               int32_t indent = 0) {
  if (!root) {
    return;
  }

  std::stringstream serialization;
  serialization << "Node[this=" << root.get() << ";left=" << root->left.get()
                << ";right=" << root->right.get() << ";value=" << root->value
                << "]" << std::endl;
  int32_t extra_indent = serialization.tellp() / 3;

  CerrTree(root->left, indent + extra_indent);
  for (int i = 0; i < indent; ++i) {
    std::cerr << " ";
  }
  std::cerr << serialization.str();
  CerrTree(root->right, indent + extra_indent);
}

int32_t AssignValues(std::shared_ptr<splay::Node<int32_t>> root,
                     int32_t current_value = 0) {
  if (!root) {
    return current_value;
  }
  current_value = AssignValues(root->left, current_value);
  root->value = current_value;
  ++current_value;
  auto res = AssignValues(root->right, current_value);
  return res;
}

template <int32_t size>
std::array<std::shared_ptr<splay::Node<int32_t>>, size> GenerateSimpleTree() {
  std::array<std::shared_ptr<splay::Node<int32_t>>, size> result;
  for (int32_t i = 0; i < size; ++i) {
    auto node = std::make_shared<Node<int32_t>>();
    result[i] = node;
    if (i) {
      node->parent = result[(i - 1) / 2];
    }
  }
  for (int32_t i = 0; 2 * i + 1 < size; ++i) {
    auto& node = result[i];
    node->left = result[i * 2 + 1];
    if (i * 2 + 2 < size) {
      node->right = result[i * 2 + 2];
    }
  }
  AssignValues(result[0]);
  return result;
}

template <typename T>
std::tuple<bool, T, T, bool> IsSearchTree(std::shared_ptr<Node<T>> node) {
  if (!node) {
    return {true, T(), T(), true};
  }

  const auto [is_left_search_tree, left_min_element, left_max_element,
              skip_left] = IsSearchTree(node->left);

  const auto [is_right_search_tree, right_min_element, right_max_element,
              skip_right] = IsSearchTree(node->right);

  const auto& current_min = skip_left ? node->value : left_min_element;
  const auto& current_max = skip_right ? node->value : right_max_element;
  if (!is_left_search_tree || !is_right_search_tree) {
    return {false, current_min, current_max, false};
  }

  if ((!skip_left && node->value < left_max_element) ||
      (!skip_right && node->value >= right_min_element)) {
    return {false, current_min, current_max, false};
  }

  return {true, current_min, current_max, false};
}

template <typename T>
void ExpectNodeNeighbours(const std::string_view node_name,
                          const std::shared_ptr<T> node,
                          const std::shared_ptr<T> parent,
                          const std::shared_ptr<T> expected_left,
                          const std::shared_ptr<T> expected_right) {
  ASSERT_TRUE(node);

  EXPECT_EQ(node->parent.lock(), parent)
      << "incorrect parent for " << node_name;
  if (parent) {
    EXPECT_TRUE(parent->left == node || parent->right == node)
        << node_name << " is not a child of its parent";
  }
  EXPECT_EQ(node->left, expected_left) << " unexpected left for " << node_name;
  if (expected_left) {
    EXPECT_EQ(expected_left->parent.lock(), node)
        << node_name << " is not a parent of its left child";
  }
  EXPECT_EQ(node->right, expected_right)
      << " unexpected right for " << node_name;
  if (expected_right) {
    EXPECT_EQ(expected_right->parent.lock(), node)
        << node_name << " is not a parent of its right child";
  }
}

}  // namespace

TEST(Splay, RotateRight) {
  using splay::RotateRight;

  auto tree = GenerateSimpleTree<kTestsSize>();
  /* *
   *  State after GenerateSimpleTree<7>()
   *
   *              tree[0]
   *          /            \
   *       tree[1]         tree[2]
   *      /      \        /       \
   *   tree[3]  tree[4] tree[5]   tree[6]
   * */
  RotateRight(tree[0]);
  /* *
   *  Expected after RotateLeft()
   *
   *          tree[1]
   *      /            \
   *   tree[3]         tree[0]
   *                 /       \
   *              tree[4]   tree[2]
   *                        /     \
   *                      tree[5]  tree[6]
   * */

  ExpectNodeNeighbours<Node<int32_t>>("tree[1]", tree[1], nullptr, tree[3],
                                      tree[0]);
  ExpectNodeNeighbours<Node<int32_t>>("tree[3]", tree[3], tree[1], nullptr,
                                      nullptr);
  ExpectNodeNeighbours<Node<int32_t>>("tree[0]", tree[0], tree[1], tree[4],
                                      tree[2]);
  ExpectNodeNeighbours<Node<int32_t>>("tree[4]", tree[4], tree[0], nullptr,
                                      nullptr);
  ExpectNodeNeighbours<Node<int32_t>>("tree[2]", tree[2], tree[0], tree[5],
                                      tree[6]);
  ExpectNodeNeighbours<Node<int32_t>>("tree[5]", tree[5], tree[2], nullptr,
                                      nullptr);
  ExpectNodeNeighbours<Node<int32_t>>("tree[6]", tree[6], tree[2], nullptr,
                                      nullptr);
}

TEST(Splay, RotateLeft) {
  using splay::RotateLeft;

  auto tree = GenerateSimpleTree<kTestsSize>();
  /* *
   *  State after GenerateSimpleTree<7>()
   *
   *              tree[0]
   *          /            \
   *       tree[1]         tree[2]
   *      /      \        /       \
   *   tree[3]  tree[4] tree[5]   tree[6]
   * */
  RotateLeft(tree[0]);
  /* *
   *  Expected after RotateLeft()
   *              tree[2]
   *          /            \
   *       tree[0]         tree[6]
   *      /      \
   *   tree[1]  tree[5]
   *    /    \
   * tree[3]  tree[4]
   *
   * */

  ExpectNodeNeighbours<Node<int32_t>>("tree[2]", tree[2], nullptr, tree[0],
                                      tree[6]);
  ExpectNodeNeighbours<Node<int32_t>>("tree[0]", tree[0], tree[2], tree[1],
                                      tree[5]);
  ExpectNodeNeighbours<Node<int32_t>>("tree[6]", tree[6], tree[2], nullptr,
                                      nullptr);
  ExpectNodeNeighbours<Node<int32_t>>("tree[1]", tree[1], tree[0], tree[3],
                                      tree[4]);
  ExpectNodeNeighbours<Node<int32_t>>("tree[5]", tree[5], tree[0], nullptr,
                                      nullptr);
  ExpectNodeNeighbours<Node<int32_t>>("tree[3]", tree[3], tree[1], nullptr,
                                      nullptr);
  ExpectNodeNeighbours<Node<int32_t>>("tree[4]", tree[4], tree[1], nullptr,
                                      nullptr);
}

TEST(Splay, Find) {
  auto tree = GenerateSimpleTree<kTestsSize>();
  auto root = tree[0];
  for (int i = 0; i < kTestsSize; ++i) {
    EXPECT_EQ(IsSearchTree(root), (std::tuple<bool, int32_t, int32_t, bool>(
                                      true, 0, kTestsSize - 1, false)));
    root = Find(root, i);
    EXPECT_EQ(root->value, i);
    EXPECT_EQ(IsSearchTree(root), (std::tuple<bool, int32_t, int32_t, bool>(
                                      true, 0, kTestsSize - 1, false)))
        << "search tree is broken after Find(" << i << ")";
  }
}

TEST(Splay, FindWithNotFound) {
  auto tree = GenerateSimpleTree<kTestsSize>();
  auto root = tree[0];
  root = Find(root, kTestsSize);
  EXPECT_EQ(root->value, kTestsSize - 1);
  EXPECT_EQ(IsSearchTree(root), (std::tuple<bool, int32_t, int32_t, bool>(
                                    true, 0, kTestsSize - 1, false)))
      << "search tree is broken after Find(" << kTestsSize << ")";
}
