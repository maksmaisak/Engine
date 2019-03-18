//
// Created by Maksym Maisak on 24/10/18.
//

#ifndef SAXION_Y2Q1_CPP_RECEIVER_H
#define SAXION_Y2Q1_CPP_RECEIVER_H

#include <set>

namespace en {

    template<typename... T>
    class Receiver;

    template<typename TMessage>
    class Receiver<TMessage> {

    public:

        Receiver()  { m_receivers.insert(this); }
        ~Receiver() { m_receivers.erase(this);  }

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
            for (Receiver<TMessage>* receiver : m_receivers) {
                receiver->receive(info);
            }
        }

        static std::set<Receiver*> m_receivers;
    };

    template<typename TMessage, typename... Rest>
    class Receiver<TMessage, Rest...> : Receiver<TMessage>, Receiver<Rest...> {};

    template<typename TMessage>
    inline std::set<Receiver<TMessage>*> Receiver<TMessage>::m_receivers;
}


#endif //SAXION_Y2Q1_CPP_RECEIVER_H
