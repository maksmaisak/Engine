//
// Created by Maksym Maisak on 27/10/18.
//

#ifndef SAXION_Y2Q1_CPP_ENTITIES_H
#define SAXION_Y2Q1_CPP_ENTITIES_H

#include "Entity.h"
#include <vector>

namespace en {

    /// An efficient collection of entities with entity id reuse.
    class Entities {

    public:
        class iterator {

            inline bool shouldSkip() {

                // IF entity is not contained
                const Entity entity = *m_underlyingIterator;
                return (*m_underlyingCollectionPtr)[getId(entity)] != entity;
            }

        public:
            using underlying_collection_type = std::vector<Entity>;
            using underlying_iterator_type = underlying_collection_type::const_iterator;
            using difference_type   = underlying_iterator_type::difference_type;
            using value_type        = underlying_iterator_type::value_type;
            using pointer           = underlying_iterator_type::pointer;
            using reference         = underlying_iterator_type::reference;
            using iterator_category = std::forward_iterator_tag;

            iterator(
                const underlying_collection_type* underlyingCollectionPtr,
                underlying_iterator_type underlyingBegin,
                underlying_iterator_type underlyingEnd
            ) : m_underlyingCollectionPtr(underlyingCollectionPtr),
                m_underlyingIterator(underlyingBegin),
                m_underlyingEnd(underlyingEnd) {}

            inline reference operator  *() const {return m_underlyingIterator.operator *();}
            inline pointer   operator ->() const {return m_underlyingIterator.operator->();}

            inline bool operator ==(const iterator& other) {return m_underlyingIterator == other.m_underlyingIterator;}
            inline bool operator !=(const iterator& other) {return m_underlyingIterator != other.m_underlyingIterator;}

            inline iterator& operator++() {

                do ++m_underlyingIterator;
                while (m_underlyingIterator != m_underlyingEnd && shouldSkip());

                return *this;
            }

        private:
            const underlying_collection_type* m_underlyingCollectionPtr;
            underlying_iterator_type m_underlyingIterator;
            underlying_iterator_type m_underlyingEnd;
        };
        using const_iterator = iterator;

        bool contains(Entity entity) const;
        Entity add();
        void remove(Entity entity);
        void removeAll();
        void reset();

        inline const_iterator cbegin() const {return const_iterator(&m_entities, m_entities.cbegin(), m_entities.cend());}
        inline const_iterator cend()   const {return const_iterator(&m_entities, m_entities.cend()  , m_entities.cend());}
        inline iterator begin()              {return cbegin();}
        inline iterator end()                {return cend();  }

    private:
        std::vector<Entity> m_entities = {nullEntity};
        Entity m_nextFree = nullEntity;
    };
}

#endif //SAXION_Y2Q1_CPP_ENTITIES_H
