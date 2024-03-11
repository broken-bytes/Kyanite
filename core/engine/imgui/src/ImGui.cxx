#include "imgui/ImGui.hxx"

#include <imgui.h>
#include <imgui_internal.h>

#include <filesystem>
#include <sstream>
#include <vector>

namespace kyanite::engine::imgui {
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

	auto TextField(const char* label, char* buffer, int bufferSize) -> bool {
		return ImGui::InputText(label, buffer, bufferSize, ImGuiInputTextFlags_EnterReturnsTrue);
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

	auto TreeNode(const char* label) -> bool {
		return ImGui::TreeNode(label);
	}

	auto EndTreeNode() -> void {
		ImGui::TreePop();
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
			ImGui::DockBuilderDockWindow("Terminal", dockDown);

			ImGui::DockBuilderFinish(dockId);
		}
		ImGui::End();
	}

    
	auto CreateImGuiContext() -> ImGuiContext* {
		// Create ImGui context
		auto context = ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 16.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.1f, 0.5f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(100.0f, 100.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 0.0f);
		// Now set the colors to be darker
		auto& style = ImGui::GetStyle();
		auto& colors = style.Colors;
		colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
		colors[ImGuiCol_Border] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.3f, 0.2f, 0.3f, 1.0f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.4f, 0.3f, 0.4f, 1.0f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
		colors[ImGuiCol_Button] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
		colors[ImGuiCol_Header] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

		// Add the Roboto font
		auto robotoFont = std::filesystem::current_path() / "assets/fonts/RobotoCondensed-VariableFont_wght.ttf";
		ImGui::GetIO().Fonts->AddFontFromFileTTF(
			robotoFont.string().c_str(),
			20.0f,
			nullptr,
			ImGui::GetIO().Fonts->GetGlyphRangesDefault()
		);

		// Add the Open Font Icon font
		auto openIconFont = std::filesystem::current_path() / "assets/fonts/OpenFontIcons.ttf";
		// Merge with the default font
		ImFontConfig config;
		config.MergeMode = true;
		config.PixelSnapH = true;
		static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 }; // Will not be copied by AddFont* so keep in scope.

		ImGui::GetIO().Fonts->AddFontFromFileTTF(
			openIconFont.string().c_str(),
			48.0f,
			&config,
			icons_ranges
		);

		ImGui::GetIO().Fonts->Build();

		ImGui::GetIO().ConfigFlags |=
			ImGuiConfigFlags_ViewportsEnable |
			ImGuiConfigFlags_DockingEnable;

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();

		return context;
	}

	auto GetImGuiContext() -> ImGuiContext* {
		return ImGui::GetCurrentContext();
	}
}