#include "miko/widgets/Panel.h"
#include "miko/core/Renderer.h"
#include "miko/layout/Layout.h"

namespace miko {

Panel::Panel()
    : clipChildren(false)
    , scrollable(false)
    , scrollOffset(0, 0)
    , contentSize(0, 0)
    , showHorizontalScrollBar(false)
    , showVerticalScrollBar(false)
{
    SetSize(Size(200, 150));
    SetBackgroundColor(Color::Transparent);
}



bool Panel::OnMouseEvent(const MouseEvent& event) {
    // Handle scroll bar interactions if scrollable
    if (scrollable) {
        // Placeholder for scroll bar handling
    }
    
    // Pass to base class
    return Widget::OnMouseEvent(event);
}

Size Panel::MeasureDesiredSize(const Size& availableSize) {
    auto layout = GetLayout();
    if (layout) {
        return layout->MeasureDesiredSize(GetChildren(), availableSize);
    }
    
    // Default behavior: measure children and find the minimum size needed
    Size desiredSize(0, 0);
    
    for (auto& child : GetChildren()) {
        if (child && child->IsVisible()) {
            Size childDesiredSize = child->MeasureDesiredSize(availableSize);
            
            // For panels without layout, we assume children are positioned manually
            // So we need to account for their position + size
            Point childPos = child->GetPosition();
            Margin childMargin = child->GetMargin();
            
            float childRight = childPos.x + childMargin.left + childDesiredSize.width + childMargin.right;
            float childBottom = childPos.y + childMargin.top + childDesiredSize.height + childMargin.bottom;
            
            desiredSize.width = std::max(desiredSize.width, childRight);
            desiredSize.height = std::max(desiredSize.height, childBottom);
        }
    }
    
    // Add padding
    Margin padding = GetPadding();
    desiredSize.width += padding.left + padding.right;
    desiredSize.height += padding.top + padding.bottom;
    
    return Size(
        Clamp(desiredSize.width, GetMinSize().width, GetMaxSize().width),
        Clamp(desiredSize.height, GetMinSize().height, GetMaxSize().height)
    );
}

void Panel::ArrangeChildren(const Rect& finalRect) {
    auto layout = GetLayout();
    if (layout) {
        // Use the layout to arrange children
        layout->ArrangeChildren(GetChildren(), finalRect);
    } else {
        // No layout - children maintain their manual positions
        // Just ensure they're positioned relative to this panel
        Margin padding = GetPadding();
        
        for (auto& child : GetChildren()) {
            if (child && child->IsVisible()) {
                Point childPos = child->GetPosition();
                Size childSize = child->GetSize();
                
                Rect childBounds(
                    finalRect.Left() + padding.left + childPos.x,
                    finalRect.Top() + padding.top + childPos.y,
                    childSize.width,
                    childSize.height
                );
                
                child->Arrange(childBounds);
            }
        }
    }
}

void Panel::OnRender(std::shared_ptr<Renderer> renderer) {
    if (!IsVisible() || !renderer) return;
    
    // Draw background if specified
    Color bgColor = GetBackgroundColor();
    if (bgColor.a > 0) {
        void* backgroundBrush = renderer->CreateBrush(bgColor);
        float cornerRadius = GetCornerRadius();
        if (cornerRadius > 0) {
            Brush brush;
            brush.color = bgColor;
            renderer->FillRoundedRectangle(GetBounds(), cornerRadius, cornerRadius, brush);
        } else {
            Brush brush;
            brush.color = bgColor;
            renderer->FillRectangle(GetBounds(), brush);
        }
        renderer->ReleaseBrush(backgroundBrush);
    }
    
    // Draw border if specified
    float borderWidth = GetBorderWidth();
    Color borderColor = GetBorderColor();
    if (borderWidth > 0 && borderColor.a > 0) {
        void* borderPen = renderer->CreatePen(borderColor, borderWidth);
        float cornerRadius = GetCornerRadius();
        if (cornerRadius > 0) {
            Pen pen;
            pen.color = borderColor;
            pen.width = borderWidth;
            renderer->DrawRoundedRectangle(GetBounds(), cornerRadius, cornerRadius, pen);
        } else {
            Pen pen;
            pen.color = borderColor;
            pen.width = borderWidth;
            renderer->DrawRectangle(GetBounds(), pen);
        }
        renderer->ReleasePen(borderPen);
    }
    
    // Render children using the RenderChildren method
    RenderChildren(renderer);
}

void Panel::SetScrollOffset(const Point& offset) {
    scrollOffset = offset;
    ClampScrollOffset();
    Invalidate();
}

void Panel::ScrollTo(const Point& position) {
    SetScrollOffset(position);
}

void Panel::ScrollBy(const Point& delta) {
    SetScrollOffset(Point(scrollOffset.x + delta.x, scrollOffset.y + delta.y));
}

void Panel::UpdateScrollBars() {
    // Implementation for scroll bar updates
    // This is a placeholder - full implementation would calculate scroll bar visibility and size
}

void Panel::OnScroll(const Point& delta) {
    ScrollBy(delta);
}

void Panel::ClampScrollOffset() {
    // Clamp scroll offset to valid range
    Size clientSize = GetSize();
    scrollOffset.x = std::max(0.0f, std::min(scrollOffset.x, std::max(0.0f, contentSize.width - clientSize.width)));
    scrollOffset.y = std::max(0.0f, std::min(scrollOffset.y, std::max(0.0f, contentSize.height - clientSize.height)));
}

bool Panel::IsPointInScrollBar(const Point& point, bool& isHorizontal) const {
    // Placeholder implementation
    isHorizontal = false;
    return false;
}

void Panel::HandleScrollBarClick(const Point& point, bool isHorizontal) {
    // Placeholder implementation
}

void Panel::RenderChildren(std::shared_ptr<Renderer> renderer) {
    if (!renderer) return;
    
    // Apply clipping if enabled
    if (clipChildren) {
        Rect bounds = GetBounds();
        renderer->PushClipRect(bounds);
    }
    
    // Apply scroll offset if scrollable
    if (scrollable) {
        renderer->PushTransform();
        renderer->Translate(-scrollOffset.x, -scrollOffset.y);
    }
    
    // Render all visible children
    for (auto& child : GetChildren()) {
        if (child && child->IsVisible()) {
            // Call the public Render method instead of protected OnRender
            child->Render(renderer);
        }
    }
    
    // Restore transforms
    if (scrollable) {
        renderer->PopTransform();
    }
    
    if (clipChildren) {
        renderer->PopClipRect();
    }
}

} // namespace miko