#include "miko/platform/D2DRenderer.h"
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")

namespace miko {

D2DRenderer::D2DRenderer()
    : d2dFactory(nullptr)
    , renderTarget(nullptr)
    , writeFactory(nullptr)
    , dpiScaleX(1.0f)
    , dpiScaleY(1.0f)
{
}

D2DRenderer::~D2DRenderer() {
    Shutdown();
}

bool D2DRenderer::Initialize(void* windowHandle) {
    HWND hwnd = static_cast<HWND>(windowHandle);
    this->hwnd = hwnd;
    HRESULT hr = S_OK;
    
    // Create D2D factory
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, d2dFactory.GetAddressOf());
    if (FAILED(hr)) return false;
    
    // Create DirectWrite factory
    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
                            reinterpret_cast<IUnknown**>(writeFactory.GetAddressOf()));
    if (FAILED(hr)) return false;
    
    // Get window size
    RECT rect;
    GetClientRect(hwnd, &rect);
    D2D1_SIZE_U size = D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top);
    
    // Create render target
    hr = d2dFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(hwnd, size),
        renderTarget.GetAddressOf()
    );
    if (FAILED(hr)) return false;
    
    // Get DPI scaling
    renderTarget->GetDpi(&dpiScaleX, &dpiScaleY);
    dpiScaleX /= 96.0f;
    dpiScaleY /= 96.0f;
    
    return true;
}

void D2DRenderer::Shutdown() {
    // Release cached resources
    for (auto& pair : brushCache) {
        if (pair.second) pair.second->Release();
    }
    brushCache.clear();
    
    for (auto& pair : fontCache) {
        if (pair.second) pair.second->Release();
    }
    fontCache.clear();
    
    // Release DirectWrite factory
    if (writeFactory) {
        writeFactory->Release();
        writeFactory = nullptr;
    }
    
    // Release render target
    if (renderTarget) {
        renderTarget->Release();
        renderTarget = nullptr;
    }
    
    // Release D2D factory
    if (d2dFactory) {
        d2dFactory->Release();
        d2dFactory = nullptr;
    }
}

void D2DRenderer::Resize(int width, int height) {
    if (renderTarget) {
        D2D1_SIZE_U pixelSize = D2D1::SizeU((UINT32)width, (UINT32)height);
        renderTarget->Resize(pixelSize);
    }
}

void D2DRenderer::BeginDraw() {
    if (renderTarget) {
        renderTarget->BeginDraw();
    }
}

void D2DRenderer::EndDraw() {
    if (renderTarget) {
        HRESULT hr = renderTarget->EndDraw();
        if (hr == D2DERR_RECREATE_TARGET) {
            // Device lost, need to recreate resources
            // This would be handled in a real implementation
        }
    }
}

void D2DRenderer::Clear(const Color& color) {
    if (renderTarget) {
        renderTarget->Clear(ColorToD2D(color));
    }
}

void D2DRenderer::DrawLine(const Point& start, const Point& end, const Pen& pen) {
    if (!renderTarget) return;
    
    ComPtr<ID2D1SolidColorBrush> brush = GetOrCreateBrush(pen.color);
    if (!brush) return;
    
    renderTarget->DrawLine(
        PointToD2D(start),
        PointToD2D(end),
        brush.Get(),
        pen.width
    );
}

void D2DRenderer::DrawRectangle(const Rect& rect, const Pen& pen) {
    if (!renderTarget) return;
    
    ComPtr<ID2D1SolidColorBrush> brush = GetOrCreateBrush(pen.color);
    if (!brush) return;
    
    renderTarget->DrawRectangle(RectToD2D(rect), brush.Get(), pen.width);
}

void D2DRenderer::FillRectangle(const Rect& rect, const Brush& brush) {
    if (!renderTarget) return;
    
    ComPtr<ID2D1SolidColorBrush> d2dBrush = GetOrCreateBrush(brush.color);
    if (!d2dBrush) return;
    
    renderTarget->FillRectangle(RectToD2D(rect), d2dBrush.Get());
}

