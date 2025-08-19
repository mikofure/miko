#include "miko/layout/StackLayout.h"
#include "miko/widgets/Widget.h"
#include <algorithm>
#include <cassert>

namespace miko {

    StackLayout::StackLayout(Orientation orientation)
        : orientation(orientation)
        , fillLastChild(false)
        , verticalAlignment(StackAlignment::Start)
        , horizontalAlignment(StackAlignment::Start)
    {
    }

    // Utility helper implementations
    float StackLayout::CalculateTotalSpacing(size_t childCount) const {
        return childCount > 1 ? spacing * static_cast<float>(childCount - 1) : 0.0f;
    }

    bool StackLayout::IsValidChild(const std::shared_ptr<Widget>& child) {
        return child != nullptr;
    }

    float StackLayout::CalculateAlignmentOffset(float totalSize, float usedSize, StackAlignment alignment) {
        const float remainingSize = totalSize - usedSize;
        
        switch (alignment) {
            case StackAlignment::Center:
                return remainingSize * 0.5f;
            case StackAlignment::End:
                return remainingSize;
            case StackAlignment::Start:
            default:
                return 0.0f;
        }
    }

    Size StackLayout::MeasureDesiredSize(const std::vector<std::shared_ptr<Widget>>& children, const Size& availableSize) {
        // Validate input parameters
        if (availableSize.width < 0.0f || availableSize.height < 0.0f) {
            assert(false && "Available size cannot be negative");
            return Size(0.0f, 0.0f);
        }
        
        // Early return for empty container
        if (children.empty()) {
            return Size(0.0f, 0.0f);
        }

        // Delegate to orientation-specific measurement
        return (orientation == Orientation::Horizontal) 
            ? MeasureHorizontal(children, availableSize)
            : MeasureVertical(children, availableSize);
    }

    void StackLayout::ArrangeChildren(const std::vector<std::shared_ptr<Widget>>& children, const Rect& finalRect) {
        // Validate input parameters
        const Size rectSize = finalRect.GetSize();
        if (rectSize.width < 0.0f || rectSize.height < 0.0f) {
            assert(false && "Final rectangle size cannot be negative");
            return;
        }
        
        // Early return for empty container
        if (children.empty()) {
            return;
        }

        // Delegate to orientation-specific arrangement
        if (orientation == Orientation::Horizontal) {
            ArrangeHorizontal(children, finalRect);
        } else {
            ArrangeVertical(children, finalRect);
        }
    }

    Size StackLayout::MeasureHorizontal(const std::vector<std::shared_ptr<Widget>>& children, const Size& availableSize) {
        float totalRequiredWidth = 0.0f;
        float maxRequiredHeight = 0.0f;
        size_t validChildCount = 0;
        
        // Measure all valid children to determine total space requirements
        for (const auto& child : children) {
            if (!IsValidChild(child)) {
                continue;
            }
            // Add margin to each child's desired size
            const Size childDesiredSize = child->MeasureDesiredSize(availableSize);
            const auto& margin = child->GetMargin();
            totalRequiredWidth += childDesiredSize.width + margin.Horizontal();
            maxRequiredHeight = std::max(maxRequiredHeight, childDesiredSize.height + margin.Vertical());
            ++validChildCount;
        }
        
        // Add spacing between children to total width requirement
        totalRequiredWidth += CalculateTotalSpacing(validChildCount);
        
        return Size(totalRequiredWidth, maxRequiredHeight);
    }

    Size StackLayout::MeasureVertical(const std::vector<std::shared_ptr<Widget>>& children, const Size& availableSize) {
        float maxRequiredWidth = 0.0f;
        float totalRequiredHeight = 0.0f;
        size_t validChildCount = 0;
        
        // Measure all valid children to determine total space requirements
        for (const auto& child : children) {
            if (!IsValidChild(child)) {
                continue;
            }
            // Add margin to each child's desired size
            const Size childDesiredSize = child->MeasureDesiredSize(availableSize);
            const auto& margin = child->GetMargin();
            maxRequiredWidth = std::max(maxRequiredWidth, childDesiredSize.width + margin.Horizontal());
            totalRequiredHeight += childDesiredSize.height + margin.Vertical();
            ++validChildCount;
        }
        
        // Add spacing between children to total height requirement
        totalRequiredHeight += CalculateTotalSpacing(validChildCount);
        
        return Size(maxRequiredWidth, totalRequiredHeight);
    }

