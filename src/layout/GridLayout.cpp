#include "miko/layout/GridLayout.h"
#include "miko/widgets/Widget.h"
#include <algorithm>
#include <numeric>

namespace miko {

    GridLayout::GridLayout() {
        // Default to one auto-sized row and column
        rowDefinitions.emplace_back(0.0f);
        columnDefinitions.emplace_back(0.0f);
    }

    GridLayout::GridLayout(int rows, int columns) {
        // Initialize with specified number of auto-sized rows and columns
        for (int i = 0; i < rows; ++i) {
            rowDefinitions.emplace_back(0.0f);
        }
        for (int i = 0; i < columns; ++i) {
            columnDefinitions.emplace_back(0.0f);
        }
    }

    Size GridLayout::MeasureDesiredSize(const std::vector<std::shared_ptr<Widget>>& children, const Size& availableSize) {
        if (children.empty()) {
            return Size(0, 0);
        }

        // Get cell information for all children
        auto cellInfos = GetCellInfos(children);
        
        // Calculate desired sizes for each cell
        for (auto& cellInfo : cellInfos) {
            if (cellInfo.widget) {
                Size childDesired = cellInfo.widget->MeasureDesiredSize(availableSize);
                const auto& margin = cellInfo.widget->GetMargin();
                cellInfo.desiredSize = Size(childDesired.width + margin.Horizontal(), childDesired.height + margin.Vertical());
            }
        }

        // Calculate row heights and column widths
        auto rowHeights = CalculateRowHeights(cellInfos, availableSize.height);
        auto columnWidths = CalculateColumnWidths(cellInfos, availableSize.width);

        // Sum up total size
        float totalWidth = std::accumulate(columnWidths.begin(), columnWidths.end(), 0.0f);
        float totalHeight = std::accumulate(rowHeights.begin(), rowHeights.end(), 0.0f);

        return Size(totalWidth, totalHeight);
    }

    void GridLayout::ArrangeChildren(const std::vector<std::shared_ptr<Widget>>& children, const Rect& finalRect) {
        if (children.empty()) {
            return;
        }

        // Get cell information for all children
        auto cellInfos = GetCellInfos(children);
        
        // Calculate desired sizes for each cell
        for (auto& cellInfo : cellInfos) {
            if (cellInfo.widget) {
                Size childDesired = cellInfo.widget->MeasureDesiredSize(finalRect.GetSize());
                const auto& margin = cellInfo.widget->GetMargin();
                cellInfo.desiredSize = Size(childDesired.width + margin.Horizontal(), childDesired.height + margin.Vertical());
            }
        }

        // Calculate row heights and column widths
        auto rowHeights = CalculateRowHeights(cellInfos, finalRect.GetSize().height);
        auto columnWidths = CalculateColumnWidths(cellInfos, finalRect.GetSize().width);

        // Calculate row and column positions
        std::vector<float> rowPositions(rowHeights.size() + 1, 0.0f);
        std::vector<float> columnPositions(columnWidths.size() + 1, 0.0f);
        
        for (size_t i = 0; i < rowHeights.size(); ++i) {
            rowPositions[i + 1] = rowPositions[i] + rowHeights[i];
        }
        
        for (size_t i = 0; i < columnWidths.size(); ++i) {
            columnPositions[i + 1] = columnPositions[i] + columnWidths[i];
        }

        // Arrange each child widget
        for (const auto& cellInfo : cellInfos) {
            if (cellInfo.widget) {
                int row = cellInfo.position.row;
                int column = cellInfo.position.column;
                int rowSpan = cellInfo.position.rowSpan;
                int columnSpan = cellInfo.position.columnSpan;

                // Calculate cell bounds
                float left = finalRect.Left() + columnPositions[column];
                float top = finalRect.Top() + rowPositions[row];
                float right = finalRect.Left() + columnPositions[column + columnSpan];
                float bottom = finalRect.Top() + rowPositions[row + rowSpan];

                const auto& margin = cellInfo.widget->GetMargin();
                Rect cellRect(left + margin.left, top + margin.top, (right - left) - margin.Horizontal(), (bottom - top) - margin.Vertical());
                // Apply alignment within the cell
                Rect alignedRect = ApplyAlignment(cellInfo.widget, cellRect, cellInfo.desiredSize);
                cellInfo.widget->Arrange(alignedRect);
            }
        }
    }

