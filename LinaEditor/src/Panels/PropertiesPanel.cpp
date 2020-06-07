/*
Author: Inan Evin
www.inanevin.com
https://github.com/inanevin/LinaEngine

Copyright 2020~ Inan Evin

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions
and limitations under the License.

Class: PropertiesPanel
Timestamp: 6/7/2020 5:13:42 PM

*/


#include "Panels/PropertiesPanel.hpp"
#include "Core/GUILayer.hpp"
#include "ECS/Components/TransformComponent.hpp"
#include "ECS/Components/CameraComponent.hpp"
#include "ECS/Components/LightComponent.hpp"
#include "ECS/Components/FreeLookComponent.hpp"
#include "ECS/Components/MeshRendererComponent.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace LinaEditor
{
	using namespace LinaEngine::ECS;
	using namespace LinaEngine;
	static bool openCompExistsModal;
	const char* entityComponents[] = { "Transform", "Mesh Renderer", "Camera", "Directional Light", "Point Light", "Spot Light", "Free Look" };

	void PropertiesPanel::Setup()
	{
		m_ECS = m_GUILayer->GetECS();
	}

	void PropertiesPanel::Draw()
	{
		if (m_Show)
		{
			// Component already exists popup modal.
			if (openCompExistsModal)
				ImGui::OpenPopup("Component Exists!");
			if (ImGui::BeginPopupModal("Component Exists!", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("This component already exists!\n\n");
				if (ImGui::Button("OK", ImVec2(120, 0))) { openCompExistsModal = false; ImGui::CloseCurrentPopup(); }
				ImGui::EndPopup();
			}

			// Set window properties.
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImVec2 work_area_pos = viewport->GetWorkPos();
			ImVec2 panelSize = ImVec2(m_Size.x, m_Size.y);
			ImGui::SetNextWindowSize(panelSize, ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowBgAlpha(1.0f);
			ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;

			if (ImGui::Begin("Properties", &m_Show, flags))
			{
				if (m_CurrentDrawType == DrawType::ENTITIES)
					DrawEntityProperties();
			}

			ImGui::End();
		}
	}

	void PropertiesPanel::DrawEntityProperties()
	{
		static int componentsComboCurrentItem = 0;

		// Buttons down below.
		if (m_ECS->valid(m_SelectedEntity))
		{
			ImGui::BeginChild("Component View", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
			if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
			{
				// Component view tab
				bool isValid = m_ECS->valid(m_SelectedEntity);
				std::string selectedName = m_ECS->valid(m_SelectedEntity) ? "Component View" : ("Component View: " + m_ECS->GetEntityName(m_SelectedEntity));
				char titleName[256];
				strcpy(titleName, selectedName.c_str());
				if (ImGui::BeginTabItem(titleName))
				{
					if (!isValid)
						ImGui::TextWrapped("No entity is selected.");
					else
						DrawComponents(m_SelectedEntity);
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::EndChild();

			if (ImGui::Button("Add Component"))
				AddComponentToEntity(componentsComboCurrentItem);

			ImGui::SameLine();

			// Combo box for components.
			static ImGuiComboFlags flags = 0;
			const char* combo_label = entityComponents[componentsComboCurrentItem];  // Label to preview before opening the combo (technically could be anything)(
			if (ImGui::BeginCombo("ComponentsCombo", combo_label, flags))
			{
				for (int n = 0; n < IM_ARRAYSIZE(entityComponents); n++)
				{
					const bool is_selected = (componentsComboCurrentItem == n);
					if (ImGui::Selectable(entityComponents[n], is_selected))
						componentsComboCurrentItem = n;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
		}


	}

	void PropertiesPanel::AddComponentToEntity(int componentID)
	{
		if (!m_ECS->valid(m_SelectedEntity)) return;

		// Add the indexed component to target entity.
		if (componentID == 0)
		{
			if (m_ECS->has<LinaEngine::ECS::TransformComponent>(m_SelectedEntity))
				openCompExistsModal = true;
			else
			{
				auto& e = m_ECS->emplace<LinaEngine::ECS::TransformComponent>(m_SelectedEntity);
			}
		}
		else if (componentID == 1)
		{
			if (m_ECS->has<LinaEngine::ECS::MeshRendererComponent>(m_SelectedEntity))
				openCompExistsModal = true;
			else
			{
				auto& e = m_ECS->emplace<LinaEngine::ECS::MeshRendererComponent>(m_SelectedEntity);
			}
		}
		else if (componentID == 2)
		{
			if (m_ECS->has<LinaEngine::ECS::CameraComponent>(m_SelectedEntity))
				openCompExistsModal = true;
			else
			{
				auto& e = m_ECS->emplace<LinaEngine::ECS::CameraComponent>(m_SelectedEntity);
			}

		}
		else if (componentID == 3)
		{
			if (m_ECS->has<LinaEngine::ECS::DirectionalLightComponent>(m_SelectedEntity))
				openCompExistsModal = true;
			else
			{
				auto& e = m_ECS->emplace<LinaEngine::ECS::DirectionalLightComponent>(m_SelectedEntity);
			}

		}
		else if (componentID == 4)
		{
			if (m_ECS->has<LinaEngine::ECS::PointLightComponent>(m_SelectedEntity))
				openCompExistsModal = true;
			else
			{
				auto& e = m_ECS->emplace<LinaEngine::ECS::PointLightComponent>(m_SelectedEntity);
			}

		}
		else if (componentID == 5)
		{
			if (m_ECS->has<LinaEngine::ECS::SpotLightComponent>(m_SelectedEntity))
				openCompExistsModal = true;
			else
			{
				auto& e = m_ECS->emplace<LinaEngine::ECS::SpotLightComponent>(m_SelectedEntity);
			}

		}
		else if (componentID == 6)
		{
			if (m_ECS->has<LinaEngine::ECS::FreeLookComponent>(m_SelectedEntity))
				openCompExistsModal = true;
			else
			{
				auto& e = m_ECS->emplace<LinaEngine::ECS::FreeLookComponent>(m_SelectedEntity);
			}
		}

	}

	void PropertiesPanel::DrawComponents(LinaEngine::ECS::ECSEntity& entity)
	{
		// Check if entity has any component.
		if (!m_ECS->any<TransformComponent, MeshRendererComponent, CameraComponent, DirectionalLightComponent, SpotLightComponent, PointLightComponent, FreeLookComponent>(entity))
			ImGui::TextWrapped("This entity doesn't contain any components");

		// Draw transform component.
		if (m_ECS->has<TransformComponent>(entity))
		{
			float dragSensitivity = 0.05f;
			float dragSensitivityRotation = 0.25f;
			TransformComponent* transform = &m_ECS->get<TransformComponent>(entity);

			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_None))
			{
				ImGui::Indent();

				float location[3] = { transform->transform.location.x, transform->transform.location.y, transform->transform.location.z };
				float rot[3] = { transform->transform.rotation.GetEuler().x, transform->transform.rotation.GetEuler().y, transform->transform.rotation.GetEuler().z };
				float scale[3] = { transform->transform.scale.x, transform->transform.scale.y, transform->transform.scale.z };
				ImGui::DragFloat3("Location", location, dragSensitivity);
				ImGui::DragFloat3("Rotation", rot, dragSensitivityRotation);
				ImGui::DragFloat3("Scale", scale, dragSensitivity);
				transform->transform.location = Vector3(location[0], location[1], location[2]);
				transform->transform.scale = Vector3(scale[0], scale[1], scale[2]);
				transform->transform.rotation = Quaternion::Euler(rot[0], rot[1], rot[2]);

				ImGui::Unindent();
			}

		}


		// Draw Point Light component.
		if (m_ECS->has<PointLightComponent>(entity))
		{
			PointLightComponent* light = &m_ECS->get<PointLightComponent>(entity);
			MeshRendererComponent* lightRenderer = m_ECS->has<MeshRendererComponent>(entity) ? &m_ECS->get<MeshRendererComponent>(entity) : nullptr;

			float dragSensitivity = 0.05f;
			if (ImGui::CollapsingHeader("Point Light", ImGuiTreeNodeFlags_None))
			{
				ImGui::Indent();

				ImVec4 col = ImVec4(light->color.r, light->color.g, light->color.b, light->color.a);
				float d = light->distance;
				EditorUtility::ColorButton(Vector4(col.x, col.y, col.z, col.w));
				ImGui::DragFloat("Distance ", &d, dragSensitivity);
				light->distance = d;
				light->color = Color(col.x, col.y, col.z, col.w);

				if (lightRenderer != nullptr)
					lightRenderer->material->SetColor(MAT_OBJECTCOLORPROPERTY, light->color);

				ImGui::Unindent();
			}

		}

		// Draw spot light component.
		if (m_ECS->has<SpotLightComponent>(entity))
		{
			SpotLightComponent* light = &m_ECS->get<SpotLightComponent>(entity);
			MeshRendererComponent* lightRenderer = m_ECS->has<MeshRendererComponent>(entity) ? &m_ECS->get<MeshRendererComponent>(entity) : nullptr;

			float dragSensitivity = 0.005f;
			if (ImGui::CollapsingHeader("Spot Light", ImGuiTreeNodeFlags_None))
			{
				ImGui::Indent();

				ImVec4 col = ImVec4(light->color.r, light->color.g, light->color.b, light->color.a);
				float d = light->distance;
				float cutOff = light->cutOff;
				float outerCutOff = light->outerCutOff;
				EditorUtility::ColorButton(Vector4(col.x, col.y, col.z, col.w));
				ImGui::DragFloat("Distance ", &d, dragSensitivity);
				ImGui::DragFloat("CutOff ", &cutOff, dragSensitivity);
				ImGui::DragFloat("Outer Cutoff ", &outerCutOff, dragSensitivity);
				light->distance = d;
				light->cutOff = cutOff;
				light->outerCutOff = outerCutOff;
				light->color = Color(col.x, col.y, col.z, col.w);


				if (lightRenderer != nullptr)
					lightRenderer->material->SetColor(MAT_OBJECTCOLORPROPERTY, light->color);

				ImGui::Unindent();
			}
		}

		// Draw directional light component.
		if (m_ECS->has<DirectionalLightComponent>(entity))
		{
			DirectionalLightComponent* light = &m_ECS->get<DirectionalLightComponent>(entity);
			MeshRendererComponent* lightRenderer = m_ECS->has<MeshRendererComponent>(entity) ? &m_ECS->get<MeshRendererComponent>(entity) : nullptr;

			float dragSensitivity = 0.005f;
			if (ImGui::CollapsingHeader("Directional Light", ImGuiTreeNodeFlags_None))
			{
				ImGui::Indent();

				ImVec4 col = ImVec4(light->color.r, light->color.g, light->color.b, light->color.a);
				float direction[3] = { light->direction.x, light->direction.y, light->direction.z };

				EditorUtility::ColorButton(Vector4(col.x, col.y, col.z, col.w));
				ImGui::DragFloat3("Direction ", direction, dragSensitivity);
				light->direction = Vector3(direction[0], direction[1], direction[2]);
				light->color = Color(col.x, col.y, col.z, col.w);
				if (lightRenderer != nullptr)
					lightRenderer->material->SetColor(MAT_OBJECTCOLORPROPERTY, light->color);

				ImGui::Unindent();
			}
		}

		// Draw mesh renderer component
		if (m_ECS->has<MeshRendererComponent>(entity))
		{
			MeshRendererComponent* renderer = m_ECS->has<MeshRendererComponent>(entity) ? &m_ECS->get<MeshRendererComponent>(entity) : nullptr;


			float dragSensitivity = 0.005f;
			if (ImGui::CollapsingHeader("Mesh Renderer", ImGuiTreeNodeFlags_None))
			{
				ImGui::Indent();
				ImGui::Unindent();
			}
		}

	}
}