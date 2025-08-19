#include <miko/miko.h>
#include <memory>

// Prevent Windows API macros from interfering
#ifdef CreateWindow
#undef CreateWindow
#endif

using namespace miko;

class WidgetShowcaseApp : public Application {
public:
    bool Initialize() override {
        if (!Application::Initialize()) {
            return false;
        }
        
        // Create main window
        auto window = CreateWindow("Miko Framework - Widget Showcase", 1000, 700);
        if (!window) {
            return false;
        }
        
        // Create menu bar
        CreateMenuBar(window);
        
        // Create root panel with horizontal layout
        auto rootPanel = std::make_shared<Panel>();
        rootPanel->SetBackgroundColor(Color::WindowBackground);
        auto rootLayout = std::make_shared<StackLayout>(Orientation::Horizontal);
        rootPanel->SetLayout(rootLayout);
        window->SetRootWidget(rootPanel);
        
        // Create sidebar
        CreateSidebar(rootPanel);
        
        // Create main content area
        CreateMainContent(rootPanel);
        
        // Show the window
        window->Show();
        
        return true;
    }
    
private:
    void CreateMenuBar(std::shared_ptr<Window> window) {
        // Note: Menu bar implementation would use Win32 API
        // This is a placeholder for the actual menu creation
        // HMENU menuBar = CreateMenu();
        // HMENU fileMenu = CreatePopupMenu();
        // AppendMenu(fileMenu, MF_STRING, 1001, L"New");
        // AppendMenu(fileMenu, MF_STRING, 1002, L"Open");
        // AppendMenu(fileMenu, MF_SEPARATOR, 0, nullptr);
        // AppendMenu(fileMenu, MF_STRING, 1003, L"Exit");
        // AppendMenu(menuBar, MF_POPUP, (UINT_PTR)fileMenu, L"File");
        // window->SetMenuBar(menuBar);
    }
    
    void CreateSidebar(std::shared_ptr<Panel> parent) {
        auto sidebar = std::make_shared<Panel>();
        sidebar->SetSize(Size(200, 0));
        sidebar->SetBackgroundColor(Color::ControlBackground);
        sidebar->SetBorderColor(Color::BorderColor);
        sidebar->SetBorderWidth(1.0f);
        
        auto sidebarLayout = std::make_shared<StackLayout>(Orientation::Vertical);
        sidebarLayout->SetSpacing(5.0f);
        sidebarLayout->SetMargin(Margin(10.0f));
        sidebar->SetLayout(sidebarLayout);
        
        // Sidebar title
        auto title = std::make_shared<Label>("Widget Categories");
        title->SetFont(Font("Segoe UI", 14.0f, FontWeight::Bold));
        title->SetTextColor(Color::TextColor);
        sidebar->AddChild(title);
        
        // Category buttons
        auto categories = std::vector<std::string>{
            "Basic Widgets",
            "Input Controls",
            "Layout Panels",
            "Custom Widgets"
        };
        
        for (const auto& category : categories) {
            auto button = std::make_shared<Button>(category);
            button->SetHorizontalAlignment(HorizontalAlignment::Stretch);
            button->SetSize(Size(0, 30));
            button->OnClick = [this, category]() {
                ShowCategory(category);
            };
            sidebar->AddChild(button);
        }
        
        parent->AddChild(sidebar);
    }
    
    void CreateMainContent(std::shared_ptr<Panel> parent) {
        mainContent = std::make_shared<Panel>();
        mainContent->SetHorizontalAlignment(HorizontalAlignment::Stretch);
        mainContent->SetVerticalAlignment(VerticalAlignment::Stretch);
        mainContent->SetBackgroundColor(Color(0.98f, 0.98f, 0.98f, 1.0f)); // Light gray background like redesign
        mainContent->SetMargin(Margin(10.0f, 0, 0, 0));
        
        auto contentLayout = std::make_shared<StackLayout>(Orientation::Vertical);
        contentLayout->SetSpacing(0.0f); // Remove spacing since sections have their own margins
        contentLayout->SetMargin(Margin(24.0f)); // Increased padding for better spacing
        mainContent->SetLayout(contentLayout);
        
        // Show basic widgets by default
        ShowCategory("Basic Widgets");
        
        parent->AddChild(mainContent);
    }
    
