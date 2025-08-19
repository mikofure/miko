#pragma once

#ifndef MIKO_STACKLAYOUT_H
#define MIKO_STACKLAYOUT_H

#include "Layout.h"
#include "../widgets/Widget.h"

namespace miko {

    /**
     * @brief Defines the orientation for stacking child widgets
     */
    enum class Orientation {
        Horizontal,  ///< Stack widgets horizontally (left to right)
        Vertical     ///< Stack widgets vertically (top to bottom)
    };
    
    /**
     * @brief Defines alignment options for widgets within the stack
     */
    enum class StackAlignment {
        Start,   ///< Align to the start (left/top)
        Center,  ///< Align to the center
        End      ///< Align to the end (right/bottom)
    };

    /**
     * @brief A layout that arranges child widgets in a single row or column
     * 
     * StackLayout provides a simple way to arrange widgets either horizontally
     * or vertically with configurable spacing, alignment, and fill behavior.
     * The last child can optionally be configured to fill remaining space.
     */
    class StackLayout : public Layout {
    public:
        /**
         * @brief Constructs a StackLayout with the specified orientation
         * @param orientation The stacking direction (default: Vertical)
         */
        explicit StackLayout(Orientation orientation = Orientation::Vertical);
        virtual ~StackLayout() = default;
        
        // Layout interface implementation
        /**
         * @brief Calculates the desired size needed to accommodate all children
         * @param children The child widgets to measure
         * @param availableSize The available space for layout
         * @return The minimum size required for all children
         */
        Size MeasureDesiredSize(const std::vector<std::shared_ptr<Widget>>& children, const Size& availableSize) override;
        
        /**
         * @brief Arranges child widgets within the specified rectangle
         * @param children The child widgets to arrange
         * @param finalRect The final bounds for the layout
         */
        void ArrangeChildren(const std::vector<std::shared_ptr<Widget>>& children, const Rect& finalRect) override;
        
        // Orientation properties
        /**
         * @brief Sets the stacking orientation
         * @param orientation The new orientation (Horizontal or Vertical)
         */
        void SetOrientation(Orientation orientation) { this->orientation = orientation; }
        
        /**
         * @brief Gets the current stacking orientation
         * @return The current orientation
         */
        Orientation GetOrientation() const { return orientation; }
        
        // Fill behavior properties
        /**
         * @brief Sets whether the last child should fill remaining space
         * @param fill True to make the last child fill remaining space
         */
        void SetFillLastChild(bool fill) { fillLastChild = fill; }
        
        /**
         * @brief Gets whether the last child fills remaining space
         * @return True if the last child fills remaining space
         */
        bool GetFillLastChild() const { return fillLastChild; }
        
        // Alignment properties
        /**
         * @brief Sets the vertical alignment for children (applies to horizontal stacks)
         * @param alignment The vertical alignment option
         */
        void SetVerticalAlignment(StackAlignment alignment) { verticalAlignment = alignment; }
        
        /**
         * @brief Gets the current vertical alignment
         * @return The current vertical alignment
         */
        StackAlignment GetVerticalAlignment() const { return verticalAlignment; }
        
        /**
         * @brief Sets the horizontal alignment for children (applies to vertical stacks)
         * @param alignment The horizontal alignment option
         */
        void SetHorizontalAlignment(StackAlignment alignment) { horizontalAlignment = alignment; }
        
        /**
         * @brief Gets the current horizontal alignment
         * @return The current horizontal alignment
         */
        StackAlignment GetHorizontalAlignment() const { return horizontalAlignment; }
        
    private:
        // Layout configuration
        Orientation orientation;              ///< The stacking direction
        bool fillLastChild;                   ///< Whether the last child fills remaining space
        StackAlignment verticalAlignment;     ///< Vertical alignment for horizontal stacks
        StackAlignment horizontalAlignment;   ///< Horizontal alignment for vertical stacks
        
        // Measurement helpers
        /**
         * @brief Measures desired size for horizontal layout
         * @param children The child widgets to measure
         * @param availableSize The available space
         * @return The required size for horizontal arrangement
         */
        Size MeasureHorizontal(const std::vector<std::shared_ptr<Widget>>& children, const Size& availableSize);
        
        /**
         * @brief Measures desired size for vertical layout
         * @param children The child widgets to measure
         * @param availableSize The available space
         * @return The required size for vertical arrangement
         */
        Size MeasureVertical(const std::vector<std::shared_ptr<Widget>>& children, const Size& availableSize);
        
        // Arrangement helpers
        /**
         * @brief Arranges children horizontally within the given rectangle
         * @param children The child widgets to arrange
         * @param finalRect The bounds for arrangement
         */
        void ArrangeHorizontal(const std::vector<std::shared_ptr<Widget>>& children, const Rect& finalRect);
        
        /**
         * @brief Arranges children vertically within the given rectangle
         * @param children The child widgets to arrange
         * @param finalRect The bounds for arrangement
         */
        void ArrangeVertical(const std::vector<std::shared_ptr<Widget>>& children, const Rect& finalRect);
        
        // Utility helpers
        /**
         * @brief Calculates total spacing needed between children
         * @param childCount The number of children
         * @return The total spacing required
         */
        float CalculateTotalSpacing(size_t childCount) const;
        
        /**
         * @brief Validates that a child widget pointer is valid
         * @param child The child widget to validate
         * @return True if the child is valid (not null)
         */
        static bool IsValidChild(const std::shared_ptr<Widget>& child);
        
        /**
          * @brief Calculates the starting position based on alignment
          * @param totalSize The total available size
          * @param usedSize The size used by content
          * @param alignment The alignment option
          * @return The starting position offset
          */
         static float CalculateAlignmentOffset(float totalSize, float usedSize, StackAlignment alignment);
    };

} // namespace miko

#endif // MIKO_STACKLAYOUT_H