    void GridLayout::SetRowCount(int count) {
        if (count < 1) count = 1;
        rowDefinitions.resize(count, GridDefinition(0.0f));
    }

    void GridLayout::SetColumnCount(int count) {
        if (count < 1) count = 1;
        columnDefinitions.resize(count, GridDefinition(0.0f));
    }

    void GridLayout::SetRowDefinition(int row, const GridDefinition& definition) {
        if (row >= 0 && row < static_cast<int>(rowDefinitions.size())) {
            rowDefinitions[row] = definition;
        }
    }

    const GridDefinition& GridLayout::GetRowDefinition(int row) const {
        static GridDefinition defaultDef;
        if (row >= 0 && row < static_cast<int>(rowDefinitions.size())) {
            return rowDefinitions[row];
        }
        return defaultDef;
    }

    void GridLayout::SetColumnDefinition(int column, const GridDefinition& definition) {
        if (column >= 0 && column < static_cast<int>(columnDefinitions.size())) {
            columnDefinitions[column] = definition;
        }
    }

    const GridDefinition& GridLayout::GetColumnDefinition(int column) const {
        static GridDefinition defaultDef;
        if (column >= 0 && column < static_cast<int>(columnDefinitions.size())) {
            return columnDefinitions[column];
        }
        return defaultDef;
    }

    void GridLayout::SetGridPosition(std::shared_ptr<Widget> widget, const GridPosition& position) {
        if (widget) {
            // Store position as widget property (implementation depends on Widget class)
            // For now, we'll assume Widget has a SetProperty method or similar
        }
    }

    GridPosition GridLayout::GetGridPosition(std::shared_ptr<Widget> widget) {
        if (widget) {
            // Retrieve position from widget property (implementation depends on Widget class)
            // For now, return default position
        }
        return GridPosition(0, 0, 1, 1);
    }

    std::vector<GridLayout::CellInfo> GridLayout::GetCellInfos(const std::vector<std::shared_ptr<Widget>>& children) const {
        std::vector<CellInfo> cellInfos;
        
        for (const auto& child : children) {
            if (child) {
                CellInfo info;
                info.widget = child;
                info.position = GetGridPosition(child);
                cellInfos.push_back(info);
            }
        }
        
        return cellInfos;
    }

    std::vector<float> GridLayout::CalculateRowHeights(const std::vector<CellInfo>& cells, float availableHeight) const {
        std::vector<float> heights(rowDefinitions.size(), 0.0f);
        
        // Calculate auto and fixed sizes first
        for (size_t i = 0; i < rowDefinitions.size(); ++i) {
            const auto& def = rowDefinitions[i];
            if (def.IsFixed()) {
                heights[i] = def.size;
            }
        }
        
        // Calculate auto sizes based on content
        for (const auto& cell : cells) {
            int row = cell.position.row;
            if (row >= 0 && row < static_cast<int>(heights.size()) && rowDefinitions[row].IsAuto()) {
                heights[row] = std::max(heights[row], cell.desiredSize.height / cell.position.rowSpan);
            }
        }
        
        // Distribute star sizes
        DistributeStarSize(heights, rowDefinitions, availableHeight);
        
        return heights;
    }

