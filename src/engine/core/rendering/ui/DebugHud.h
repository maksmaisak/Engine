#ifndef DEBUGHUD_HPP
#define DEBUGHUD_HPP

#include <utility>

namespace en {

	class DebugHud final {

	public:
		struct Info {
			double fps = 0.0;
			std::int64_t frameTimeMicroseconds = 0;
		};

		void draw(const Info& info);
	};
}

#endif // DEBUGHUD_HPP
