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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <SDL_ttf.h>

#pragma warning(push, 0)
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
#include "glm/gtx/hash.hpp"
#include <SDL.h>

// IMGUI
#include <imgui.h>
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_opengl2.h>
#include <imgui_plot.h>

// Curl and Json
#include "single_include/nlohmann/json.hpp"
#include "curl/curl.h"
#pragma warning (pop)

#include "Locator.h"

//=== Management ===
#define SAFE_DELETE(p) if (p) { delete (p); (p) = nullptr; }