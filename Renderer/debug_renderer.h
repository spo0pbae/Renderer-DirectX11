#ifndef DEBUG_RENDERER_H
#define DEBUG_RENDERER_H

#include "math_types.h"

// Interface to the debug renderer
namespace end
{
	namespace debug_renderer
	{
		void add_line(float3 point_a, float3 point_b, float4 color_a, float4 color_b);
		inline void add_line(float3 p, float3 q, float4 color) { add_line(p, q, color, color); }
		void clear_lines();
		const pixel* get_line_verts();
		size_t get_line_vert_count();
		size_t get_line_vert_capacity();
		void add_matrix_transform(const float4x4&);
		void add_matrix_transform_extended_z(const float4x4& _mx);
		void add_grid(float _size, float4 _color);
		// Draws frustum with given vertices using debug_renderer
		void add_cube(float4* _vertices, float4 _color);
	}
} // namespace end

#endif