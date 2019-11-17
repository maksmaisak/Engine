//
// Created by Maksym Maisak on 15/11/19.
//

#ifndef ENGINE_DEFER_H
#define ENGINE_DEFER_H

#include <functional>

namespace utils::detail {

    template<typename F>
    struct ExecuteOnDestruct {

        inline ~ExecuteOnDestruct() {function();}
        F function;
    };

    struct DeferSyntaxSupport {

        template<typename F>
        inline ExecuteOnDestruct<F> operator+(F&& function) {
            return {std::forward<F>(function)};
        }
    };
}

/// Defers execution of the following code until the end of current scope.
/// Like Swift's defer.
/// Use like this:
///
/// defer { /*block of code*/ };
///
#define defer const auto _deferIn___FILE___At___Line__ = utils::detail::DeferSyntaxSupport() + [&]

#endif //ENGINE_DEFER_H
