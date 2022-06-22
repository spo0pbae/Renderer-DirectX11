#ifndef FRUSTUM_CULLING_H
#define FRUSTUM_CULLING_H

#define FRUST_VERT_COUNT 8

#include <array>
#include "debug_renderer.h"
#include "math_types.h"
#include "view.h"

// Note: You are free to make adjustments/additions to the declarations provided here.
namespace end
{

	struct sphere_t { float3 center; float radius; };			// Alterative: using sphere_t = float4;
	struct aabb_t	{ float3 min; float3 max; float4 col; };	// Alternative: aabb_t { float3 center; float3 extents; };
	struct plane_t	{ float3 normal; float offset; };			// Alterative: using plane_t = float4;
	using frustum_t = std::array<plane_t, 6>;

	__declspec(selectany) float4 frustumVerts[FRUST_VERT_COUNT]
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

	// Calculates the plane of a triangle from three points.
	plane_t calculate_plane(float3 _a, float3 _b, float3 _c);

	// Calculates a frustum (6 planes) from the input view parameter.
	//
	// Calculate the eight corner points of the frustum. 
	// Use your debug renderer to draw the edges.
	// 
	// Calculate the frustum planes.
	// Use your debug renderer to draw the plane normals as line segments.
	void calculate_frustum(frustum_t& _frustum, const view_t& _view, float4x4 _world);

	// Calculates which side of a plane the sphere is on.
	//
	// Returns -1 if the sphere is completely behind the plane.
	// Returns 1 if the sphere is completely in front of the plane.
	// Otherwise returns 0 (Sphere overlaps the plane)
	int classify_sphere_to_plane(const sphere_t& _sphere, const plane_t& _plane);

	// Calculates which side of a plane the aabb is on.
	//
	// Returns -1 if the aabb is completely behind the plane.
	// Returns 1 if the aabb is completely in front of the plane.
	// Otherwise returns 0 (aabb overlaps the plane)
	// MUST BE IMPLEMENTED UsING THE PROJECTED RADIUS TEST
	int classify_aabb_to_plane(const aabb_t& _aabb, const plane_t& _plane);

	// Determines if the aabb is inside the frustum.
	//
	// Returns false if the aabb is completely behind any plane.
	// Otherwise returns true.
	bool aabb_to_frustum(const aabb_t& _aabb, const frustum_t& _frustum);

	// Returns the average position of given vertices, used for drawing normals of a plane
	float3 get_avg_verts(float3 _a, float3 _b, float3 _c, float3 _d);

	// Draws frustum with given vertices using debug_renderer
	void add_frustum(float4* _vertices);

	// Draws 12 edges of AABB box using debug_renderer
	void add_aabb(aabb_t _aabb);

}// namespace end

#endif