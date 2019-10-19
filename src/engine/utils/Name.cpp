//
// Created by Maksym Maisak on 13/10/19.
//

#include "Name.h"
#include <unordered_map>
#include <cassert>
#include <shared_mutex>

using namespace en;

namespace {

    std::unordered_map<std::string, id_t> stringToId;
    std::unordered_map<id_t, std::string> idToString;
    std::shared_mutex nameTableMutex;
}

Name::Name() : m_id(0) {}

Name::Name(const std::string& string) {

    const std::unique_lock writeLock(nameTableMutex);

    const auto it = stringToId.find(string);
    if (it != stringToId.end()) {

        // Existing name
        m_id = it->second;

    } else {

        // New name
        m_id = std::hash<std::string>{}(string);
        stringToId.emplace(string, m_id);
        idToString.emplace(m_id, string);
    }
}

Name::Name(const char* string) :
    Name(std::string(string))
{}

bool Name::isValid() const {
    return m_id;
}

const std::string& Name::getString() const {

    const std::shared_lock readerLock(nameTableMutex);

    assert(isValid());
    const auto it = idToString.find(m_id);
    assert(it != idToString.end());
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

