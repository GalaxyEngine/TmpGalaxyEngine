#pragma once
#include "GalaxyAPI.h"
#include "IResource.h"

#include <unordered_map>
#include <deque>


namespace GALAXY {
	namespace Wrapper { class Renderer; class OpenGLRenderer; }
	namespace Resource {
		class VertexShader;
		class GeometryShader;
		class FragmentShader;
		class Shader : public IResource
		{
		public:
			explicit Shader(const Path& fullPath) : IResource(fullPath) {}
			Shader& operator=(const Shader& other) = default;
			Shader(const Shader&) = default;
			Shader(Shader&&) noexcept = default;
			~Shader() override = default;

			virtual void Load() override;
			void Send() override;
			void Save();

			const char* GetResourceName() const override { return "Shader"; }

			// Get the enum with the class
			static ResourceType GetResourceType() { return ResourceType::Shader; }

			virtual void ShowInInspector() override;

			void SetVertex(const Shared<VertexShader>& vertexShader, const Weak<Shader>& weak_this, bool createVariant = true);
			void SetFragment(const Shared<FragmentShader>& fragmentShader, const Weak<Shader>& weak_this);
			void SetGeometry(const Shared<GeometryShader>& geometryShader, const Weak<Shader>& weak_this);

			Weak<VertexShader> GetVertex() const { return std::get<0>(p_subShaders); }
			Weak<GeometryShader> GetGeometry() const { return std::get<1>(p_subShaders); }
			Weak<FragmentShader> GetFragment() const { return std::get<2>(p_subShaders); }

			void Recompile() const;

			void Use();
			int GetLocation(const char* locationName);

			void SendInt(const char* locationName, int value);
			void SendFloat(const char* locationName, float value);
			void SendDouble(const char* locationName, double value);
			void SendVec2f(const char* locationName, const Vec2f& value);
			void SendVec3f(const char* locationName, const Vec3f& value);
			void SendVec4f(const char* locationName, const Vec4f& value);
			void SendVec2i(const char* locationName, const Vec2i& value);
			void SendVec3i(const char* locationName, const Vec3i& value);
			void SendVec4i(const char* locationName, const Vec4i& value);
			void SendMat4(const char* locationName, const Mat4& value);

			static Shared<Shader> Create(const Path& vertPath, const Path& fragPath);

			static Weak<Shader> Create(const Path& path);

			Weak<Shader> GetPickingVariant() const { return m_pickingVariant; }
		protected:
			void Serialize(CppSer::Serializer& serializer) const override;
			void Deserialize(CppSer::Parser& parser) override;

		protected:
			UMap<std::string, int> p_locations = {};

			uint32_t p_id = -1;

			bool p_isAVariant = false;

			std::tuple<std::weak_ptr<VertexShader>, std::weak_ptr<GeometryShader>, std::weak_ptr<FragmentShader>> p_subShaders = {};
		
		private:
			friend Wrapper::Renderer;
			friend Wrapper::OpenGLRenderer;
			friend class BaseShader;

			Shared<Shader> m_pickingVariant;
		};

		class BaseShader : public IResource
		{
		public:
			explicit BaseShader(const Path& fullPath) : IResource(fullPath) {}

			virtual void Load() override;
			void AddShader(const Weak<Shader>& shader);

			virtual void ShowInInspector() override;

			void Recompile();

			void RemoveShader(Shader* shader);

			List<Weak<Shader>> GetShaders() const { return p_shaders; }
		protected:
			List<Weak<Shader>> p_shaders;
			String p_content;
			uint32_t m_id = -1;
		private:
		};

		class VertexShader : public BaseShader
		{
		public:
			explicit VertexShader(const Path& fullPath) : BaseShader(fullPath) {}

			void Send() override;

			const char* GetResourceName() const override { return "Vertex Shader"; }

			// Get the enum with the class
			static inline ResourceType GetResourceType() { return ResourceType::VertexShader; }
		protected:

		private:
			friend Wrapper::Renderer;
			friend Wrapper::OpenGLRenderer;
		};

		class GeometryShader : public BaseShader
		{
		public:
			explicit GeometryShader(const Path& fullPath) : BaseShader(fullPath) {}

			const char* GetResourceName() const override { return "GeometryShader Shader"; }

			// Get the enum with the class
			static inline ResourceType GetResourceType() { return ResourceType::GeometryShader; }
		protected:

		private:
			friend Wrapper::Renderer;
			friend Wrapper::OpenGLRenderer;
		};

		class FragmentShader : public BaseShader
		{
		public:
			explicit FragmentShader(const Path& fullPath) : BaseShader(fullPath) {}
			~FragmentShader() override;

			void Send() override;

			const char* GetResourceName() const override { return "Fragment Shader"; }

			// Get the enum with the class
			static inline ResourceType GetResourceType() { return ResourceType::FragmentShader; }
		protected:

		private:
			friend Wrapper::Renderer;
			friend Wrapper::OpenGLRenderer;
		};
	}
}