    void StackLayout::ArrangeHorizontal(const std::vector<std::shared_ptr<Widget>>& children, const Rect& finalRect) {
        const Size containerSize = finalRect.GetSize();
        const float containerWidth = containerSize.width;
        const float containerHeight = containerSize.height;
        
        // Filter valid children and calculate layout metrics
        std::vector<std::shared_ptr<Widget>> validChildren;
        validChildren.reserve(children.size());
        
        for (const auto& child : children) {
            if (IsValidChild(child)) {
                validChildren.push_back(child);
            }
        }
        
        if (validChildren.empty()) {
            return;
        }
        
        const size_t validChildCount = validChildren.size();
        const float totalSpacingWidth = CalculateTotalSpacing(validChildCount);
        
        // Calculate used width (excluding the last child if it should fill)
        float totalUsedWidth = 0.0f;
        const size_t measureCount = fillLastChild ? validChildCount - 1 : validChildCount;
        
        for (size_t i = 0; i < measureCount; ++i) {
            const Size childDesiredSize = validChildren[i]->MeasureDesiredSize(Size(containerWidth, containerHeight));
            totalUsedWidth += childDesiredSize.width;
        }
        
        if (fillLastChild && validChildCount > 1) {
            totalUsedWidth += CalculateTotalSpacing(measureCount);
        } else {
            totalUsedWidth += totalSpacingWidth;
        }
        
        // Calculate starting position based on horizontal alignment
        float currentXPosition = finalRect.Left();
        if (!fillLastChild) {
            currentXPosition += CalculateAlignmentOffset(containerWidth, totalUsedWidth, horizontalAlignment);
        }
        
        // Arrange each child
        for (size_t childIndex = 0; childIndex < validChildCount; ++childIndex) {
            const auto& currentChild = validChildren[childIndex];
            
            float childActualWidth;
            float childActualHeight = containerHeight;
            const bool isLastChild = (childIndex == validChildCount - 1);
            const auto& margin = currentChild->GetMargin();
            if (fillLastChild && isLastChild) {
                // Last child fills remaining space, but respect min/max constraints
                const float remainingWidth = containerWidth - (currentXPosition - finalRect.Left());
                const Size childMinSize = currentChild->GetMinSize();
                const Size childMaxSize = currentChild->GetMaxSize();
                childActualWidth = std::max(childMinSize.width, std::min(childMaxSize.width, std::max(0.0f, remainingWidth)));
                if (currentChild->GetVerticalAlignment() == VerticalAlignment::Stretch) {
                    childActualHeight = std::max(childMinSize.height, std::min(childMaxSize.height, containerHeight));
                } else {
                    const Size childDesiredSize = currentChild->MeasureDesiredSize(Size(containerWidth, containerHeight));
                    childActualHeight = childDesiredSize.height;
                }
            } else {
                const Size childDesiredSize = currentChild->MeasureDesiredSize(Size(containerWidth, containerHeight));
                childActualWidth = childDesiredSize.width;
                if (currentChild->GetVerticalAlignment() == VerticalAlignment::Stretch) {
                    const Size childMinSize = currentChild->GetMinSize();
                    const Size childMaxSize = currentChild->GetMaxSize();
                    childActualHeight = std::max(childMinSize.height, std::min(childMaxSize.height, containerHeight));
                } else {
                    childActualHeight = childDesiredSize.height;
                }
            }
            // Create bounds that include space for the child's margin
            const Rect childBounds(currentXPosition + margin.left, finalRect.Top() + margin.top, childActualWidth, childActualHeight);
            currentChild->Arrange(childBounds);
            // Move to next position (add spacing only between children)
            currentXPosition += childActualWidth + margin.Horizontal();
            if (childIndex < validChildCount - 1) {
                currentXPosition += spacing;
            }
        }
    }

