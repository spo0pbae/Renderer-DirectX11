#include "frustum_culling.h"

namespace end
{
	using namespace DirectX;

	plane_t calculate_plane(float3 _a, float3 _b, float3 _c) 
	{
		plane_t plane;
		XMVECTOR offset;

		// calculate normal
		XMVECTOR ba = XMVectorSubtract((XMVECTOR&)_b, (XMVECTOR&)_a);
		XMVECTOR cb = XMVectorSubtract((XMVECTOR&)_c, (XMVECTOR&)_b);;
		(XMVECTOR&)plane.normal = XMVector3Normalize(XMVector3Cross((XMVECTOR&)ba, (XMVECTOR&)cb));

		// calculate offset
		offset = XMVector3Dot((XMVECTOR&)_a, (XMVECTOR&)plane.normal);
		plane.offset = XMVectorGetX(offset);

		return plane;
	}

	void calculate_frustum(frustum_t& _frustum, const view_t& _view, float4x4 _world) 
	{
		// frustum vertices
		float4 vertices[8]
		{
			{-1.0f, -1.0f, 0.0f, 1.0f },
			{-1.0f,  1.0f, 0.0f, 1.0f },
			{ 1.0f,  1.0f, 0.0f, 1.0f },
			{ 1.0f, -1.0f, 0.0f, 1.0f },
			{-1.0f, -1.0f, 1.0f, 1.0f },
			{-1.0f,  1.0f, 1.0f, 1.0f },
			{ 1.0f,  1.0f, 1.0f, 1.0f },
			{ 1.0f, -1.0f, 1.0f, 1.0f }
		};

		



		// calculate corner points of frustum
	}

	int classify_sphere_to_plane(const sphere_t& _sphere, const plane_t& _plane) 
	{
		XMVECTOR v = XMVector3Dot((XMVECTOR&)_sphere.center, (XMVECTOR&)_plane.normal);
		float val = XMVectorGetX(v) - _plane.offset;

		// in front of the plane
		if (val > _sphere.radius)
			return 1;

		// Behind the plane
		else if (val < -_sphere.radius)
			return -1;

		// overlaps plane
		else
			return 0;
	}

	int classify_aabb_to_plane(const aabb_t& _aabb, const plane_t& _plane) 
	{
		// using a sphere to describe aabb
		sphere_t sphere;
		sphere.center = (_aabb.min + _aabb.max) * 0.5f;
		float3 extents = _aabb.max - sphere.center;

		// projected radius = dot(normal,extent) 
		sphere.radius = (extents.x * abs(_plane.normal.x)) + (extents.y * abs(_plane.normal.y)) + (extents.z * abs(_plane.normal.z));

		return classify_sphere_to_plane(sphere, _plane);
	}

	bool aabb_to_frustum(const aabb_t& _aabb, const frustum_t& _frustum) 
	{
		for (int i = 0; i < _frustum.size(); ++i)
			if (classify_aabb_to_plane(_aabb, _frustum[i]) == -1) return false;

		return true;
	}

}// namespace end