    void ShowCategory(const std::string& category) {
        // Clear existing content
        mainContent->RemoveAllChildren();
        
        // Add category title
        auto title = std::make_shared<Label>(category);
        title->SetFont(Font("Segoe UI", 20.0f, FontWeight::Bold));
        title->SetTextColor(Color::TextColor);
        mainContent->AddChild(title);
        
        if (category == "Basic Widgets") {
            ShowBasicWidgets();
        } else if (category == "Input Controls") {
            ShowInputControls();
        } else if (category == "Layout Panels") {
            ShowLayoutPanels();
        } else if (category == "Custom Widgets") {
            ShowCustomWidgets();
        }
    }
    
    void ShowBasicWidgets() {
        // Labels section
        auto labelSection = CreateSection("Labels");
        
        auto simpleLabel = std::make_shared<Label>("Simple Label");
        simpleLabel->SetPadding(Padding(8.0f, 4.0f));
        labelSection->AddChild(simpleLabel);

        auto styledLabel = std::make_shared<Label>("Styled Label with Bold Font");
        styledLabel->SetFont(Font("Segoe UI", 14.0f, FontWeight::Bold));
        styledLabel->SetTextColor(Color::Blue);
        styledLabel->SetPadding(Padding(8.0f, 4.0f));
        labelSection->AddChild(styledLabel);

        auto wrappedLabel = std::make_shared<Label>(
            "This is a long label that demonstrates text wrapping functionality. "
            "The text will wrap to multiple lines when it exceeds the available width."
        );
        wrappedLabel->SetWordWrap(true);
        wrappedLabel->SetHorizontalAlignment(HorizontalAlignment::Stretch);
        wrappedLabel->SetPadding(Padding(8.0f, 4.0f));
        labelSection->AddChild(wrappedLabel);
        
        mainContent->AddChild(labelSection);
        
        // Buttons section
        auto buttonSection = CreateSection("Buttons");
        
        auto buttonPanel = std::make_shared<Panel>();
        buttonPanel->SetHorizontalAlignment(HorizontalAlignment::Left);
        
        auto buttonLayout = std::make_shared<StackLayout>(Orientation::Horizontal);
        buttonLayout->SetSpacing(12.0f); // Increased spacing between buttons
        buttonPanel->SetLayout(buttonLayout);
        
        // Create buttons with modern styling matching redesign
        auto normalButton = std::make_shared<Button>("Normal Button");
        normalButton->SetSize(Size(140.0f, 36.0f)); // Slightly larger
    normalButton->SetPadding(Padding(12.0f, 8.0f)); // More padding
        normalButton->SetCornerRadius(8.0f); // Rounded corners
        buttonPanel->AddChild(normalButton);
        
        auto styledButton = std::make_shared<Button>("Styled Button");
        styledButton->SetSize(Size(140.0f, 36.0f));
    styledButton->SetPadding(Padding(12.0f, 8.0f));
        styledButton->SetBackgroundColor(Color(0.2f, 0.4f, 1.0f, 1.0f)); // Modern blue
        styledButton->SetTextColor(Color::White);
        styledButton->SetCornerRadius(8.0f);
        buttonPanel->AddChild(styledButton);
        
        auto disabledButton = std::make_shared<Button>("Disabled Button");
        disabledButton->SetSize(Size(140.0f, 36.0f));
    disabledButton->SetPadding(Padding(12.0f, 8.0f));
        disabledButton->SetCornerRadius(8.0f);
        disabledButton->SetEnabled(false);
        buttonPanel->AddChild(disabledButton);
        
        buttonSection->AddChild(buttonPanel);
        mainContent->AddChild(buttonSection);
    }
    
    void ShowInputControls() {
        // Text input section
        auto textSection = CreateSection("Text Input");
        
        auto simpleTextBox = std::make_shared<TextBox>();
        simpleTextBox->SetPlaceholderText("Enter text here...");
        simpleTextBox->SetHorizontalAlignment(HorizontalAlignment::Stretch);
        textSection->AddChild(simpleTextBox);
        
        auto passwordBox = std::make_shared<TextBox>();
        passwordBox->SetPlaceholderText("Password");
        passwordBox->SetPasswordMode(true);
        passwordBox->SetHorizontalAlignment(HorizontalAlignment::Stretch);
        textSection->AddChild(passwordBox);
        
        auto multilineBox = std::make_shared<TextBox>();
        multilineBox->SetPlaceholderText("Multiline text area...");
        multilineBox->SetMultiline(true);
        multilineBox->SetSize(Size(0, 100));
        multilineBox->SetHorizontalAlignment(HorizontalAlignment::Stretch);
        textSection->AddChild(multilineBox);
        
        mainContent->AddChild(textSection);
    }
    
