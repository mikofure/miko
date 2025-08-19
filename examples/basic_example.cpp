#include <miko/miko.h>
#include <memory>

// Prevent Windows API macros from interfering
#ifdef CreateWindow
#undef CreateWindow
#endif

using namespace miko;

class BasicExampleApp : public Application {
public:
    bool Initialize() override {
        if (!Application::Initialize()) {
            return false;
        }
        
        // Create main window
        auto window = CreateWindow("Miko Framework - Basic Example", 800, 600);
        if (!window) {
            return false;
        }
        
        // Create root panel
        auto rootPanel = std::make_shared<Panel>();
        rootPanel->SetBackgroundColor(Color::WindowBackground);
        window->SetRootWidget(rootPanel);
        
        // Create a vertical stack layout
        auto layout = std::make_shared<StackLayout>(Orientation::Vertical);
        layout->SetSpacing(10.0f);
        layout->SetVerticalAlignment(StackAlignment::Center);
    layout->SetMargin(Spacing(20.0f));
        rootPanel->SetLayout(layout);
        
        // Create title label
        auto titleLabel = std::make_shared<Label>("Welcome to Miko Framework!");
        titleLabel->SetFont(Font("Segoe UI", 24.0f, FontWeight::Bold));
        titleLabel->SetTextColor(Color::TextColor);
        titleLabel->SetTextAlignment(TextAlignment::Center);
        titleLabel->SetHorizontalAlignment(HorizontalAlignment::Stretch);
        rootPanel->AddChild(titleLabel);
        
        // Create description label
        auto descLabel = std::make_shared<Label>(
            "This is a demonstration of the Miko GUI framework built with Direct2D, DirectWrite, and Win32 API. "
            "The framework provides modern, hardware-accelerated 2D graphics without traditional Win32 controls."
        );
        descLabel->SetFont(Font("Segoe UI", 12.0f));
        descLabel->SetTextColor(Color::TextColor);
        descLabel->SetWordWrap(true);
        descLabel->SetHorizontalAlignment(HorizontalAlignment::Stretch);
        rootPanel->AddChild(descLabel);
        
        // Create a horizontal panel for buttons
        auto buttonPanel = std::make_shared<Panel>();
        buttonPanel->SetHorizontalAlignment(HorizontalAlignment::Stretch);
        auto buttonLayout = std::make_shared<StackLayout>(Orientation::Horizontal);
        buttonLayout->SetSpacing(10.0f);
        buttonPanel->SetLayout(buttonLayout);
        rootPanel->AddChild(buttonPanel);
        
        // Create buttons
        auto button1 = std::make_shared<Button>("Click Me!");
        button1->SetSize(Size(120, 35));
        button1->OnClick = []() {
            // Handle button click
        };
        buttonPanel->AddChild(button1);
        
        auto button2 = std::make_shared<Button>("Another Button");
        button2->SetSize(Size(120, 35));
        button2->OnClick = []() {
            // Handle button click
        };
        buttonPanel->AddChild(button2);
        
        // Create text input
        auto textBox = std::make_shared<TextBox>();
        textBox->SetPlaceholderText("Enter some text here...");
        textBox->SetHorizontalAlignment(HorizontalAlignment::Stretch);
        textBox->SetSize(Size(0, 30));
        textBox->OnTextChanged = [](const std::string& text) {
            // Handle text change
        };
        rootPanel->AddChild(textBox);
        
        // Create a grid panel
        auto gridPanel = std::make_shared<Panel>();
        gridPanel->SetHorizontalAlignment(HorizontalAlignment::Stretch);
        gridPanel->SetVerticalAlignment(VerticalAlignment::Stretch);
        gridPanel->SetBackgroundColor(Color::ControlBackground);
        gridPanel->SetBorderColor(Color::BorderColor);
        gridPanel->SetBorderWidth(1.0f);
        gridPanel->SetCornerRadius(5.0f);
        
        auto gridLayout = std::make_shared<GridLayout>(3, 3);
    gridLayout->SetMargin(Spacing(10.0f));
        gridLayout->SetSpacing(5.0f);
        
        // Add star columns and rows for responsive layout
        for (int i = 0; i < 3; i++) {
            gridLayout->AddStarColumn(1.0f);
            gridLayout->AddStarRow(1.0f);
        }
        
        gridPanel->SetLayout(gridLayout);
        
        // Add some widgets to the grid
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                auto cellButton = std::make_shared<Button>("(" + std::to_string(row) + "," + std::to_string(col) + ")");
                GridLayout::SetGridPosition(cellButton, GridPosition(row, col));
                gridPanel->AddChild(cellButton);
            }
        }
        
        rootPanel->AddChild(gridPanel);
        
        // Show the window
        window->Show();
        
        return true;
    }
    
    void Update(float deltaTime) override {
        // Update application logic here
    }
    
    void Render() override {
        // Custom rendering if needed
    }
};

MIKO_IMPLEMENT_APPLICATION(BasicExampleApp)