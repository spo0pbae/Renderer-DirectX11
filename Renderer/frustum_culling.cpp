#include "frustum_culling.h"

namespace end
{
	plane_t calculate_plane(float3 _a, float3 _b, float3 _c) 
	{
		plane_t plane;
		DirectX::XMVECTOR offset;

		// calculate normal
		DirectX::XMVECTOR ab = DirectX::XMVectorSubtract((DirectX::XMVECTOR&)_a, (DirectX::XMVECTOR&)_b);
		DirectX::XMVECTOR ca = DirectX::XMVectorSubtract((DirectX::XMVECTOR&)_c, (DirectX::XMVECTOR&)_a);;
		(DirectX::XMVECTOR&)plane.normal = DirectX::XMVector3Cross((DirectX::XMVECTOR&)ab, (DirectX::XMVECTOR&)ca);
		(DirectX::XMVECTOR&)plane.normal = DirectX::XMVector3Normalize((DirectX::XMVECTOR&)plane.normal);

		// calculate offset
		offset = DirectX::XMVector3Dot((DirectX::XMVECTOR&)_a, (DirectX::XMVECTOR&)plane.normal);
		plane.offset = DirectX::XMVectorGetX(offset);

		return plane;
	}

	void calculate_frustum(frustum_t& _frustum, const view_t& _view, float4x4 _world) 
	{
		float xPos = 1280 * 0.5f;
		float yPos = 720  * 0.5f;
		float depth = 0.0f;
		
		// init center point
		DirectX::XMVECTOR center = DirectX::XMVectorSet(xPos, yPos, depth, 1);

		center = DirectX::XMVector3Unproject
		(
			center, 
			vpData.topleftX, vpData.topleftY, 
			vpData.width,    vpData.height, 
			vpData.minDepth, vpData.maxDepth,
			(DirectX::XMMATRIX&)_view.proj_mat,
			(DirectX::XMMATRIX&)_view.view_mat,
			(DirectX::XMMATRIX&)_world
		);

		// calculate corner points of frustum
	}

	int classify_sphere_to_plane(const sphere_t& _sphere, const plane_t& _plane) 
	{
		int val;
		return val;
	}

	int classify_aabb_to_plane(const aabb_t& _aabb, const plane_t& _plane) 
	{
		int val;
		return val;
	}

	bool aabb_to_frustum(const aabb_t& _aabb, const frustum_t& _frustum) 
	{
		bool temp;
		return temp;
	}

}// namespace end