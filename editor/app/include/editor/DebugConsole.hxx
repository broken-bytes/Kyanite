#pragma once

#include <logger/ILogger.hxx>

namespace kyanite::editor {
	class DebugConsole: public ILogger {
	public:
		DebugConsole();
		~DebugConsole();


	};
}