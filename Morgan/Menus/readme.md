# Menu Implementation Documentation

The system comprises a base class `menuItem` and derived classes `Button` and `homeScreen`.

## Class: `menuItem`

The `menuItem` class serves as the abstract base class for all menu items. It defines the common interface and properties for menu items.

### Constructor

- `menuItem(float x, float y, float width, float height, float r, float g, float b)`
  Initializes a menu item with position, size, and color.

### Member Functions

- `virtual void draw() const = 0`
  Pure virtual function for drawing the menu item. Must be implemented by derived classes. **Currently not sure how to implement as I need to speak to Const about openGL**

- `virtual void select()`
  Pure virtual function for selecting the menu item and creating a select animation. Must be implemented by derived classes. **Currently not sure how to implement as I need to speak to Const about openGL**

- `void addItem(std::unique_ptr<menuItem> item)`
  Adds a child menu item to the current menu item. Used for composing complex menu structures.

### Protected Members

- `float x, y, width, height`
  Position and size of the menu item.

- `float color[3]`
  RGB color of the menu item.

- `std::vector<std::unique_ptr<menuItem>> items`
  Child menu items.

## Class: `Button` (Derived from `menuItem`)

Represents a button in the menu.

### Constructor

- `Button(float x, float y, float width, float height, float r, float g, float b, std::string name)`
  Initializes a button with position, size, color, and name.

### Member Functions

- `void draw() const override`
  Implements drawing logic for the button, displaying its name and appearance. **Currently not sure how to implement as I need to speak to Const about openGL**

- `void select() override`
  Implements selection logic for the button, showing an animation or effect when selected. **Currently not sure how to implement as I need to speak to Const about openGL**

## Class: `homeScreen` (Derived from `menuItem`)

Represents the home screen of the menu system, containing multiple menu items like buttons.

### Constructor

- `homeScreen(float x, float y, float width, float height)`
  Initializes the home screen with position, size, and default color. It also initializes buttons and adds them to the home screen.

### Member Functions

- `void draw() const override`
  Draws the home screen and its items (e.g., buttons).

- `void select() override`
  Handles user input to select menu items on the home screen. It prompts the user to choose an option and performs the corresponding action based on the selection.

## Implementation Notes

- Further menus should be implemented following a similar layout. 

- I have only implemented a home menu so far as that was all that made sense. However we could implement a replay menu if we plan to store more than one replay or a best score leaderboard
