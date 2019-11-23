//
// Created by Maksym Maisak on 24/10/18.
//

#ifndef ENGINE_RECEIVER_H
#define ENGINE_RECEIVER_H

#include <unordered_set>

namespace en {

    template<typename... T>
    class Receiver;

    template<typename TMessage>
    class Receiver<TMessage> {

    public:

        Receiver()  { m_receivers.insert(this); }
        ~Receiver() { m_receivers.erase(this);  }
        Receiver(const Receiver& other) {m_receivers.insert(this);}
        Receiver& operator=(const Receiver& other) = default;
        Receiver(Receiver&& other) noexcept {
            if (&other != this) {
                m_receivers.erase(&other);
                m_receivers.insert(this);
            }
        }
        Receiver& operator=(Receiver&& other) noexcept {
            if (&other != this) {
                m_receivers.erase(&other);
                m_receivers.insert(this);
            }
        }

        template<typename... Args>
        inline static void broadcast(Args&&... args) {
            broadcastInternal(TMessage{args...});
        }

        inline static void broadcast(const TMessage& info) {
            broadcastInternal(info);
        }

        virtual void receive(const TMessage& info) = 0;

    private:
        inline static void broadcastInternal(const TMessage& info) {
            for (Receiver<TMessage>* const receiver : m_receivers) {
                receiver->receive(info);
            }
        }

        static std::unordered_set<Receiver*> m_receivers;
    };

    template<typename TMessage, typename... Rest>
    class Receiver<TMessage, Rest...> : Receiver<TMessage>, Receiver<Rest>... {};

    template<typename TMessage>
    inline std::unordered_set<Receiver<TMessage>*> Receiver<TMessage>::m_receivers;
}


#endif //ENGINE_RECEIVER_H
