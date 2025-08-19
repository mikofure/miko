#include "miko/widgets/TextBox.h"
#include "miko/core/Renderer.h"
#include <algorithm>

namespace miko {

TextBox::TextBox()
    : m_text()
    , m_placeholderText()
    , m_font("Segoe UI", 12.0f, FontWeight::Normal, FontStyle::Normal)
    , m_textColor(Color::TextColor)
    , m_placeholderColor(Color(128, 128, 128, 255))
    , m_selectionColor(Color(0, 120, 215, 100))
    , m_caretColor(Color::TextColor)
    , m_isReadOnly(false)
    , m_passwordMode(false)
    , m_passwordChar('*')
    , m_maxLength(0)
    , m_caretPosition(0)
    , m_selectionStart(0)
    , m_selectionEnd(0)
    , m_scrollOffset(0.0f)
    , m_caretVisible(true)
    , m_lastCaretBlink(std::chrono::steady_clock::now())
{
    SetSize(Size(150, 25));
    SetBackgroundColor(Color::White);
    SetBorderColor(Color(128, 128, 128, 255));
    SetBorderWidth(1.0f);
    SetPadding(Margin(4, 2, 4, 2));
}

TextBox::TextBox(const std::string& text) : TextBox() {
    SetText(text);
}



void TextBox::SetText(const std::string& text) {
    if (m_text != text) {
        std::string newText = text;
        if (m_maxLength > 0 && newText.length() > m_maxLength) {
            newText = newText.substr(0, m_maxLength);
        }
        
        m_text = newText;
        m_caretPosition = std::min(m_caretPosition, (int)m_text.length());
        ClearSelection();
        
        if (OnTextChanged) {
            OnTextChanged(m_text);
        }
    }
}

void TextBox::SetPlaceholderText(const std::string& placeholderText) {
    if (m_placeholderText != placeholderText) {
        m_placeholderText = placeholderText;
        Invalidate();
    }
}





void TextBox::SetPasswordChar(char passwordChar) {
    m_passwordChar = passwordChar;
}

char TextBox::GetPasswordChar() const {
    return m_passwordChar;
}

void TextBox::SetCaretPosition(int position) {
    m_caretPosition = Clamp(position, 0, (int)m_text.length());
    ClearSelection();
    EnsureCaretVisible();
}

void TextBox::SelectAll() {
    m_selectionStart = 0;
    m_selectionEnd = (int)m_text.length();
    m_caretPosition = m_selectionEnd;
}

void TextBox::SetSelection(int start, int end) {
    m_selectionStart = std::max(0, std::min(start, (int)m_text.length()));
    m_selectionEnd = std::max(0, std::min(end, (int)m_text.length()));
    m_caretPosition = m_selectionEnd;
}

void TextBox::GetSelection(int& start, int& end) const {
    start = m_selectionStart;
    end = m_selectionEnd;
}

void TextBox::ClearSelection() {
    m_selectionStart = m_selectionEnd = m_caretPosition;
}

std::string TextBox::GetSelectedText() const {
    if (!HasSelection()) return "";
    
    int start = std::min(m_selectionStart, m_selectionEnd);
    int end = std::max(m_selectionStart, m_selectionEnd);
    return m_text.substr(start, end - start);
}

void TextBox::DeleteSelection() {
    if (!HasSelection() || m_isReadOnly) return;
    
    int start = std::min(m_selectionStart, m_selectionEnd);
    int end = std::max(m_selectionStart, m_selectionEnd);
    
    m_text.erase(start, end - start);
    m_caretPosition = start;
    ClearSelection();
    
    if (OnTextChanged) {
        OnTextChanged(m_text);
    }
}

void TextBox::InsertText(const std::string& text) {
    if (m_isReadOnly) return;
    
    DeleteSelection();
    
    std::string newText = m_text;
    newText.insert(m_caretPosition, text);
    
    if (m_maxLength > 0 && newText.length() > m_maxLength) {
        int allowedLength = m_maxLength - m_text.length();
        if (allowedLength <= 0) return;
        newText = m_text;
        newText.insert(m_caretPosition, text.substr(0, allowedLength));
    }
    
    m_text = newText;
    m_caretPosition += text.length();
    
    if (OnTextChanged) {
        OnTextChanged(m_text);
    }
    
    EnsureCaretVisible();
}

void TextBox::Insert(const std::string& text) {
    InsertText(text);
}

void TextBox::Delete(int start, int count) {
    if (start < 0 || start >= m_text.length() || count <= 0) return;
    
    int end = std::min(start + count, (int)m_text.length());
    m_text.erase(start, end - start);
    
    if (m_caretPosition > start) {
        m_caretPosition = std::max(start, m_caretPosition - (end - start));
    }
    
    if (OnTextChanged) {
        OnTextChanged(m_text);
    }
    
    EnsureCaretVisible();
}

void TextBox::Clear() {
    m_text.clear();
    m_caretPosition = 0;
    m_selectionStart = 0;
    m_selectionEnd = 0;
    
    if (OnTextChanged) {
        OnTextChanged(m_text);
    }
    
    EnsureCaretVisible();
}

Size TextBox::MeasureDesiredSize(const Size& availableSize) {
    float lineHeight = m_font.size * 1.2f;
    
    Size desiredSize(
        150.0f + GetPadding().left + GetPadding().right,
        lineHeight + GetPadding().top + GetPadding().bottom
    );
    
    return Size(
        Clamp(desiredSize.width, GetMinSize().width, GetMaxSize().width),
        Clamp(desiredSize.height, GetMinSize().height, GetMaxSize().height)
    );
}

void TextBox::OnRender(std::shared_ptr<Renderer> renderer) {
    if (!IsVisible() || !renderer) return;
    
    // Draw background
    Color bgColor = GetBackgroundColor();
    if (!IsEnabled()) {
        bgColor = Color(240, 240, 240, 255); // Light gray for disabled
    }
    
    Brush backgroundBrush(bgColor);
    if (GetCornerRadius() > 0) {
        renderer->FillRoundedRectangle(GetBounds(), GetCornerRadius(), GetCornerRadius(), backgroundBrush);
    } else {
        renderer->FillRectangle(GetBounds(), backgroundBrush);
    }
    
    // Draw border
    Color borderColor = GetBorderColor();
    if (IsFocused()) {
        borderColor = Color(0, 120, 215, 255); // Blue for focused
    }
    
    if (GetBorderWidth() > 0) {
        Pen borderPen(borderColor, GetBorderWidth());
        if (GetCornerRadius() > 0) {
            renderer->DrawRoundedRectangle(GetBounds(), GetCornerRadius(), GetCornerRadius(), borderPen);
        } else {
            renderer->DrawRectangle(GetBounds(), borderPen);
        }
    }
    
    // Calculate text area
    Margin padding = GetPadding();
    Rect textRect(
        GetBounds().x + padding.left,
        GetBounds().y + padding.top,
        GetBounds().width - padding.left - padding.right,
        GetBounds().height - padding.top - padding.bottom
    );
    
    // Set clipping to text area
    renderer->PushClipRect(textRect);
    
    // Draw selection background
    if (HasSelection() && IsFocused()) {
        DrawSelection(renderer, textRect);
    }
    
    // Draw text or placeholder
    std::string displayText = GetDisplayText();
    if (!displayText.empty()) {
        Color textColor = IsEnabled() ? m_textColor : Color(128, 128, 128, 255);
        Brush textBrush(textColor);
        
        // Apply scroll offset
        Rect scrolledTextRect = textRect;
        scrolledTextRect.x -= m_scrollOffset;
        
        renderer->DrawText(displayText, scrolledTextRect, m_font, textBrush, TextAlignment::Left);
    } else if (!m_placeholderText.empty() && !IsFocused()) {
        Brush placeholderBrush(m_placeholderColor);
        renderer->DrawText(m_placeholderText, textRect, m_font, placeholderBrush, TextAlignment::Left);
    }
    
    // Draw caret
    if (IsFocused() && m_caretVisible && IsEnabled() && !m_isReadOnly) {
        DrawCaret(renderer, textRect);
    }
    
    renderer->PopClipRect();
    
    // Render children
    for (auto& child : GetChildren()) {
        if (child && child->IsVisible()) {
            child->Render(renderer);
        }
    }
}

bool TextBox::OnMouseEvent(const MouseEvent& event) {
    if (!IsEnabled()) return false;
    
    if (event.type == EventType::MouseButtonPressed && event.button == MouseButton::Left) {
        SetFocused(true);
        
        // Calculate caret position from mouse position
        int newCaretPos = GetCharacterIndexAt(event.position);
        
        if (event.shiftPressed) {
            // Extend selection
            m_selectionEnd = newCaretPos;
            m_caretPosition = newCaretPos;
        } else {
            // Start new selection
            m_caretPosition = newCaretPos;
            m_selectionStart = m_selectionEnd = newCaretPos;
        }
        
        EnsureCaretVisible();
        return true;
    }
    
    return false;
}

bool TextBox::OnKeyEvent(const KeyEvent& event) {
    if (!IsEnabled()) {
        return false;
    }
    
    if (event.type != EventType::KeyPressed) {
        return false;
    }
    
    bool handled = true;
    
    switch (event.keyCode) {
        case KeyCode::Left:
            if (event.shiftPressed) {
                if (m_caretPosition > 0) {
                    m_caretPosition--;
                    m_selectionEnd = m_caretPosition;
                }
            } else {
                if (m_caretPosition > 0) {
                    m_caretPosition--;
                }
                m_selectionStart = m_selectionEnd = m_caretPosition;
            }
            handled = true;
            break;
            
        case KeyCode::Right:
            if (event.shiftPressed) {
                if (m_caretPosition < (int)m_text.length()) {
                    m_caretPosition++;
                    m_selectionEnd = m_caretPosition;
                }
            } else {
                if (m_caretPosition < (int)m_text.length()) {
                    m_caretPosition++;
                }
                m_selectionStart = m_selectionEnd = m_caretPosition;
            }
            handled = true;
            break;
            
        case KeyCode::Backspace:
            if (!m_isReadOnly) {
                if (HasSelection()) {
                    DeleteSelection();
                } else if (m_caretPosition > 0) {
                    m_text.erase(m_caretPosition - 1, 1);
                    m_caretPosition--;
                    if (OnTextChanged) {
                        OnTextChanged(m_text);
                    }
                }
                EnsureCaretVisible();
            }
            break;
            
        case KeyCode::Delete:
            if (!m_isReadOnly) {
                if (HasSelection()) {
                    DeleteSelection();
                } else if (m_caretPosition < (int)m_text.length()) {
                    m_text.erase(m_caretPosition, 1);
                    if (OnTextChanged) {
                        OnTextChanged(m_text);
                    }
                }
                EnsureCaretVisible();
            }
            break;
            
        case KeyCode::A:
            if (event.ctrlPressed) {
                SelectAll();
            } else {
                handled = false;
            }
            break;
            
        default:
            handled = false;
            break;
    }
    
    return handled;
}



void TextBox::OnFocusGained() {
    m_caretVisible = true;
}

void TextBox::OnFocusLost() {
    ClearSelection();
    m_caretVisible = false;
}

std::string TextBox::GetDisplayText() const {
    if (m_passwordMode && !m_text.empty()) {
        return std::string(m_text.length(), m_passwordChar);
    }
    return m_text;
}

void TextBox::MoveCaret(int delta, bool extendSelection) {
    int newPos = Clamp(m_caretPosition + delta, 0, (int)m_text.length());
    
    if (extendSelection) {
        m_selectionEnd = newPos;
    } else {
        m_selectionStart = m_selectionEnd = newPos;
    }
    
    m_caretPosition = newPos;
    EnsureCaretVisible();
}

int TextBox::GetCaretPositionFromPoint(const Point& point) {
    // Simplified implementation - in a real implementation, this would
    // use the renderer to measure text accurately
    Margin padding = GetPadding();
    float textX = point.x - GetBounds().x - padding.left + m_scrollOffset;
    
    if (textX <= 0) return 0;
    
    float charWidth = m_font.size * 0.6f; // Approximate character width
    int position = (int)(textX / charWidth);
    
    return Clamp(position, 0, (int)m_text.length());
}

void TextBox::EnsureCaretVisible() {
    // Simplified scrolling implementation
    Margin padding = GetPadding();
    float textAreaWidth = GetBounds().width - padding.left - padding.right;
    float charWidth = m_font.size * 0.6f;
    float caretX = m_caretPosition * charWidth;
    
    if (caretX < m_scrollOffset) {
        m_scrollOffset = caretX;
    } else if (caretX > m_scrollOffset + textAreaWidth) {
        m_scrollOffset = caretX - textAreaWidth;
    }
    
    if (m_scrollOffset < 0) {
        m_scrollOffset = 0;
    }
}

void TextBox::DrawSelection(std::shared_ptr<Renderer> renderer, const Rect& textRect) {
    if (!HasSelection()) return;
    
    int start = std::min(m_selectionStart, m_selectionEnd);
    int end = std::max(m_selectionStart, m_selectionEnd);
    
    float charWidth = m_font.size * 0.6f;
    float startX = start * charWidth - m_scrollOffset;
    float endX = end * charWidth - m_scrollOffset;
    
    Rect selectionRect(
        textRect.x + startX,
        textRect.y,
        endX - startX,
        textRect.height
    );
    
    // Clip selection to text area
    selectionRect.x = std::max(selectionRect.x, textRect.x);
    float rightEdge = std::min(selectionRect.x + selectionRect.width, textRect.x + textRect.width);
    selectionRect.width = rightEdge - selectionRect.x;
    
    if (selectionRect.width > 0) {
        Brush selectionBrush(m_selectionColor);
        renderer->FillRectangle(selectionRect, selectionBrush);
    }
}

void TextBox::DrawCaret(std::shared_ptr<Renderer> renderer, const Rect& textRect) {
    float charWidth = m_font.size * 0.6f;
    float caretX = m_caretPosition * charWidth - m_scrollOffset;
    
    if (caretX >= 0 && caretX <= textRect.width) {
        Pen caretPen(m_caretColor, 1.0f);
        
        Point start(textRect.x + caretX, textRect.y + 1);
        Point end(textRect.x + caretX, textRect.y + textRect.height - 1);
        
        renderer->DrawLine(start, end, caretPen);
    }
}

void TextBox::Copy() {
    if (HasSelection()) {
        std::string selectedText = GetSelectedText();
        // TODO: Implement clipboard copy functionality
        // For now, this is a placeholder
    }
}

void TextBox::Cut() {
    if (HasSelection() && !m_isReadOnly) {
        Copy();
        DeleteSelection();
    }
}

void TextBox::Paste() {
    if (!m_isReadOnly) {
        // TODO: Implement clipboard paste functionality
        // For now, this is a placeholder
        // std::string clipboardText = GetClipboardText();
        // InsertText(clipboardText);
    }
}

int TextBox::GetCharacterIndexAt(const Point& position) const {
    // Simple implementation - just return position based on character width
    // This is a placeholder implementation
    Rect bounds = GetBounds();
    Rect textRect = Rect(
        bounds.x + GetPadding().left,
        bounds.y + GetPadding().top,
        bounds.width - GetPadding().left - GetPadding().right,
        bounds.height - GetPadding().top - GetPadding().bottom
    );
    
    if (position.x <= textRect.x) {
        return 0;
    }
    
    // Estimate character position based on average character width
    float avgCharWidth = 8.0f; // Placeholder value
    int charIndex = (int)((position.x - textRect.x) / avgCharWidth);
    
    return std::min(charIndex, (int)m_text.length());
}

} // namespace miko