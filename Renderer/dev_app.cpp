#include <iostream>

#include "dev_app.h"
#include "math_types.h"
#include "debug_renderer.h"

namespace end
{
	float3 gravity		= { 0.0f, -9.8f, 0.0f };
	double delta_time	= 0.0;

	// For grid color changing
	bool colorFlag = false;
	float4 gridCol = { 0.15f, 0.15f, 0.15f, 1.0f };

	double dev_app_t::get_delta_time()const
	{
		return delta_time;
	}

	dev_app_t::dev_app_t()
	{
		std::cout << "Log whatever you need here.\n"; // Don’t forget to include <iostream>
		(DirectX::XMMATRIX&)mx1 = DirectX::XMMatrixIdentity();
		mx1[3].xyz = { 0.0f, 0.5f, 0.0f };

		// Set keystates?
		inputMap['w'] = e_input::FWD;
		inputMap[87]  = e_input::FWD;
		//inputMap['w'] = e_input::FWD;

		inputMap['s'] = e_input::BACK;
		inputMap[83]  = e_input::BACK;
		//inputMap['s'] = e_input::BACK;

		inputMap['d'] = e_input::RIGHT;
		inputMap[68]  = e_input::RIGHT;
		//inputMap['d'] = e_input::RIGHT;

		inputMap['a'] = e_input::LEFT;
		inputMap[65]  = e_input::LEFT;
		//inputMap['a'] = e_input::LEFT;
	}
	
	double calc_delta_time()
	{
		static std::chrono::time_point<std::chrono::high_resolution_clock> last_time = std::chrono::high_resolution_clock::now();

		std::chrono::time_point<std::chrono::high_resolution_clock> new_time = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed_seconds = new_time - last_time;
		last_time = new_time;

		return std::min(1.0 / 15.0, elapsed_seconds.count());
	}

