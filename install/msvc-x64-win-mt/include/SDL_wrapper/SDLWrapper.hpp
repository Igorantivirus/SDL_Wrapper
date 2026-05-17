#pragma once

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>

#include <SDL_wrapper/Clock.hpp>
#include <SDL_wrapper/EventRegistrator.hpp>
#include <SDL_wrapper/FileWorker.hpp>
#include <SDL_wrapper/Names.hpp>
#include <SDL_wrapper/Rect.hpp>
#include <SDL_wrapper/Texture.hpp>

#include <SDL_wrapper/Audio/AudioDevice.hpp>

#include <SDL_wrapper/Math/Colors.hpp>
#include <SDL_wrapper/Math/Matrix3x3.hpp>
#include <SDL_wrapper/Math/VectorMath.hpp>

#include <SDL_wrapper/ObjectBase/Drawable.hpp>
#include <SDL_wrapper/ObjectBase/Shape.hpp>
#include <SDL_wrapper/ObjectBase/Transformable.hpp>

#include <SDL_wrapper/DrawTransformObjects/CircleShape.hpp>
#include <SDL_wrapper/DrawTransformObjects/EllipseShape.hpp>
#include <SDL_wrapper/DrawTransformObjects/RectangleShape.hpp>
#include <SDL_wrapper/DrawTransformObjects/Sprite.hpp>

#include <SDL_wrapper/Renders/RenderTarget.hpp>
#include <SDL_wrapper/Renders/RenderWindow.hpp>
#include <SDL_wrapper/Renders/VideoMode.hpp>
#include <SDL_wrapper/Renders/View.hpp>