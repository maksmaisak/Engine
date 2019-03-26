//
// Created by Maksym Maisak on 2019-03-20.
//

#ifndef ENGINE_AVERAGE_H
#define ENGINE_AVERAGE_H

namespace utils {

    template<typename T>
    class Average {

    public:

        inline void addSample(const T& sample) {
            m_sum += sample;
            m_sampleCount += 1;
        }

        inline T get() const {return m_sampleCount ? m_sum / m_sampleCount : static_cast<T>(0);};
        inline operator T() const {return get();}

    private:
        T m_sum = {};
        int m_sampleCount = 0;
    };
}

#endif //ENGINE_AVERAGE_H
