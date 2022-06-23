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
		offset = XMVector3Dot((XMVECTOR&)plane.normal, (XMVECTOR&)_a);
		plane.offset = XMVectorGetX(offset);

		return plane;
	}

	void calculate_frustum(frustum_t& _frustum, const view_t& _view, float4x4 _world) 
	{
		frustum_t temp;
		float3 normPos;
	
		// moved from .h because it wouldn't draw
		float4 frustumVerts[FRUST_VERT_COUNT] =
		{
			{-1.0f, -1.0f, 0.0f, 1.0f},
			{-1.0f,  1.0f, 0.0f, 1.0f},
			{ 1.0f,  1.0f, 0.0f, 1.0f},
			{ 1.0f, -1.0f, 0.0f, 1.0f},
			{-1.0f, -1.0f, 1.0f, 1.0f},
			{-1.0f,  1.0f, 1.0f, 1.0f},
			{ 1.0f,  1.0f, 1.0f, 1.0f},
			{ 1.0f, -1.0f, 1.0f, 1.0f}
		};
	
		XMMATRIX vp = XMMatrixMultiply((XMMATRIX&)_view.view_mat, (XMMATRIX&)_view.proj_mat);
		XMMATRIX vpw = XMMatrixInverse({ 0 }, vp);
		for (int i = 0; i < FRUST_VERT_COUNT; ++i)
		{
			// Get vertices into projection space
			(XMVECTOR&)frustumVerts[i] = XMVector4Transform((XMVECTOR&)frustumVerts[i], vpw);
			(XMVECTOR&)frustumVerts[i] /= XMVectorGetW((XMVECTOR&)frustumVerts[i]); // perspective divide
		}
		
		// draw the frustum
		add_frustum(frustumVerts);
		
		// calculate norms and draw their lines in the middle of the plane
		// near
		temp[0]			= calculate_plane((float3&)frustumVerts[3], (float3&)frustumVerts[2], (float3&)frustumVerts[1]);
		normPos			= get_avg_verts(frustumVerts[0].xyz, frustumVerts[1].xyz, frustumVerts[2].xyz, frustumVerts[3].xyz);
		temp[0].normal	+= (float3&)normPos;
		debug_renderer::add_line((float3&)normPos, temp[0].normal, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
		
		// far
		temp[1]			= calculate_plane((float3&)frustumVerts[7], (float3&)frustumVerts[5], (float3&)frustumVerts[6]);
		normPos			= get_avg_verts(frustumVerts[4].xyz, frustumVerts[5].xyz, frustumVerts[6].xyz, frustumVerts[7].xyz);
		temp[1].normal	+= (float3&)normPos;
		debug_renderer::add_line((float3&)normPos, temp[1].normal, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
		
		// top
		temp[2]			= calculate_plane((float3&)frustumVerts[6], (float3&)frustumVerts[1], (float3&)frustumVerts[2]);
		normPos			= get_avg_verts(frustumVerts[1].xyz, frustumVerts[2].xyz, frustumVerts[5].xyz, frustumVerts[6].xyz);
		temp[2].normal	+= (float3&)normPos;
		debug_renderer::add_line((float3&)normPos, temp[2].normal, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
		
		// bottom
		temp[3]			= calculate_plane((float3&)frustumVerts[3], (float3&)frustumVerts[0], (float3&)frustumVerts[7]);
		normPos			= get_avg_verts(frustumVerts[4].xyz, frustumVerts[7].xyz, frustumVerts[0].xyz, frustumVerts[3].xyz);
		temp[3].normal	+= (float3&)normPos;
		debug_renderer::add_line((float3&)normPos, temp[3].normal, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
		
		// right
		temp[4]			= calculate_plane((float3&)frustumVerts[6], (float3&)frustumVerts[2], (float3&)frustumVerts[7]);
		normPos			= get_avg_verts(frustumVerts[3].xyz, frustumVerts[2].xyz, frustumVerts[6].xyz, frustumVerts[7].xyz);
		temp[4].normal	+= (float3&)normPos;
		debug_renderer::add_line((float3&)normPos, temp[4].normal, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
		
		// left
		temp[5]			= calculate_plane((float3&)frustumVerts[4], (float3&)frustumVerts[1], (float3&)frustumVerts[5]);
		normPos			= get_avg_verts(frustumVerts[0].xyz, frustumVerts[1].xyz, frustumVerts[4].xyz, frustumVerts[5].xyz);
		temp[5].normal	+= (float3&)normPos;
		debug_renderer::add_line((float3&)normPos, temp[5].normal, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
		
		// Get each vertex into world space
		for (int i = 0; i < FRUST_VERT_COUNT; ++i)
			(XMVECTOR&)frustumVerts[i] = XMVector3Transform((XMVECTOR&)frustumVerts[i], (XMMATRIX&)_world);
		
		temp[0] = calculate_plane(frustumVerts[3].xyz, frustumVerts[2].xyz, frustumVerts[1].xyz);
		temp[1] = calculate_plane(frustumVerts[7].xyz, frustumVerts[5].xyz, frustumVerts[6].xyz);
		temp[2] = calculate_plane(frustumVerts[6].xyz, frustumVerts[1].xyz, frustumVerts[2].xyz);
		temp[3] = calculate_plane(frustumVerts[3].xyz, frustumVerts[0].xyz, frustumVerts[7].xyz);
		temp[4] = calculate_plane(frustumVerts[6].xyz, frustumVerts[2].xyz, frustumVerts[7].xyz);
		temp[5] = calculate_plane(frustumVerts[4].xyz, frustumVerts[1].xyz, frustumVerts[5].xyz);
		
		for (int i = 0; i < _frustum.size();++i)
			_frustum[i] = temp[i];
	}

	int classify_sphere_to_plane(const sphere_t& _sphere, const plane_t& _plane) 
	{
		XMVECTOR v = XMVector3Dot((XMVECTOR&)_sphere.center, (XMVECTOR&)_plane.normal);
		float val = XMVectorGetX(v) - _plane.offset;

		if (val > _sphere.radius)		// in front of the plane
			return 1;	

		else if (val < -_sphere.radius)	// Behind the plane
			return -1;

		else							// overlaps plane
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

	float3 get_avg_verts(float3 _a, float3 _b, float3 _c, float3 _d)
	{
		XMVECTOR avg =
		{
			(XMVectorGetX((XMVECTOR&)_a) + XMVectorGetX((XMVECTOR&)_b) + XMVectorGetX((XMVECTOR&)_c) + XMVectorGetX((XMVECTOR&)_d)) * 0.25f,
			(XMVectorGetY((XMVECTOR&)_a) + XMVectorGetY((XMVECTOR&)_b) + XMVectorGetY((XMVECTOR&)_c) + XMVectorGetY((XMVECTOR&)_d)) * 0.25f,
			(XMVectorGetZ((XMVECTOR&)_a) + XMVectorGetZ((XMVECTOR&)_b) + XMVectorGetZ((XMVECTOR&)_c) + XMVectorGetZ((XMVECTOR&)_d)) * 0.25f
		};
		return (float3&)avg;
	}

	void add_frustum(float4* _vertices)
	{
		float4 color = { 0.55f, 0.0f, 1.0f, 1.0f };

		// Near plane
		debug_renderer::add_line(_vertices[0].xyz, _vertices[3].xyz, color);
		debug_renderer::add_line(_vertices[1].xyz, _vertices[2].xyz, color);
		debug_renderer::add_line(_vertices[0].xyz, _vertices[1].xyz, color);
		debug_renderer::add_line(_vertices[3].xyz, _vertices[2].xyz, color);

		// Far plane
		debug_renderer::add_line(_vertices[7].xyz, _vertices[4].xyz, color);
		debug_renderer::add_line(_vertices[6].xyz, _vertices[5].xyz, color);
		debug_renderer::add_line(_vertices[4].xyz, _vertices[5].xyz, color);
		debug_renderer::add_line(_vertices[6].xyz, _vertices[7].xyz, color);

		// Right plane
		debug_renderer::add_line(_vertices[7].xyz, _vertices[3].xyz, color);
		debug_renderer::add_line(_vertices[6].xyz, _vertices[2].xyz, color);

		// Left plane
		debug_renderer::add_line(_vertices[0].xyz, _vertices[4].xyz, color);
		debug_renderer::add_line(_vertices[1].xyz, _vertices[5].xyz, color);
	}

	void add_aabb(aabb_t _aabb)
	{
		// front horizontal																									   
		debug_renderer::add_line({ _aabb.max.x, _aabb.max.y, _aabb.min.z }, { _aabb.min.x, _aabb.max.y, _aabb.min.z }, _aabb.col);
		debug_renderer::add_line({ _aabb.max.x, _aabb.min.y, _aabb.min.z }, { _aabb.min.x, _aabb.min.y, _aabb.min.z }, _aabb.col);

		// front vertical
		debug_renderer::add_line({ _aabb.max.x, _aabb.max.y, _aabb.min.z }, { _aabb.max.x, _aabb.min.y, _aabb.min.z }, _aabb.col);
		debug_renderer::add_line({ _aabb.min.x, _aabb.max.y, _aabb.min.z }, { _aabb.min.x, _aabb.min.y, _aabb.min.z }, _aabb.col);

		// back horizontal																									 
		debug_renderer::add_line({ _aabb.max.x, _aabb.max.y, _aabb.max.z }, { _aabb.min.x, _aabb.max.y, _aabb.max.z }, _aabb.col);
		debug_renderer::add_line({ _aabb.max.x, _aabb.min.y, _aabb.max.z }, { _aabb.min.x, _aabb.min.y, _aabb.max.z }, _aabb.col);

		// back vertical
		debug_renderer::add_line({ _aabb.max.x, _aabb.max.y, _aabb.max.z }, { _aabb.max.x, _aabb.min.y, _aabb.max.z }, _aabb.col);
		debug_renderer::add_line({ _aabb.min.x, _aabb.max.y, _aabb.max.z }, { _aabb.min.x, _aabb.min.y, _aabb.max.z }, _aabb.col);
																										   
		// right horizontal																											 
		debug_renderer::add_line({ _aabb.max.x, _aabb.max.y, _aabb.min.z }, { _aabb.max.x, _aabb.max.y, _aabb.max.z }, _aabb.col);
		debug_renderer::add_line({ _aabb.max.x, _aabb.min.y, _aabb.min.z }, { _aabb.max.x, _aabb.min.y, _aabb.max.z }, _aabb.col);

		// left horizontal
		debug_renderer::add_line({ _aabb.min.x, _aabb.max.y, _aabb.min.z }, { _aabb.min.x, _aabb.max.y, _aabb.max.z }, _aabb.col);
		debug_renderer::add_line({ _aabb.min.x, _aabb.min.y, _aabb.min.z }, { _aabb.min.x, _aabb.min.y, _aabb.max.z }, _aabb.col);
	}

}// namespace end