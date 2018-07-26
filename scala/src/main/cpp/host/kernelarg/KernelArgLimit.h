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
        KernelArgLimit() = default;

        KernelArgLimit(const KernelArgLimit &) = default;

        KernelArgLimit(const std::shared_ptr<TypeTag> &argTypeTag, KernelArgMode mode);

        bool operator==(const KernelArgLimit &rhs) const;

        KernelArgMode getMode() const override;

        const std::shared_ptr<TypeTag> &getTypeTag() const override;

        bool isVoid() const override;

        bool isPrimitive() const override;

        bool isArray() const override;


        NativeTypeID getTypeID() const override;

        NativeTypeID getElemTypeID() const override;

        const std::shared_ptr<TypeTag> &getElemType() const override;

        size_t getSize() const override;

        size_t getElemSize() const override;

        size_t getArrayLength() const override;

    private:
        KernelArgMode mode = KernelArgMode::mode_input;
        std::shared_ptr<TypeTag> argTypeTag = NULL;
    };

}
#endif //SAOCLIB_CPP_KERNELARGLIMIT_H
