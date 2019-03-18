//
// Created by Maksym Maisak on 28/10/18.
//

#ifndef SAXION_Y2Q1_CPP_CUSTOMTYPEINDEX_H
#define SAXION_Y2Q1_CPP_CUSTOMTYPEINDEX_H

#include <numeric>
#include <vector>

namespace utils {

    /// Assigns indices to types.
    /// If a specialization's `index` is accessed with 10 different types (index<T1>, index<T2>, ... , index<T10>),
    /// values 0-9 will be assigned to those types, though order is not guaranteed.
    template<typename Dummy>
    class CustomTypeIndex {

        inline static std::size_t m_nextIndex = 0;

    public:
        template<typename T>
        inline static const std::size_t index = m_nextIndex++;
    };

    // A mapping from type to value
    template<typename Dummy, typename TValue>
    class CustomTypeMap {

        using Indices = CustomTypeIndex<Dummy>;

    public:

        template<typename T, std::size_t index = Indices::template index<T>>
        inline static TValue value;

        template<typename T>
        inline TValue get() {
            return m_values[ensureSize<T>()];
        }

        template<typename T>
        inline void set(const TValue& value) {
            m_values[ensureSize<T>()] = value;
        }

    private:

        template<typename T>
        inline std::size_t ensureSize() {

            const std::size_t index = Indices::template index<T>;

            if (index >= m_values.size()) {
                m_values.resize(index + 1);
            }

            return index;
        }

        std::vector<TValue> m_values;
    };
}

#endif //SAXION_Y2Q1_CPP_CUSTOMTYPEINDEX_H
