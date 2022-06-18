#ifndef EMITTERS_H
#define EMITTERS_H

#include "math_types.h"
#include "pools.h"

namespace end
{
	struct particle
	{
		float3 vel;
		float3 pos;
		float3 prevPos;
		float4 color;
	};

	// Emitter for free pool
	struct emitter
	{
		float3 spawnPos;
		float4 spawnCol;

		// indices into the shared pool. 4 emitters indices fill up the shared pool
		sorted_pool_t<int16_t, 256> indices;
	};

	// Emitter for a sorted pool
	struct emitter_s
	{
		float3 spawnPos;
		float4 spawnCol;
		sorted_pool_t<particle, 1024> sortedPool;
	};
}// namespace end

#endif