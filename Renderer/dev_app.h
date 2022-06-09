#pragma once
#include <cstdint>
#include <chrono>
#include "Emitters.h"

namespace end
{
	// Simple app class for development and testing purposes
	struct dev_app_t
	{
		emitter_s center;	// sorted pool center
		emitter corners[4];	// free pool corners
		pool_t<particle, 1024> sharedPool;

		// constructor
		dev_app_t();

		double get_delta_time()const;
		void update();
		void update_camera();

		// get a random number between 2 numbers
		float RandNumToNum(float _a, float _b);
	};
}