#include "miko/widgets/Label.h"
#include "miko/core/Renderer.h"

namespace miko {

Label::Label(const std::string& text)
    : text(text)
    , font("Segoe UI", 12.0f, FontWeight::Normal, FontStyle::Normal)
    , textColor(Color::TextColor)
    , textAlignment(TextAlignment::Left)
    , wordWrap(false)
    , autoSize(false)
{
    SetSize(Size(100, 20));
}

void Label::SetText(const std::string& text) {
    if (this->text != text) {
        this->text = text;
        Invalidate();
        InvalidateLayout();
    }
}



Size Label::MeasureDesiredSize(const Size& availableSize) {
    if (text.empty()) {
        return Size(0, 0);
    }
    
    // Rough text measurement (in a real implementation, this would use the renderer)
    float charWidth = font.size * 0.6f; // Approximate character width
    float lineHeight = font.size * 1.2f; // Approximate line height
    
    Size textSize;
    
    if (wordWrap && availableSize.width > 0) {
        // Calculate wrapped text size
        float maxWidth = availableSize.width - GetPadding().left - GetPadding().right;
        int charsPerLine = (int)(maxWidth / charWidth);
        if (charsPerLine <= 0) charsPerLine = 1;
        
        int lines = ((int)text.length() + charsPerLine - 1) / charsPerLine;
        if (lines == 0) lines = 1;
        
        textSize.width = std::min(maxWidth, (float)text.length() * charWidth);
        textSize.height = lines * lineHeight;
    } else {
        // Single line text
        textSize.width = text.length() * charWidth;
        textSize.height = lineHeight;
    }
    
    Size desiredSize(
        textSize.width + GetPadding().left + GetPadding().right,
        textSize.height + GetPadding().top + GetPadding().bottom
    );
    
    if (autoSize) {
        return desiredSize;
    }
    
    return Size(
        Clamp(desiredSize.width, GetMinSize().width, GetMaxSize().width),
        Clamp(desiredSize.height, GetMinSize().height, GetMaxSize().height)
    );
}

void Label::OnRender(std::shared_ptr<Renderer> renderer) {
    if (!IsVisible() || !renderer) return;
    
    // Draw background if specified
    if (GetBackgroundColor().a > 0) {
        Brush backgroundBrush(GetBackgroundColor());
        if (GetCornerRadius() > 0) {
            renderer->FillRoundedRectangle(GetBounds(), GetCornerRadius(), GetCornerRadius(), backgroundBrush);
        } else {
            renderer->FillRectangle(GetBounds(), backgroundBrush);
        }
    }
    
    // Draw border if specified
    if (GetBorderWidth() > 0 && GetBorderColor().a > 0) {
        Pen borderPen(GetBorderColor(), GetBorderWidth());
        if (GetCornerRadius() > 0) {
            renderer->DrawRoundedRectangle(GetBounds(), GetCornerRadius(), GetCornerRadius(), borderPen);
        } else {
            renderer->DrawRectangle(GetBounds(), borderPen);
        }
    }
    
    // Draw text
    if (!text.empty()) {
        Brush textBrush(textColor);
        // Calculate text area (excluding padding)
        const Spacing& padding = this->GetPadding();
        Rect textRect(
            this->GetBounds().x + padding.left,
            this->GetBounds().y + padding.top,
            this->GetBounds().width - padding.left - padding.right,
            this->GetBounds().height - padding.top - padding.bottom
        );
        renderer->DrawText(text, textRect, font, textBrush, textAlignment);
    }
    
    // Render children
    for (auto& child : this->GetChildren()) {
        if (child && child->IsVisible()) {
            child->Render(renderer);
        }
    }
}

} // namespace miko