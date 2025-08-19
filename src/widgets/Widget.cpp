#include "miko/widgets/Widget.h"
#include "miko/core/Renderer.h"
#include "miko/layout/Layout.h"
#include <algorithm>

namespace miko {

Widget::Widget()
    : bounds(0, 0, 0, 0)
    , margin(0, 0, 0, 0)
    , padding(0, 0, 0, 0)
    , minSize(0, 0)
    , maxSize(10000, 10000)
    , hAlignment(HorizontalAlignment::Left)
    , vAlignment(VerticalAlignment::Top)
    , visibility(Visibility::Visible)
    , enabled(true)
    , focused(false)
    , hovered(false)
    , backgroundColor(Color::Transparent)
    , borderColor(Color::Transparent)
    , borderWidth(0.0f)
    , cornerRadius(0.0f)
    , layout(nullptr)
    , OnClick(nullptr)
    , OnMouseMove(nullptr)
    , OnKeyPress(nullptr)
{
}

Widget::~Widget() = default;

void Widget::AddChild(std::shared_ptr<Widget> child) {
    if (!child || child->parent.lock()) return;
    
    child->parent = shared_from_this();
    children.push_back(child);
    
    InvalidateLayout();
}

void Widget::RemoveChild(std::shared_ptr<Widget> child) {
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
        children.erase(it);
        child->parent.reset();
        InvalidateLayout();
    }
}

void Widget::RemoveAllChildren() {
    for (auto& child : children) {
        child->parent.reset();
    }
    children.clear();
    InvalidateLayout();
}

Size Widget::MeasureDesiredSize(const Size& availableSize) {
    if (layout) {
        // Use layout to measure desired size
        Size layoutSize = layout->MeasureDesiredSize(children, availableSize);
        
        // Apply padding
        Size desiredSize = Size(
            layoutSize.width + padding.left + padding.right,
            layoutSize.height + padding.top + padding.bottom
        );
        
        // Apply constraints
        Size constrainedSize = Size(
            std::max(minSize.width, std::min(maxSize.width, desiredSize.width)),
            std::max(minSize.height, std::min(maxSize.height, desiredSize.height))
        );
        
        return constrainedSize;
    }
    
    // Default implementation - return minimum size
    return GetMinSize();
}

void Widget::SetBounds(const Rect& bounds) {
    this->bounds = bounds;
    InvalidateLayout();
}

bool Widget::HitTest(const Point& point) const {
    return bounds.Contains(point);
}

void Widget::SetSize(const Size& size) {
    bounds.width = size.width;
    bounds.height = size.height;
    InvalidateLayout();
}

void Widget::Invalidate() {
    // Default implementation - mark for redraw
    // This would typically notify the parent window to redraw this widget
}

void Widget::ArrangeChildren(const Rect& finalRect) {
    // Default implementation - arrange children using layout if available
    if (layout) {
        layout->ArrangeChildren(children, finalRect);
    }
}

void Widget::Arrange(const Rect& finalRect) {
    SetBounds(finalRect);
    
    if (layout) {
        // Calculate content area (excluding padding)
        Rect contentRect(
            padding.left,
            padding.top,
            finalRect.width - padding.left - padding.right,
            finalRect.height - padding.top - padding.bottom
        );
        
        layout->ArrangeChildren(children, contentRect);
    }
}

void Widget::SetVisibility(Visibility visibility) {
    if (this->visibility != visibility) {
        this->visibility = visibility;
        Invalidate();
        InvalidateLayout();
    }
}

void Widget::SetEnabled(bool enabled) {
    if (this->enabled != enabled) {
        this->enabled = enabled;
        Invalidate();
    }
}

void Widget::SetFocused(bool focused) {
    if (this->focused != focused) {
        this->focused = focused;
        if (focused) {
            OnFocusGained();
        } else {
            OnFocusLost();
        }
    }
}

