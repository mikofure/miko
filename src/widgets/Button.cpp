#include "miko/widgets/Button.h"
#include "miko/core/Renderer.h"

namespace miko {

Button::Button(const std::string& text)
    : text(text)
    , font("Segoe UI", 12.0f, FontWeight::Normal, FontStyle::Normal)
    , textColor(Color::TextColor)
    , textAlignment(TextAlignment::Center)
    , normalColor(Color::ControlBackground)
    , hoverColor(Color::HoverColor)
    , pressedColor(Color::PressedColor)
    , disabledColor(Color::LightGray)
    , buttonState(ButtonState::Normal)
    , mousePressed(false)
{
    SetSize(Size(100, 30));
}



Size Button::MeasureDesiredSize(const Size& availableSize) {
    // Measure text size if we have a renderer context
    // For now, return a default size based on text length
    Size textSize(text.length() * 8.0f, 20.0f); // Rough estimate
    
    Size desiredSize(
        textSize.width + this->GetPadding().left + this->GetPadding().right + 20.0f,
        textSize.height + this->GetPadding().top + this->GetPadding().bottom + 10.0f
    );
    
    return Size(
        Clamp(desiredSize.width, this->GetMinSize().width, this->GetMaxSize().width),
        Clamp(desiredSize.height, this->GetMinSize().height, this->GetMaxSize().height)
    );
}

void Button::OnRender(std::shared_ptr<Renderer> renderer) {
    if (!IsVisible() || !renderer) return;
    
    // Determine button color based on state
    Color buttonColor = normalColor;
    if (!this->IsEnabled()) {
        buttonColor = disabledColor;
    } else if (mousePressed) {
        buttonColor = pressedColor;
    } else if (this->IsHovered()) {
        buttonColor = hoverColor;
    }
    
    // Draw button background
    Brush backgroundBrush(buttonColor);
    if (GetCornerRadius() > 0) {
        renderer->FillRoundedRectangle(GetBounds(), GetCornerRadius(), GetCornerRadius(), backgroundBrush);
    } else {
        renderer->FillRectangle(GetBounds(), backgroundBrush);
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
        Margin padding = this->GetPadding();
        Rect textRect(
            this->GetBounds().x + padding.left,
            this->GetBounds().y + padding.top,
            this->GetBounds().width - padding.left - padding.right,
            this->GetBounds().height - padding.top - padding.bottom
        );
        
        renderer->DrawText(text, textRect, font, textBrush, textAlignment);
    }
    
    // Render children
    for (auto& child : GetChildren()) {
        if (child && child->IsVisible()) {
            child->Render(renderer);
        }
    }
}

bool Button::OnMouseEvent(const MouseEvent& event) {
    if (!this->IsEnabled()) {
        return Widget::OnMouseEvent(event);
    }
    
    if (event.type == EventType::MouseButtonPressed && this->HitTest(event.position)) {
        mousePressed = true;
        buttonState = ButtonState::Pressed;
        return true;
    } else if (event.type == EventType::MouseButtonReleased) {
        if (mousePressed && this->HitTest(event.position)) {
            mousePressed = false;
            buttonState = ButtonState::Hovered;
            if (OnClick) {
                OnClick();
            }
            return true;
        } else {
            mousePressed = false;
            buttonState = ButtonState::Normal;
        }
    }
    
    return Widget::OnMouseEvent(event);
}

void Button::OnHoverEnter() {
    Widget::OnHoverEnter();
    if (buttonState == ButtonState::Normal) {
        buttonState = ButtonState::Hovered;
    }
}

void Button::OnHoverExit() {
    Widget::OnHoverExit();
    mousePressed = false;
    buttonState = ButtonState::Normal;
}

} // namespace miko