    std::vector<float> GridLayout::CalculateColumnWidths(const std::vector<CellInfo>& cells, float availableWidth) const {
        std::vector<float> widths(columnDefinitions.size(), 0.0f);
        
        // Calculate auto and fixed sizes first
        for (size_t i = 0; i < columnDefinitions.size(); ++i) {
            const auto& def = columnDefinitions[i];
            if (def.IsFixed()) {
                widths[i] = def.size;
            }
        }
        
        // Calculate auto sizes based on content
        for (const auto& cell : cells) {
            int column = cell.position.column;
            if (column >= 0 && column < static_cast<int>(widths.size()) && columnDefinitions[column].IsAuto()) {
                widths[column] = std::max(widths[column], cell.desiredSize.width / cell.position.columnSpan);
            }
        }
        
        // Distribute star sizes
        DistributeStarSize(widths, columnDefinitions, availableWidth);
        
        return widths;
    }

    void GridLayout::DistributeAutoSize(std::vector<float>& sizes, const std::vector<GridDefinition>& definitions, 
                                       const std::vector<float>& desiredSizes) const {
        // Auto sizes are already calculated in CalculateRowHeights/CalculateColumnWidths
    }

    void GridLayout::DistributeStarSize(std::vector<float>& sizes, const std::vector<GridDefinition>& definitions, 
                                       float availableSize) const {
        // Calculate used size by non-star definitions
        float usedSize = 0.0f;
        for (size_t i = 0; i < definitions.size(); ++i) {
            if (!definitions[i].IsStar()) {
                usedSize += sizes[i];
            }
        }
        
        float remainingSize = std::max(0.0f, availableSize - usedSize);
        
        // Iterative distribution to handle minimum size constraints
        std::vector<bool> isFixed(definitions.size(), false);
        
        while (remainingSize > 0.0f) {
            // Calculate total star weight for unfixed star definitions
            float totalStarWeight = 0.0f;
            for (size_t i = 0; i < definitions.size(); ++i) {
                if (definitions[i].IsStar() && !isFixed[i]) {
                    totalStarWeight += definitions[i].GetStarValue();
                }
            }
            
            if (totalStarWeight <= 0.0f) {
                break; // No more star definitions to distribute to
            }
            
            float starUnit = remainingSize / totalStarWeight;
            bool anyFixed = false;
            
            // Distribute space and check for minimum constraints
            for (size_t i = 0; i < definitions.size(); ++i) {
                if (definitions[i].IsStar() && !isFixed[i]) {
                    float proposedSize = starUnit * definitions[i].GetStarValue();
                    
                    // Apply minimum size constraint (use current size as minimum)
                    float minSize = sizes[i];
                    if (proposedSize < minSize) {
                        // Fix this definition at its minimum size
                        remainingSize -= (minSize - sizes[i]);
                        sizes[i] = minSize;
                        isFixed[i] = true;
                        anyFixed = true;
                    } else {
                        sizes[i] = proposedSize;
                    }
                }
            }
            
            if (!anyFixed) {
                // No constraints violated, distribution complete
                break;
            }
        }
    }

    Rect GridLayout::ApplyAlignment(std::shared_ptr<Widget> widget, const Rect& cellRect, const Size& desiredSize) const {
        if (!widget) {
            return cellRect;
        }
        
        // Get widget's alignment properties
        HorizontalAlignment hAlign = widget->GetHorizontalAlignment();
        VerticalAlignment vAlign = widget->GetVerticalAlignment();
        
        // Apply constraints to the desired size
        Size constrainedSize = Layout::ApplyConstraints(desiredSize, widget->GetMinSize(), widget->GetMaxSize());
        
        // Use the base class ApplyAlignment method with the widget's alignment properties
        return Layout::ApplyAlignment(cellRect, constrainedSize, hAlign, vAlign);
    }

    void GridLayout::EnsureGridSize(int rows, int columns) {
        if (static_cast<int>(rowDefinitions.size()) < rows) {
            SetRowCount(rows);
        }
        if (static_cast<int>(columnDefinitions.size()) < columns) {
            SetColumnCount(columns);
        }
    }

} // namespace miko