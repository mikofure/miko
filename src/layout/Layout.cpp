#include "miko/layout/Layout.h"
#include "miko/widgets/Widget.h"
#include <algorithm>

namespace miko {

    Size Layout::ApplyConstraints(const Size& desiredSize, const Size& minSize, const Size& maxSize) const {
        return Size(
            std::max(minSize.width, std::min(desiredSize.width, maxSize.width)),
            std::max(minSize.height, std::min(desiredSize.height, maxSize.height))
        );
    }

    Rect Layout::ApplyAlignment(const Rect& bounds, const Size& desiredSize, 
                               HorizontalAlignment hAlign, VerticalAlignment vAlign) const {
        float x = bounds.Left();
        float y = bounds.Top();
        
        // Apply horizontal alignment
        switch (hAlign) {
            case HorizontalAlignment::Left:
                x = bounds.Left();
                break;
            case HorizontalAlignment::Center:
                x = bounds.Left() + (bounds.GetSize().width - desiredSize.width) / 2.0f;
                break;
            case HorizontalAlignment::Right:
                x = bounds.Right() - desiredSize.width;
                break;
            case HorizontalAlignment::Stretch:
                x = bounds.Left();
                break;
        }
        
        // Apply vertical alignment
        switch (vAlign) {
            case VerticalAlignment::Top:
                y = bounds.Top();
                break;
            case VerticalAlignment::Center:
                y = bounds.Top() + (bounds.GetSize().height - desiredSize.height) / 2.0f;
                break;
            case VerticalAlignment::Bottom:
                y = bounds.Bottom() - desiredSize.height;
                break;
            case VerticalAlignment::Stretch:
                y = bounds.Top();
                break;
        }
        
        // Set result size based on alignment
        Size finalSize = desiredSize;
        if (hAlign == HorizontalAlignment::Stretch) {
            finalSize.width = bounds.GetSize().width;
        }
        if (vAlign == VerticalAlignment::Stretch) {
            finalSize.height = bounds.GetSize().height;
        }
        
        return Rect(x, y, finalSize.width, finalSize.height);
    }

    Size Layout::GetAvailableSize(const Size& containerSize) const {
        return Size(
            std::max(0.0f, containerSize.width - margin.left - margin.right),
            std::max(0.0f, containerSize.height - margin.top - margin.bottom)
        );
    }

    Rect Layout::GetContentRect(const Rect& containerRect) const {
        return Rect(
            containerRect.Left() + margin.left,
            containerRect.Top() + margin.top,
            std::max(0.0f, containerRect.GetSize().width - margin.left - margin.right),
            std::max(0.0f, containerRect.GetSize().height - margin.top - margin.bottom)
        );
    }

} // namespace miko