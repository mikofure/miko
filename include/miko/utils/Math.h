#pragma once

#ifndef MIKO_MATH_H
#define MIKO_MATH_H

#include <cmath>
#include <algorithm>

namespace miko {

    struct Point {
        float x, y;
        
        Point() : x(0.0f), y(0.0f) {}
        Point(float x, float y) : x(x), y(y) {}
        
        Point operator+(const Point& other) const {
            return Point(x + other.x, y + other.y);
        }
        
        Point operator-(const Point& other) const {
            return Point(x - other.x, y - other.y);
        }
        
        Point operator*(float scalar) const {
            return Point(x * scalar, y * scalar);
        }
        
        bool operator==(const Point& other) const {
            return std::abs(x - other.x) < 0.001f && std::abs(y - other.y) < 0.001f;
        }
    };

    struct Size {
        float width, height;
        
        Size() : width(0.0f), height(0.0f) {}
        Size(float width, float height) : width(width), height(height) {}
        
        bool IsEmpty() const {
            return width <= 0.0f || height <= 0.0f;
        }
        
        bool operator==(const Size& other) const {
            return std::abs(width - other.width) < 0.001f && std::abs(height - other.height) < 0.001f;
        }
        
        bool operator!=(const Size& other) const {
            return !(*this == other);
        }
    };

    struct Rect {
        float x, y, width, height;
        
        Rect() : x(0.0f), y(0.0f), width(0.0f), height(0.0f) {}
        Rect(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {}
        Rect(const Point& position, const Size& size) : x(position.x), y(position.y), width(size.width), height(size.height) {}
        
        float Left() const { return x; }
        float Top() const { return y; }
        float Right() const { return x + width; }
        float Bottom() const { return y + height; }
        
        Point TopLeft() const { return Point(x, y); }
        Point TopRight() const { return Point(x + width, y); }
        Point BottomLeft() const { return Point(x, y + height); }
        Point BottomRight() const { return Point(x + width, y + height); }
        Point Center() const { return Point(x + width * 0.5f, y + height * 0.5f); }
        
        Size GetSize() const { return Size(width, height); }
        
        bool Contains(const Point& point) const {
            return point.x >= x && point.x <= x + width && 
                   point.y >= y && point.y <= y + height;
        }
        
        bool Intersects(const Rect& other) const {
            return !(other.x > x + width || other.x + other.width < x ||
                     other.y > y + height || other.y + other.height < y);
        }
        
        Rect Union(const Rect& other) const {
            float left = std::min(x, other.x);
            float top = std::min(y, other.y);
            float right = std::max(x + width, other.x + other.width);
            float bottom = std::max(y + height, other.y + other.height);
            return Rect(left, top, right - left, bottom - top);
        }
        
        bool IsEmpty() const {
            return width <= 0.0f || height <= 0.0f;
        }
        
        bool operator==(const Rect& other) const {
            return std::abs(x - other.x) < 0.001f && std::abs(y - other.y) < 0.001f &&
                   std::abs(width - other.width) < 0.001f && std::abs(height - other.height) < 0.001f;
        }
        
        bool operator!=(const Rect& other) const {
            return !(*this == other);
        }
    };

    struct Margin {
        float left, top, right, bottom;
        
        Margin() : left(0.0f), top(0.0f), right(0.0f), bottom(0.0f) {}
        Margin(float all) : left(all), top(all), right(all), bottom(all) {}
        Margin(float horizontal, float vertical) : left(horizontal), top(vertical), right(horizontal), bottom(vertical) {}
        Margin(float left, float top, float right, float bottom) : left(left), top(top), right(right), bottom(bottom) {}
        
        float Horizontal() const { return left + right; }
        float Vertical() const { return top + bottom; }
        
        bool operator==(const Margin& other) const {
            return std::abs(left - other.left) < 0.001f && std::abs(top - other.top) < 0.001f &&
                   std::abs(right - other.right) < 0.001f && std::abs(bottom - other.bottom) < 0.001f;
        }
        
        bool operator!=(const Margin& other) const {
            return !(*this == other);
        }
    };

    // Utility functions
    inline float Clamp(float value, float min, float max) {
        return std::max(min, std::min(max, value));
    }
    
    inline float Lerp(float a, float b, float t) {
        return a + t * (b - a);
    }
    
    inline float Distance(const Point& a, const Point& b) {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        return std::sqrt(dx * dx + dy * dy);
    }

} // namespace miko

#endif // MIKO_MATH_H