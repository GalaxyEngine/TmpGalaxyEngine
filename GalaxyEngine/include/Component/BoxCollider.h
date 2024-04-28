#pragma once
#include "GalaxyAPI.h"
#include "Component/IComponent.h"

namespace GALAXY 
{
    namespace Component
    {
        class GALAXY_API BoxCollider : public IComponent<BoxCollider>
        {
        public:
            BoxCollider() = default;
            BoxCollider& operator=(const BoxCollider& other) = default;
            BoxCollider(const BoxCollider&) = default;
            BoxCollider(BoxCollider&&) noexcept = default;
            ~BoxCollider() override = default;

            const char* GetComponentName() const override { return "BoxCollider"; }

            void ShowInInspector() override;

            void OnStart() override;
            void OnDestroy() override;

            void OnEditorDraw() override;

            void SetSize(const Vec3f& inSize) { m_size = inSize; }

            Vec3f GetSize() const { return m_size; }
        private:
            Vec3f m_size = Vec3f(1, 1, 1);
            
        };
    }
}
