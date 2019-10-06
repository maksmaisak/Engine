#ifndef MYGLM_H
#define MYGLM_H

#include <iostream>
#include <iomanip>

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <glm/glm.hpp>
#include <glm/ext.hpp>

template<typename Stream, glm::length_t L, typename T, glm::qualifier Q>
inline Stream& operator <<(Stream& stream, const glm::vec<L, T, Q>& vec) {

    stream << "(";

    stream << vec[0];
    for (glm::length_t i = 1; i < L; ++i)
    	stream << "," << vec[i];

    return stream << ")";
}

template<typename Stream, typename T, glm::qualifier Q>
inline Stream& operator <<(Stream& stream, const glm::qua<T, Q>& q) {

	return stream << "(" << q.x << "," << q.y << "," << q.z << "," << q.w << ")";
}

template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline std::ostream& operator <<(std::ostream& stream, const glm::mat<C, R, T, Q>& m) {

    stream.precision(1);
	for (glm::length_t r = 0; r < R; ++r) {
		for (glm::length_t c = 0; c < C; ++c) {
            stream << std::showpos << std::fixed << m[c][r] << " ";
		}
        stream << std::endl;
	}
	return stream;
}

#endif // MYGLM_H
