//
// Created by Maksym Maisak on 2019-01-15.
// Adapted from sol2
//

#ifndef SAXION_Y2Q2_RENDERING_DEMANGLE_H
#define SAXION_Y2Q2_RENDERING_DEMANGLE_H

#include <array>

namespace utils {

#if defined(__GNUC__) || defined(__clang__)

    template<typename T, class separator_mark = int>
    inline std::string demangleOnce() {

        using namespace std;
        static const std::array<std::string, 2> removals = {{"{anonymous}", "(anonymous namespace)"}};
        std::string name = __PRETTY_FUNCTION__;
        std::size_t start = name.find_first_of('[');
        start = name.find_first_of('=', start);
        std::size_t end = name.find_last_of(']');
        if (end == std::string::npos)
            end = name.size();
        if (start == std::string::npos)
            start = 0;
        if (start < name.size() - 1)
            start += 1;
        name = name.substr(start, end - start);
        start = name.rfind("separator_mark");
        if (start != std::string::npos) {
            name.erase(start - 2, name.length());
        }
        while (!name.empty() && isblank(name.front()))
            name.erase(name.begin());
        while (!name.empty() && isblank(name.back()))
            name.pop_back();

        for (std::size_t r = 0; r < removals.size(); ++r) {
            auto found = name.find(removals[r]);
            while (found != std::string::npos) {
                name.erase(found, removals[r].size());
                found = name.find(removals[r]);
            }
        }

        return name;
    }

#elif defined(_MSC_VER)

    template <typename T>
    inline std::string demangleOnce() {
        static const std::array<std::string, 7> removals = {{"public:", "private:", "protected:", "struct ", "class ", "`anonymous-namespace'", "`anonymous namespace'"}};
        std::string name = __FUNCSIG__;
        std::size_t start = name.find("get_type_name");
        if (start == std::string::npos)
            start = 0;
        else
            start += 13;
        if (start < name.size() - 1)
            start += 1;
        std::size_t end = name.find_last_of('>');
        if (end == std::string::npos)
            end = name.size();
        name = name.substr(start, end - start);
        if (name.find("struct", 0) == 0)
            name.replace(0, 6, "", 0);
        if (name.find("class", 0) == 0)
            name.replace(0, 5, "", 0);
        while (!name.empty() && isblank(name.front()))
            name.erase(name.begin());
        while (!name.empty() && isblank(name.back()))
            name.pop_back();

        for (std::size_t r = 0; r < removals.size(); ++r) {
            auto found = name.find(removals[r]);
            while (found != std::string::npos) {
                name.erase(found, removals[r].size());
                found = name.find(removals[r]);
            }
        }

        return name;
    }
#endif // compilers

    template<typename T>
    inline const std::string& demangle() {

        static std::string name = demangleOnce<T>();
        return name;
    }
}

#endif //SAXION_Y2Q2_RENDERING_DEMANGLE_H
