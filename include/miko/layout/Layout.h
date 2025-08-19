#pragma once

#ifndef MIKO_LAYOUT_H
#define MIKO_LAYOUT_H

#include "../utils/Math.h"
#include <memory>
#include <vector>

namespace miko {

    class Widget;
    enum class HorizontalAlignment;
    enum class VerticalAlignment;

    class Layout {
    public:
        Layout() = default;
        virtual ~Layout() = default;
        
        // Layout interface
        virtual Size MeasureDesiredSize(const std::vector<std::shared_ptr<Widget>>& children, const Size& availableSize) = 0;
        virtual void ArrangeChildren(const std::vector<std::shared_ptr<Widget>>& children, const Rect& finalRect) = 0;
        
        // Layout properties
    void SetSpacing(float spacing) { this->spacing = spacing; }
    float GetSpacing() const { return spacing; }

    void SetMargin(const Margin& margin) { this->margin = margin; }
    const Margin& GetMargin() const { return margin; }

    void SetPadding(const Padding& padding) { this->padding = padding; }
    const Padding& GetPadding() const { return padding; }
        
    protected:
        float spacing = 0.0f;
        Margin margin;
        Padding padding;
        
        // Helper methods
    Size ApplyConstraints(const Size& desiredSize, const Size& minSize, const Size& maxSize) const;
    Rect ApplyAlignment(const Rect& bounds, const Size& desiredSize, HorizontalAlignment hAlign, VerticalAlignment vAlign) const;
    Size GetAvailableSize(const Size& containerSize) const;
    Rect GetContentRect(const Rect& containerRect) const;
    };

} // namespace miko

#endif // MIKO_LAYOUT_H