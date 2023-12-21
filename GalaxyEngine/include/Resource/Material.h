#pragma once
#include "GalaxyAPI.h"
#include "IResource.h"
namespace GALAXY
{
	namespace Wrapper
	{
		class MTLLoader;
	}
	namespace Render { class Framebuffer; }
	namespace Resource {
		class Texture;
		class Material : public IResource
		{
		public:
			explicit Material(const Path& fullPath);
			Material& operator=(const Material& other) = default;
			Material(const Material&) = default;
			Material(Material&&) noexcept = default;
			~Material() override {}

			void Load() override;

			const char* GetResourceName() const override { return "Material"; }

			bool LoadMatFile();

			static bool LoadMTLFile();

			void Save() const;

			void ShowInInspector() override;

			void DisplayTexture(const char* textureLabel, Weak<Texture>& textureRef) const;

			Weak<Shader> SendValues(uint64_t id = -1) const;

			static inline ResourceType GetResourceType() { return ResourceType::Material; }

			static Weak<Material> Create(const std::filesystem::path& path);

			inline Shared<Shader> GetShader() const { return m_shader.lock(); }
			inline Vec4f GetAmbient() const { return m_ambient; }
			inline Vec4f GetDiffuse() const { return m_diffuse; }
			inline Vec4f GetSpecular() const { return m_specular; }
			inline Weak<Texture> GetAlbedo() const { return m_albedo; }
			inline Weak<Texture> GetNormalMap() const { return m_normalMap; }
			inline Weak<Texture> GetParallaxMap() const { return m_parallaxMap; }
			inline float GetHeightScale() const { return m_heightScale; }

			inline void SetShader(const Weak<Shader>& val) { m_shader = val; }
			inline void SetAmbient(const Vec4f val) { m_ambient = val; }
			inline void SetDiffuse(const Vec4f val) { m_diffuse = val; }
			inline void SetSpecular(const Vec4f val) { m_specular = val; }
			inline void SetAlbedo(const Weak<Texture>& val) { m_albedo = val; }
			inline void SetNormalMap(const Weak<Texture>& val) { m_normalMap = val; }
			inline void SetParallaxMap(const Weak<Texture>& val) { m_parallaxMap = val; }
			inline void SetHeightScale(const float val) { m_heightScale = val; }
		private:
			friend Wrapper::MTLLoader;
			friend Render::Framebuffer;

			Weak<Shader> m_shader;

			Vec4f m_ambient;
			Vec4f m_diffuse;
			Vec4f m_specular;

			Weak<Texture> m_albedo;
			Weak<Texture> m_normalMap;
			Weak<Texture> m_parallaxMap;
			float m_heightScale;


		};
	}
}