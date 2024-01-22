#pragma once

#include "IAssetLoader.hxx"

#include <shared/Exported.hxx>
#include <shared/Serializable.hxx>

#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace kyanite::engine::assetpackages {
	class AssetPackage;

	EXPORTED auto Initialize(IAssetLoader* loader) -> void;

	/**
		@brief Loads an asset from a file
		@param path The path to the asset file
		@return The loaded asset
		@note This function is not type safe, and will return a void pointer. Used internally by the engine. Caller clears the memory
	*/
	auto LoadAssetFromPackage(const AssetPackage* package, std::string path) -> void*;

	/**
		@brief Loads a package from a file
		@param path The path to the package file
		@return The package
		@note The package data is not loaded, only the metadata
	*/
	EXPORTED auto LoadPackage(const std::string path) -> AssetPackage*;

	/**
		@brief Checks if the package has an asset
		@param metadata The package metadata
		@param path The path to the asset
		@return Whether the package has the asset
	*/
	EXPORTED auto PackageHasAsset(const AssetPackage* package, std::string path) -> bool;

	/**
		@brief Loads an asset from the package
		@param package The package to load the asset from
		@param path The path to the asset
		@return The loaded asset
	*/
	template<typename T>
	EXPORTED auto LoadAssetFromPackage(const AssetPackage* package, std::string path) -> T;

	/**
		@brief Creates a new package
		@param name The path of the package
		@return The created package
	*/
	EXPORTED auto CreateAssetPackage(std::string path) -> AssetPackage*;

	/**
		@brief Adds an asset to the package
		@param package The package to add the asset to
		@param path The path to the asset
		@param data The data of the asset
	*/
	template<typename T>
	EXPORTED auto AddToAssetPackage(AssetPackage* package, std::string path, T data) -> void;
}