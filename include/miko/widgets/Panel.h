#pragma once

#ifndef MIKO_PANEL_H
#define MIKO_PANEL_H

#include "Widget.h"

namespace miko {

    class Panel : public Widget {
    public:
        Panel();
        virtual ~Panel() = default;
        
        // Panel-specific properties
        void SetClipChildren(bool clip) { clipChildren = clip; Invalidate(); }
        bool GetClipChildren() const { return clipChildren; }
        
        void SetScrollable(bool scrollable) { this->scrollable = scrollable; }
        bool IsScrollable() const { return scrollable; }
        
        // Scrolling
        void SetScrollOffset(const Point& offset);
        const Point& GetScrollOffset() const { return scrollOffset; }
        
        void ScrollTo(const Point& position);
        void ScrollBy(const Point& delta);
        
        // Content size (for scrolling)
        void SetContentSize(const Size& size) { contentSize = size; UpdateScrollBars(); }
        const Size& GetContentSize() const { return contentSize; }
        
        // Widget overrides
        bool OnMouseEvent(const MouseEvent& event) override;
        Size MeasureDesiredSize(const Size& availableSize) override;
        void ArrangeChildren(const Rect& finalRect) override;
        
    protected:
        void OnRender(std::shared_ptr<Renderer> renderer) override;
        void RenderChildren(std::shared_ptr<Renderer> renderer) override;
        
        // Scrolling helpers
        virtual void UpdateScrollBars();
        virtual void OnScroll(const Point& delta);
        
    private:
        bool clipChildren;
        bool scrollable;
        Point scrollOffset;
        Size contentSize;
        
        // Scroll bars (if needed)
        bool showHorizontalScrollBar;
        bool showVerticalScrollBar;
        Rect horizontalScrollBarRect;
        Rect verticalScrollBarRect;
        
        void Initialize();
        void ClampScrollOffset();
        bool IsPointInScrollBar(const Point& point, bool& isHorizontal) const;
        void HandleScrollBarClick(const Point& point, bool isHorizontal);
    };

} // namespace miko

#endif // MIKO_PANEL_H