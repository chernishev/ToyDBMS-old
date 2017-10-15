#include "pcrossproductnode.h"
#include "../utils/utils.h"

PCrossProductNode::PCrossProductNode(PGetNextNode* left, PGetNextNode* right, LCrossProductNode* source)
  : PGetNextNode(left, right, source) {
  const auto left_results = left->GetNext();
  const auto right_results = left->GetNext();

  for (const auto &left_result : left_results) {
    for (const auto &right_result : right_results) {
      auto tmp_result = left_result;
      utils::append_to_back(tmp_result, right_result);
      data.push_back(tmp_result);
    }
  }
}

std::vector<std::vector<Value>> PCrossProductNode::GetNext() {
  return data;
}

void PCrossProductNode::Print(int indent) {
  for (int i = 0; i < indent; i++){
    std::cout << " ";
  }
  std::cout << "CROSS-PRODUCT" << std::endl;
  left->Print(indent + 2);
  right->Print(indent + 2);
}

PCrossProductNode::~PCrossProductNode() {
  delete left;
  delete right;
}