void D2DRenderer::DrawRoundedRectangle(const Rect& rect, float radiusX, float radiusY, const Pen& pen) {
    if (!renderTarget) return;
    
    ComPtr<ID2D1SolidColorBrush> brush = GetOrCreateBrush(pen.color);
    if (!brush) return;
    
    D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(RectToD2D(rect), radiusX, radiusY);
    renderTarget->DrawRoundedRectangle(roundedRect, brush.Get(), pen.width);
}

void D2DRenderer::FillRoundedRectangle(const Rect& rect, float radiusX, float radiusY, const Brush& brush) {
    if (!renderTarget) return;
    
    ComPtr<ID2D1SolidColorBrush> d2dBrush = GetOrCreateBrush(brush.color);
    if (!d2dBrush) return;
    
    D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(RectToD2D(rect), radiusX, radiusY);
    renderTarget->FillRoundedRectangle(roundedRect, d2dBrush.Get());
}

void D2DRenderer::DrawEllipse(const Point& center, float radiusX, float radiusY, const Pen& pen) {
    if (!renderTarget) return;
    
    ComPtr<ID2D1SolidColorBrush> brush = GetOrCreateBrush(pen.color);
    if (!brush) return;
    
    D2D1_ELLIPSE ellipse = D2D1::Ellipse(PointToD2D(center), radiusX, radiusY);
    renderTarget->DrawEllipse(ellipse, brush.Get(), pen.width);
}

void D2DRenderer::FillEllipse(const Point& center, float radiusX, float radiusY, const Brush& brush) {
    if (!renderTarget) return;
    
    ComPtr<ID2D1SolidColorBrush> d2dBrush = GetOrCreateBrush(brush.color);
    if (!d2dBrush) return;
    
    D2D1_ELLIPSE ellipse = D2D1::Ellipse(PointToD2D(center), radiusX, radiusY);
    renderTarget->FillEllipse(ellipse, d2dBrush.Get());
}

void D2DRenderer::DrawText(const std::string& text, const Rect& rect, const Font& font, const Brush& brush, TextAlignment alignment) {
    if (!renderTarget || !writeFactory) return;
    
    // Convert text to wide string
    int textLen = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, nullptr, 0);
    std::wstring wideText(textLen, 0);
    MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, &wideText[0], textLen);
    
    // Get or create text format
    ComPtr<IDWriteTextFormat> textFormat = GetOrCreateTextFormat(font);
    if (!textFormat) return;
    
    // Set text alignment
    DWRITE_TEXT_ALIGNMENT dwriteAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
    switch (alignment) {
        case TextAlignment::Left:
            dwriteAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
            break;
        case TextAlignment::Center:
            dwriteAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
            break;
        case TextAlignment::Right:
            dwriteAlignment = DWRITE_TEXT_ALIGNMENT_TRAILING;
            break;
    }
    textFormat->SetTextAlignment(dwriteAlignment);
    
    // Get brush
    ComPtr<ID2D1SolidColorBrush> d2dBrush = GetOrCreateBrush(brush.color);
    if (d2dBrush) {
        renderTarget->DrawTextW(
            wideText.c_str(),
            (UINT32)wideText.length() - 1, // Exclude null terminator
            textFormat.Get(),
            RectToD2D(rect),
            d2dBrush.Get()
        );
    }
}