    void ShowLayoutPanels() {
        // Stack layout demo
        auto stackSection = CreateSection("Stack Layout");
        
        auto stackDemo = std::make_shared<Panel>();
        stackDemo->SetBackgroundColor(Color::LightGray);
        stackDemo->SetSize(Size(0, 150));
        stackDemo->SetHorizontalAlignment(HorizontalAlignment::Stretch);
        
        auto stackLayout = std::make_shared<StackLayout>(Orientation::Horizontal);
        stackLayout->SetSpacing(10.0f);
        stackLayout->SetMargin(Margin(10.0f));
        stackDemo->SetLayout(stackLayout);
        
        for (int i = 1; i <= 4; i++) {
            auto item = std::make_shared<Button>("Item " + std::to_string(i));
            item->SetSize(Size(80, 40));
            stackDemo->AddChild(item);
        }
        
        stackSection->AddChild(stackDemo);
        mainContent->AddChild(stackSection);
        
        // Grid layout demo
        auto gridSection = CreateSection("Grid Layout");
        
        auto gridDemo = std::make_shared<Panel>();
        gridDemo->SetBackgroundColor(Color::LightGray);
        gridDemo->SetSize(Size(0, 200));
        gridDemo->SetHorizontalAlignment(HorizontalAlignment::Stretch);
        
        auto gridLayout = std::make_shared<GridLayout>(3, 4);
        gridLayout->SetSpacing(5.0f);
        gridLayout->SetMargin(Margin(10.0f));
        
        // Set up responsive columns
        for (int i = 0; i < 4; i++) {
            gridLayout->AddStarColumn(1.0f);
        }
        for (int i = 0; i < 3; i++) {
            gridLayout->AddAutoRow();
        }
        
        gridDemo->SetLayout(gridLayout);
        
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 4; col++) {
                auto cell = std::make_shared<Button>(std::to_string(row * 4 + col + 1));
                GridLayout::SetGridPosition(cell, GridPosition(row, col));
                gridDemo->AddChild(cell);
            }
        }
        
        gridSection->AddChild(gridDemo);
        mainContent->AddChild(gridSection);
    }
    
    void ShowCustomWidgets() {
        auto customSection = CreateSection("Custom Widgets");
        
        auto info = std::make_shared<Label>(
            "Custom widgets can be created by inheriting from the base Widget class "
            "and implementing custom rendering and interaction logic."
        );
        info->SetWordWrap(true);
        info->SetHorizontalAlignment(HorizontalAlignment::Stretch);
        customSection->AddChild(info);
        
        mainContent->AddChild(customSection);
    }
    
    std::shared_ptr<Panel> CreateSection(const std::string& title) {
        auto section = std::make_shared<Panel>();
        section->SetHorizontalAlignment(HorizontalAlignment::Stretch);
        section->SetBackgroundColor(Color::White);
        section->SetBorderColor(Color(0.9f, 0.9f, 0.9f, 1.0f)); // Light gray border
        section->SetBorderWidth(1.0f);
        section->SetCornerRadius(12.0f); // More rounded corners like redesign
    section->SetPadding(Padding(20.0f)); // Increased padding for better spacing
        section->SetMargin(Margin(0, 0, 16.0f, 0)); // Bottom margin between sections
        
        auto layout = std::make_shared<StackLayout>(Orientation::Vertical);
        layout->SetSpacing(16.0f); // Increased spacing
        section->SetLayout(layout);
        
        auto sectionTitle = std::make_shared<Label>(title);
        sectionTitle->SetFont(Font("Segoe UI", 18.0f, FontWeight::Bold)); // Larger title
        sectionTitle->SetTextColor(Color::Black);
        section->AddChild(sectionTitle);
        
        return section;
    }
    
    std::shared_ptr<Panel> mainContent;
};

MIKO_IMPLEMENT_APPLICATION(WidgetShowcaseApp)