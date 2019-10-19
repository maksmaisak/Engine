//
// Created by Maksym Maisak on 13/10/19.
//

#include "Name.h"
#include <unordered_map>
#include <cassert>
#include <shared_mutex>

using namespace en;

namespace {

    struct NameTables {

        std::shared_mutex mutex;
        std::unordered_map<std::string, Name::id_t> stringToId;
        std::unordered_map<Name::id_t, std::string> idToString;
    };

    NameTables& getNameTables() {

        // Done through a static local to ensure that nameTables is initialized when needed.
        // For example, to prevent static initialization order fiasco when there's a static Name.
        static NameTables nameTables;
        return nameTables;
    }
}

Name::Name()  : m_id(0) {}

Name::Name(const std::string& string) {

    NameTables& tables = getNameTables();
    const std::unique_lock writeLock(tables.mutex);

    const auto it = tables.stringToId.find(string);
    if (it != tables.stringToId.end()) {

        // Existing name
        m_id = it->second;

    } else {

        // New name
        m_id = std::hash<std::string>{}(string);
        tables.stringToId.emplace(string, m_id);
        tables.idToString.emplace(m_id, string);
    }
}

Name::Name(const char* string) :
    Name(std::string(string))
{}

bool Name::isValid() const {
    return m_id;
}

const std::string& Name::getString() const {

    NameTables& tables = getNameTables();
    const std::shared_lock readerLock(tables.mutex);

    assert(isValid());
    const auto it = tables.idToString.find(m_id);
    assert(it != tables.idToString.end());
    return it->second;
}

Name::operator const std::string&() const {
    return getString();
}

Name::operator const char*() const {
    return getString().c_str();
}

Name::operator bool() const {
    return isValid();
}

bool en::operator==(const Name& lhs, const Name& rhs) {
    return lhs.m_id == rhs.m_id;
}

std::ostream& en::operator<<(std::ostream& stream, const Name& name) {
    return stream << name.getString();
}

