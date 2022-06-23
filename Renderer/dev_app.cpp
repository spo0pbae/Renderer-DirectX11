#include <iostream>

#include "dev_app.h"

namespace end
{
	using namespace DirectX;

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
		//std::cout << "Log whatever you need here.\n";
		init_content();
	}

	void dev_app_t::init_content()
	{
		// Lab 2
		// Initialize matrix positions
		(XMMATRIX&)target = XMMatrixIdentity();
		target[3].xyz = { 0.0f, 0.1f, 0.0f };

		(XMMATRIX&)viewer1 = XMMatrixIdentity();
		viewer1[3].xyz = { -1.5f, 1.75f, 0.0f };

		(XMMATRIX&)viewer2 = XMMatrixIdentity();
		viewer2[3].xyz = { 0.5f, 0.75f, 1.5f };

		// Lab 3
		XMVECTOR eye = XMVectorSet(0.0f, 0.0f, -0.1f, 1.0f);
		XMVECTOR at	 = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMVECTOR up	 = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		// set viewing info
		targetView.m_aspectRatio = 1.85609400f;
		targetView.m_fov = 3.1415926f * 0.25f;

		targetView.view_mat = (float4x4_a&)XMMatrixInverse(nullptr, XMMatrixLookAtLH(eye, at, up));
		targetView.proj_mat = (float4x4_a&)XMMatrixPerspectiveFovLH(targetView.m_fov, targetView.m_aspectRatio, 1.0f, 10.0f);

		AABB[0].min = {-4.0f, 0.1f, 3.0f };
		AABB[0].max = {-2.0f, 2.0f, 5.0f };
		
		AABB[1].min = { 4.0f, 0.1f, 3.0f };
		AABB[1].max = { 5.0f, 0.75f, 5.0f };
		
		AABB[2].min = { -3.5f, 0.1f, -2.0f };
		AABB[2].max = { -1.0f, 1.1f, -1.0f };

		for (int i = 0; i < NUM_AABBS; ++i)
			AABB[i].col = { 0.0f, 1.0f, 1.0f, 1.0f };	// col
	}

	void dev_app_t::update()
	{
		delta_time = calc_delta_time();

		// Draw grid
		end:debug_renderer::add_grid(20, gridCol);

		update_grid_color();

		// LAB 1 PARTICLES
#pragma region Lab 1
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
#pragma endregion

#pragma region Lab 2
		// LAB 2 MATRIX BEHAVIOURS
		// Draw transforms
		end::debug_renderer::add_matrix_transform_extended_z(target);
		end::debug_renderer::add_matrix_transform(viewer1);
		end::debug_renderer::add_matrix_transform(viewer2);

		// Handle movement inputs for matrix transform and camera
		const float speed		= 2.0f;
		const float camSpeed	= 5.0f;
		move_transform(speed);
		move_camera(camSpeed);

		// Look-At and Turn-To
		float4 eye = viewer1[3];
		float4 at  = target[3];
		float4 up  = { 0.0f, 1.0f, 0.0f, 0.0f };

		viewer1 = matrix_look_at(eye, at, up);
		viewer2 = matrix_turn_to(viewer2, at, delta_time);

		// MOUSE
		float deltaX = currMouseX - prevMouseX;
		prevMouseX = currMouseX;

		float deltaY = currMouseY - prevMouseY;
		prevMouseY = currMouseY;

		float sens = 0.003f;
		cam->view_mat = mouse_look(cam->view_mat, deltaX, deltaY, sens);
#pragma endregion

		// LAB 3 FRUSTUM CULLING
		for (size_t i = 0; i < NUM_AABBS; ++i)
			add_aabb(AABB[i]);
		
		calculate_frustum(frustum, targetView, target);
		
		// check aabbs against frustum and change colors based on the result
		for (int i = 0; i < NUM_AABBS; ++i)
		{
			bool inFrust = aabb_to_frustum(AABB[i], frustum);
		
			if (inFrust == false) 
				AABB[i].col = { 0.0f, 1.0f, 1.0f, 1.0f };
			else 
				AABB[i].col = { 1.0f, 1.0f, 0.0f, 1.0f };
		}
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
		XMVECTOR x, y, z;

		// Define vector between target and viewer
		z = XMVectorSubtract((XMVECTOR&)_targetPos, (XMVECTOR&)_viewerPos);
		z = XMVector4Normalize(z);

		// Define Up
		x = XMVector3Cross((XMVECTOR&)_localUp, (XMVECTOR&)z);
		x = XMVector4Normalize(x);

		// Calculate new Y
		y = XMVector3Cross(z, x);
		y = XMVector4Normalize(y);

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

		XMVECTOR z;
		XMVECTOR viewerNorm = (XMVECTOR&)temp[0]; // viewer's x axis
		XMVECTOR viewerPos  = (XMVECTOR&)temp[3];

		// Define vector between target and viewer
		z = XMVectorSubtract((XMVECTOR&)_targetPos, (XMVECTOR&)viewerPos);
		z = XMVector4Normalize(z);

		// Calculate turn rate
		XMVECTOR directionRateY = XMVector4Dot(z, viewerNorm);
		XMVECTOR directionRateX = XMVector4Dot(z, (XMVECTOR&)temp[1]);

		// rotate on Y and X
		XMMATRIX rotY = XMMatrixRotationY(XMVectorGetY(directionRateY) * _rotationSpeed);
		XMMATRIX rotX = XMMatrixRotationX(-XMVectorGetX(directionRateX) * _rotationSpeed);

		(XMMATRIX&)temp = XMMatrixMultiply(rotY, (XMMATRIX&)temp);
		(XMMATRIX&)temp = XMMatrixMultiply(rotX, (XMMATRIX&)temp);

		// orthonormalize to keep the matrix upright
		return matrix_orthonormalize(temp, { 0.0f, 1.0f, 0.0f, 0.0f });//temp[1]);
	}

	float4x4 dev_app_t::matrix_orthonormalize(float4x4 _mx, float4 _up)
	{
		XMVECTOR z, x, y;

		z = (XMVECTOR&)_mx[2];
		z = XMVector4Normalize(z);

		x = XMVector3Cross((XMVECTOR&)_up, z);
		x = XMVector4Normalize(x);

		y = XMVector3Cross(z, x);
		y = XMVector4Normalize(y);

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
		XMVECTOR z, x, y;

		z = (XMVECTOR&)_mx[2];
		z = XMVector4Normalize(z);

		x = XMVector3Cross((XMVECTOR&)_worldUp, z);
		x = XMVector4Normalize(x);

		y = XMVector3Cross(z, x);
		y = XMVector4Normalize(y);

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
		(XMMATRIX&)target = XMMatrixInverse({ 0 }, (XMMATRIX&)target);
		if (keys[38] == true)
		{
			target[3].z -= 1.0f * _speed * static_cast<float>(delta_time);
		}
		else if (keys[40] == true)
		{
			target[3].z += 1.0f * _speed * static_cast<float>(delta_time);
		}
		else if (keys[39] == true)
		{
			(XMMATRIX&)target *= XMMatrixRotationY(-_speed * static_cast<float>(delta_time));
		}
		else if (keys[37] == true)
		{
			(XMMATRIX&)target *= XMMatrixRotationY(_speed * static_cast<float>(delta_time));
		}
		(XMMATRIX&)target = XMMatrixInverse({ 0 }, (XMMATRIX&)target);
	}

	// Translates camera along X/Z in local space
	void dev_app_t::move_camera(const float _speed)
	{
		(XMMATRIX&)cam->view_mat = XMMatrixInverse({ 0 }, (XMMATRIX&)cam->view_mat);
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
		(XMMATRIX&)cam->view_mat = XMMatrixInverse({ 0 }, (XMMATRIX&)cam->view_mat);
	}

	float4x4_a dev_app_t::mouse_look(float4x4_a _viewMx, float _deltaX, float _deltaY, float _sensitivity)
	{
		float4x4_a temp = _viewMx;
		XMMATRIX rotY = XMMatrixRotationY(_deltaX * _sensitivity);
		XMMATRIX rotX = XMMatrixRotationX(_deltaY * _sensitivity);

		// apply rotation
		(XMMATRIX&)temp = XMMatrixMultiply(rotX, (XMMATRIX&)temp);
		(XMMATRIX&)temp = XMMatrixMultiply(rotY, (XMMATRIX&)temp);

		return matrix_orthonormalize_a(temp, {0.0f, 1.0f, 0.0f, 0.0f});
	}

	float dev_app_t::rand_num_to_num(float _a, float _b)
	{
		float rando = rand() / (RAND_MAX + 1.0f);
		float diff  = _b - _a;
		float r		= rando * diff;
		return _a + r;
	}

	float dev_app_t::degrees_to_radians(float _angle)
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