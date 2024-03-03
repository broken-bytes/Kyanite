#include "editor/core/EditorCore.hxx"
#include <core/Core.hxx>
#include <rendering/Bridge_Rendering.h>

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include <sstream>
#include <vector>

namespace kyanite::editor::core {
	EXPORTED auto Init(ImGuiContext* context) -> void {
		ImGui::SetCurrentContext(context);
	}

	auto BeginWindow(std::string title, bool* open, int flags) -> void {
		Rendering_StartWindow(title.c_str(), open);
	}

	auto EndWindow() -> void {
		Rendering_EndWindow();
	}

	auto DrawReferenceSelector(NativePointer window, std::string label, std::function<void(std::string)> onReferenceSet) -> void {

	}

	auto DrawButton(NativePointer window, std::string label, std::function<void()> action) -> void {

	}

	auto DrawLabel(NativePointer window, std::string label) -> void {

	}

	auto Clear(NativePointer window) -> void {

	}

	auto DrawDefaultImGuiDocking() -> void {
		ImGui::Begin(
			"Editor",
			0,
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus |
			ImGuiWindowFlags_NoNav
		);
		ImGuiID dockId = ImGui::GetID("MyDockSpace");
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
		ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::DockSpace(dockId, ImVec2(0.0f, 0.0f), dockspace_flags);
		static auto first_time = true;
		if (first_time) {
			first_time = false;
			ImGui::DockBuilderRemoveNode(dockId);
			ImGui::DockBuilderAddNode(dockId, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(dockId, viewport->Size);

			auto dockLeft = ImGui::DockBuilderSplitNode(
				dockId,
				ImGuiDir_Left,
				0.2f,
				nullptr,
				&dockId
			);
			auto dockDown = ImGui::DockBuilderSplitNode(
				dockId,
				ImGuiDir_Down,
				0.25f,
				nullptr,
				&dockId
			);
			auto dockRight = ImGui::DockBuilderSplitNode(
				dockId,
				ImGuiDir_Right,
				0.2f,
				nullptr,
				&dockId
			);

			ImGui::DockBuilderDockWindow("Scene", dockId);
			ImGui::DockBuilderDockWindow("Game View", dockId);
			ImGui::DockBuilderDockWindow("Hierarchy", dockLeft);
			ImGui::DockBuilderDockWindow("Inspector", dockRight);
			ImGui::DockBuilderDockWindow("Content Browser", dockDown);
			ImGui::DockBuilderDockWindow("Debug Console", dockDown);

			ImGui::DockBuilderFinish(dockId);
		}
		ImGui::End();
	}
}