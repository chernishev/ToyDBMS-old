#pragma once

#include "pgetnextnode.h"

class PProjectNode : public PGetNextNode {
public:
    explicit PProjectNode(PGetNextNode* from, LProjectNode* lProjectNode);

    void Print(int indent) override;

    std::vector<std::vector<Value>> GetNext() override;
};