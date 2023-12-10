#include "pch.h"
#include "Render/Grid.h"
#include "Render/Camera.h"

#include "Resource/ResourceManager.h"
#include "Resource/ResourceManager.h"

#include "Core/SceneHolder.h"

#include <glad/glad.h>

namespace GALAXY
{

	void Render::Grid::Initialize()
	{
		m_shader = Resource::ResourceManager::GetOrLoad<Resource::Shader>(GRID_PATH);

		//TODO: Move to Renderer
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Render::Grid::Draw() const
	{
		if (const Shared<Resource::Shader> gridShader = m_shader.lock(); gridShader && gridShader->HasBeenSent()) {

			const auto renderer = Wrapper::Renderer::GetInstance();
			const auto currentCamera = Render::Camera::GetCurrentCamera().lock();

			renderer->BindVertexArray(m_vao);

			gridShader->Use();

			gridShader->SendMat4("view", currentCamera->GetViewMatrix());
			gridShader->SendMat4("proj", currentCamera->GetProjectionMatrix());
			gridShader->SendFloat("near", currentCamera->GetNear());
			gridShader->SendFloat("far", currentCamera->GetFar());

			renderer->DrawArrays(0, 6);
			renderer->UnbindVertexArray();
		}
	}
}