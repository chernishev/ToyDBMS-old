#include "pprojectnode.h"

using namespace std;

PProjectNode::PProjectNode(PGetNextNode* from, LProjectNode* lProjectNode)
    : PGetNextNode(from, nullptr, lProjectNode) {
  auto previous_data = from->GetNext();
  for (auto &record: previous_data) {
    vector<Value> projected_record;
    for (auto &field_index: lProjectNode->offsets) {
      projected_record.push_back(record[field_index]);
    }
    data.push_back(projected_record);
  }
}

vector<vector<Value>> PProjectNode::GetNext() {
  return data;
}

void PProjectNode::Print(int indent) {
  for (int i = 0; i < indent; i++){
    std::cout << " ";
  }
  std::cout << "Project Node, keeped names: ";
  for (auto& name_to_keep: dynamic_cast<LProjectNode*>(prototype)->fieldNames) {
    std::cout << name_to_keep[0] << " ";
  }
  cout << endl;
  left->Print(indent + 2);
  cout << endl;
}
