#pragma once

/**
 * A material holds everything required by the shader
 * it is associated to (uniform and textures)
 */
class Material {
public:
	virtual void setup () = 0;
	virtual void cleanup () = 0;
}