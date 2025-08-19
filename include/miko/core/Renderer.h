#pragma once

#ifndef MIKO_RENDERER_H
#define MIKO_RENDERER_H

#include "../utils/Math.h"
#include "../utils/Color.h"
#include <string>
#include <memory>

// Prevent Windows API macros from interfering with our method names
#ifdef DrawText
#undef DrawText
#endif
#ifdef CreateFont
#undef CreateFont
#endif

namespace miko {

    enum class TextAlignment {
        Left,
        Center,
        Right,
        Justify
    };

    enum class FontWeight {
        Thin = 100,
        Light = 300,
        Normal = 400,
        Medium = 500,
        Bold = 700,
        Black = 900
    };

    enum class FontStyle {
        Normal,
        Italic,
        Oblique
    };

    struct Font {
        std::string family = "Segoe UI";
        float size = 12.0f;
        FontWeight weight = FontWeight::Normal;
        FontStyle style = FontStyle::Normal;
        
        Font() = default;
        Font(const std::string& family, float size, FontWeight weight = FontWeight::Normal, FontStyle style = FontStyle::Normal)
            : family(family), size(size), weight(weight), style(style) {}
    };

    struct Brush {
        Color color;
        
        Brush() = default;
        Brush(const Color& color) : color(color) {}
    };

    struct Pen {
        Color color;
        float width = 1.0f;
        
        Pen() = default;
        Pen(const Color& color, float width = 1.0f) : color(color), width(width) {}
    };

    class Renderer {
    public:
        Renderer() = default;
        virtual ~Renderer() = default;
        
        // Renderer lifecycle
        virtual bool Initialize(void* windowHandle) = 0;
        virtual void Shutdown() = 0;
        virtual void Resize(int width, int height) = 0;
        
        // Rendering operations
        virtual void BeginDraw() = 0;
        virtual void EndDraw() = 0;
        virtual void Clear(const Color& color) = 0;
        
        // Basic shapes
        virtual void DrawLine(const Point& start, const Point& end, const Pen& pen) = 0;
        virtual void DrawRectangle(const Rect& rect, const Pen& pen) = 0;
        virtual void FillRectangle(const Rect& rect, const Brush& brush) = 0;
        virtual void DrawRoundedRectangle(const Rect& rect, float radiusX, float radiusY, const Pen& pen) = 0;
        virtual void FillRoundedRectangle(const Rect& rect, float radiusX, float radiusY, const Brush& brush) = 0;
        virtual void DrawEllipse(const Point& center, float radiusX, float radiusY, const Pen& pen) = 0;
        virtual void FillEllipse(const Point& center, float radiusX, float radiusY, const Brush& brush) = 0;
        
        // Text rendering
        virtual void DrawText(const std::string& text, const Rect& rect, const Font& font, const Brush& brush, TextAlignment alignment = TextAlignment::Left) = 0;
        virtual Size MeasureText(const std::string& text, const Font& font, float maxWidth = 0.0f) = 0;
        
        // Clipping
        virtual void PushClipRect(const Rect& rect) = 0;
        virtual void PopClipRect() = 0;
        
        // Transform
        virtual void PushTransform() = 0;
        virtual void PopTransform() = 0;
        virtual void Translate(float x, float y) = 0;
        virtual void Scale(float x, float y) = 0;
        virtual void Rotate(float angle) = 0;
        
        // Properties
        virtual Size GetSize() const = 0;
        virtual float GetDpiScale() const = 0;
        
        // Resource management
        virtual void* CreateBrush(const Color& color) = 0;
        virtual void* CreatePen(const Color& color, float width) = 0;
        virtual void* CreateFont(const Font& font) = 0;
        virtual void ReleaseBrush(void* brush) = 0;
        virtual void ReleasePen(void* pen) = 0;
        virtual void ReleaseFont(void* font) = 0;
    };

    // Factory function for creating platform-specific renderer
    std::shared_ptr<Renderer> CreatePlatformRenderer();

} // namespace miko

#endif // MIKO_RENDERER_H