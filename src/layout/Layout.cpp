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
        // Get content rect accounting for margins
        Rect contentBounds = GetContentRect(bounds);
        
        float x = contentBounds.Left();
        float y = contentBounds.Top();
        
        // Apply horizontal alignment
        switch (hAlign) {
            case HorizontalAlignment::Left:
                x = contentBounds.Left();
                break;
            case HorizontalAlignment::Center:
                x = contentBounds.Left() + (contentBounds.GetSize().width - desiredSize.width) / 2.0f;
                break;
            case HorizontalAlignment::Right:
                x = contentBounds.Right() - desiredSize.width;
                break;
            case HorizontalAlignment::Stretch:
                x = contentBounds.Left();
                break;
        }
        
        // Apply vertical alignment
        switch (vAlign) {
            case VerticalAlignment::Top:
                y = contentBounds.Top();
                break;
            case VerticalAlignment::Center:
                y = contentBounds.Top() + (contentBounds.GetSize().height - desiredSize.height) / 2.0f;
                break;
            case VerticalAlignment::Bottom:
                y = contentBounds.Bottom() - desiredSize.height;
                break;
            case VerticalAlignment::Stretch:
                y = contentBounds.Top();
                break;
        }
        
        // Set result size based on alignment
        Size finalSize = desiredSize;
        if (hAlign == HorizontalAlignment::Stretch) {
            finalSize.width = contentBounds.GetSize().width;
        }
        if (vAlign == VerticalAlignment::Stretch) {
            finalSize.height = contentBounds.GetSize().height;
        }
        
        return Rect(x, y, finalSize.width, finalSize.height);
    }

    Size Layout::GetAvailableSize(const Size& containerSize) const {
        return Size(
            std::max(0.0f, containerSize.width - margin.Horizontal() - padding.Horizontal()),
            std::max(0.0f, containerSize.height - margin.Vertical() - padding.Vertical())
        );
    }

    Rect Layout::GetContentRect(const Rect& containerRect) const {
        return Rect(
            containerRect.Left() + margin.left + padding.left,
            containerRect.Top() + margin.top + padding.top,
            std::max(0.0f, containerRect.GetSize().width - margin.Horizontal() - padding.Horizontal()),
            std::max(0.0f, containerRect.GetSize().height - margin.Vertical() - padding.Vertical())
        );
    }

} // namespace miko