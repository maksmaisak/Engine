#include <utility>

//
// Created by Maksym Maisak on 4/11/18.
//

#ifndef SAXION_Y2Q1_CPP_SCHEDULER_H
#define SAXION_Y2Q1_CPP_SCHEDULER_H

#include <functional>
#include <queue>
#include <SFML/System.hpp>

/// Schedules operations to be done at a specific time
class Scheduler {

public:
    using func_t = std::function<void()>;

    void update(float dt);
    void delay(sf::Time timeDelay, const func_t& func);
    void schedule(sf::Time time, const func_t& func);

private:

    struct ScheduleItem {

        sf::Time time;
        func_t func;

        ScheduleItem(sf::Time time, func_t func) : time(time), func(std::move(func)) {}
    };

    struct ScheduleItemCompare {
        bool operator()(const ScheduleItem& lhs, const ScheduleItem& rhs) {
            return lhs.time > rhs.time;
        }
    };

    std::priority_queue<ScheduleItem, std::vector<ScheduleItem>, ScheduleItemCompare> m_scheduled;
};


#endif //SAXION_Y2Q1_CPP_SCHEDULER_H
