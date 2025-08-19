#pragma once

#ifndef MIKO_GRIDLAYOUT_H
#define MIKO_GRIDLAYOUT_H

#include "Layout.h"
#include <vector>

namespace miko {

    struct GridDefinition {
        float size = 0.0f;  // 0 = auto, positive = fixed, negative = star (proportional)
        float minSize = 0.0f;
        float maxSize = std::numeric_limits<float>::max();
        
        GridDefinition() = default;
        GridDefinition(float size) : size(size) {}
        GridDefinition(float size, float minSize, float maxSize) : size(size), minSize(minSize), maxSize(maxSize) {}
        
        bool IsAuto() const { return size == 0.0f; }
        bool IsFixed() const { return size > 0.0f; }
        bool IsStar() const { return size < 0.0f; }
        float GetStarValue() const { return -size; }
    };

    struct GridPosition {
        int row = 0;
        int column = 0;
        int rowSpan = 1;
        int columnSpan = 1;
        
        GridPosition() = default;
        GridPosition(int row, int column) : row(row), column(column) {}
        GridPosition(int row, int column, int rowSpan, int columnSpan) 
            : row(row), column(column), rowSpan(rowSpan), columnSpan(columnSpan) {}
    };

    class GridLayout : public Layout {
    public:
        GridLayout();
        GridLayout(int rows, int columns);
        virtual ~GridLayout() = default;
        
        // Layout interface
        Size MeasureDesiredSize(const std::vector<std::shared_ptr<Widget>>& children, const Size& availableSize) override;
        void ArrangeChildren(const std::vector<std::shared_ptr<Widget>>& children, const Rect& finalRect) override;
        
        // Grid structure
        void SetRowCount(int count);
        int GetRowCount() const { return static_cast<int>(rowDefinitions.size()); }
        
        void SetColumnCount(int count);
        int GetColumnCount() const { return static_cast<int>(columnDefinitions.size()); }
        
        // Row and column definitions
        void SetRowDefinition(int row, const GridDefinition& definition);
        const GridDefinition& GetRowDefinition(int row) const;
        
        void SetColumnDefinition(int column, const GridDefinition& definition);
        const GridDefinition& GetColumnDefinition(int column) const;
        
        // Widget positioning
        static void SetGridPosition(std::shared_ptr<Widget> widget, const GridPosition& position);
        static GridPosition GetGridPosition(std::shared_ptr<Widget> widget);
        
        // Convenience methods
        void AddAutoRow() { rowDefinitions.emplace_back(0.0f); }
        void AddFixedRow(float height) { rowDefinitions.emplace_back(height); }
        void AddStarRow(float weight = 1.0f) { rowDefinitions.emplace_back(-weight); }
        
        void AddAutoColumn() { columnDefinitions.emplace_back(0.0f); }
        void AddFixedColumn(float width) { columnDefinitions.emplace_back(width); }
        void AddStarColumn(float weight = 1.0f) { columnDefinitions.emplace_back(-weight); }
        
    private:
        std::vector<GridDefinition> rowDefinitions;
        std::vector<GridDefinition> columnDefinitions;
        
        struct CellInfo {
            std::shared_ptr<Widget> widget;
            GridPosition position;
            Size desiredSize;
        };
        
        std::vector<CellInfo> GetCellInfos(const std::vector<std::shared_ptr<Widget>>& children) const;
        std::vector<float> CalculateRowHeights(const std::vector<CellInfo>& cells, float availableHeight) const;
        std::vector<float> CalculateColumnWidths(const std::vector<CellInfo>& cells, float availableWidth) const;
        
        void DistributeAutoSize(std::vector<float>& sizes, const std::vector<GridDefinition>& definitions, 
                               const std::vector<float>& desiredSizes) const;
        void DistributeStarSize(std::vector<float>& sizes, const std::vector<GridDefinition>& definitions, 
                               float availableSize) const;
        
        void EnsureGridSize(int rows, int columns);
    };

} // namespace miko

#endif // MIKO_GRIDLAYOUT_H