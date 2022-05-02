#include "MiniginPCH.h"
#include "InputManager.h"

#include "EventQueue.h"
#include "Locator.h"

bool dae::InputManager::ProcessInput()
{
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(0, &m_CurrentState);

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
				dae::Locator::GetAudio().TogglePause();
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
	}

	return true;
}

bool dae::InputManager::IsPressed(const ControllerButton button) const
{
	switch (button)
	{
	case ControllerButton::ButtonA:
		return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_A;
	case ControllerButton::ButtonB:
		return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_B;
	case ControllerButton::ButtonX:
		return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_X;
	case ControllerButton::ButtonY:
		return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_Y;
	default: return false;
	}
}
