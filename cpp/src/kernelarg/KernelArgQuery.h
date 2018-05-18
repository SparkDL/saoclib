//
// Created by pcz on 18-5-17.
//

#ifndef SAOCLIB_CPP_KERNELARGQUERY_H
#define SAOCLIB_CPP_KERNELARGQUERY_H

#include <cstddef>
#include <cstdio>
#include "KernelArgMode.h"
#include "TypeTag.h"
#include "TypeTagQuery.h"


namespace saoclib {
    class KernelArgQuery : public TypeTagQuery {
    public:
        /**
         * Get arg mode: input/output/input_output...
         * @return arg mode
         */
        virtual KernelArgMode getMode() const = 0;

        virtual const TypeTag *getTypeTag() const = 0;

        std::string toString() const override;
    };

}

#endif //SAOCLIB_CPP_KERNELARGQUERY_H
