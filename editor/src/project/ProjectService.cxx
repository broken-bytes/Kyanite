#include "project/ProjectService.hxx"
#include <core/Core.hxx>

#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>

#include <filesystem>
#include <sstream>

namespace kyanite::editor {
	ProjectService::ProjectService() {
	}

	ProjectService::~ProjectService() {
	}

	auto ProjectService::LoadProject(const std::string& path) -> Project {
		Project project;
		std::stringstream ss;
		{
			auto buffer = kyanite::engine::core::LoadFileToBuffer(path);
			std::string str(buffer.begin(), buffer.end());
			ss << str;
			cereal::JSONInputArchive  archive(ss);
			archive(project);
		}

		return project;
	}

	auto ProjectService::CreateProject(const std::string& path, std::string& name) -> Project {
		CreateFolderStructure(path, name);
		CreateProjectFile(path, name);
		
		std::filesystem::path projPath(path);
		projPath /= name + ".kproj";

		return LoadProject(projPath.string());
	}

	auto ProjectService::CreateFolderStructure(const std::string& path, const std::string& name) -> void {
		std::filesystem::path projPath(path);
		auto cachePath = projPath / ".cache";
		auto blobsPath = cachePath / "blobs";
		auto buildPath = cachePath / "build";
		auto assembliesPath = cachePath / "assemblies";
		auto contentPath = projPath / "content";
		auto srcPath = projPath / "src";
		auto logsPath = projPath / ".logs";

		kyanite::engine::core::CreateDirectory(cachePath.string());
		kyanite::engine::core::CreateDirectory(blobsPath.string());
		kyanite::engine::core::CreateDirectory(contentPath.string());
		kyanite::engine::core::CreateDirectory(buildPath.string());
		kyanite::engine::core::CreateDirectory(srcPath.string());
		kyanite::engine::core::CreateDirectory(assembliesPath.string());
		kyanite::engine::core::CreateDirectory(logsPath.string());

	}

	auto ProjectService::CreateProjectFile(const std::string& path, const std::string& name) -> void {

		Project project;
		project.name = name;
		project.description = "A new project";
		project.icon = "icon";
		std::stringstream ss; 
		{
			cereal::JSONOutputArchive archive(ss);
			archive(cereal::make_nvp("project", project));
		};

		// Convert the string to vector of bytes
		auto str = ss.str();
		std::vector<uint8_t> bytes(str.begin(), str.end());

		std::filesystem::path projPath(path);
		projPath /= name + ".kproj";


		kyanite::engine::core::SaveBufferToFile(projPath.string(), bytes);
	}

	auto ProjectService::CreateAssetDatabase(const std::string& path, const std::string& name) -> void {

	}
}