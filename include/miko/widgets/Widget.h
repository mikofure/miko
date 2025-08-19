#pragma once

#ifndef MIKO_WIDGET_H
#define MIKO_WIDGET_H

#include "../utils/Math.h"
#include "../utils/Color.h"
#include "../utils/Event.h"
#include "../core/Renderer.h"
#include <memory>
#include <vector>
#include <string>
#include <functional>

namespace miko {

    class Layout;

    enum class Visibility {
        Visible,
        Hidden,
        Collapsed
    };

    enum class HorizontalAlignment {
        Left,
        Center,
        Right,
        Stretch
    };

    enum class VerticalAlignment {
        Top,
        Center,
        Bottom,
        Stretch
    };

    class Widget : public std::enable_shared_from_this<Widget> {
    public:
        Widget();
        virtual ~Widget();
        
        // Hierarchy management
        void AddChild(std::shared_ptr<Widget> child);
        void RemoveChild(std::shared_ptr<Widget> child);
        void RemoveAllChildren();
        std::shared_ptr<Widget> GetParent() const { return parent.lock(); }
        const std::vector<std::shared_ptr<Widget>>& GetChildren() const { return children; }
        
        // Layout and positioning
        virtual void SetBounds(const Rect& bounds);
        const Rect& GetBounds() const { return bounds; }
        
        void SetPosition(const Point& position);
        Point GetPosition() const { return Point(bounds.x, bounds.y); }
        
        void SetSize(const Size& size);
        Size GetSize() const { return Size(bounds.width, bounds.height); }
        
    void SetMargin(const Spacing& margin) { this->margin = margin; InvalidateLayout(); }
    const Spacing& GetMargin() const { return margin; }

    void SetPadding(const Spacing& padding) { this->padding = padding; InvalidateLayout(); }
    const Spacing& GetPadding() const { return padding; }
        
        // Alignment
        void SetHorizontalAlignment(HorizontalAlignment alignment) { hAlignment = alignment; InvalidateLayout(); }
        HorizontalAlignment GetHorizontalAlignment() const { return hAlignment; }
        
        void SetVerticalAlignment(VerticalAlignment alignment) { vAlignment = alignment; InvalidateLayout(); }
        VerticalAlignment GetVerticalAlignment() const { return vAlignment; }
        
        // Size constraints
        void SetMinSize(const Size& size) { minSize = size; InvalidateLayout(); }
        const Size& GetMinSize() const { return minSize; }
        
        void SetMaxSize(const Size& size) { maxSize = size; InvalidateLayout(); }
        const Size& GetMaxSize() const { return maxSize; }
        
        // Visibility and state
        void SetVisibility(Visibility visibility);
        Visibility GetVisibility() const { return visibility; }
        bool IsVisible() const { return visibility == Visibility::Visible; }
        
        void SetEnabled(bool enabled);
        bool IsEnabled() const { return enabled; }
        
        void SetFocused(bool focused);
        bool IsFocused() const { return focused; }
        
        void SetHovered(bool hovered);
        bool IsHovered() const { return hovered; }
        
        // Appearance
        void SetBackgroundColor(const Color& color) { backgroundColor = color; Invalidate(); }
        const Color& GetBackgroundColor() const { return backgroundColor; }
        
        void SetBorderColor(const Color& color) { borderColor = color; Invalidate(); }
        const Color& GetBorderColor() const { return borderColor; }
        
        void SetBorderWidth(float width) { borderWidth = width; Invalidate(); }
        float GetBorderWidth() const { return borderWidth; }
        
        void SetCornerRadius(float radius) { cornerRadius = radius; Invalidate(); }
        float GetCornerRadius() const { return cornerRadius; }
        
        // Layout management
        void SetLayout(std::shared_ptr<Layout> layout);
        std::shared_ptr<Layout> GetLayout() const { return layout; }
        
        // Rendering
        virtual void Render(std::shared_ptr<Renderer> renderer);
        void Invalidate();
        void InvalidateLayout();
        
        // Event handling
        virtual bool OnMouseEvent(const MouseEvent& event);
        virtual bool OnKeyEvent(const KeyEvent& event);
        virtual void OnFocusGained() {}
        virtual void OnFocusLost() {}
        virtual void OnHoverEnter() {}
        virtual void OnHoverExit() {}
        
        // Hit testing
        virtual bool HitTest(const Point& point) const;
        std::shared_ptr<Widget> FindWidgetAt(const Point& point);
        
        // Measurement and layout// Layout helpers
        virtual Size MeasureDesiredSize(const Size& availableSize);
        virtual void ArrangeChildren(const Rect& finalRect);
        virtual void Arrange(const Rect& finalRect);
        
        // Utility
        Point LocalToGlobal(const Point& localPoint) const;
        Point GlobalToLocal(const Point& globalPoint) const;
        Rect GetClientRect() const;
        
        // Properties
        void SetName(const std::string& name) { this->name = name; }
        const std::string& GetName() const { return name; }
        
        void SetTag(void* tag) { this->tag = tag; }
        void* GetTag() const { return tag; }
        
        // Event callbacks
        std::function<void()> OnClick;
        std::function<void()> OnDoubleClick;
        std::function<void()> OnRightClick;
        std::function<void(const MouseEvent&)> OnMouseMove;
        std::function<void(const KeyEvent&)> OnKeyPress;
        
    protected:
        // Virtual rendering methods
        virtual void OnRender(std::shared_ptr<Renderer> renderer) {}
        virtual void RenderBackground(std::shared_ptr<Renderer> renderer);
        virtual void RenderBorder(std::shared_ptr<Renderer> renderer);
        virtual void RenderChildren(std::shared_ptr<Renderer> renderer);
        
        // Layout helpers
        virtual void UpdateLayout();
        virtual Size CalculateDesiredSize(const Size& availableSize);
        
    private:
        std::weak_ptr<Widget> parent;
        std::vector<std::shared_ptr<Widget>> children;
        std::shared_ptr<Layout> layout;
        
        // Geometry
        Rect bounds;
    Spacing margin;
    Spacing padding;
        Size minSize;
        Size maxSize;
        
        // Alignment
        HorizontalAlignment hAlignment;
        VerticalAlignment vAlignment;
        
        // State
        Visibility visibility;
        bool enabled;
        bool focused;
        bool hovered;
        bool layoutInvalid;
        bool renderInvalid;
        
        // Appearance
        Color backgroundColor;
        Color borderColor;
        float borderWidth;
        float cornerRadius;
        
        // Properties
        std::string name;
        void* tag;
        
        void SetParent(std::shared_ptr<Widget> parent) { this->parent = parent; }
        friend class Layout;
    };

} // namespace miko

#endif // MIKO_WIDGET_H