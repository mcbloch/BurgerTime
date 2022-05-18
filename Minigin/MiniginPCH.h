#pragma once

#include <stdio.h>
#include <iostream> // std::cout
#include <sstream> // stringstream
#include <memory> // smart pointers
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <ranges>

#ifdef __linux__
#elif _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else

#endif

#include <SDL_ttf.h>

//#pragma warning(push, 0)
//#include <glm/glm.hpp>
#include <glm>
#include <SDL.h>
//#pragma warning (pop)

// IMGUI
#include <imgui.h>
//#include <backends/imgui_impl_sdl.h>
#include <imgui_impl_sdl.h>
//#include <backends/imgui_impl_opengl2.h>
#include <imgui_impl_opengl2.h>
#include <imgui_plot.h>

#include "Locator.h"
