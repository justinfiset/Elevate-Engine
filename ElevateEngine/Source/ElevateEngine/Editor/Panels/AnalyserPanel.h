#pragma once

#ifdef EE_EDITOR_BUILD

#include <memory>
#include <typeindex>
#include <map>
#include <vector>

#include <ElevateEngine/Editor/EditorWidget.h>

import Elevate.Core.Types.TypeLayout;
import Elevate.Core.Types.TypeRegistry;
import Elevate.Core.Types.TypeField;
import Elevate.Core.Objects.Category;
import Elevate.Core.Objects.ObjectPtr;
import Elevate.Core.Scene:GameObject;

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
		AnalyserPanel();
		void OnImGuiRender() override;
	private:
		void RenderComponent(Component* component);
		void RenderComponentLayout(const TypeLayout& layout, Component* component = nullptr);
		void RenderField(const TypeField& field);

		// TODO MOVE SOMEWHERE ELSE !!!! (ALL THE FOLLOWING UNDERNEATH)
		struct CategoryMenu
		{
			EECategory category;
			std::vector<const TypeRegistry::Entry*> items;

			std::vector<CategoryMenu> childs;
		};
		void InsertCategory(CategoryMenu& root, const TypeRegistry::Entry& entry);
		void DrawCategoryChildren(const CategoryMenu& category, std::weak_ptr<GameObject> obj);
		void DrawCategoryMenu(const CategoryMenu& menu, std::weak_ptr<GameObject> obj);

		std::vector<std::type_index> m_alredyAddedComponents;

		std::map<Component*, const void*> m_textureCache;
		std::shared_ptr<Texture> pickerIcon;
		std::shared_ptr<Texture> noneIcon;
		std::shared_ptr<Texture> navigateToIcon;
	};
}

#endif // EE_EDITOR_BUILD