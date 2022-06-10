#include "debug_renderer.h"
#include <array>

// Anonymous namespace
namespace
{
	// Declarations in an anonymous namespace are global BUT only have internal linkage.
	// In other words, these variables are global but are only visible in this source file.

	// Maximum number of debug lines at one time (i.e: Capacity)
	constexpr size_t MAX_LINE_VERTS = 4096; 

	// CPU-side buffer of debug-line verts
	// Copied to the GPU and reset every frame.
	size_t line_vert_count = 0;
	std::array<end::colored_vertex, MAX_LINE_VERTS> line_verts;
}

namespace end
{
	namespace debug_renderer
	{
		void add_line(float3 point_a, float3 point_b, float4 color_a, float4 color_b)
		{
			if (line_vert_count + 2 < MAX_LINE_VERTS)
			{
				// Draw a line from point A to point B
				line_verts[line_vert_count++] = colored_vertex{ point_a, color_a };
				line_verts[line_vert_count++] = colored_vertex{ point_b, color_b };
			}
		}

		// Resets line_vert_count to 0
		void clear_lines()
		{
			line_vert_count = 0;
		}

		// Returns the line vert array pointer
		const colored_vertex* get_line_verts()
		{ 
			return line_verts.data();
		}

		// Returns how many vertices there are now
		size_t get_line_vert_count() 
		{ 
			return line_vert_count;
		}

		// Returns the maximum size the line vert array
		size_t get_line_vert_capacity()
		{
			return MAX_LINE_VERTS;
		}

		// const?
		void add_matrix_transform(const float4x4& _mx)
		{
			// matrix position
			const float3 start = _mx[3].xyz;

			// for each line in the transform
			for (int i = 0; i < 3; ++i)
			{
				// set the color for the respective direction (XYZ)
				float4 color{ 0.0f, 0.0f, 0.0f, 1.0f };
				color[i] = 1.0f;

				//end = matrix.pos + matrix
				float3 end = start + _mx[i].xyz;
				add_line(start, end, color, color);
			}
		}

		void create_grid(float _size, float4 _color)
		{
			float spacing	= 0.5f;
			int lineCount	= (int)(_size / spacing);

			float x = -_size / 2.0f;
			float y = -_size / 2.0f;
			float xS = spacing, yS = spacing;

			y = -_size / 2.0f;
			for (int i = 0; i <= lineCount; ++i)
			{
				if (i != lineCount / 2)
					add_line({ x, 0.0f, y }, { x + _size, 0.0f, y }, _color);
				else
					add_line({ x, 0.0f, y }, { x + _size, 0.0f, y }, { 1.0f, 1.0f, 1.0f, 1.0f });
				y += yS;
			}

			y = -_size / 2.0f;
			x = -_size / 2.0f;

			for (int i = 0; i <= lineCount; ++i)
			{
				if (i != lineCount / 2)
					add_line({ x, 0.0f, y }, { x , 0.0f, y + _size }, _color);
				else
					add_line({ x, 0.0f, y }, { x , 0.0f, y + _size }, { 1.0f , 1.0f, 1.0f, 1.0f });
				x += xS;
			}
		}
	}
}