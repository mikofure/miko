#pragma once

#ifndef MIKO_STACKLAYOUT_H
#define MIKO_STACKLAYOUT_H

#include "Layout.h"
#include "../widgets/Widget.h"

namespace miko {

    enum class Orientation {
        Horizontal,
        Vertical
    };
    
    enum class StackAlignment {
        Start,
        Center,
        End
    };

    class StackLayout : public Layout {
    public:
        StackLayout(Orientation orientation = Orientation::Vertical);
        virtual ~StackLayout() = default;
        
        // Layout interface
        Size MeasureDesiredSize(const std::vector<std::shared_ptr<Widget>>& children, const Size& availableSize) override;
        void ArrangeChildren(const std::vector<std::shared_ptr<Widget>>& children, const Rect& finalRect) override;
        
        // StackLayout properties
        void SetOrientation(Orientation orientation) { this->orientation = orientation; }
        Orientation GetOrientation() const { return orientation; }
        
        void SetFillLastChild(bool fill) { fillLastChild = fill; }
        bool GetFillLastChild() const { return fillLastChild; }
        
        void SetVerticalAlignment(StackAlignment alignment) { verticalAlignment = alignment; }
        StackAlignment GetVerticalAlignment() const { return verticalAlignment; }
        
        void SetHorizontalAlignment(StackAlignment alignment) { horizontalAlignment = alignment; }
        StackAlignment GetHorizontalAlignment() const { return horizontalAlignment; }
        
    private:
        Orientation orientation;
        bool fillLastChild;
        StackAlignment verticalAlignment;
        StackAlignment horizontalAlignment;
        
        Size MeasureHorizontal(const std::vector<std::shared_ptr<Widget>>& children, const Size& availableSize);
        Size MeasureVertical(const std::vector<std::shared_ptr<Widget>>& children, const Size& availableSize);
        
        void ArrangeHorizontal(const std::vector<std::shared_ptr<Widget>>& children, const Rect& finalRect);
        void ArrangeVertical(const std::vector<std::shared_ptr<Widget>>& children, const Rect& finalRect);
    };

} // namespace miko

#endif // MIKO_STACKLAYOUT_H