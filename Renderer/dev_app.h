#pragma once
#include <cstdint>
#include <chrono>

#include "InputMap.h"
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

		// constructor
		dev_app_t();

		double get_delta_time()const;
		void update();
		void update_camera();

		// math helper functions
		float RandNumToNum(float _a, float _b);
		float DegreesToRadians(float _angle);
	};
}