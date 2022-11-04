#ifndef DEV_APP_H
#define DEV_APP_H

#include <cstdint>
#include <chrono>

#include "debug_renderer.h"
#include "Emitters.h"
#include "view.h"
#include "Input.h"
#include "frustum_culling.h"

namespace end
{
#define NUM_AABBS 6

	// Simple app class for development and testing purposes
	struct dev_app_t
	{
		// Lab 1 - Memory management
		emitter_s center;	// sorted pool center fountain
		emitter corners[4];	// free pool corner fountains
		pool_t<particle, 1024> sharedPool;

		// Lab 2 - Matrix behaviours
		float4x4 target;
		float4x4 viewer1;
		float4x4 viewer2;

		view_t* cam; // used to get camera from renderer.h

		// Lab 3 - Frustum Culling
		end::view_t targetView;
		end::frustum_t frustum;
		end::aabb_t AABB[NUM_AABBS];

		// Lab 4 - BVH

		// constructor
		dev_app_t();

		double get_delta_time()const;
		double calc_delta_time();

		void update();
		void init_content();
		void update_grid_color();

		// Translates matrix transform along X/Z in local space
		void move_transform(const float _speed);

		// Translates camera along X/Z in local space, and Y in world space
		void move_camera(const float _speed);

		float4x4 matrix_look_at(float4 _viewerPos, float4 _targetPos, float4 _localUp);
		float4x4 matrix_turn_to(float4x4 _viewer, float4 _targetPos, float _rotationSpeed);
		float4x4 matrix_orthonormalize(float4x4 _mx, float4 _up);
		float4x4_a matrix_orthonormalize_a(float4x4_a _mx, float4 _worldUp);
		float4x4_a mouse_look(float4x4_a _viewMx, float _deltaX, float _deltaY, float _sensitivity);

		// math helper functions
		float rand_num_to_num(float _a, float _b);
		float degrees_to_radians(float _angle);
	};

}// namespace end

#endif