#pragma once
#include "GalaxyAPI.h"
#include <Maths/Maths.h>
#include "Component/Transform.h"
namespace GALAXY
{
	namespace Render
	{
		class Camera
		{
		public:
			Camera();
			Camera& operator=(const Camera& other) = default;
			Camera(const Camera&) = default;
			Camera(Camera&&) noexcept = default;
			virtual ~Camera();

			static std::shared_ptr<Camera> GetEditorCamera();
			static std::weak_ptr<Camera> GetCurrentCamera();

			void Begin();

			void End();

			void Update();

			void DisplayCameraSettings();

			virtual Component::Transform* GetTransform();

			void SetSize(const Vec2i& framebufferSize);

			virtual bool IsVisible();

			Mat4 GetViewMatrix();
			Mat4 GetProjectionMatrix();
			Mat4 GetViewProjectionMatrix();
			Vec4f GetClearColor() const { return p_clearColor; }

			std::weak_ptr<Resource::Texture> GetRenderTexture();

			float GetFar() const { return p_far; }
			void SetFar(float val) { p_far = val; }

			float GetNear() const { return p_near; }
			void SetNear(float val) { p_near = val; }

			float GetFOV() const { return p_fov; }
			void SetFOV(float val) { p_fov = val; }
			class Framebuffer* GetOutlineFramebuffer() const { return m_outlineFramebuffer; }
		private:
			void StartLooking();
			void StopLooking();

			class Framebuffer* m_outlineFramebuffer = nullptr;
		protected:
			float p_fov = 70.f;
			float p_far = 1000.f;
			float p_near = 0.03f;
			float p_aspectRatio = 4.f / 3.f;
			Vec2i p_framebufferSize;

			Vec4f p_clearColor = Vec4f(0.45f, 0.55f, 0.60f, 1.00f);

			class Framebuffer* m_framebuffer = nullptr;

		private:
			float m_movementSpeed = 10.f;
			float m_fastMovementSpeed = 100.f;
			float m_freeLookSensitivity = 100.f;
			float m_zoomSensitivity = 1.f;
			float m_fastZoomSensitivity = 5.f;

			bool m_looking = false;

			std::unique_ptr<Component::Transform> m_transform;

		};
	}
}
