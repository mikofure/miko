#pragma once

#ifndef MIKO_LABEL_H
#define MIKO_LABEL_H

#include "Widget.h"
#include "../core/Renderer.h"

namespace miko {

    class Label : public Widget {
    public:
        Label();
        Label(const std::string& text);
        virtual ~Label() = default;
        
        // Text properties
        void SetText(const std::string& text);
        const std::string& GetText() const { return text; }
        
        void SetFont(const Font& font) { this->font = font; Invalidate(); InvalidateLayout(); }
        const Font& GetFont() const { return font; }
        
        void SetTextColor(const Color& color) { textColor = color; Invalidate(); }
        const Color& GetTextColor() const { return textColor; }
        
        void SetTextAlignment(TextAlignment alignment) { textAlignment = alignment; Invalidate(); }
        TextAlignment GetTextAlignment() const { return textAlignment; }
        
        // Text wrapping
        void SetWordWrap(bool wrap) { wordWrap = wrap; Invalidate(); InvalidateLayout(); }
        bool GetWordWrap() const { return wordWrap; }
        
        void SetAutoSize(bool autoSize) { this->autoSize = autoSize; InvalidateLayout(); }
        bool GetAutoSize() const { return autoSize; }
        
        // Widget overrides
        Size MeasureDesiredSize(const Size& availableSize) override;
        
    protected:
        void OnRender(std::shared_ptr<Renderer> renderer) override;
        
    private:
        std::string text;
        Font font;
        Color textColor;
        TextAlignment textAlignment;
        bool wordWrap;
        bool autoSize;
        
        void Initialize();
        Rect GetTextRect() const;
    };

} // namespace miko

#endif // MIKO_LABEL_H