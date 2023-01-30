#pragma once

#include <string>

#define D3D12_OBJECT() ()

class D3D12Object {
    protected:


    private:
    std::string _name;
    
    auto SetName(std::string name) {
        this->_name = name;
    }

};