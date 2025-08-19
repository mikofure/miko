#pragma once

#ifndef MIKO_COLOR_H
#define MIKO_COLOR_H

#include <cstdint>
#include <algorithm>

namespace miko {

    struct Color {
        float r, g, b, a;
        
        Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
        Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}
        
        // Create from 8-bit values
        static Color FromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
            return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
        }
        
        // Create from hex value
        static Color FromHex(uint32_t hex) {
            uint8_t r = (hex >> 24) & 0xFF;
            uint8_t g = (hex >> 16) & 0xFF;
            uint8_t b = (hex >> 8) & 0xFF;
            uint8_t a = hex & 0xFF;
            return FromRGBA(r, g, b, a);
        }
        
        // Convert to 32-bit ARGB
        uint32_t ToARGB() const {
            uint8_t ar = static_cast<uint8_t>(a * 255.0f);
            uint8_t rr = static_cast<uint8_t>(r * 255.0f);
            uint8_t gg = static_cast<uint8_t>(g * 255.0f);
            uint8_t bb = static_cast<uint8_t>(b * 255.0f);
            return (ar << 24) | (rr << 16) | (gg << 8) | bb;
        }
        
        // Blend with another color
        Color Blend(const Color& other, float factor) const {
            float invFactor = 1.0f - factor;
            return Color(
                r * invFactor + other.r * factor,
                g * invFactor + other.g * factor,
                b * invFactor + other.b * factor,
                a * invFactor + other.a * factor
            );
        }
        
        // Lighten/darken
        Color Lighten(float factor) const {
            return Color(
                std::min(1.0f, r + factor),
                std::min(1.0f, g + factor),
                std::min(1.0f, b + factor),
                a
            );
        }
        
        Color Darken(float factor) const {
            return Color(
                std::max(0.0f, r - factor),
                std::max(0.0f, g - factor),
                std::max(0.0f, b - factor),
                a
            );
        }
        
        // With alpha
        Color WithAlpha(float alpha) const {
            return Color(r, g, b, alpha);
        }
        
        // Predefined colors
        static const Color Transparent;
        static const Color Black;
        static const Color White;
        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color Yellow;
        static const Color Cyan;
        static const Color Magenta;
        static const Color Gray;
        static const Color LightGray;
        static const Color DarkGray;
        
        // Windows theme colors
        static const Color WindowBackground;
        static const Color ControlBackground;
        static const Color TextColor;
        static const Color AccentColor;
        static const Color BorderColor;
        static const Color HoverColor;
        static const Color PressedColor;
    };

} // namespace miko

#endif // MIKO_COLOR_H