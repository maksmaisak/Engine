//
// Created by Maksym Maisak on 21/10/18.
//

#ifndef SAXION_Y2Q1_CPP_ENTITY_H
#define SAXION_Y2Q1_CPP_ENTITY_H

#include <cinttypes>
#include <type_traits>
#include <memory>

namespace en {

    struct EntityInfo {
        static const unsigned int NumIdBits = 32;
        static const unsigned int NumVersionBits = 64 - NumIdBits;
        static const std::uint64_t IdMask      = ~(0ull) >> NumVersionBits;
        static const std::uint64_t VersionMask = ~(0ull) << NumIdBits;

        using entity_type  = std::uint64_t;
        using id_type      = std::uint32_t;
        using version_type = std::uint32_t;
    };

    /// The low 32 bits are the entity number, the high 32 bits are the version number.
    using Entity = EntityInfo::entity_type;
    const Entity nullEntity = {};

    inline EntityInfo::id_type getId(const Entity entity) noexcept {
        return static_cast<EntityInfo::id_type>(entity);
    }

    inline EntityInfo::version_type getVersion(const Entity entity) noexcept {
        return static_cast<EntityInfo::version_type>(entity >> EntityInfo::NumIdBits);
    }

    inline Entity setId(const Entity entity, EntityInfo::id_type id) noexcept {
        return (entity & EntityInfo::VersionMask) | id;
    }

    inline Entity setVersion(const Entity entity, EntityInfo::version_type version) noexcept {
        return (static_cast<Entity>(version) << EntityInfo::NumIdBits) | getId(entity);
    }

    inline bool isNullEntity(const Entity entity) noexcept {
        return getId(entity) == nullEntity;
    }
}

#endif //SAXION_Y2Q1_CPP_ENTITY_H