	void dev_app_t::update()
	{
		delta_time = calc_delta_time();
		float colorTime = delta_time;

		// Draw grid
		end:debug_renderer::create_grid(10, gridCol);

		// TODO: do your updates here
		// color updates
		if (colorFlag == true)
			colorTime = -colorTime;
		else
			gridCol[0] += colorTime; // update red if flag is false

		// always update blue
		gridCol[2] += colorTime;

		if (gridCol[2] >= 0.75f)
			colorFlag = true;
		else if (gridCol[2] <= 0.15f)
			colorFlag = false;

		int spawnCount = 3;

		// LAB 1 PARTICLES
#if 0
		center.spawnCol = { 1.0f, 1.0f, 1.0f, 1.0f };
		center.spawnPos = { 0.0f, 0.0f, 0.0f };			// The location the particles originate?

		// Spawn particles from sorted pool
		for (int i = 0; i < spawnCount; ++i)
		{
			int16_t ndx = center.sortedPool.alloc();

			// safety check index
			if (ndx != -1)
			{
				float randR = RandNumToNum(0.0f, 1.0f);
				float randG = RandNumToNum(0.0f, 1.0f);
				float randB = RandNumToNum(0.0f, 1.0f);

				float randX = RandNumToNum(-0.75, 0.75);
				float randY = RandNumToNum(5.0, 10.0);
				float randZ = RandNumToNum(-0.75, 0.75);

				particle& p = center.sortedPool[ndx];
				p.pos		= center.spawnPos;
				p.color		= { randR, randG, randB, 1.0f };
				p.vel		= { randX, randY, randZ };
			}
		}

		// update sorted pool particles
		for (int i = 0; i < center.sortedPool.size(); ++i) // size returns active count
		{
			particle& p = center.sortedPool[i];
			p.prevPos = p.pos;
			p.pos += p.vel * delta_time;
			p.vel += gravity * delta_time;

			// Free index if it falls below grid
			if (p.pos[1] <= 0.0f)
			{
				center.sortedPool.free(i);
				i--;
			}
			else
			{
				// Render the particle
				end::debug_renderer::add_line(p.pos, p.prevPos, p.color);
			}
		}

		// Free pool
		corners[0].spawnPos = { -5.0f, 0.0f,-5.0f };
		corners[0].spawnCol = { 1.0f, 0.0f, 0.0f, 1.0f };	// red
		corners[1].spawnPos = { 5.0f, 0.0f,-5.0f };
		corners[1].spawnCol = { 0.0f, 1.0f, 0.0f, 1.0f };	// green
		corners[2].spawnPos = { -5.0f, 0.0f, 5.0f };
		corners[2].spawnCol = { 0.0f, 0.0f, 1.0f, 1.0f };	// blue
		corners[3].spawnPos = { 5.0f, 0.0f, 5.0f };
		corners[3].spawnCol = { 1.0f, 1.0f, 0.0f, 1.0f };	// yellow

		// For each emitter
		for (int j = 0; j < 4; ++j)
		{
			// Spawn particles from free pool
			for (int i = 0; i < spawnCount; ++i)
			{
				// call alloc on free pool to give you index of particle activated in the free pool
				int16_t particleNdx = sharedPool.alloc();
				if (particleNdx != -1)
				{
					// allocate space in index pool (like push back)
					int16_t  emitterNdx = corners[j].indices.alloc(); // index into sorted 
					if (emitterNdx != -1)
					{
						// fill the index with the index from sharedpool?
						corners[j].indices[emitterNdx] = particleNdx;

						particle& p = sharedPool[particleNdx];
						p.color		= corners[j].spawnCol;
						p.pos		= corners[j].spawnPos;

						float randX = RandNumToNum(-0.75, 0.75 );
						float randY = RandNumToNum( 5.0, 10.0);
						float randZ = RandNumToNum(-0.75, 0.75 );
						p.vel		= { randX, randY, randZ };
					}
					else
					{
						// if the index returns invalid, free index in free pool because it can't be stored
						sharedPool.free(particleNdx);
					}
				}
			}
		}

		// UPDATE
		for (int i = 0; i < 4; ++i)// for each emitter
		{
			for (int j = 0; j < corners[i].indices.size(); ++j) // for each index
			{
				// find corresponding particle and update
				particle& p = sharedPool[corners[i].indices[j]];
				p.prevPos	= p.pos;
				p.pos		+= p.vel   * (float)delta_time;
				p.vel		+= gravity * (float)delta_time;

				// Free index if it falls below grid
				if (p.pos[1] <= 0.0f)
				{
					sharedPool.free(corners[i].indices[j]);
					corners[i].indices.free(j);
					j--;
				}
				else
				{
					// Render the particle
					end::debug_renderer::add_line(p.pos, p.prevPos, p.color);
				}
			}
		}
#endif

		// LAB 2 TRANSFORMS
		//mx1[3].xyz = { 0.0f, 0.5f, 0.0f };
		end::debug_renderer::add_matrix_transform(mx1);
		
		float3 translation{0.0f, 0.0f, 1.0f};

		float speed = 2.0f;

		if (keystate[inputMap['w']] == true)
		{
			mx1[3].z += 1.0f * speed * (float)delta_time;
		}
		else if (keystate[inputMap['s']] == true)
		{
			mx1[3].z -= 1.0f * speed * (float)delta_time;
		}
		else if (keystate[inputMap['d']] == true)
		{
			//(DirectX::XMMATRIX&)mx1 = DirectX::XMMatrixRotationY(DegreesToRadians(60)) * delta_time;;
		}
		else if (keystate[inputMap['a']] == true)
		{
			//(DirectX::XMMATRIX&)mx1 = DirectX::XMMatrixRotationY(DegreesToRadians(60)) * delta_time;;
		}
	}

	void dev_app_t::update_camera()
	{
		// do some camera stuff
	}

	void dev_app_t::matrix_turn_to()
	{
		// need up/down rotation matrix
		// need left/right rotation matrix
	}
	void dev_app_t:: matrix_look_at()
	{

	}

	float dev_app_t::RandNumToNum(float _a, float _b)
	{
		float rando = rand() / (RAND_MAX + 1.0f);
		float diff  = _b - _a;
		float r		= rando * diff;
		return _a + r;
	}

	float dev_app_t::DegreesToRadians(float _angle)
	{
		return _angle * (3.14 / 180);
	}
}