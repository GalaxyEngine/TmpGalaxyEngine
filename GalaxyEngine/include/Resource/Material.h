#pragma once
#include "GalaxyAPI.h"
#include "IResource.h"
namespace GALAXY
{
	namespace Wrapper
	{
		class OBJLoader;
	}
	namespace Resource {
		class Material : public IResource
		{
		public:
			Material(const std::filesystem::path& fullPath);
			Material& operator=(const Material& other) = default;
			Material(const Material&) = default;
			Material(Material&&) noexcept = default;
			virtual ~Material() {}

			void Load() override;

			void Save();

			void ShowInInspector() override;

			void SendValues();

			static ResourceType GetResourceType() { return ResourceType::Material; }

			std::weak_ptr<class Shader> GetShader() const { return m_shader; }

			static Weak<Material> Create(const std::filesystem::path& path);

			Vec4f GetAmbient() const { return m_ambient; }
			Vec4f GetDiffuse() const { return m_diffuse; }
			Vec4f GetSpecular() const { return m_specular; }

			void SetAmbient(Vec4f val) { m_ambient = val; }
			void SetDiffuse(Vec4f val) { m_diffuse = val; }
			void SetSpecular(Vec4f val) { m_specular = val; }
		private:
			friend Wrapper::OBJLoader;
			std::weak_ptr<class Shader> m_shader;

			Vec4f m_ambient;
			Vec4f m_diffuse;
			Vec4f m_specular;

			std::weak_ptr<class Texture> m_albedo;
			std::weak_ptr<class Texture> m_normal;

		};
	}
}