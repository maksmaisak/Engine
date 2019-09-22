#ifndef DEBUGHUD_HPP
#define DEBUGHUD_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <utility>
#include "Engine.h"
#include "Text.h"
#include "VertexRenderer.h"

namespace en {

	class DebugHud {

	public:
		struct Info {
			double fps = 0.0;
			std::int64_t frameTimeMicroseconds = 0;
		};

		DebugHud(Engine& engine, VertexRenderer& vertexRenderer);
		virtual ~DebugHud() = default;

		void draw();
		void setDebugInfo(const Info& info);

	private:
		Engine* m_engine;
		VertexRenderer* m_vertexRenderer;

		en::Text m_text;
	};
}

#endif // DEBUGHUD_HPP
