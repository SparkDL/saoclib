//
// Created by pcz on 18-5-11.
//

#ifndef SAOCLIB_CPP_KERNELARGLIMIT_HPP
#define SAOCLIB_CPP_KERNELARGLIMIT_HPP
namespace saoclib {
    enum class KernelArgMode {
        Input,
        Output,
        InputOutput
    };

    enum class KernelArgType {
        Void, /// Void
        Primitive, /// Primitive data, such as int, float...
        AlignedBuffer  /// Aligned buffer.
    };

    class KernelArgQuery {
    public:
        /**
         * Get arg mode: input/output
         * @return arg mode
         */
        virtual KernelArgMode getMode() const = 0;

        /**
         * Get arg type, which could be KernelDataType::Void/Primitive/AlignedPointer...
         * @return arg type
         */
        virtual KernelArgType getType() const = 0;

        /**
         * Get the total size of data stored in the inside pointer.
         * For Void class, it will be 0.
         * For Primitive class, it will be the size of the primitive data, for example, sizeof a int/float/char...
         * For AlignedBuffer class, it will be element size * array length.
         * @return the total size of data
         */
        virtual size_t getSize() const =0;

        /**
         * Get size of an element, only used for AlignedBuffer class;
         * @return element size
         * @note this function is only used for AlignedBuffer class
         */
        virtual size_t getElemSize() const =0;

        /**
         * Get length of array, only used for AlignedBuffer class;
         * @return length of array store
         * @note this function is only used for AlignedBuffer class
         */
        virtual size_t getArrayLength() const =0;


        /**
         * Print data info.
         */
        void print() {
            const char *mode = NULL;
            switch (getMode()) {
                case KernelArgMode::Input:
                    mode = "input";
                    break;
                case KernelArgMode::Output:
                    mode = "output";
                    break;
                case KernelArgMode::InputOutput:
                    mode = "input+output";
            }
            switch (getType()) {
                case KernelArgType::Void:
                    printf("<%s,void>\n", mode);
                    break;
                case KernelArgType::Primitive:
                    printf("<%s,primitive,size: %lu>\n", mode, getSize());
                    break;
                case KernelArgType::AlignedBuffer:
                    printf("<%s,aligned buffer,elem size: %lu,length: %lu>\n", mode, getElemSize(), getArrayLength());
                    break;
            }
        }
    };

    class KernelArgLimit : public KernelArgQuery {
    public:
        template<class T>
        static KernelArgLimit PrimitiveInput() {
            return {KernelArgMode::Input, KernelArgType::Primitive, sizeof(T), 0};
        }

        template<class T>
        static KernelArgLimit AlignedBufferInput(size_t array_length) {
            return {KernelArgMode::Input, KernelArgType::AlignedBuffer, sizeof(T), array_length};
        }


        static KernelArgLimit VoidOutput() {
            return {KernelArgMode::Output, KernelArgType::Void, 0, 0};
        }

        template<class T>
        static KernelArgLimit PrimitiveOutput() {
            return {KernelArgMode::Output, KernelArgType::Primitive, sizeof(T), 0};
        }

        template<class T>
        static KernelArgLimit AlignedBufferOutput(size_t array_length) {
            return {KernelArgMode::Output, KernelArgType::AlignedBuffer, sizeof(T), array_length};
        }

        KernelArgLimit() = default;

        KernelArgMode getMode() const override {
            return mode;
        }

        KernelArgType getType() const override {
            return type;
        }

        size_t getSize() const override {
            if (type == KernelArgType::Void) {
                return 0;
            }
            if (type == KernelArgType::Primitive) {
                return elem_size;
            }
            if (type == KernelArgType::AlignedBuffer) {
                return elem_size * array_length;
            }
        }

        size_t getElemSize() const override {
            return elem_size;
        }

        size_t getArrayLength() const override {
            return array_length;
        }

    private:
        KernelArgLimit(KernelArgMode mode, KernelArgType type,
                       size_t elem_size, size_t array_length)
                : mode(mode),
                  type(type),
                  elem_size(elem_size),
                  array_length(array_length) {}

        KernelArgMode mode = KernelArgMode::Input;
        KernelArgType type = KernelArgType::Void;
        size_t elem_size = 0;
        size_t array_length = 0;
    };

}
#endif //SAOCLIB_CPP_KERNELARGLIMIT_HPP
