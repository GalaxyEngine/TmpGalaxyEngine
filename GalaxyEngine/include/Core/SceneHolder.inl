#pragma once
#include "Core/SceneHolder.h"
namespace GALAXY 
{
	inline void Core::SceneHolder::SwitchScene(const Weak<Resource::Scene>& scene)
	{
		m_nextScene = scene.lock();
	}
}
