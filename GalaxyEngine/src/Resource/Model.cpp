#include "pch.h"
#include "Resource/Model.h"
#include "Resource/Mesh.h"

#include "Wrapper/OBJLoader.h"

namespace GALAXY {
	void Resource::Model::Load()
	{
		if (p_fileInfo.GetExtension() == ".fbx")
		{
			m_modelType = Resource::ModelExtension::FBX;
		}
		else if (p_fileInfo.GetExtension() == ".obj")
		{
			m_modelType = Resource::ModelExtension::OBJ;
			Wrapper::OBJLoader::Load(p_fileInfo.GetFullPath(), this);
		}

		CreateFiles();
	}

	void Resource::Model::CreateFiles()
	{
		auto fileWithoutExt = p_fileInfo.GetFullPath().parent_path() / p_fileInfo.GetFileName().stem().wstring();
		std::filesystem::path path = fileWithoutExt.wstring() + L".gdata";
		if (auto& file = Utils::FileSystem::GenerateFile(path); file.is_open()) {
			file << "Origin: " << p_fileInfo.GetFullPath().string() << std::endl;
			for (auto& mesh : m_meshes)
			{
				file << '\t' << mesh->GetFileInfo().GetFileName().string() << std::endl;
			}
			file.close();
		}
	}
}