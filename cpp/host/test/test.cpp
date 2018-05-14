//
// Created by pcz on 18-5-14.
//

#include "../src/saoclib.hpp"

int main() {
    using namespace saoclib;
    auto t = static_cast<saoclib::KernelArgType>(2);
    switch (t) {
        case KernelArgType::Void:
            printf("<void>\n");
            break;
        case KernelArgType::Primitive:
            printf("<primitive,size>\n");
            break;
        case KernelArgType::AlignedBuffer:
            printf("<aligned buffer>\n");
            break;
    }
}