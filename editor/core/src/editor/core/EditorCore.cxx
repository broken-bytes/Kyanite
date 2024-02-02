#include "editor/core/EditorCore.hxx"
#include <core/Core.hxx>

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>

#include <sstream>
#include <vector>

auto DrawReferenceSelector(NativePointer window, std::string label, std::function<void(std::string)> onReferenceSet) -> void {

}

auto DrawButton(NativePointer window, std::string label, std::function<void()> action) -> void {

}

auto DrawLabel(NativePointer window, std::string label) -> void {

}

auto Clear(NativePointer window) -> void {

}