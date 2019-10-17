//
// Created by Maksym Maisak on 4/11/18.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <functional>
#include <queue>
#include <SFML/System.hpp>

namespace en {

    struct ScheduleItem {

        using function_t = std::function<void()>;
        using id_t = std::uint64_t;

        sf::Time time;
        function_t function;
        id_t id;
        bool isCancelled;

        ScheduleItem(sf::Time time, function_t function, id_t id);
    };

    class TimerHandle {

    public:
        TimerHandle();
        bool isAssigned() const;
        bool isInProgress() const;
        bool cancel();

    private:
        friend class Scheduler;
        TimerHandle(class Scheduler* scheduler, ScheduleItem::id_t scheduleItemId);
        Scheduler* m_scheduler;
        ScheduleItem::id_t m_scheduleItemId;
    };

    /// Schedules operations to be done at a specific time
    class Scheduler {

    public:
        Scheduler();
        void update(float dt);
        TimerHandle delay(sf::Time timeDelay, const ScheduleItem::function_t& function = nullptr);
        TimerHandle schedule(sf::Time time, const ScheduleItem::function_t& function = nullptr);

    private:
        friend class TimerHandle;

        struct ScheduleItemCompare {
            bool operator()(const ScheduleItem& lhs, const ScheduleItem& rhs) {
                return lhs.time > rhs.time;
            }
        };

        std::priority_queue<ScheduleItem, std::vector<ScheduleItem>, ScheduleItemCompare> m_scheduled;
        ScheduleItem::id_t m_nextFreeId;
    };
}

#endif //SCHEDULER_H
