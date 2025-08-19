#include "miko/layout/StackLayout.h"
#include "miko/widgets/Widget.h"
#include <algorithm>

namespace miko {

    StackLayout::StackLayout(Orientation orientation)
        : orientation(orientation)
        , fillLastChild(false)
        , verticalAlignment(StackAlignment::Start)
        , horizontalAlignment(StackAlignment::Start)
    {
    }

    Size StackLayout::MeasureDesiredSize(const std::vector<std::shared_ptr<Widget>>& children, const Size& availableSize) {
        if (children.empty()) {
            return Size(0, 0);
        }

        if (orientation == Orientation::Horizontal) {
            return MeasureHorizontal(children, availableSize);
        } else {
            return MeasureVertical(children, availableSize);
        }
    }

    void StackLayout::ArrangeChildren(const std::vector<std::shared_ptr<Widget>>& children, const Rect& finalRect) {
        if (children.empty()) {
            return;
        }

        if (orientation == Orientation::Horizontal) {
            ArrangeHorizontal(children, finalRect);
        } else {
            ArrangeVertical(children, finalRect);
        }
    }

    Size StackLayout::MeasureHorizontal(const std::vector<std::shared_ptr<Widget>>& children, const Size& availableSize) {
        float totalWidth = 0;
        float maxHeight = 0;
        
        for (const auto& child : children) {
            if (child) {
                Size childDesiredSize = child->MeasureDesiredSize(availableSize);
                totalWidth += childDesiredSize.width;
                maxHeight = std::max(maxHeight, childDesiredSize.height);
                
                if (&child != &children.back()) {
                    totalWidth += spacing;
                }
            }
        }
        
        return Size(totalWidth, maxHeight);
    }

    Size StackLayout::MeasureVertical(const std::vector<std::shared_ptr<Widget>>& children, const Size& availableSize) {
        float maxWidth = 0;
        float totalHeight = 0;
        
        for (const auto& child : children) {
            if (child) {
                Size childDesiredSize = child->MeasureDesiredSize(availableSize);
                maxWidth = std::max(maxWidth, childDesiredSize.width);
                totalHeight += childDesiredSize.height;
                
                if (&child != &children.back()) {
                    totalHeight += spacing;
                }
            }
        }
        
        return Size(maxWidth, totalHeight);
    }

    void StackLayout::ArrangeHorizontal(const std::vector<std::shared_ptr<Widget>>& children, const Rect& finalRect) {
        float currentX = finalRect.Left();
        float availableWidth = finalRect.GetSize().width;
        
        // Calculate total width needed by all children except the last one (if fillLastChild is true)
        float usedWidth = 0;
        size_t childCount = children.size();
        
        for (size_t i = 0; i < childCount; ++i) {
            const auto& child = children[i];
            if (!child) continue;
            
            if (fillLastChild && i == childCount - 1) {
                // Skip the last child for now
                continue;
            }
            
            Size childDesiredSize = child->MeasureDesiredSize(Size(availableWidth, finalRect.GetSize().height));
            usedWidth += childDesiredSize.width;
            
            if (i < childCount - 1) {
                usedWidth += spacing;
            }
        }
        
        // Arrange children
        for (size_t i = 0; i < childCount; ++i) {
            const auto& child = children[i];
            if (!child) continue;
            
            float childWidth;
            
            if (fillLastChild && i == childCount - 1) {
                // Last child fills remaining space
                childWidth = std::max(0.0f, availableWidth - usedWidth);
            } else {
                Size childDesiredSize = child->MeasureDesiredSize(Size(availableWidth, finalRect.GetSize().height));
                childWidth = childDesiredSize.width;
            }
            
            float childHeight = finalRect.GetSize().height;
            
            Rect childBounds(
                currentX,
                finalRect.Top(),
                childWidth,
                childHeight
            );
            
            child->ArrangeChildren(childBounds);
            currentX += childWidth + spacing;
        }
    }

    void StackLayout::ArrangeVertical(const std::vector<std::shared_ptr<Widget>>& children, const Rect& finalRect) {
        float availableHeight = finalRect.GetSize().height;
        
        // Calculate total height needed by all children except the last one (if fillLastChild is true)
        float usedHeight = 0;
        size_t childCount = children.size();
        
        for (size_t i = 0; i < childCount; ++i) {
            const auto& child = children[i];
            if (!child) continue;
            
            if (fillLastChild && i == childCount - 1) {
                // Skip the last child for now
                continue;
            }
            
            Size childDesiredSize = child->MeasureDesiredSize(Size(finalRect.GetSize().width, availableHeight));
            usedHeight += childDesiredSize.height;
            
            if (i < childCount - 1 && !(fillLastChild && i == childCount - 2)) {
                usedHeight += spacing;
            }
        }
        
        // Calculate starting Y position based on vertical alignment
        float currentY = finalRect.Top();
        if (verticalAlignment == StackAlignment::Center) {
            float remainingHeight = availableHeight - usedHeight;
            currentY += remainingHeight / 2.0f;
        } else if (verticalAlignment == StackAlignment::End) {
            float remainingHeight = availableHeight - usedHeight;
            currentY += remainingHeight;
        }
        
        // Arrange children
        for (size_t i = 0; i < childCount; ++i) {
            const auto& child = children[i];
            if (!child) continue;
            
            float childHeight;
            
            if (fillLastChild && i == childCount - 1) {
                // Last child fills remaining space
                childHeight = std::max(0.0f, availableHeight - usedHeight);
            } else {
                Size childDesiredSize = child->MeasureDesiredSize(Size(finalRect.GetSize().width, availableHeight));
                childHeight = childDesiredSize.height;
            }
            
            float childWidth = finalRect.GetSize().width;
            
            Rect childBounds(
                finalRect.Left(),
                currentY,
                childWidth,
                childHeight
            );
            
            child->ArrangeChildren(childBounds);
            currentY += childHeight;
            
            // Add spacing only between children, not after the last one
            if (i < childCount - 1) {
                currentY += spacing;
            }
        }
    }

} // namespace miko