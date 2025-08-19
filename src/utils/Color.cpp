#include "miko/utils/Color.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cmath>

namespace miko {

// Static color constants
const Color Color::Transparent(0.0f, 0.0f, 0.0f, 0.0f);
const Color Color::Black(0.0f, 0.0f, 0.0f, 1.0f);
const Color Color::White(1.0f, 1.0f, 1.0f, 1.0f);
const Color Color::Red(1.0f, 0.0f, 0.0f, 1.0f);
const Color Color::Green(0.0f, 1.0f, 0.0f, 1.0f);
const Color Color::Blue(0.0f, 0.0f, 1.0f, 1.0f);
const Color Color::Yellow(1.0f, 1.0f, 0.0f, 1.0f);
const Color Color::Cyan(0.0f, 1.0f, 1.0f, 1.0f);
const Color Color::Magenta(1.0f, 0.0f, 1.0f, 1.0f);
const Color Color::Gray(0.5f, 0.5f, 0.5f, 1.0f);
const Color Color::LightGray(0.75f, 0.75f, 0.75f, 1.0f);
const Color Color::DarkGray(0.25f, 0.25f, 0.25f, 1.0f);

// Windows theme colors
const Color Color::WindowBackground = Color::FromRGBA(240, 240, 240, 255);
const Color Color::ControlBackground = Color::FromRGBA(255, 255, 255, 255);
const Color Color::TextColor = Color::FromRGBA(0, 0, 0, 255);
const Color Color::AccentColor = Color::FromRGBA(0, 120, 215, 255);
const Color Color::BorderColor = Color::FromRGBA(128, 128, 128, 255);
const Color Color::HoverColor = Color::FromRGBA(229, 241, 251, 255);
const Color Color::PressedColor = Color::FromRGBA(204, 228, 247, 255);

} // namespace miko