void Widget::SetHovered(bool hovered) {
        if (this->hovered != hovered) {
            this->hovered = hovered;
            if (hovered) {
                OnHoverEnter();
            } else {
                OnHoverExit();
            }
        }
    }
    
    void Widget::SetLayout(std::shared_ptr<Layout> layout) {
        this->layout = layout;
        InvalidateLayout();
    }
    
    Point Widget::LocalToGlobal(const Point& localPoint) const {
        Point globalPoint = localPoint;
        globalPoint.x += bounds.x;
        globalPoint.y += bounds.y;
        
        auto parentWidget = parent.lock();
        if (parentWidget) {
            globalPoint = parentWidget->LocalToGlobal(globalPoint);
        }
        
        return globalPoint;
    }
    
    Point Widget::GlobalToLocal(const Point& globalPoint) const {
        Point localPoint = globalPoint;
        
        auto parentWidget = parent.lock();
        if (parentWidget) {
            localPoint = parentWidget->GlobalToLocal(localPoint);
        }
        
        localPoint.x -= bounds.x;
        localPoint.y -= bounds.y;
        
        return localPoint;
    }
    
    Rect Widget::GetClientRect() const {
        return Rect(padding.left, padding.top, 
                   bounds.width - padding.Horizontal(), 
                   bounds.height - padding.Vertical());
    }
    
    void Widget::RenderBackground(std::shared_ptr<Renderer> renderer) {
        if (backgroundColor.a > 0.0f) {
            Brush brush(backgroundColor);
            renderer->FillRectangle(bounds, brush);
        }
    }
    
    void Widget::RenderBorder(std::shared_ptr<Renderer> renderer) {
        if (borderWidth > 0.0f && borderColor.a > 0.0f) {
            Pen pen(borderColor, borderWidth);
            renderer->DrawRectangle(bounds, pen);
        }
    }
    
    void Widget::RenderChildren(std::shared_ptr<Renderer> renderer) {
        for (auto& child : children) {
            if (child && child->IsVisible()) {
                child->Render(renderer);
            }
        }
    }
    
    void Widget::UpdateLayout() {
        if (layoutInvalid) {
            if (layout) {
                Size availableSize = Size(bounds.width - padding.Horizontal(), 
                                         bounds.height - padding.Vertical());
                layout->MeasureDesiredSize(children, availableSize);
                
                Rect contentRect = GetClientRect();
                layout->ArrangeChildren(children, contentRect);
            }
            layoutInvalid = false;
        }
    }
    
    Size Widget::CalculateDesiredSize(const Size& availableSize) {
        Size desiredSize = minSize;
        
        if (layout) {
            Size layoutSize = layout->MeasureDesiredSize(children, availableSize);
            desiredSize.width = std::max(desiredSize.width, layoutSize.width + padding.Horizontal());
            desiredSize.height = std::max(desiredSize.height, layoutSize.height + padding.Vertical());
        }
        
        // Apply max size constraints
        if (maxSize.width > 0.0f) {
            desiredSize.width = std::min(desiredSize.width, maxSize.width);
        }
        if (maxSize.height > 0.0f) {
            desiredSize.height = std::min(desiredSize.height, maxSize.height);
        }
        
        return desiredSize;
    }
bool Widget::OnMouseEvent(const MouseEvent& event) {
    // Update hover state
    bool wasHovered = hovered;
    hovered = HitTest(event.position);
    
    if (hovered && !wasHovered) {
        OnHoverEnter();
    } else if (!hovered && wasHovered) {
        OnHoverExit();
    }
    
    if (OnMouseMove) {
        OnMouseMove(event);
    }
    
    // Propagate to children
    for (auto& child : children) {
        if (child && child->IsVisible()) {
            if (child->OnMouseEvent(event)) {
                return true;
            }
        }
    }
    
    return false;
}

bool Widget::OnKeyEvent(const KeyEvent& event) {
    // Default implementation - propagate to focused child
    for (auto& child : children) {
        if (child && child->IsFocused()) {
            if (child->OnKeyEvent(event)) {
                return true;
            }
        }
    }
    
    if (OnKeyPress) {
        OnKeyPress(event);
        return true;
    }
    
    return false;
}



void Widget::InvalidateLayout() {
    // Mark layout as needing update
    // In a real implementation, this would trigger a layout pass
}

void Widget::Render(std::shared_ptr<Renderer> renderer) {
    if (!this->IsVisible() || !renderer) return;
    
    // Call the protected OnRender method
    OnRender(renderer);
}

} // namespace miko