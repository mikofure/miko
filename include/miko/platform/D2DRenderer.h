#pragma once

#ifndef MIKO_D2DRENDERER_H
#define MIKO_D2DRENDERER_H

#ifdef _WIN32

#include "../core/Renderer.h"
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <wrl/client.h>
#include <stack>
#include <unordered_map>

// Prevent UNICODE macros from affecting our method names
#ifdef DrawText
#undef DrawText
#endif
#ifdef CreateFont
#undef CreateFont
#endif

namespace miko {

    using Microsoft::WRL::ComPtr;

    class D2DRenderer : public Renderer {
    public:
        D2DRenderer();
        virtual ~D2DRenderer();
        
        // Renderer interface implementation
        bool Initialize(void* windowHandle) override;
        void Shutdown() override;
        void Resize(int width, int height) override;
        
        void BeginDraw() override;
        void EndDraw() override;
        void Clear(const Color& color) override;
        
        // Basic shapes
        void DrawLine(const Point& start, const Point& end, const Pen& pen) override;
        void DrawRectangle(const Rect& rect, const Pen& pen) override;
        void FillRectangle(const Rect& rect, const Brush& brush) override;
        void DrawRoundedRectangle(const Rect& rect, float radiusX, float radiusY, const Pen& pen) override;
        void FillRoundedRectangle(const Rect& rect, float radiusX, float radiusY, const Brush& brush) override;
        void DrawEllipse(const Point& center, float radiusX, float radiusY, const Pen& pen) override;
        void FillEllipse(const Point& center, float radiusX, float radiusY, const Brush& brush) override;
        
        // Text rendering
        void DrawText(const std::string& text, const Rect& rect, const Font& font, const Brush& brush, TextAlignment alignment = TextAlignment::Left) override;
        Size MeasureText(const std::string& text, const Font& font, float maxWidth = 0.0f) override;
        
        // Clipping
        void PushClipRect(const Rect& rect) override;
        void PopClipRect() override;
        
        // Transform
        void PushTransform() override;
        void PopTransform() override;
        void Translate(float x, float y) override;
        void Scale(float x, float y) override;
        void Rotate(float angle) override;
        
        // Properties
        Size GetSize() const override;
        float GetDpiScale() const override;
        
        // Resource management
        void* CreateBrush(const Color& color) override;
        void* CreatePen(const Color& color, float width) override;
        void* CreateFont(const Font& font) override;
        void ReleaseBrush(void* brush) override;
        void ReleasePen(void* pen) override;
        void ReleaseFont(void* font) override;
        
        // Direct2D specific
        ID2D1RenderTarget* GetRenderTarget() const { return renderTarget.Get(); }
        IDWriteFactory* GetWriteFactory() const { return writeFactory.Get(); }
        
    private:
        HWND hwnd;
        ComPtr<ID2D1Factory> d2dFactory;
        ComPtr<ID2D1HwndRenderTarget> renderTarget;
        ComPtr<IDWriteFactory> writeFactory;
        ComPtr<IWICImagingFactory> wicFactory;
        
        // Resource caches
        std::unordered_map<uint32_t, ComPtr<ID2D1SolidColorBrush>> brushCache;
        std::unordered_map<std::string, ComPtr<IDWriteTextFormat>> fontCache;
        
        // Transform and clipping stacks
        std::stack<D2D1::Matrix3x2F> transformStack;
        std::stack<D2D1_RECT_F> clipStack;
        
        float dpiScaleX;
        float dpiScaleY;
        
        // Helper methods
        bool CreateDeviceIndependentResources();
        bool CreateDeviceDependentResources();
        void DiscardDeviceDependentResources();
        
        D2D1_COLOR_F ColorToD2D(const Color& color);
        D2D1_RECT_F RectToD2D(const Rect& rect);
        D2D1_POINT_2F PointToD2D(const Point& point);
        D2D1_ELLIPSE EllipseToD2D(const Point& center, float radiusX, float radiusY);
        D2D1_ROUNDED_RECT RoundedRectToD2D(const Rect& rect, float radiusX, float radiusY);
        
        DWRITE_TEXT_ALIGNMENT TextAlignmentToD2D(TextAlignment alignment);
        DWRITE_FONT_WEIGHT FontWeightToD2D(FontWeight weight);
        DWRITE_FONT_STYLE FontStyleToD2D(FontStyle style);
        
        ComPtr<ID2D1SolidColorBrush> GetOrCreateBrush(const Color& color);
        ComPtr<IDWriteTextFormat> GetOrCreateTextFormat(const Font& font);
        
        std::wstring StringToWString(const std::string& str);
        uint32_t ColorToHash(const Color& color);
        std::string FontToString(const Font& font);
    };

} // namespace miko

#endif // _WIN32

#endif // MIKO_D2DRENDERER_H