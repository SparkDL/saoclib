//
// Created by pcz on 18-5-17.
//

#include "KernelArgQuery.h"

namespace saoclib {

    bool KernelArgQuery::isInput() const{
        switch (this->getMode()) {
            case KernelArgMode::mode_output:
                return false;
            case KernelArgMode::mode_input:
            case KernelArgMode::mode_input_output:
                return true;
        }
    }

    bool KernelArgQuery::isOutput() const{
        switch (this->getMode()) {
            case KernelArgMode::mode_input:
                return false;
            case KernelArgMode::mode_output:
            case KernelArgMode::mode_input_output:
                return true;
        }
    }

    std::string KernelArgQuery::toString() const {
        return std::string("<") + getTypeTag()->toString() + "," + argModeRepr(getMode()) + ">";
    }
}