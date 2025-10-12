#pragma once
#include <ElevateEngine/Editor/EditorWidget.h>
#include <ElevateEngine/Editor/Serialization/ComponentLayout.h>
#include <ElevateEngine/Core/EEObject.h>
#include <ElevateEngine/Core/ComponentRegistry.h>

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
		void RenderComponentLayout(const ComponentLayout& layout, Component* component = nullptr);
		void RenderField(const ComponentField& field) const;

		// TODO MOVE SOMEWHERE ELSE !!!! (ALL THE FOLLOWING UNDERNEATH)
		struct CategoryMenu
		{
			EECategory category;
			std::vector<ComponentRegistry::Entry> items;

			std::vector<CategoryMenu> childs;
		};
		void InsertCategory(CategoryMenu& root, const ComponentRegistry::Entry& entry);
		void DrawCategoryChildren(const CategoryMenu& category, std::weak_ptr<GameObject> obj);
		void DrawCategoryMenu(const CategoryMenu& menu, std::weak_ptr<GameObject> obj);

		std::vector<std::type_index> m_alredyAddedComponents;

		std::map<Component*, const void*> m_textureCache;
	};
}