#pragma once

#ifdef EE_EDITOR_BUILD

#include <ElevateEngine/Editor/EditorWidget.h>
#include <ElevateEngine/Core/TypeLayout.h>
#include <ElevateEngine/Core/EEObject.h>
#include <ElevateEngine/Core/TypeRegistry.h>

namespace Elevate
{
	class Component;
	class Texture;
}

namespace Elevate::Editor
{
	class AnalyserPanel : public EditorWidget
	{
	public:
		void OnImGuiRender() override;
	private:
		void RenderComponent(Component* component);
		void RenderComponentLayout(const TypeLayout& layout, Component* component = nullptr);
		void RenderField(const TypeField& field) const;

		// TODO MOVE SOMEWHERE ELSE !!!! (ALL THE FOLLOWING UNDERNEATH)
		struct CategoryMenu
		{
			EECategory category;
			std::vector<TypeRegistry::Entry> items;

			std::vector<CategoryMenu> childs;
		};
		void InsertCategory(CategoryMenu& root, const TypeRegistry::Entry& entry);
		void DrawCategoryChildren(const CategoryMenu& category, std::weak_ptr<GameObject> obj);
		void DrawCategoryMenu(const CategoryMenu& menu, std::weak_ptr<GameObject> obj);

		std::vector<std::type_index> m_alredyAddedComponents;

		std::map<Component*, const void*> m_textureCache;
	};
}

#endif // EE_EDITOR_BUILD