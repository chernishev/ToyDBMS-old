#pragma once
#include "pgetnextnode.h"

class PCrossProductNode : public PGetNextNode {
public:
    PCrossProductNode(PGetNextNode* left, PGetNextNode* right, LCrossProductNode* source);
    std::vector<std::vector<Value>> GetNext() override;
    void Print(int indent) override;
    ~PCrossProductNode() override;
};