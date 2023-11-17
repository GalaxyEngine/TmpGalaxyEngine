#pragma once
#include "GalaxyAPI.h"
#include "Component/Light.h"

namespace GALAXY 
{
	namespace Component
	{
		class PointLight : public Light
		{
		public:
			PointLight() {}
			PointLight& operator=(const PointLight& other) = default;
			PointLight(const PointLight&) = default;
			PointLight(PointLight&&) noexcept = default;
			virtual ~PointLight() {}

			inline const char* GetComponentName() const override { return "Point Light"; }

			inline virtual Shared<Component::BaseComponent> Clone() override {
				return std::make_shared<PointLight>(*dynamic_cast<PointLight*>(this));
			}
			
			inline Type GetLightType() override { return Light::Type::Point; };

			void SendLightValues(Resource::Shader* shader) override;

			void ShowInInspector() override;

			inline float GetConstant() const { return p_constant.value; }
			inline void SetConstant(float val) { p_constant.value = val; SetDirty(); }

			inline float GetLinear() const { return p_linear.value; }
			inline void SetLinear(float val) { p_linear.value = val; SetDirty(); }

			inline float GetQuadratic() const { return p_quadratic.value; }
			inline void SetQuadratic(float val) { p_quadratic.value = val; SetDirty(); }

			void ComputeLocationName() override;

			void Serialize(Utils::Serializer& serializer) override;
			void Deserialize(Utils::Parser& parser) override;
		protected:
			LightData<Vec3f> p_position;
			LightData<float> p_constant = 1.0f;
			LightData<float> p_linear = 0.35f;
			LightData<float> p_quadratic = 0.44f;
		};
	}
}
