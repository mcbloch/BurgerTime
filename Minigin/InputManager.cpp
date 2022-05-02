#include "MiniginPCH.h"
#include "InputManager.h"

#include "EventQueue.h"
#include "Locator.h"


#include <iostream>
#include <map>

#define WIN32_LEAN_AND_MEAN
#include <functional>
#include <windows.h>
#include <XInput.h>
#pragma comment(lib, "xinput.lib")

#include "InputManager.h"

class dae::InputManager::InputManagerImpl
{
	XINPUT_STATE previousState{};
	XINPUT_STATE currentState{};

	int mainController           = 0;
	int buttonsPressedThisFrame  = 0;
	int buttonsReleasedThisFrame = 0;

	std::vector<short> analogValues{};

	std::vector<bool> notifiedControllerNotConnected;

	using ControllerButtonCommandMap = std::map<ControllerButtonData, std::unique_ptr<Command>>;
	using ControllerAnalogCommandMap = std::map<ControllerAnalog, std::unique_ptr<AnalogCommand>>;
	ControllerButtonCommandMap buttonCommandMapping{};
	ControllerAnalogCommandMap analogCommandMapping{};

public:
	InputManagerImpl() :
		analogValues(6, 0),
		notifiedControllerNotConnected(XUSER_MAX_COUNT, false)
	{
		ZeroMemory(&previousState, sizeof(XINPUT_STATE));
		ZeroMemory(&currentState, sizeof(XINPUT_STATE));
	}

	~InputManagerImpl()
	{
		for (auto& [buttonData, command] : buttonCommandMapping)
		{
			command.reset();
		}
		for (auto& [analogData, command] : analogCommandMapping)
		{
			command.reset();
		}
	}

	/*
	 * TODO: Separate these. Coop can use different input types for different characters
	 */
	bool ProcessInput()
	{
		// Keyboard stuff
		/*
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				return false;
			}
			if (e.type == SDL_KEYDOWN)
			{
				if ((e.key.keysym.sym & SDLK_SPACE) != 0)
				{
					Locator::GetAudio().TogglePause();
				}
				// if (e.key & SDLK_d)
				// {
				// 	EventQueue::SendEvent(BurgerDrop, nullptr);
				// }
			}
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
			}
			// Process event for ImGUI
			ImGui_ImplSDL2_ProcessEvent(&e);
		}*/

		// Controller stuff

		DWORD dwResult;
		// for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
		// {
		// TODO Add support for multiple controllers with the commented loop
		const auto i = mainController;
		CopyMemory(&previousState, &currentState, sizeof(XINPUT_STATE));
		ZeroMemory(&currentState, sizeof(XINPUT_STATE));

		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(i, &currentState);

		if (dwResult == ERROR_SUCCESS)
		{
			// Buttons
			const auto buttonChanges = currentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
			buttonsPressedThisFrame  = buttonChanges & currentState.Gamepad.wButtons;
			buttonsReleasedThisFrame = buttonChanges & (~currentState.Gamepad.wButtons);

			// Sticks
			analogValues[static_cast<int>(ControllerAnalog::ThumbLeftX)]  = currentState.Gamepad.sThumbLX;
			analogValues[static_cast<int>(ControllerAnalog::ThumbLeftY)]  = currentState.Gamepad.sThumbLY;
			analogValues[static_cast<int>(ControllerAnalog::ThumbRightX)] = currentState.Gamepad.sThumbRX;
			analogValues[static_cast<int>(ControllerAnalog::ThumbRightY)] = currentState.Gamepad.sThumbRY;

			// Triggers
			analogValues[static_cast<int>(ControllerAnalog::TriggerLeft)]  = currentState.Gamepad.bLeftTrigger;
			analogValues[static_cast<int>(ControllerAnalog::TriggerRight)] = currentState.Gamepad.bRightTrigger;

			// Controller is connected
			// Reset our connection tracker
			if (notifiedControllerNotConnected[i])
			{
				std::cout << "Controller " << i << " connected." << std::endl;
				notifiedControllerNotConnected[i] = false;
			}
		}
		else
		{
			// Controller is not connected
			if (!notifiedControllerNotConnected[i])
			{
				std::cout << "Controller " << i << " is not connected, could not read input" << std::endl;
				notifiedControllerNotConnected[i] = true;
			}
		}
		// }

		return true;
	}

	[[nodiscard]] bool IsDown(ControllerButton button) const
	{
		return buttonsPressedThisFrame & static_cast<unsigned int>(button);
	}

	[[nodiscard]] bool IsUp(ControllerButton button) const
	{
		return buttonsReleasedThisFrame & static_cast<unsigned int>(button);
	}

	[[nodiscard]] bool IsPressed(ControllerButton button) const
	{
		return currentState.Gamepad.wButtons & static_cast<unsigned int>(button);
	}


	[[nodiscard]] bool HasState(const ControllerButton button, const ButtonState bState) const
	{
		switch (bState)
		{
		case ButtonState::Down: return IsDown(button);
		case ButtonState::Up: return IsUp(button);
		case ButtonState::Pressed: return IsPressed(button);
		}
		return false;
	}

	void HandleInput() const
	{
		for (auto& [buttonData, command] : buttonCommandMapping)
		{
			if (HasState(buttonData.button, buttonData.state))
			{
				command->Execute();
			}
		}

		for (auto& [analog, command] : analogCommandMapping)
		{
			command->Execute(analogValues[static_cast<int>(analog)]);
		}
	}

	void BindButtonCommand(const ControllerButtonData cbd,
	                       std::unique_ptr<Command>   command)
	{
		buttonCommandMapping.insert_or_assign(cbd, std::move(command));
	}

	void BindAnalogCommand(const ControllerAnalog         cad,
	                       std::unique_ptr<AnalogCommand> command)
	{
		analogCommandMapping.insert_or_assign(cad, std::move(command));
	}
};


// ************
// InputManager
// ------------

dae::InputManager::InputManager() :
	pImpl(new InputManagerImpl())
{
}

dae::InputManager::~InputManager()
{
	delete pImpl;
}

bool dae::InputManager::ProcessInput() const
{
	return pImpl->ProcessInput();
}

bool dae::InputManager::HasState(const ControllerButton button, const ButtonState bState) const
{
	return pImpl->HasState(button, bState);
}

void dae::InputManager::HandleInput() const
{
	pImpl->HandleInput();
}

void dae::InputManager::BindButtonCommand(const ControllerButtonData data, std::unique_ptr<Command> command) const
{
	pImpl->BindButtonCommand(data, std::move(command));
}

void dae::InputManager::BindAnalogCommand(const ControllerAnalog d, std::unique_ptr<AnalogCommand> c) const
{
	pImpl->BindAnalogCommand(d, std::move(c));
}