    void StackLayout::ArrangeVertical(const std::vector<std::shared_ptr<Widget>>& children, const Rect& finalRect) {
        const Size containerSize = finalRect.GetSize();
        const float containerWidth = containerSize.width;
        const float containerHeight = containerSize.height;
        
        // Filter valid children and calculate layout metrics
        std::vector<std::shared_ptr<Widget>> validChildren;
        validChildren.reserve(children.size());
        
        for (const auto& child : children) {
            if (IsValidChild(child)) {
                validChildren.push_back(child);
            }
        }
        
        if (validChildren.empty()) {
            return;
        }
        
        const size_t validChildCount = validChildren.size();
        const float totalSpacingHeight = CalculateTotalSpacing(validChildCount);
        
        // Calculate used height (excluding the last child if it should fill)
        float totalUsedHeight = 0.0f;
        const size_t measureCount = fillLastChild ? validChildCount - 1 : validChildCount;
        
        for (size_t i = 0; i < measureCount; ++i) {
            const Size childDesiredSize = validChildren[i]->MeasureDesiredSize(Size(containerWidth, containerHeight));
            totalUsedHeight += childDesiredSize.height;
        }
        
        if (fillLastChild && validChildCount > 1) {
            totalUsedHeight += CalculateTotalSpacing(measureCount);
        } else {
            totalUsedHeight += totalSpacingHeight;
        }
        
        // Calculate starting position based on vertical alignment
        float currentYPosition = finalRect.Top();
        if (!fillLastChild) {
            currentYPosition += CalculateAlignmentOffset(containerHeight, totalUsedHeight, verticalAlignment);
        }
        
        // Arrange each child
        for (size_t childIndex = 0; childIndex < validChildCount; ++childIndex) {
            const auto& currentChild = validChildren[childIndex];
            
            float childActualHeight;
            float childActualWidth = containerWidth;
            const bool isLastChild = (childIndex == validChildCount - 1);
            const auto& margin = currentChild->GetMargin();
            if (fillLastChild && isLastChild) {
                // Last child fills remaining space, but respect min/max constraints
                const float remainingHeight = containerHeight - (currentYPosition - finalRect.Top());
                const Size childMinSize = currentChild->GetMinSize();
                const Size childMaxSize = currentChild->GetMaxSize();
                childActualHeight = std::max(childMinSize.height, std::min(childMaxSize.height, std::max(0.0f, remainingHeight)));
                if (currentChild->GetHorizontalAlignment() == HorizontalAlignment::Stretch) {
                    childActualWidth = std::max(childMinSize.width, std::min(childMaxSize.width, containerWidth));
                } else {
                    const Size childDesiredSize = currentChild->MeasureDesiredSize(Size(containerWidth, containerHeight));
                    childActualWidth = childDesiredSize.width;
                }
            } else {
                const Size childDesiredSize = currentChild->MeasureDesiredSize(Size(containerWidth, containerHeight));
                childActualHeight = childDesiredSize.height;
                if (currentChild->GetHorizontalAlignment() == HorizontalAlignment::Stretch) {
                    const Size childMinSize = currentChild->GetMinSize();
                    const Size childMaxSize = currentChild->GetMaxSize();
                    childActualWidth = std::max(childMinSize.width, std::min(childMaxSize.width, containerWidth));
                } else {
                    childActualWidth = childDesiredSize.width;
                }
            }
            // Create bounds that include space for the child's margin
            const Rect childBounds(finalRect.Left() + margin.left, currentYPosition + margin.top, childActualWidth, childActualHeight);
            currentChild->Arrange(childBounds);
            // Move to next position (add spacing only between children)
            // Note: childActualHeight already includes the child's margin from MeasureDesiredSize
            currentYPosition += childActualHeight + margin.Vertical();
            if (childIndex < validChildCount - 1) {
                currentYPosition += spacing;
            }
        }
    }

} // namespace miko