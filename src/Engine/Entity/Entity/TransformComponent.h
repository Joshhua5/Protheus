#pragma once

#include <cstdint>

#include "ComponentData.h"
#include "Component.h"

#include "../../Utilities/ProMath.h"

using namespace std;
using namespace Pro::Math;

namespace Pro {
	namespace Entity {

		struct TransformData : ComponentData {
			Vector3<float> Rotation;
			Vector3<float> Position;
		};

		class TransformComponent : Component<TransformData> {
			 
		public:
			constexpr static unsigned UUID() {
				return FowlerNollVoHash(__PRETTY_FUNCTION__, sizeof(__PRETTY_FUNCTION__));
			}

			static void Start(TransformData* data) {
				data->Rotation.Set(0, 0, 0);
				data->Position.Set(0, 0, 0);
			}

			static void Update(TransformData* data) { }
		};

	}
}