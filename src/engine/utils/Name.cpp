//
// Created by Maksym Maisak on 13/10/19.
//

#include "Name.h"
#include <unordered_map>
#include <cassert>

using namespace en;

namespace {

    std::unordered_map<std::string, id_t> stringToId;
    std::unordered_map<id_t, std::string> idToString;

    id_t nextUnusedId = 1;
}

Name::Name() : m_id(0) {}

Name::Name(const std::string& string) {

    const auto it = stringToId.find(string);
    if (it != stringToId.end()) {
        m_id = it->second;
        return;
    }

    // New name
    m_id = nextUnusedId++;
    stringToId.emplace(string, m_id);
    idToString.emplace(m_id, string);
}

Name::Name(const char* string) :
    Name(std::string(string))
{}

bool Name::isValid() const {
    return m_id;
}

Name::operator bool() const {
    return isValid();
}

Name::operator const std::string&() const {

    assert(*this);
    const auto it = idToString.find(m_id);
    assert(it != idToString.end());
    return it->second;
}

Name::operator const char*() const {
    return (operator const std::string&)().c_str();
}

bool en::operator==(const Name& lhs, const Name& rhs) {
    return lhs.m_id == rhs.m_id;
}

