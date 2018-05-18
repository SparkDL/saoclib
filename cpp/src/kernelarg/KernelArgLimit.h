//
// Created by pcz on 18-5-11.
//

#ifndef SAOCLIB_CPP_KERNELARGLIMIT_H
#define SAOCLIB_CPP_KERNELARGLIMIT_H

#include <cstddef>
#include <cstdio>

#include "TypeTag.h"
#include "KernelArgMode.h"
#include "KernelArgQuery.h"

namespace saoclib {

    class KernelArgLimit : public KernelArgQuery {
    public:
        KernelArgLimit(const std::shared_ptr<TypeTag> &argTypeTag, KernelArgMode mode)
                : argTypeTag(argTypeTag), mode(mode) {}

        KernelArgLimit() = default;

        KernelArgLimit(const KernelArgLimit &) = default;

        bool operator==(const KernelArgLimit &rhs) const {
            return mode == rhs.getMode() && argTypeTag->equals(rhs.getTypeTag());
        }

        KernelArgMode getMode() const override {
            return mode;
        }

        const std::shared_ptr<TypeTag> &getTypeTag() const override {
            return argTypeTag;
        }

        bool isVoid() const override {
            return argTypeTag->isVoid();
        }

        bool isPrimitive() const override {
            return argTypeTag->isPrimitive();
        }

        bool isArray() const override {
            return argTypeTag->isArray();
        }


        NativeTypeID getTypeID() const override {
            return argTypeTag->getTypeID();
        }

        NativeTypeID getElemTypeID() const override {
            return argTypeTag->getElemTypeID();
        }

        const std::shared_ptr<TypeTag> &getElemType() const override {
            return argTypeTag->getElemType();
        }

        size_t getSize() const override {
            return argTypeTag->getSize();
        }

        size_t getElemSize() const override {
            return argTypeTag->getElemSize();
        }

        size_t getArrayLength() const override {
            return argTypeTag->getArrayLength();
        }

    private:
        KernelArgMode mode = KernelArgMode::mode_input;
        std::shared_ptr<TypeTag> argTypeTag = NULL;
    };

}
#endif //SAOCLIB_CPP_KERNELARGLIMIT_H
