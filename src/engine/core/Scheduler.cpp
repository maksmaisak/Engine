//
// Created by Maksym Maisak on 4/11/18.
//

#include "Scheduler.h"
#include <algorithm>
#include "GameTime.h"

using namespace en;

Scheduler::Scheduler() :
    m_nextFreeId(1)
{}

void Scheduler::update(float dt) {

    while (!m_scheduled.empty() && m_scheduled.top().time <= GameTime::nowSFTime()) {

        const ScheduleItem& scheduleItem = m_scheduled.top();
        if (!scheduleItem.isCancelled) {
            if (scheduleItem.function) {
                scheduleItem.function();
            }
        }

        m_scheduled.pop();
    }
}

Timer Scheduler::delay(sf::Time timeDelay, const ScheduleItem::function_t& function) {

    return schedule(GameTime::nowSFTime() + timeDelay, function);
}

Timer Scheduler::schedule(sf::Time time, const ScheduleItem::function_t& function) {

    const ScheduleItem::id_t id = m_nextFreeId++;
    m_scheduled.emplace(time, function, id);
    return Timer(this, id);
}

ScheduleItem::ScheduleItem(sf::Time time, ScheduleItem::function_t function, ScheduleItem::id_t id) :
    time(time),
    function(std::move(function)),
    id(id),
    isCancelled(false)
{}

namespace {

    /// A way to get the underlying container of an STL adapter type like priority_queue, queue, deque etc.
    /// Exploits the fact that they have the underlying container in a protected variable `c`.
    template<class TAdapter>
    typename TAdapter::container_type& getUnderlyingContainer(TAdapter& adapter) {

        struct getter : TAdapter {
            static typename TAdapter::container_type& get(TAdapter& adapter) {
                return adapter.*(&getter::c);
            }
        };

        return getter::get(adapter);
    }

    ScheduleItem* findScheduleItemById(std::vector<ScheduleItem>& items, ScheduleItem::id_t id) {

        if (!id) {
            return nullptr;
        }

        const auto it = std::find_if(items.begin(), items.end(), [id](const ScheduleItem& item) {
            return item.id == id;
        });

        return it != items.end() ? &*it : nullptr;
    }
}

Timer::Timer() :
    m_scheduler(nullptr),
    m_scheduleItemId(0)
{}

Timer::Timer(struct Scheduler* scheduler, ScheduleItem::id_t scheduleItemId) :
    m_scheduler(scheduler),
    m_scheduleItemId(scheduleItemId)
{}

bool Timer::isAssigned() const {

    return m_scheduler && m_scheduleItemId;
}

bool Timer::isInProgress() const {

    if (isAssigned()) {
        if (ScheduleItem* const scheduleItem = findScheduleItemById(getUnderlyingContainer(m_scheduler->m_scheduled), m_scheduleItemId)) {
            if (!scheduleItem->isCancelled) {
                return true;
            }
        }
    }

    return false;
}

bool Timer::cancel() {

    if (isAssigned()) {
        if (ScheduleItem* const scheduleItem = findScheduleItemById(getUnderlyingContainer(m_scheduler->m_scheduled), m_scheduleItemId)) {
            if (!scheduleItem->isCancelled) {
                scheduleItem->isCancelled = true;
                return true;
            }
        }
    }

    return false;
}
