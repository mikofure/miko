#pragma once

#ifndef MIKO_BUTTON_H
#define MIKO_BUTTON_H

#include "Widget.h"
#include "../core/Renderer.h"

namespace miko {

    enum class ButtonState {
        Normal,
        Hovered,
        Pressed,
        Disabled
    };

    class Button : public Widget {
    public:
        Button();
        Button(const std::string& text);
        virtual ~Button() = default;
        
        // Text properties
        void SetText(const std::string& text);
        const std::string& GetText() const { return text; }
        
        void SetFont(const Font& font) { this->font = font; Invalidate(); }
        const Font& GetFont() const { return font; }
        
        void SetTextColor(const Color& color) { textColor = color; Invalidate(); }
        const Color& GetTextColor() const { return textColor; }
        
        void SetTextAlignment(TextAlignment alignment) { textAlignment = alignment; Invalidate(); }
        TextAlignment GetTextAlignment() const { return textAlignment; }
        
        // Button state colors
        void SetNormalColor(const Color& color) { normalColor = color; Invalidate(); }
        const Color& GetNormalColor() const { return normalColor; }
        
        void SetHoverColor(const Color& color) { hoverColor = color; Invalidate(); }
        const Color& GetHoverColor() const { return hoverColor; }
        
        void SetPressedColor(const Color& color) { pressedColor = color; Invalidate(); }
        const Color& GetPressedColor() const { return pressedColor; }
        
        void SetDisabledColor(const Color& color) { disabledColor = color; Invalidate(); }
        const Color& GetDisabledColor() const { return disabledColor; }
        
        // Button state
        ButtonState GetButtonState() const { return buttonState; }
        bool IsPressed() const { return buttonState == ButtonState::Pressed; }
        
        // Widget overrides
        bool OnMouseEvent(const MouseEvent& event) override;
        void OnHoverEnter() override;
        void OnHoverExit() override;
        Size MeasureDesiredSize(const Size& availableSize) override;
        
    protected:
        void OnRender(std::shared_ptr<Renderer> renderer) override;
        void UpdateButtonState();
        Color GetCurrentBackgroundColor() const;
        
    private:
        std::string text;
        Font font;
        Color textColor;
        TextAlignment textAlignment;
        
        // State colors
        Color normalColor;
        Color hoverColor;
        Color pressedColor;
        Color disabledColor;
        
        ButtonState buttonState;
        bool mousePressed;
        
        void Initialize();
    };

} // namespace miko

#endif // MIKO_BUTTON_H