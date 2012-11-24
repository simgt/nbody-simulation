#pragma once

#include "common.hh"
#include "mesh.hh"
#include "shader.hh"

namespace te {
	class Driver {
	public:
		Driver (Vec2i viewport_size, float near = 0.1f, float far = 100.0f, float fov = 0.7f)
			: viewport_size_ (viewport_size),
			  focal_length_ (1.0f / std::tan(fov / 2.0f)),
			  projection_ (te::perspective(fov, float(viewport_size.x()) / viewport_size.y(), near, far)),
			  modelview_ (Affine3f::Identity()) {
		}

		/// \name Accessors
		///@{

		Vec2i viewport_size () const { return viewport_size_; }
		float focal_length () const { return focal_length_; }

		const Affine3f& modelview () const { return modelview_; }
		void modelview (const Affine3f& m) { modelview_ = m; }

		///@}

		void draw (const Mesh& mesh, const Shader& shader, GLenum mode) const;

	private:
		Vec2i viewport_size_;
		float focal_length_;

		Projective3f projection_;
		Affine3f modelview_;
	};
}