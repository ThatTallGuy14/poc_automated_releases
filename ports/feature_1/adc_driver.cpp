enum ButtonState { Pressed, Unpressed };

class ButtonDriver {
  ButtonState getButtonState() { return ButtonState::Pressed; }
};