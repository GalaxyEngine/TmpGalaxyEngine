#pragma once
#include "GalaxyAPI.h"
#include <vector>
#include <optional>

namespace GALAXY
{
	namespace Resource
	{
		class Model;
	}
	namespace Wrapper
	{
		class OBJLoader
		{
		public:
			~OBJLoader();

			static void Load(const std::filesystem::path& fullPath, Resource::Model* outputModel);
		private:
			struct OBJMaterial
			{
				std::filesystem::path name;
			};

			struct OBJSubMesh
			{
				size_t startIndex = -1;
				size_t count = -1;
				std::optional<OBJMaterial> material;
			};

			struct OBJMesh
			{
				std::filesystem::path name;
				std::vector<OBJSubMesh> subMeshes;
				std::vector<Vec3f> positions;
				std::vector<Vec2f> textureUVs;
				std::vector<Vec3f> normals;
				std::vector<Vec3f> tangents;
				std::vector<Vec3i> indices;
				std::vector<float> finalVertices;
			};

			std::vector<OBJMesh> m_meshes;

			std::filesystem::path m_path;
			std::optional<std::filesystem::path> m_mtlPath;
		private:
			bool Parse();

			static void ParseFaceIndex(Vec3i& indices, const std::string& indexStr);

			static void ComputeVertices(OBJMesh& mesh);

			static bool ReadMtl(const std::filesystem::path& mtlPath);
		};
	}
}