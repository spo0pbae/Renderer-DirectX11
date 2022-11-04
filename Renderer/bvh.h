#ifndef BVH_H
#define BVH_H

#include <cstdint>
#include <cassert>

#include <vector>

#include "math_types.h"
#include "frustum_culling.h"

//aabb_t{ float3 min; float3 max; };

namespace end
{
	// bvh_node_t
	//
	// BVH will be stored as a vector/array of this type
	// A node will always be at a higher index/address than its parent
	class bvh_node_t 
	{
	public:
		// TODO: This constructor is the only function for you to implement in this file.
		bvh_node_t(bvh_node_t* _root, uint32_t _leftIndex, uint32_t _rightIndex);

		bvh_node_t(const aabb_t& _bounds, uint32_t _id) : m_aabb{ _bounds }, n_id{ _id } {}

		bvh_node_t() = default;
		bvh_node_t(const bvh_node_t&) = default;

		// Root must be at [0], so parent index must be invalid
		inline bool is_root()const { return n_parent == UINT32_MAX; }

		inline bool is_branch()const { return n_is_branch; }

		inline bool is_leaf()const { return !is_branch(); }
		
		inline uint32_t element_id()const 
		{
			assert(is_leaf());
			return n_id; 
		}

		inline uint32_t& left()
		{
			assert(is_branch());
			return n_left;
		}

		inline uint32_t left()const
		{
			assert(is_branch());
			return n_left;
		}

		inline uint32_t& right()
		{
			assert(is_branch());
			return n_right;
		}

		inline uint32_t right()const
		{
			assert(is_branch());
			return n_right;
		}

		inline uint32_t parent()const
		{
			assert(!is_root());
			return n_parent;
		}

		uint32_t& parent()
		{
			assert(!is_root());
			return n_parent;
		}

		aabb_t& aabb()
		{
			return m_aabb;
		}

		const aabb_t& aabb()const
		{
			return m_aabb;
		}

	private:

		// Indices for connected nodes
		uint32_t n_parent = UINT32_MAX;

		// Index for left child.
		// Branches must have two children.
		// If "this" is a leaf, _left/_is_branch will be 0.
		union
		{
			uint32_t n_left;
			uint32_t n_is_branch;
		};

		// Index for right child.
		// Branches must have two children.
		// If "this" is a leaf, _right/_id stores the element id.
		union
		{
			uint32_t n_right;
			uint32_t n_id;
		};

		// AABB for this node
		aabb_t m_aabb;
	}; // Class bvh_node_t 

	struct bounding_volume_hierarchy_t
	{
		std::vector<bvh_node_t> m_bvh;

		inline bvh_node_t& node_at(uint32_t _i) { return m_bvh[_i]; }

		inline size_t node_count()const { return m_bvh.size(); }

		static float cost(const bvh_node_t& _a, const bvh_node_t& _b);

		//TODO
		//You will need to create a traversal function to render the aabb's as they collide on the way down
		//You could create a callback function as well and that function does the collision tests and drawws the aabb lines
		//a recursive depth-first function could work

		// Add an aabb/element_id pair to the bvh
		void insert(const aabb_t& _aabb, uint32_t _elementID);
	}; // struct bounding_volume_hierarchy_t

	// Declares a short-hand alias
	using bvh_t = bounding_volume_hierarchy_t;

} // namespace end

#endif