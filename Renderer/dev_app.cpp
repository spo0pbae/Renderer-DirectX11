#include <iostream>

#include "dev_app.h"
#include "debug_renderer.h"
#include "frustum_culling.h"

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
		init_content();
	}

	void dev_app_t::init_content()
	{
		// Initialize matrix positions
		(DirectX::XMMATRIX&)mx1 = DirectX::XMMatrixIdentity();
		mx1[3].xyz = { 0.0f, 0.1f, 0.0f };

		(DirectX::XMMATRIX&)mx2 = DirectX::XMMatrixIdentity();
		mx2[3].xyz = { -1.5f, 1.75f, 0.0f };

		(DirectX::XMMATRIX&)mx3 = DirectX::XMMatrixIdentity();
		mx3[3].xyz = { 0.5f, 0.75f, 1.5f };
	}

	void dev_app_t::update()
	{
		delta_time = calc_delta_time();

		// Draw grid
		end:debug_renderer::create_grid(10, gridCol);

		update_grid_color();

		// LAB 1 PARTICLES
#if 0
		int spawnCount = 3;
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

		// LAB 2 MATRIX BEHAVIOURS
		// Draw transforms
		end::debug_renderer::add_matrix_transform_extended_z(mx1);
		end::debug_renderer::add_matrix_transform(mx2);
		end::debug_renderer::add_matrix_transform(mx3);

		// Handle movement inputs for matrix transform and camera
		const float speed		= 2.0f;
		const float camSpeed	= 5.0f;
		move_transform(speed);
		move_camera(camSpeed);

		// Look-At and Turn-To
		float4 eye = mx2[3];
		float4 at  = mx1[3];
		float4 up  = { 0.0f, 1.0f, 0.0f, 0.0f };

		mx2 = matrix_look_at(eye, at, up);
		mx3 = matrix_turn_to(mx3, at, delta_time);

		// MOUSE
		float deltaX = currMouseX - prevMouseX;
		prevMouseX = currMouseX;

		float deltaY = currMouseY - prevMouseY;
		prevMouseY = currMouseY;

		float sens = 0.003f;
		cam->view_mat = mouse_look(cam->view_mat, deltaX, deltaY, sens);
	}

	void dev_app_t::update_grid_color()
	{
		float colorTime = delta_time * 0.25f;

		// check to see if we need to change flag
		if (gridCol[2] >= 0.65f) colorFlag = true;	
		else if (gridCol[2] <= 0.1f) colorFlag = false;
	
		// update color based on flag
		if (colorFlag == true) colorTime = -colorTime;
		else gridCol[0] += colorTime;
		
		// always update blue
		gridCol[2] += colorTime;
	}

	float4x4 dev_app_t::matrix_look_at(float4 _viewerPos, float4 _targetPos, float4 _localUp)
	{
		DirectX::XMVECTOR x, y, z;

		// Define vector between target and viewer
		z = DirectX::XMVectorSubtract((DirectX::XMVECTOR&)_targetPos, (DirectX::XMVECTOR&)_viewerPos);
		z = DirectX::XMVector4Normalize(z);

		// Define Up
		x = DirectX::XMVector3Cross((DirectX::XMVECTOR&)_localUp, (DirectX::XMVECTOR&)z);
		x = DirectX::XMVector4Normalize(x);

		// Calculate new Y
		y = DirectX::XMVector3Cross(z, x);
		y = DirectX::XMVector4Normalize(y);

		float4x4 temp =
		{
			(float4&)x,
			(float4&)y,
			(float4&)z,
			_viewerPos
		};
		return temp;
	}

	float4x4 dev_app_t::matrix_turn_to(float4x4 _viewer, float4 _targetPos, float _rotationSpeed)
	{
		float4x4 temp = _viewer;

		DirectX::XMVECTOR z;
		DirectX::XMVECTOR viewerNorm = (DirectX::XMVECTOR&)temp[0]; // viewer's x axis
		DirectX::XMVECTOR viewerPos  = (DirectX::XMVECTOR&)temp[3];

		// Define vector between target and viewer
		z = DirectX::XMVectorSubtract((DirectX::XMVECTOR&)_targetPos, (DirectX::XMVECTOR&)viewerPos);
		z = DirectX::XMVector4Normalize(z);

		// Calculate turn rate
		DirectX::XMVECTOR directionRateY = DirectX::XMVector4Dot(z, viewerNorm);
		DirectX::XMVECTOR directionRateX = DirectX::XMVector4Dot(z, (DirectX::XMVECTOR&)temp[1]);

		// rotate on Y and X
		DirectX::XMMATRIX rotY = DirectX::XMMatrixRotationY(DirectX::XMVectorGetY(directionRateY) * _rotationSpeed);
		DirectX::XMMATRIX rotX = DirectX::XMMatrixRotationX(-DirectX::XMVectorGetX(directionRateX) * _rotationSpeed);

		(DirectX::XMMATRIX&)temp = DirectX::XMMatrixMultiply(rotY, (DirectX::XMMATRIX&)temp);
		(DirectX::XMMATRIX&)temp = DirectX::XMMatrixMultiply(rotX, (DirectX::XMMATRIX&)temp);

		// orthonormalize to keep the matrix upright
		return matrix_orthonormalize(temp, temp[1]);
	}

	float4x4 dev_app_t::matrix_orthonormalize(float4x4 _mx, float4 _up)
	{
		DirectX::XMVECTOR z, x, y;

		z = (DirectX::XMVECTOR&)_mx[2];
		z = DirectX::XMVector4Normalize(z);

		x = DirectX::XMVector3Cross((DirectX::XMVECTOR&)_up, z);
		x = DirectX::XMVector4Normalize(x);

		y = DirectX::XMVector3Cross(z, x);
		y = DirectX::XMVector4Normalize(y);

		float4x4 temp =
		{
			(float4&)x,
			(float4&)y,
			(float4&)z,
			_mx[3]
		};
		return temp;
	}

	float4x4_a dev_app_t::matrix_orthonormalize_a(float4x4_a _mx, float4 _worldUp)
	{
		DirectX::XMVECTOR z, x, y;

		z = (DirectX::XMVECTOR&)_mx[2];
		z = DirectX::XMVector4Normalize(z);

		x = DirectX::XMVector3Cross((DirectX::XMVECTOR&)_worldUp, z);
		x = DirectX::XMVector4Normalize(x);

		y = DirectX::XMVector3Cross(z, x);
		y = DirectX::XMVector4Normalize(y);

		float4x4_a temp =
		{
			(float4_a&)x,
			(float4_a&)y,
			(float4_a&)z,
			_mx[3]
		};
		return temp;
	}

	// Translates matrix transform along X/Z in local space
	void dev_app_t::move_transform(const float _speed)
	{
		(DirectX::XMMATRIX&)mx1 = DirectX::XMMatrixInverse({ 0 }, (DirectX::XMMATRIX&)mx1);
		if (keys[38] == true)
		{
			mx1[3].z -= 1.0f * _speed * static_cast<float>(delta_time);
		}
		else if (keys[40] == true)
		{
			mx1[3].z += 1.0f * _speed * static_cast<float>(delta_time);
		}
		else if (keys[39] == true)
		{
			(DirectX::XMMATRIX&)mx1 *= DirectX::XMMatrixRotationY(-_speed * static_cast<float>(delta_time));
		}
		else if (keys[37] == true)
		{
			(DirectX::XMMATRIX&)mx1 *= DirectX::XMMatrixRotationY(_speed * static_cast<float>(delta_time));
		}
		(DirectX::XMMATRIX&)mx1 = DirectX::XMMatrixInverse({ 0 }, (DirectX::XMMATRIX&)mx1);
	}

	// Translates camera along X/Z in local space
	void dev_app_t::move_camera(const float _speed)
	{
		(DirectX::XMMATRIX&)cam->view_mat = DirectX::XMMatrixInverse({ 0 }, (DirectX::XMMATRIX&)cam->view_mat);
		if (keys[87] == true)		// W
		{
			cam->view_mat[3].z -= _speed * static_cast<float>(delta_time);
		}
		else if (keys[83] == true)	// S
		{
			cam->view_mat[3].z += _speed * static_cast<float>(delta_time);
		}
		else if (keys[68] == true)	// D
		{
			cam->view_mat[3].x -= _speed * static_cast<float>(delta_time);
		}
		else if (keys[65] == true)	// A 
		{
			cam->view_mat[3].x += _speed * static_cast<float>(delta_time);
		}
		else if (keys[32] == true)	// SPACE
		{
			cam->view_mat[3].y -= _speed * static_cast<float>(delta_time);
		}
		else if (keys[16] == true)	// LSHIFT
		{
			cam->view_mat[3].y += _speed * static_cast<float>(delta_time);
		}
		(DirectX::XMMATRIX&)cam->view_mat = DirectX::XMMatrixInverse({ 0 }, (DirectX::XMMATRIX&)cam->view_mat);
	}

	float4x4_a dev_app_t::mouse_look(float4x4_a _viewMx, float _deltaX, float _deltaY, float _sensitivity)
	{
		float4x4_a temp = _viewMx;
		DirectX::XMMATRIX rotY = DirectX::XMMatrixRotationY(_deltaX * _sensitivity);
		DirectX::XMMATRIX rotX = DirectX::XMMatrixRotationX(_deltaY * _sensitivity);

		// apply rotation
		(DirectX::XMMATRIX&)temp = DirectX::XMMatrixMultiply(rotX, (DirectX::XMMATRIX&)temp);
		(DirectX::XMMATRIX&)temp = DirectX::XMMatrixMultiply(rotY, (DirectX::XMMATRIX&)temp);

		return matrix_orthonormalize_a(temp, {0.0f, 1.0f, 0.0f, 0.0f});
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

	double dev_app_t::calc_delta_time()
	{
		static std::chrono::time_point<std::chrono::high_resolution_clock> last_time = std::chrono::high_resolution_clock::now();

		std::chrono::time_point<std::chrono::high_resolution_clock> new_time = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed_seconds = new_time - last_time;
		last_time = new_time;

		return std::min(1.0 / 15.0, elapsed_seconds.count());
	}
}