#include "interface.h"
#include "../utils/utils.h"

// offsets to keep
LProjectNode::LProjectNode(LAbstractNode* child, std::vector<std::string> tokeep)
      : LAbstractNode(child, nullptr) {

  for (size_t field_index = 0; field_index < child->fieldNames.size(); field_index++) {
    auto &names_of_field = child->fieldNames[field_index];
    for (auto &name_to_keep: tokeep) {
      if (utils::contains(names_of_field, name_to_keep)) {
        offsets.push_back(field_index);
        fieldNames.push_back(names_of_field);
        fieldOrders.push_back(child->fieldOrders[field_index]);
        fieldTypes.push_back(child->fieldTypes[field_index]);
      }
    }
  }

}

LProjectNode::~LProjectNode() {
  delete left;
};