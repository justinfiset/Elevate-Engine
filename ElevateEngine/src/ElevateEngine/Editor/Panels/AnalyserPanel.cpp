#include "eepch.h"

#include "./AnalyserPanel.h"

#ifdef EE_EDITOR_BUILD

#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ElevateEngine/ImGui/CustomImGuiCommand.h>
#include <ElevateEngine/Renderer/Texture/TextureManager.h>

#include <ElevateEngine/Editor/Commands/ComponentCommand.h>
#include <ElevateEngine/Editor/EditorLayer.h>

#include <ElevateEngine/Core/AssetRegistry.h>
#include <ElevateEngine/Core/TypeRegistry.h>
#include <ElevateEngine/Core/Component.h>

Elevate::Editor::AnalyserPanel::AnalyserPanel()
{
	pickerIcon = Texture::CreateFromFile("editor://Icons/Light/adjust.png");
	noneIcon = Texture::CreateFromFile("editor://Icons/Light/block.png");
	navigateToIcon = Texture::CreateFromFile("editor://Icons/Light/arrow_top_right.png");
}

void Elevate::Editor::AnalyserPanel::OnImGuiRender()
{
	ImGui::Begin("Analyse");

	std::shared_ptr<EEObject> lockedSelected = EditorLayer::Get().GetSelectedObject().lock();
	std::shared_ptr<GameObject> obj = nullptr;
	if (lockedSelected)
	{
		obj = std::dynamic_pointer_cast<GameObject>(lockedSelected);
	}

	if (obj)
	{
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

			std::shared_ptr<EEObject> obj = EditorLayer::Get().GetSelectedObject().lock();
			if (auto gameObject = std::dynamic_pointer_cast<GameObject>(obj))
			{
				for (auto& cat : root.childs)
				{
					DrawCategoryMenu(cat, gameObject);
				}
				DrawCategoryChildren(root, gameObject);
			}
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

void Elevate::Editor::AnalyserPanel::RenderField(const TypeField& field)
{
	if (!field.data && field.type != EngineDataType::Custom)
	{
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "[Null Data Pointer] %s", field.GetDisplayName().c_str());
		return;
	}

	ImGui::BeginDisabled(field.readOnly);

	ImGui::PushID(field.data);

	switch (field.type)
	{
	case EngineDataType::Int:
		ImGui::InputInt(field.GetDisplayName().c_str(), (int*)(field.data));
		break;
	case EngineDataType::Int2:
		ImGui::InputInt2(field.GetDisplayName().c_str(), (int*)(field.data));
		break;
	case EngineDataType::Int3:
		ImGui::InputInt3(field.GetDisplayName().c_str(), (int*)(field.data));
		break;
	case EngineDataType::Int4:
		ImGui::InputInt4(field.GetDisplayName().c_str(), (int*)(field.data));
		break;

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

	case EngineDataType::GUID: // We do not display the guid
		break;

	case EngineDataType::ObjectPtr:
	{
		// Convert to a non const EEObjectPtr of EEObject
		auto* eePtr = const_cast<EEObjectPtr<EEObject>*>(reinterpret_cast<const EEObjectPtr<EEObject>*>(field.data));
		auto assetPtr = const_cast<EEObjectPtr<Asset>*>(reinterpret_cast<const EEObjectPtr<Asset>*>(field.data));

		if (eePtr)
		{
			auto* assetEntry = assetPtr ? AssetRegistry::GetEntry(assetPtr->GetGuid()) : nullptr;
			std::string displayName = (*eePtr) ? (assetEntry ? assetEntry->AssetName : "[Unknown Asset]") : "[None (EEObject)]";
			char buf[128];
			strncpy(buf, displayName.c_str(), sizeof(buf) - 1);
			buf[sizeof(buf) - 1] = '\0';

			float buttonSize = ImGui::GetFrameHeight() - ImGui::GetStyle().ItemInnerSpacing.y;
			float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
			float totalWidth = ImGui::CalcItemWidth();

			ImGui::SetNextItemWidth(totalWidth - buttonSize - spacing);
			ImGuiStyle& style = ImGui::GetStyle();

			ImVec4 textDisabled = style.Colors[ImGuiCol_TextDisabled];
			ImVec4 frameBgDisabled = style.Colors[ImGuiCol_FrameBg];
			frameBgDisabled.w *= style.DisabledAlpha;

			ImGui::PushStyleColor(ImGuiCol_FrameBg, frameBgDisabled);
			ImGui::PushStyleColor(ImGuiCol_Text, textDisabled);

			std::string inputID = "##" + field.name;
			ImGui::InputText(inputID.c_str(), buf, sizeof(buf), ImGuiInputTextFlags_ReadOnly);

			ImGui::PopStyleColor(2);

			ImGui::SameLine(0, spacing);
			if (ImGui::ImageButton("##picker", (ImTextureID)pickerIcon->GetNativeHandle(), ImVec2(buttonSize, buttonSize)))
			{
				ImGui::OpenPopup("AssetPickerPopup");
			}

			ImGui::SameLine(0, ImGui::GetStyle().ItemSpacing.x);
			
			
			ImGui::BeginDisabled(!assetEntry || !assetEntry->isOnDisk);
			if (ImGui::ImageButton("##navigateto", (ImTextureID)navigateToIcon->GetNativeHandle(), ImVec2(buttonSize, buttonSize)))
			{
				AssetBrowserPanel::SelectAsset(assetPtr->get());
			}
			ImGui::EndDisabled();
			ImGui::SameLine(0, ImGui::GetStyle().ItemSpacing.x);
			ImGui::TextUnformatted(field.GetDisplayName().c_str());

			if (ImGui::BeginPopup("AssetPickerPopup"))
			{
				ImGui::TextDisabled("Select %s", field.GetDisplayName().c_str());
				ImGui::Separator();

				if (ImGui::Selectable("##"))
				{
					eePtr->reset();
				}
				ImGui::SameLine(ImGui::GetStyle().ItemSpacing.x);
				ImGui::Image((ImTextureID)noneIcon->GetNativeHandle(), ImVec2(buttonSize, buttonSize));
				ImGui::SameLine();
				ImGui::Text("None");
				
				for (auto& guid : AssetRegistry::GetAssetsOfType(field.targetType))
				{
					auto* entry = AssetRegistry::GetEntry(guid);
					if (entry)
					{
						if (ImGui::Selectable(entry->AssetName.c_str()))
						{
							eePtr->SetGuid(guid);
						}
					}
				}
				ImGui::EndPopup();
			}
		}
		else
		{
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "[Null Data EEObjectPtr] %s", field.GetDisplayName().c_str());
		}
		break;
	}

	default:
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Unsupported data type: %s", field.name.c_str());
		break;
	}

	ImGui::PopID();

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