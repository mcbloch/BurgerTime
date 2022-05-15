#pragma once
#include <memory>
#include <tuple>

#include "Command.h"
#include "Singleton.h"

namespace dae
{
	enum class ButtonState
	{
		Down,
		Up,
		Pressed
	};

	enum class ControllerButton
	{
		DPadUp = 0x0001,
		DPadDown = 0x0002,
		DPadLeft = 0x0004,
		DPadRight = 0x0008,
		Start = 0x0010,
		Back = 0x0020,
		LeftThumb = 0x0040,
		RightThumb = 0x0080,
		LeftShoulder = 0x0100,
		RightShoulder = 0x0200,
		ButtonA = 0x1000,
		ButtonB = 0x2000,
		ButtonX = 0x4000,
		ButtonY = 0x8000,
	};

	struct ControllerButtonData
	{
		ControllerButton button;
		ButtonState state;
	};

	enum class ControllerAnalog
	{
		ThumbLeftX,
		ThumbLeftY,
		ThumbRightX,
		ThumbRightY,
		TriggerLeft,
		TriggerRight
	};

	struct ControllerAnalogData
	{
		ControllerAnalog what;
		short value;
	};

	struct KeyboardKeyData
	{
		SDL_Scancode key;
		ButtonState state;
	};

	inline bool operator<(const ControllerButtonData& lhs, const ControllerButtonData& rhs)
	{
		return
			std::tie(lhs.button, lhs.state) <
			std::tie(rhs.button, rhs.state);
	}

	inline bool operator<(const ControllerAnalogData& lhs, const ControllerAnalogData& rhs)
	{
		return
			std::tie(lhs.what, lhs.value) <
			std::tie(rhs.what, rhs.value);
	}


	inline bool operator<(const KeyboardKeyData& lhs, const KeyboardKeyData& rhs)
	{
		return
			std::tie(lhs.key, lhs.state) <
			std::tie(rhs.key, rhs.state);
	}

	static int ControllerButtonCount = static_cast<int>(ControllerButton::ButtonY) + 1;

	class InputManager final : public Singleton<InputManager>
	{
		class InputManagerImpl;
		InputManagerImpl* pImpl;

	public:
		InputManager();
		~InputManager();

		void ProcessInput() const;
		// Should be called after ProcessInput as the former reset the current state
		void ProcessSDLKeyEvent(SDL_Event e) const;

		[[nodiscard]] bool HasState(ControllerButton button, ButtonState bState) const;
		void HandleInput() const;

		void BindButtonCommand(ControllerButtonData, std::unique_ptr<Command>) const;
		void BindAnalogCommand(ControllerAnalog, std::unique_ptr<AnalogCommand>) const;
		void BindKeyCommand(KeyboardKeyData, std::unique_ptr<Command>) const;
	};
}
