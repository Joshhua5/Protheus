/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 17:1:2016 Waring J.

*************************************************************************/
#pragma once

#include "Vector2.h"

namespace Pro {
	namespace Util {

		template<typename T, int Quantity>
		struct QuadTreeData {
			struct DataContainer {
				T data_;
				Math::Vector2<float> data_point_;
			} data_[Quantity];

			QuadTreeData() {
				for (int x = 0; x < Quantity; ++x)
					data_[x].data_ = nullptr;
			}

			//! Returns false if there wasn't enough room to store this object 
			bool Insert(const T& value, const Vector2<float>& point) {
				for (int x = 0; x < Quantity; ++x) {
					if (data_[x].data_ == nullptr) {
						data_[x].data_point_ = point;
						data_[x].data_ = value;
						return true;
					}
				}
				return false;
			}

			DataContainer* Get(const T& value) {
				for (int x = 0; x < Quantity; ++x)
					if (data_[x].data_ == value)
						return data_[x];
				return nullptr;
			}

			inline bool Contains(const T& value) {
				return Get(value) != nullptr;
			}


		};

		//! MAX is equal to the maximum amount of elements that can be stored in a single node
		//! before being subdivided
		template<typename T, int MAX>
		class QuadTree {
			QuadTree<T, MAX>* children_[4];
			QuadTreeData<T, MAX>* node_data_;
			const unsigned depth_;

			Math::Vector2<float> origin_;
			Math::Vector2<float> half_dimensions_;

			inline unsigned GetQuadrant(const Math::Vector2<float>& point) {
				unsigned oct;
				oct = (point.x >= origin_.x) << 0; // Right is true
				oct |= (point.y >= origin_.y) << 1; // Up is true
				return oct;
			}

			QuadTree(const Math::Vector2<float>& origin, const  Math::Vector2<float>& dimensions, unsigned depth) :
				origin_(origin), half_dimensions_(dimensions / 2), depth_(depth) { }

			void Subdivide() {
				for (int x = 0; x < 4; ++x) {
					Math::Vector2<float> new_origin = origin_;

					new_origin.x += half_dimensions_.x * (x & 1) ? 0.5f : -0.5f;
					new_origin.y += half_dimensions_.y * (x & 2) ? 0.5f : -0.5f;

					children_[x] = new QuadTree<T, MAX>(new_origin, half_dimensions_, depth_ + 1);
					children_[x]->node_data_ = new QuadTreeData<T, MAX>();
				}

				auto temp_node = node_data_;
				node_data_ = nullptr;
				// Now we've subdivided we have to move our data into the lower nodes
				for (const auto& node_data : temp_node->data_)
					Insert(node_data.data_, node_data.data_point_);
				delete temp_node;
			}

		public:
			QuadTree(const Math::Vector2<float>& origin, const Math::Vector2<float>& dimensions) :
				origin_(origin), half_dimensions_(dimensions / 2), depth_(0) {
				node_data_ = new QuadTreeData<T, MAX>();
			}

			void Insert(const T& value, const Math::Vector2<float>& point) {
				if (node_data_ == nullptr)
					children_[GetQuadrant(point)]->Insert(value, point);
				else
					// If the data container is full then subdivide it and attempt to
					// insert the data again
					if (!node_data_->Insert(value, point)) {
						Subdivide();
						children_[GetQuadrant(point)]->Insert(value, point);
					}
			}

			bool Remove(const T& value, T* removed_item) {
				if (node_data_ == nullptr) {
					for (int x = 0; x < 4; ++x)
						if (children_[x]->Remove(value))
							return true;
					return false;
				}
				else {
					DataContainer* container = node_data_->Get(value);
					if (container != nullptr) {
						if(removed_item != nullptr)
							removed_item = container->data_;
						memset(container, 0, sizeof(DataContainer));
						return true;
					}
					return false;
				}
			}

			const QuadTreeData<T, MAX> GetQuadrantData(const Math::Vector2<float>& point) const {
				if (node_data_ == nullptr)
					return children_[GetQuadrant(point)].GetQuadrantData(point);
				return data_;
			} 

		};
	}
}