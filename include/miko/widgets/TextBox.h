#pragma once

#ifndef MIKO_TEXTBOX_H
#define MIKO_TEXTBOX_H

#include "Widget.h"
#include "../core/Renderer.h"
#include <chrono>
#include <string>
#include <functional>

namespace miko {

    class TextBox : public Widget {
    public:
        TextBox();
        TextBox(const std::string& text);
        virtual ~TextBox() = default;
        
        // Text properties
        void SetText(const std::string& text);
        const std::string& GetText() const { return m_text; }
        
        void SetPlaceholderText(const std::string& placeholder);
        const std::string& GetPlaceholderText() const { return m_placeholderText; }
        
        void SetFont(const Font& font) { this->m_font = font; Invalidate(); InvalidateLayout(); }
        const Font& GetFont() const { return m_font; }
        
        void SetTextColor(const Color& color) { m_textColor = color; Invalidate(); }
        const Color& GetTextColor() const { return m_textColor; }
        
        void SetPlaceholderColor(const Color& color) { m_placeholderColor = color; Invalidate(); }
        const Color& GetPlaceholderColor() const { return m_placeholderColor; }
        
        void SetSelectionColor(const Color& color) { m_selectionColor = color; Invalidate(); }
        const Color& GetSelectionColor() const { return m_selectionColor; }
        
        void SetCaretColor(const Color& color) { m_caretColor = color; Invalidate(); }
        const Color& GetCaretColor() const { return m_caretColor; }
        
        // Input properties
        void SetReadOnly(bool readOnly) { this->m_isReadOnly = readOnly; }
        bool IsReadOnly() const { return m_isReadOnly; }
        
        void SetMultiline(bool multiline) { this->m_multiline = multiline; InvalidateLayout(); }
        bool IsMultiline() const { return m_multiline; }
        
        void SetPasswordMode(bool password) { m_passwordMode = password; Invalidate(); }
        bool IsPasswordMode() const { return m_passwordMode; }
        
        void SetPasswordChar(char passwordChar);
        char GetPasswordChar() const;
        
        void SetMaxLength(int maxLength) { this->m_maxLength = maxLength; }
        int GetMaxLength() const { return m_maxLength; }
        
        // Selection and caret
        void SetSelection(int start, int end);
        void GetSelection(int& start, int& end) const;
        void SelectAll();
        void ClearSelection();
        bool HasSelection() const { return m_selectionStart != m_selectionEnd; }
        
        void SetCaretPosition(int position);
        int GetCaretPosition() const { return m_caretPosition; }
        
        // Text operations
        void Insert(const std::string& text);
        void Delete(int start, int count);
        void Clear();
        
        std::string GetSelectedText() const;
        void Copy();
        void Cut();
        void Paste();
        
        // Widget overrides
        bool OnMouseEvent(const MouseEvent& event) override;
        bool OnKeyEvent(const KeyEvent& event) override;
        void OnFocusGained() override;
        void OnFocusLost() override;
        Size MeasureDesiredSize(const Size& availableSize) override;
        
        // Events
        std::function<void(const std::string&)> OnTextChanged;
        std::function<void()> OnEnterPressed;
        
    protected:
        void OnRender(std::shared_ptr<Renderer> renderer) override;
        
    private:
        std::string m_text;
        std::string m_placeholderText;
        Font m_font;
        Color m_textColor;
        Color m_placeholderColor;
        Color m_selectionColor;
        Color m_caretColor;
        
        bool m_isReadOnly;
        bool m_multiline;
        bool m_passwordMode;
        char m_passwordChar;
        int m_maxLength;
        
        // Selection and caret
        int m_caretPosition;
        int m_selectionStart;
        int m_selectionEnd;
        bool m_caretVisible;
        std::chrono::steady_clock::time_point m_lastCaretBlink;
        
        // Scrolling (for multiline)
        float m_scrollOffset;
        
        void Initialize();
        void UpdateCaret();
        void EnsureCaretVisible();
        int GetCharacterIndexAt(const Point& position) const;
        Point GetCharacterPosition(int index) const;
        Rect GetCaretRect() const;
        Rect GetSelectionRect(int start, int end) const;
        std::string GetDisplayText() const;
        void NotifyTextChanged();
        
        // Input handling
        void InsertText(const std::string& text);
        void DeleteSelection();
        void HandleCharacterInput(char c);
        void HandleKeyInput(KeyCode key, bool shift, bool ctrl);
        void HandleMouseSelection(const Point& position, bool extend);
        void MoveCaret(int delta, bool extendSelection);
        int GetCaretPositionFromPoint(const Point& point);
        void DrawSelection(std::shared_ptr<Renderer> renderer, const Rect& textRect);
        void DrawCaret(std::shared_ptr<Renderer> renderer, const Rect& textRect);
    };

} // namespace miko

#endif // MIKO_TEXTBOX_H