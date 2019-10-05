//
// Created by Maksym Maisak on 5/10/19.
//

#ifndef ENGINE_SCOPEDBIND_H
#define ENGINE_SCOPEDBIND_H

#include "GLWrapper.h"
#include "Meta.h"

namespace gl {

    /// Represents a a bind of a wrapper around an openGL object. Unbinds in the destructor.
    /// If multiple binds exist at the same time, instead of unbinding rebinds the previous bind.
    template<class TBindable>
    class ScopedBind final {

        using TBindFunction = decltype(&TBindable::bind);
        using BindFunctionTraits = utils::functionTraits<TBindFunction>;
        using TBindTargetAsTuple = typename BindFunctionTraits::ArgsTuple;
    public:

        template<typename... Args>
        explicit inline ScopedBind(const TBindable& bindable, Args&&... args) :
            m_bindable(&bindable),
            m_bindTarget(std::forward<Args>(args)...)
        {
            static_assert(
                std::is_invocable_v<typename BindFunctionTraits::Signature, decltype(args)...>,
                "This type can not be bound without specifying a bind target. Provide parameters needed by the `bind` function of the object being bound."
            );

            bind();

            if (m_innermostBind) {
                m_innermostBind->m_innerBind = this;
            }
            m_outerBind = m_innermostBind;
            m_innermostBind = this;
        }

        inline ~ScopedBind() {

            assert(m_bindable);
            assert(m_innermostBind == this);
            assert(m_innerBind == nullptr);

            m_innermostBind = m_outerBind;

            if (m_outerBind) {

                assert(m_outerBind->m_innerBind == this);
                m_outerBind->m_innerBind = nullptr;

                // If there's a scoped bind before this one with the same bindArgs (i.e. the same binding target),
                // rebind that one, otherwise unbind.
                ScopedBind* outerBind = m_outerBind;
                while (outerBind) {
                    if (outerBind->m_bindTarget == m_bindTarget) {
                        outerBind->bind();
                        return;
                    }
                }
            }

            unbind();
        }

        ScopedBind(const ScopedBind& other) = delete;
        ScopedBind& operator=(const ScopedBind& other) = delete;
        ScopedBind(ScopedBind&& other) = delete;
        ScopedBind& operator=(ScopedBind&& other) = delete;

    private:
        inline void bind() {

            assert(m_bindable);
            std::apply([this](auto&&... args) {m_bindable->bind(std::forward<decltype(args)>(args)...);}, m_bindTarget);
        }

        inline void unbind() {

            assert(m_bindable);
            std::apply([this](auto&&... args) {m_bindable->unbind(std::forward<decltype(args)>(args)...);}, m_bindTarget);
        }

        const TBindable* m_bindable;
        TBindTargetAsTuple m_bindTarget;

        ScopedBind* m_outerBind = nullptr;
        ScopedBind* m_innerBind = nullptr;

        inline static ScopedBind* m_innermostBind = nullptr;
    };
}

#endif //ENGINE_SCOPEDBIND_H
