//
// Created by Maksym Maisak on 13/10/19.
//

#include "Name.h"
#include <algorithm>
#include <unordered_map>
#include <cassert>
#include <shared_mutex>
#include <iostream>

using namespace en;

namespace {

    struct NameInfo {

        std::string string;
        Name uppercaseName;
    };

    struct NameTables {

        std::shared_mutex mutex;
        std::unordered_map<Name, NameInfo> nameToInfo;
    };

    NameTables& getNameTables() {

        // Done through a static local to ensure that nameTables is already initialized when it is first needed.
        // E.g. to prevent static initialization order fiasco when there's a static Name.
        static NameTables nameTables;
        return nameTables;
    }

    inline Name::id_t getId(const std::string& string) {
        return std::hash<std::string>{}(string);
    }

    const NameInfo& getInfoChecked(const Name& name) {

        assert(name.isValid());

        NameTables& tables = getNameTables();
        const std::shared_lock readerLock(tables.mutex);

        const auto it = tables.nameToInfo.find(name);
        assert(it != tables.nameToInfo.end());
        return it->second;
    }
}

Name::Name() : m_id(0), m_isValid(false) {}

Name::Name(Name::id_t id) : m_id(id), m_isValid(true) {}

Name::Name(const std::string& string) : m_id(getId(string)), m_isValid(true) {

    NameTables& tables = getNameTables();
    const std::unique_lock writeLock(tables.mutex);

    const auto isNewName = [&](const Name& name){
        return tables.nameToInfo.find(name) == tables.nameToInfo.end();
    };

    if (isNewName(*this)) {

        std::string uppercaseString = string;
        std::transform(uppercaseString.begin(), uppercaseString.end(), uppercaseString.begin(), ::toupper);
        const Name uppercaseName = Name(getId(uppercaseString));
        if (uppercaseName.m_id != m_id) {
            if (isNewName(uppercaseName)) {
                tables.nameToInfo.emplace(uppercaseName, NameInfo{uppercaseString, uppercaseName});
            }
        }

        tables.nameToInfo.emplace(*this, NameInfo{string, uppercaseName});
    }
}

Name::Name(const char* string) : Name(std::string(string)) {}

bool Name::isValid() const {
    return m_isValid;
}

const std::string& Name::getString() const {
    return getInfoChecked(*this).string;
}

Name Name::getUppercase() const {

    if (!isValid()) {
        return Name();
    }

    return getInfoChecked(*this).uppercaseName;
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

    if (!lhs.m_isValid && !rhs.m_isValid) {
        return true;
    }

    return lhs.m_isValid == rhs.m_isValid && lhs.m_id == rhs.m_id;
}

std::ostream& en::operator<<(std::ostream& stream, const Name& name) {
    return stream << (name.isValid() ? name.getString() : "INVALID_NAME");
}

