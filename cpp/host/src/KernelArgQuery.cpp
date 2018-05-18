//
// Created by pcz on 18-5-17.
//

#include "KernelArgQuery.h"

namespace saoclib {

    std::string KernelArgQuery::toString() const {
        return std::string("<") + argModeRepr(getMode()) + "," + getTypeTag()->toString() + ">";
    }
}