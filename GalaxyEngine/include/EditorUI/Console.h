#pragma once
#include "GalaxyAPI.h"
#include "EditorUI/EditorWindow.h"

namespace GALAXY 
{
	namespace EditorUI
	{
		class Console : public EditorWindow
		{
		public:
			Console();

			void Draw() override;

		private:

		};
	}
}
#include "EditorUI/Console.inl" 
