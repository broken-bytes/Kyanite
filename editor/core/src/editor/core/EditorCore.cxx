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

	auto BeginWindow(std::string title, int flags, int64_t id, void (*callback)(int64_t)) -> void {
		bool isOpen = true;
		ImGui::Begin(title.c_str(), &isOpen);

		if(!isOpen) {
			callback(id);
		}
	}

	auto EndWindow() -> void {
		ImGui::End();
	}

	auto DrawReferenceSelector(NativePointer window, std::string label, std::function<void(std::string)> onReferenceSet) -> void {

	}

	auto Button(std::string_view label) -> bool {
		return ImGui::Button(label.data());
	}

	auto Label(std::string label) -> void {
		ImGui::Text(label.c_str());
	}

	auto Separator() -> void {
		ImGui::Separator();
	}

	auto TextField(const char* label, char* buffer, int bufferSize) -> void {
		ImGui::InputText(label, buffer, bufferSize);
	}

	auto FloatField(const char* label, float* value) -> void {
		ImGui::InputFloat(label, value);
	}

	auto Float2Field(const char* label, float* value) -> void {
		ImGui::InputFloat2(label, value);
	}

	auto Float3Field(const char* label, float* value) -> void {
		ImGui::InputFloat3(label, value);
	}

	auto Float4Field(const char* label, float* value) -> void {
		ImGui::InputFloat4(label, value);
	}

	auto IntField(const char* label, int* value) -> void {
		ImGui::InputInt(label, value);
	}

	auto Checkbox(const char* label, bool* value) -> void {
		ImGui::Checkbox(label, value);
	}

	auto BeginHorizontal() -> void {
		ImGui::BeginGroup();
	}

	auto EndHorizontal() -> void {
		ImGui::EndGroup();
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
			ImGuiWindowFlags_NoBringToFrontOnFocus
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