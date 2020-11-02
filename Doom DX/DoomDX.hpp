#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

// std
#include <cmath>
#include <ctime>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
// windows
#include <Windows.h>
#include <windowsx.h>
// direct x
#include <D3D11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#define Window HWND

#define HIGH_ORDER_BIT 0x8000

typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

class Camera;
class Doom;
class Line;
class Mouse;
class Shader;
class Square;
struct Vector;

#include "Vector.hpp"
#include "WAD.hpp"
#include "Camera.hpp"
#include "Doom.hpp"
#include "Line.hpp"
#include "Mouse.hpp"
#include "Shader.hpp"
#include "Square.hpp"