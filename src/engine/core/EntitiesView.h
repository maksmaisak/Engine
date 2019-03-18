//
// Created by Maksym Maisak on 22/10/18.
//

#ifndef SAXION_Y2Q1_CPP_ENTITIESVIEW_H
#define SAXION_Y2Q1_CPP_ENTITIESVIEW_H

#include <algorithm>
#include <set>
#include <tuple>
#include "Entity.h"
#include "ComponentPool.h"

namespace en {

    template<typename... TComponent>
    class EntitiesView {

        static_assert(sizeof...(TComponent) > 0, "There must be at least one component type provided.");

        using underlying_collection_type = ComponentPoolBase;
        using underlying_iterator_type = underlying_collection_type::const_iterator;

        // Iterates over all pools
        class iterator {

            inline bool shouldSkip() const {
                return !(std::get<ComponentPool<TComponent>*>(m_pools)->contains(*m_underlyingIterator) && ...);
            }

        public:

            // Typedefs required to qualify as an Iterator.
            using difference_type   = underlying_iterator_type::difference_type;
            using value_type        = underlying_iterator_type::value_type;
            using pointer           = underlying_iterator_type::pointer;
            using reference         = underlying_iterator_type::reference;
            using iterator_category = std::forward_iterator_tag;

            iterator(std::tuple<ComponentPool<TComponent>*...> pools, underlying_iterator_type underlyingIterator, underlying_iterator_type underlyingEnd)
                : m_pools(pools), m_underlyingIterator(underlyingIterator), m_underlyingEnd(underlyingEnd) {

                if (m_underlyingIterator != m_underlyingEnd && shouldSkip()) {
                    ++(*this);
                }
            }

            inline reference operator  *() const {return m_underlyingIterator.operator *();}
            inline pointer   operator ->() const {return m_underlyingIterator.operator->();}

            inline bool operator ==(const iterator& other) const {return m_underlyingIterator == other.m_underlyingIterator;}
            inline bool operator !=(const iterator& other) const {return m_underlyingIterator != other.m_underlyingIterator;}

            inline iterator& operator ++() {

                do ++m_underlyingIterator;
                while (m_underlyingIterator != m_underlyingEnd && shouldSkip());

                return *this;
            }

        private:

            std::tuple<ComponentPool<TComponent>*...> m_pools;
            underlying_iterator_type m_underlyingIterator;
            underlying_iterator_type m_underlyingEnd;
        };

    public:

        EntitiesView(ComponentPool<TComponent>&... pools) :
            m_pools{&pools...},
            m_smallestPoolPtr(getSmallestPoolPtr(std::forward<ComponentPool<TComponent>&>(pools)...))
            {};

        inline iterator begin() const {
            return iterator(m_pools, m_smallestPoolPtr->cbegin(), m_smallestPoolPtr->cend());
        }

        inline iterator end() const {
            return iterator(m_pools, m_smallestPoolPtr->cend()  , m_smallestPoolPtr->cend());
        }

        inline std::size_t count() const {
            return (std::size_t)std::distance(begin(), end());
        }

        inline Entity tryGetOne() const {
            iterator first = begin();
            return first != end() ? *first : en::nullEntity;
        }

    private:

        std::tuple<ComponentPool<TComponent>*...> m_pools;
        const ComponentPoolBase* m_smallestPoolPtr;

        inline const ComponentPoolBase* getSmallestPoolPtr(ComponentPool<TComponent>&... pools) const {

            return std::min(
                {static_cast<ComponentPoolBase*>(&pools)...},
                [](const auto* poolA, const auto* poolB) {return poolA->size() < poolB->size();}
            );
        }
    };

}

#endif //SAXION_Y2Q1_CPP_ENTITIESVIEW_H