Size D2DRenderer::MeasureText(const std::string& text, const Font& font, float maxWidth) {
    if (!writeFactory) return Size(0, 0);
    
    // Convert text to wide string
    int textLen = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, nullptr, 0);
    std::wstring wideText(textLen, 0);
    MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, &wideText[0], textLen);
    
    // Get or create text format
    ComPtr<IDWriteTextFormat> textFormat = GetOrCreateTextFormat(font);
    if (!textFormat) return Size(0, 0);
    
    // Create text layout
    IDWriteTextLayout* textLayout = nullptr;
    HRESULT hr = writeFactory->CreateTextLayout(
        wideText.c_str(),
        (UINT32)wideText.length() - 1,
        textFormat.Get(),
        maxWidth > 0 ? maxWidth : 10000.0f,
        10000.0f,
        &textLayout
    );
    
    if (FAILED(hr) || !textLayout) return Size(0, 0);
    
    // Get metrics
    DWRITE_TEXT_METRICS metrics;
    hr = textLayout->GetMetrics(&metrics);
    textLayout->Release();
    
    if (FAILED(hr)) return Size(0, 0);
    
    return Size(metrics.width, metrics.height);
}

void D2DRenderer::PushClipRect(const Rect& rect) {
    if (renderTarget) {
        clipStack.push(RectToD2D(rect));
        renderTarget->PushAxisAlignedClip(RectToD2D(rect), D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
    }
}

void D2DRenderer::PopClipRect() {
    if (renderTarget && !clipStack.empty()) {
        clipStack.pop();
        renderTarget->PopAxisAlignedClip();
    }
}

void D2DRenderer::PushTransform() {
    if (renderTarget) {
        D2D1::Matrix3x2F currentTransform;
        renderTarget->GetTransform(&currentTransform);
        transformStack.push(currentTransform);
    }
}

void D2DRenderer::PopTransform() {
    if (renderTarget && !transformStack.empty()) {
        renderTarget->SetTransform(transformStack.top());
        transformStack.pop();
    }
}

void D2DRenderer::Translate(float x, float y) {
    if (renderTarget) {
        D2D1::Matrix3x2F currentTransform;
        renderTarget->GetTransform(&currentTransform);
        D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation(x, y);
        renderTarget->SetTransform(currentTransform * translation);
    }
}

void D2DRenderer::Scale(float x, float y) {
    if (renderTarget) {
        D2D1::Matrix3x2F currentTransform;
        renderTarget->GetTransform(&currentTransform);
        D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(x, y);
        renderTarget->SetTransform(currentTransform * scale);
    }
}

void D2DRenderer::Rotate(float angle) {
    if (renderTarget) {
        D2D1::Matrix3x2F currentTransform;
        renderTarget->GetTransform(&currentTransform);
        D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(angle);
        renderTarget->SetTransform(currentTransform * rotation);
    }
}

Size D2DRenderer::GetSize() const {
    if (!renderTarget) return Size(0, 0);
    
    D2D1_SIZE_F size = renderTarget->GetSize();
    return Size(size.width, size.height);
}

float D2DRenderer::GetDpiScale() const {
    return dpiScaleX; // Return X scale, assuming uniform scaling
}



void* D2DRenderer::CreateBrush(const Color& color) {
    auto brush = GetOrCreateBrush(color);
    return brush.Get();
}

void* D2DRenderer::CreatePen(const Color& color, float width) {
    // For D2D, pens are just brushes with stroke width handled separately
    auto brush = GetOrCreateBrush(color);
    return brush.Get();
}

void* D2DRenderer::CreateFont(const Font& font) {
    auto textFormat = GetOrCreateTextFormat(font);
    return textFormat.Get();
}

void D2DRenderer::ReleaseBrush(void* brush) {
    // Brushes are cached and released in Shutdown()
}

void D2DRenderer::ReleasePen(void* pen) {
    // Pens use brushes which are cached and released in Shutdown()
}

void D2DRenderer::ReleaseFont(void* font) {
    // Fonts are cached and released in Shutdown()
}

// Helper methods
D2D1_COLOR_F D2DRenderer::ColorToD2D(const Color& color) {
    return D2D1::ColorF(color.r, color.g, color.b, color.a);
}

D2D1_POINT_2F D2DRenderer::PointToD2D(const Point& point) {
    return D2D1::Point2F(point.x, point.y);
}

D2D1_RECT_F D2DRenderer::RectToD2D(const Rect& rect) {
    return D2D1::RectF(rect.x, rect.y, rect.x + rect.width, rect.y + rect.height);
}

ComPtr<ID2D1SolidColorBrush> D2DRenderer::GetOrCreateBrush(const Color& color) {
    if (!renderTarget) return nullptr;
    
    // Create a simple hash for the color
    size_t hash = std::hash<float>{}(color.r) ^ 
                 (std::hash<float>{}(color.g) << 1) ^
                 (std::hash<float>{}(color.b) << 2) ^
                 (std::hash<float>{}(color.a) << 3);
    
    auto it = brushCache.find(hash);
    if (it != brushCache.end()) {
        return it->second;
    }
    
    // Create new brush
    ID2D1SolidColorBrush* brush = nullptr;
    HRESULT hr = renderTarget->CreateSolidColorBrush(ColorToD2D(color), &brush);
    if (SUCCEEDED(hr)) {
        brushCache[hash] = brush;
        return brush;
    }
    
    return nullptr;
}

ComPtr<IDWriteTextFormat> D2DRenderer::GetOrCreateTextFormat(const Font& font) {
    if (!writeFactory) return nullptr;
    
    // Create a string key for the font
    std::string fontKey = font.family + "_" + std::to_string(font.size) + "_" + 
                         std::to_string(static_cast<int>(font.weight)) + "_" + 
                         std::to_string(static_cast<int>(font.style));
    
    auto it = fontCache.find(fontKey);
    if (it != fontCache.end()) {
        return it->second;
    }
    
    // Convert font family to wide string
    int familyLen = MultiByteToWideChar(CP_UTF8, 0, font.family.c_str(), -1, nullptr, 0);
    std::wstring wideFamily(familyLen, 0);
    MultiByteToWideChar(CP_UTF8, 0, font.family.c_str(), -1, &wideFamily[0], familyLen);
    
    // Convert font weight
    DWRITE_FONT_WEIGHT dwriteWeight = DWRITE_FONT_WEIGHT_NORMAL;
    switch (font.weight) {
        case FontWeight::Thin: dwriteWeight = DWRITE_FONT_WEIGHT_THIN; break;
        case FontWeight::Light: dwriteWeight = DWRITE_FONT_WEIGHT_LIGHT; break;
        case FontWeight::Normal: dwriteWeight = DWRITE_FONT_WEIGHT_NORMAL; break;
        case FontWeight::Bold: dwriteWeight = DWRITE_FONT_WEIGHT_BOLD; break;
        case FontWeight::Black: dwriteWeight = DWRITE_FONT_WEIGHT_BLACK; break;
    }
    
    // Convert font style
    DWRITE_FONT_STYLE dwriteStyle = DWRITE_FONT_STYLE_NORMAL;
    switch (font.style) {
        case FontStyle::Normal: dwriteStyle = DWRITE_FONT_STYLE_NORMAL; break;
        case FontStyle::Italic: dwriteStyle = DWRITE_FONT_STYLE_ITALIC; break;
        case FontStyle::Oblique: dwriteStyle = DWRITE_FONT_STYLE_OBLIQUE; break;
    }
    
    // Create text format
    IDWriteTextFormat* textFormat = nullptr;
    HRESULT hr = writeFactory->CreateTextFormat(
        wideFamily.c_str(),
        nullptr,
        dwriteWeight,
        dwriteStyle,
        DWRITE_FONT_STRETCH_NORMAL,
        font.size,
        L"en-us",
        &textFormat
    );
    
    if (SUCCEEDED(hr)) {
        fontCache[fontKey] = textFormat;
        return textFormat;
    }
    
    return nullptr;
}

// Factory function implementation
std::shared_ptr<Renderer> CreatePlatformRenderer() {
    return std::make_shared<D2DRenderer>();
}

} // namespace miko