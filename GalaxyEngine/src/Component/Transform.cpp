#include "pch.h"
#include "Component/Transform.h"

#include "Resource/Scene.h"

#include "Core/GameObject.h"
#include "Core/SceneHolder.h"

#include "Utils/Parser.h"

#include "Editor/ActionManager.h"
/*TODO :
 * Implement missing methods
 */
namespace GALAXY
{
	Component::Transform::Transform()
	{
	}

	Component::Transform::Transform(const Vec3f& position, const Quat& rotation, const Vec3f& scale /*= { 1, 1, 1 }*/)
	{

	}

	Vec3f Component::Transform::GetWorldPosition() const
	{
		if (p_gameObject.lock() && p_gameObject.lock()->GetParent())
		{
			return p_gameObject.lock()->GetParent()->GetTransform()->GetModelMatrix() * Vec4f(m_localPosition, 1.0f);
		}
		else
		{
			return m_localPosition;
		}
	}

	Quat Component::Transform::GetWorldRotation() const
	{
		if (p_gameObject.lock() && p_gameObject.lock()->GetParent())
		{
			return GetModelMatrix().GetRotation();
		}
		else
		{
			return m_localRotation;
		}
	}

	void Component::Transform::OnUpdate()
	{
		m_wasDirty = false;
		if (!m_dirty)
			return;
		m_wasDirty = true;
		ForceUpdate();
	}

	void Component::Transform::ForceUpdate()
	{
		ASSERT(p_gameObject.lock());
		if (p_gameObject.lock() && p_gameObject.lock()->GetParent())
			ComputeModelMatrix(p_gameObject.lock()->GetParent()->GetTransform()->GetModelMatrix());
		else
			ComputeModelMatrix();


		if (p_gameObject.lock())
		{
			for (auto& child : p_gameObject.lock()->GetChildren())
			{
				child.lock()->GetTransform()->ForceUpdate();
			}
		}
	}

	Vec3f Component::Transform::GetWorldScale() const
	{
		if (p_gameObject.lock() && p_gameObject.lock()->GetParent())
		{
			return GetModelMatrix().GetScale();
		}
		else
		{
			return m_localScale;
		}
	}

	Vec3f Component::Transform::GetWorldEulerRotation() const
	{
		if (p_gameObject.lock() && p_gameObject.lock()->GetParent())
		{
			return GetModelMatrix().GetRotation().ToEuler();
		}
		else
		{
			return m_localEulerRotation;
		}
	}


	void Component::Transform::ShowInInspector()
	{
		Vec3f position = m_localPosition;
		Vec3f rotation = m_localEulerRotation;
		Vec3f scale = m_localScale;

		/* Position Vec3 control */
		{
			static Vec3f previousPosition;
			static bool previousTrue = false;
			if (Wrapper::GUI::DrawVec3Control("Position", &position.x))
			{
				if (!previousTrue)
					previousPosition = m_localPosition;
				previousTrue = true;
			}
			else if (previousTrue && !ImGui::IsMouseDown(ImGuiMouseButton_Left))
			{
				previousTrue = false;
				const Editor::Action action(
					[this, localPosition = position]()
					{
						SetLocalPosition(localPosition);
					},
					[this, localPosition = previousPosition]()
					{
						SetLocalPosition(localPosition);
					});

				Core::SceneHolder::GetCurrentScene()->GetActionManager()->AddAction(action);
			}
		}

		/* Rotation Vec3 control */
		{
			static Vec3f previousRotation;
			static bool previousTrue = false;
			if (Wrapper::GUI::DrawVec3Control("Rotation", &rotation.x))
			{
				if (!previousTrue)
					previousRotation = m_localEulerRotation;
				previousTrue = true;
			}
			else if (previousTrue && !ImGui::IsMouseDown(ImGuiMouseButton_Left))
			{
				previousTrue = false;
				const Editor::Action action(
					[this, localPosition = rotation]()
					{
						SetLocalRotation(localPosition);
					},
					[this, localPosition = previousRotation]()
					{
						SetLocalRotation(localPosition);
					});

				Core::SceneHolder::GetCurrentScene()->GetActionManager()->AddAction(action);
			}
		}

		/* Scale Vec3 control */
		{
			static Vec3f previousScale;
			static bool previousTrue = false;
			if (Wrapper::GUI::DrawVec3Control("Scale", &scale.x, 1.f, true))
			{
				if (!previousTrue)
					previousScale = m_localScale;
				previousTrue = true;
			}
			else if (previousTrue && !ImGui::IsMouseDown(ImGuiMouseButton_Left))
			{
				previousTrue = false;
				const Editor::Action action(
					[this, localPosition = scale]()
					{
						SetLocalScale(localPosition);
					},
					[this, localPosition = previousScale]()
					{
						SetLocalScale(localPosition);
					});

				Core::SceneHolder::GetCurrentScene()->GetActionManager()->AddAction(action);
			}
		}

		if (position != m_localPosition || rotation != m_localEulerRotation || scale != m_localScale) {
			SetLocalPosition(position);
			SetLocalRotation(rotation);
			SetLocalScale(scale);
		}
	}

	void Component::Transform::Serialize(Utils::Serializer& serializer)
	{
		serializer << Utils::Pair::KEY << "Position" << Utils::Pair::VALUE << m_localPosition;
		serializer << Utils::Pair::KEY << "Rotation" << Utils::Pair::VALUE << m_localRotation;
		serializer << Utils::Pair::KEY << "Scale" << Utils::Pair::VALUE << m_localScale;
	}

	void Component::Transform::Deserialize(Utils::Parser& parser)
	{
		SetLocalPosition(parser["Position"].As<Vec3f>());
		SetLocalRotation(parser["Rotation"].As<Quat>());
		SetLocalScale(parser["Scale"].As<Vec3f>());
	}

	void Component::Transform::SetWorldPosition(const Vec3f& worldPosition)
	{
		//TODO :
		SetLocalPosition(worldPosition);
	}

	void Component::Transform::SetWorldRotation(const Quat& worldRotation)
	{
		//TODO :
		SetLocalRotation(worldRotation);
	}

	void Component::Transform::SetWorldRotation(const Vec3f& worldRotation)
	{
		//TODO :
		SetLocalRotation(worldRotation);
	}

	void Component::Transform::SetWorldScale(const Vec3f& worldScale)
	{
		//TODO :
		SetLocalScale(worldScale);
	}
}