#include "eepch.h"

#include "./AnalyserPanel.h"

#ifdef EE_EDITOR_BUILD

#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ElevateEngine/Editor/EditorLayer.h>
#include <ElevateEngine/ImGui/CustomImGuiCommand.h>

#include <ElevateEngine/Editor/Commands/ComponentCommand.h>
#include <ElevateEngine/Core/TypeRegistry.h>
#include <ElevateEngine/Core/Component.h>

void Elevate::Editor::AnalyserPanel::OnImGuiRender()
{
	ImGui::Begin("Analyse");

	std::weak_ptr<GameObject> selected = EditorLayer::Get().GetSelectedObject();
	if (selected.lock())
	{
		std::shared_ptr<GameObject> obj = selected.lock();
		Elevate::UI::InputField("Name: ", obj->GetName());

		// Serialisation of the tranform and all other components
		glm::vec4 color = EECategory("Transform").GetCategoryColor() * 0.6f;

		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(color.r + 0.3f, color.g + 0.3f, color.b + 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(color.r, color.g, color.b, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(color.r + 0.1f, color.g + 0.1f, color.b + 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(color.r + 0.2f, color.g + 0.2f, color.b + 0.2f, 1.0f));

		RenderComponentLayout(obj->GetTransform().GetLayout());

		ImGui::PopStyleVar();
		
		// TODO REMOVE AND / OR CHECK IF THE GAMEOBJECT IS DIFFERENT, THIS IS A COSTLY OPERATION FOR EACH FRAME
		m_alredyAddedComponents.clear();

		std::map<EECategory, std::vector<Component*>> m_sortedComponents;
		for (Component* comp : obj->GetComponents())
		{
			m_sortedComponents[comp->GetCategory()].push_back(comp);
			m_alredyAddedComponents.push_back(comp->GetTypeIndex());
		}

		for (std::pair<EECategory, std::vector<Component*>> entry : m_sortedComponents)
		{
			const std::string& categoryName = entry.first.GetName();

			glm::vec4 color = entry.first.GetCategoryColor();
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(color.r + 0.3f, color.g + 0.3f, color.b + 0.3f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(color.r, color.g, color.b, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(color.r + 0.1f, color.g + 0.1f, color.b + 0.1f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(color.r + 0.2f, color.g + 0.2f, color.b + 0.2f, 1.0f));
			if (ImGui::CollapsingHeader(categoryName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::PopStyleColor(4);
				ImGui::PopStyleVar();

				for (Component* comp : entry.second)
				{
					if (comp)
					{
						RenderComponent(comp);
					}
				}
			}
			else 
			{
				ImGui::PopStyleColor(4);
				ImGui::PopStyleVar();
			}
		}

		if (ImGui::Button("Add Component", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
		{
			ImGui::OpenPopup("AddComponentPopup");
		}

		if (ImGui::BeginPopup("AddComponentPopup"))
		{
			ImGui::Text("Add component :");
			ImGui::Separator();

			CategoryMenu root;
			for (auto& pair : TypeRegistry::GetEntries())
			{
				InsertCategory(root, pair.second);
			}
			std::weak_ptr<GameObject> obj = EditorLayer::Get().GetSelectedObject();
			for (auto& cat : root.childs)
			{
				DrawCategoryMenu(cat, obj);
			}
			DrawCategoryChildren(root, obj);

			ImGui::EndPopup();
		}
	}

	ImGui::End();
}

void Elevate::Editor::AnalyserPanel::RenderComponent(Component* comp)
{
	const TypeLayout layout = comp->GetLayout();

	EECategory category = comp->GetCategory();

	glm::vec4 color = category.GetCategoryColor() * 0.5f;

	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(color.r + 0.3f, color.g + 0.3f, color.b + 0.3f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(color.r, color.g, color.b, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(color.r + 0.1f, color.g + 0.1f, color.b + 0.1f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(color.r + 0.2f, color.g + 0.2f, color.b + 0.2f, 1.0f));

	RenderComponentLayout(layout, comp);
}

void Elevate::Editor::AnalyserPanel::RenderComponentLayout(const TypeLayout& layout, Component* component)
{
	const void* texHandle = nullptr;
	if (component)
	{
		if (!m_textureCache.count(component))
		{
			m_textureCache.insert({ component, component->GetEditorIconHandle() });
		}
		texHandle = m_textureCache.at(component);
	}

	if (UI::EECollapsingHeader((layout.GetName()).c_str(),
		layout.GetFieldCount() > 0,
		texHandle,
		[&layout, &component]()
		{
			float menu_width = 0;

			if (component)
			{
				ImGuiStyle& style = ImGui::GetStyle();

				float customButtonWidth = 25.0f;
				float customButtonHeight = ImGui::GetFontSize() + style.FramePadding.y * 2.0f;
				menu_width = customButtonWidth + style.FramePadding.x;

				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - customButtonWidth, ImGui::GetCursorPosY()));
				ImGui::PushID(layout.GetName().c_str());
				if (ImGui::Button("...", ImVec2(customButtonWidth, customButtonHeight)))
					ImGui::OpenPopup("HeaderMenu");

				if (ImGui::BeginPopup("HeaderMenu"))
				{
					if (ImGui::MenuItem("Remove Component"))
					{
						EditorLayer::Get().PushCommand(std::make_unique<RemoveComponentCommand>(component));
					}
					ImGui::EndPopup();
				}
				ImGui::PopID();
			}

			return menu_width;
		}))
	{
		ImGui::PopStyleColor(4);

		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
		for (const TypeField& field : layout)
		{
			RenderField(field);
		}
		ImGui::PopStyleVar();
	}
	else
	{
		ImGui::PopStyleColor(4);
	}
}

void Elevate::Editor::AnalyserPanel::RenderField(const TypeField& field) const
{
	ImGui::BeginDisabled(field.readOnly);

	switch (field.type)
	{
	case EngineDataType::Float:
		ImGui::InputFloat(field.GetDisplayName().c_str(), (float*)(field.data));
		break;

	case EngineDataType::Float2:
		ImGui::InputFloat2(field.GetDisplayName().c_str(), (float*)(field.data));
		break;

	case EngineDataType::Float3:
		if (field.isColor)
			ImGui::ColorEdit3(field.GetDisplayName().c_str(), (float*)(field.data));
		else
			ImGui::InputFloat3(field.GetDisplayName().c_str(), (float*)(field.data));
		break;

	case EngineDataType::Float4:
		if (field.isColor)
			ImGui::ColorEdit4(field.GetDisplayName().c_str(), (float*)(field.data));
		else
			ImGui::InputFloat4(field.GetDisplayName().c_str(), (float*)(field.data));
		break;

	case EngineDataType::Bool:
		ImGui::Checkbox(field.GetDisplayName().c_str(), (bool*)(field.data));
		break;

	case EngineDataType::Custom:
		if (!field.flatten)
		{
			if (ImGui::TreeNodeEx(field.GetDisplayName().c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_FramePadding))
			{
				for (const auto& child : field.children)
					RenderField(child);
				ImGui::TreePop();
			}
		}
		else
		{
			for (const auto& child : field.children)
				RenderField(child);
		}
		break;

	default:
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Unsupported data type: %s", field.name.c_str());
		break;
	}

	ImGui::EndDisabled();

	if (!field.tooltip.empty())
	{
		ImGui::SetItemTooltip("%s", field.tooltip.c_str());
	}
}

void Elevate::Editor::AnalyserPanel::InsertCategory(CategoryMenu& root, const TypeRegistry::Entry& entry)
{
	auto* compTrait = entry.GetTrait<ComponentTypeTrait>();
	auto* editorTrait = entry.GetTrait<EditorTypeTrait>();

	if (!editorTrait || !editorTrait->visible || !compTrait)
	{
		return;
	}

	std::string path = compTrait->category.GetPath();

	if (path.empty())
	{
		root.items.push_back(&entry);
		return;
	}

	CategoryMenu* current = &root;
	size_t start = 0;

	while (start < path.size())
	{
		size_t pos = path.find('/', start);
		std::string part;

		if (pos == std::string::npos)
		{
			part = path.substr(start);
			start = path.size();
		}
		else
		{
			part = path.substr(start, pos - start);
			start = pos + 1;
		}

		if (part.empty())
		{
			continue;
		}

		bool found = false;
		size_t index = 0;
		for (size_t i = 0; i < current->childs.size(); ++i)
		{
			if (current->childs[i].category.GetName() == part)
			{
				index = i;
				found = true;
				break;
			}
		}

		if (!found)
		{
			CategoryMenu child;
			child.category = EECategory(part);
			current->childs.push_back(std::move(child));
			index = current->childs.size() - 1;
		}

		current = &current->childs[index];

		if (start >= path.size() || pos == std::string::npos)
		{
			current->items.push_back(&entry);
			break;
		}
	}
}

void Elevate::Editor::AnalyserPanel::DrawCategoryChildren(const CategoryMenu& category, std::weak_ptr<GameObject> obj)
{
	// Grey out the item if it is already added to the current GameObject
	for (auto& entry : category.items)
	{
		auto* compTrait = entry->GetTrait<ComponentTypeTrait>();
		bool alreadyAdded = false;
		for (auto& type : m_alredyAddedComponents)
		{
			if (type == entry->type)
			{
				alreadyAdded = true;
				break;
			}
		}

		ImGui::BeginDisabled(alreadyAdded);
		if (ImGui::Selectable(entry->name.c_str()))
		{
			if (auto go = obj.lock())
			{
				EditorLayer::Get().PushCommand(std::make_unique<AddComponentCommand>(go, compTrait->factory, compTrait->destructor));
			}
		}
		ImGui::EndDisabled();
	}
}

void Elevate::Editor::AnalyserPanel::DrawCategoryMenu(const CategoryMenu& menu, std::weak_ptr<GameObject> obj)
{
	if (!menu.category.GetName().empty())
	{
		if (ImGui::BeginMenu(menu.category.GetName().c_str()))
		{
			for (auto& child : menu.childs)
			{
				DrawCategoryMenu(child, obj);
			}

			DrawCategoryChildren(menu, obj);

			ImGui::EndMenu();
		}
	}
	else
	{
		for (auto& child : menu.childs)
		{
			DrawCategoryMenu(child, obj);
		}
	}
}

#endif // EE_EDITOR_BUILD