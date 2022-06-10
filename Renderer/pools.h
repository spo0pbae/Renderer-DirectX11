#pragma once
#include <type_traits>
#include <array>
#include <cassert>

// some tips
// freepool[index] = p;
// call allocate on free pool to give you index of particle activated in the free pool
// for each emitter loop through that emitters indices
// for each index find the corresponding particle and update
// free pool is master list of particles, dont want to loop through that, want to loop through the sorted pool

// one emitter in center using a sorted pool
// 4 emitter in corners to share a free pool
namespace end
{
	template<typename T, int16_t N>
	class sorted_pool_t
	{
	public:
		// TODO: Implement the function bodies

		// Returns the number of active elements
		size_t size()const 
		{
			return active_count;
		}

		// Returns the maximum supported number of elements 
		size_t capacity()const 
		{
			return N;
		}

		// Returns the value at the specified index
		T& operator[](int16_t _index) 
		{ 
			return pool[_index];
		}

		// Returns the value at the specified index
		const T& operator[](int16_t _index)const
		{ 
			return pool[_index];
		}

		// Returns the index of the first inactive element and updates the active count
		// Returns -1 if no inactive elements remain
		int16_t alloc()
		{
			if (active_count < N)
				return active_count++;
			else
				return -1;
		}

		// Moves the element at 'index' to the inactive region and updates the active count
		// Kills particle at index by swapping with last element
		void free(int16_t _index)
		{
			std::swap(pool[_index], pool[--active_count]);
		}

	private:
		T pool[N];
		int16_t active_count = 0;

	}; // sorted_pool_t

	// If freepool allocation fails you dont need to do anything
	template<typename T, int16_t N>
	class pool_t
	{
		public:
		// TODO: Implement the function bodies
		// Allocate by grabbing next free element and returning it
		// Removes the first element from the free list and returns its index
		// Returns -1 if no free elements remain
		int16_t alloc()
		{
			if (free_start >= N)
				return -1;

			int16_t index = free_start;
			free_start = pool[index].next;

			return index;
		}

		// Frees an element by adding the element back to the free list
		// Adds 'index' to the free list
		void free(int16_t _index)
		{
			pool[_index].next = free_start;
			free_start = _index;
		}

		// Initializes the free list by adding each element to the free list
		// store the free lists' 'next' pointers within the free block itself
		pool_t()
		{
			for (int i = 0; i < N; ++i)
			{
				pool[i].next = i + 1;
			}
		}

		// Returns the value at the specified index
		T& operator[](int16_t _index) 
		{
			return pool[_index].value;
		}

		// Returns the value at the specified index
		const T& operator[](int16_t _index)const 
		{
			return pool[_index].value;
		}

	private:
		// unions only cost as much as the largest variable. It reuses memory space for the different variables.
		// this means whatever variable is set last is the only usable number
		// memory blocks residing on the free list are, by definition, free memory blocks
		union element_t			
		{
			T value;
			int16_t next;		// gets set when the node dies	
			element_t(){}
		};
		element_t pool[N];		// list of 
		int16_t free_start = 0;
	}; // pool_t 

} // namespace end