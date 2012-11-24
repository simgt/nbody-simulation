#pragma once

// CORE

#if  defined(__APPLE__) || defined(__MACOSX)
extern "C" {
#include <OpenGL/OpenGL.h>
}
#endif

#include "core/context.hh"
#include "core/mesh.hh"
#include "core/buffer.hh"
#include "core/shader.hh"
#include "core/driver.hh"
#include "core/texture.hh"
