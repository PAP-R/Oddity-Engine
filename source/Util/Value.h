#ifndef ODDITYENGINE_VALUE_H
#define ODDITYENGINE_VALUE_H

namespace OddityEngine {
    namespace Util {
        class ValueInterface {
        public:
            bool show = true;
            virtual void apply() = 0;
        };

        template<typename T>
        class Value : public ValueInterface {
        protected:
            T element;
        public:
            Value(T element) : element(element) {}

            void apply() override {
                element();
            }
        };
    } // OddityEngine
} // Util

#endif //ODDITYENGINE_VALUE_H
