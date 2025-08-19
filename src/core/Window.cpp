#include "miko/core/Window.h"

#ifdef _WIN32
#include "miko/platform/Win32Window.h"
#endif

namespace miko {

// Default implementations for virtual methods
void Window::DispatchEvent(const Event& event) {
    // Default implementation - can be overridden by derived classes
}

void Window::UpdateLayout() {
    // Default implementation - can be overridden by derived classes
}

void Window::RenderWidgets() {
    // Default implementation - can be overridden by derived classes
}

// Factory function is implemented in platform-specific files

} // namespace miko