#pragma once

#ifndef DEV_APP_H
#define DEV_APP_H

#include <cstdint>
#include <chrono>

#include "view.h"
#include "Input.h"
#include "Emitters.h"

namespace end
{
	// Simple app class for development and testing purposes
	struct dev_app_t
	{
		// Lab1
		emitter_s center;	// sorted pool center
		emitter corners[4];	// free pool corners
		pool_t<particle, 1024> sharedPool;

		// Lab2
		float4x4 mx1;
		float4x4 mx2;
		float4x4 mx3;

		view_t* cam; // used to get camera from renderer.h via setting in main.cpp

		// constructor
		dev_app_t();

		void init_content();
		//void set_rotation(float4x4 _rot, float4x4 _mx);

		double get_delta_time()const;
		void update();

		void move_transform();
		void move_camera();


		float4x4_a mouse_look(float4x4_a _viewMx, float deltaX, float deltaY);

		float4x4 matrix_look_at(float3 _viewerPos, float3 _targetPos, float3 _localUp);
		float4x4 matrix_turn_to(float4x4 _viewer, float3 _targetPos, float _rotationSpeed);

		// math helper functions
		float RandNumToNum(float _a, float _b);
		float DegreesToRadians(float _angle);
	};
}

#endif