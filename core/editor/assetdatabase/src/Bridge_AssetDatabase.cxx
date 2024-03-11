#include "assetdatabase/Bridge_AssetDatabase.h"

#include <io/IO.hxx>

#include <sqlite3.h>

int8_t Create(void* database) {
	// Create tables
		// Asset table consists of this:
		// - Asset UUID
		// - Asset Name
		// - Asset Type
		// - Asset Blob Path
	if (sqlite3_exec(
		reinterpret_cast<sqlite3*>(database),
		"CREATE TABLE IF NOT EXISTS assets (uuid TEXT PRIMARY KEY, name TEXT, updated_at INT8, type TEXT, path TEXT)", nullptr, nullptr, nullptr) != 0) {
		return -1;
	}

	// Asset Package table consists of this:
	// - Package UUID
	// - Package Name
	// - Package Path
	if (sqlite3_exec(
		reinterpret_cast<sqlite3*>(database),
		"CREATE TABLE IF NOT EXISTS asset_packages (uuid TEXT PRIMARY KEY, name TEXT, path TEXT)", nullptr, nullptr, nullptr) != 0) {
		return -2;
	}

	// Dependency table consists of this:
	// - Dependency UUID
	// - Asset UUID
	// - Dependency UUID
	if (sqlite3_exec(
		reinterpret_cast<sqlite3*>(database),
		"CREATE TABLE IF NOT EXISTS dependencies (uuid TEXT PRIMARY KEY, assetUuid TEXT, dependencyUuid TEXT, FOREIGN KEY(assetUuid) REFERENCES assets(uuid), FOREIGN KEY(dependencyUuid) REFERENCES assets(uuid))", nullptr, nullptr, nullptr) != 0) {
		return -3;
	}

	// Labels table consists of this:
	// - Label UUID
	// - Label name
	// - Label color
	if (sqlite3_exec(
		reinterpret_cast<sqlite3*>(database),
		"CREATE TABLE IF NOT EXISTS labels (uuid TEXT PRIMARY KEY, name TEXT, color TEXT)", nullptr, nullptr, nullptr) != 0) {
		return -4;
	}

	// Asset Labels table consists of this:
	// - Asset label UUID
	// - Asset UUID
	// - Label UUID
	if (sqlite3_exec(
		reinterpret_cast<sqlite3*>(database),
		"CREATE TABLE IF NOT EXISTS asset_labels (uuid TEXT PRIMARY KEY, assetUuid TEXT, labelUuid TEXT, FOREIGN KEY(assetUuid) REFERENCES assets(uuid), FOREIGN KEY(labelUuid) REFERENCES labels(uuid))", nullptr, nullptr, nullptr) != 0) {
		return -5;
	}
}

int AssetDatabase_AddAsset(void* database, const char* uuid, const char* name, const char* path, const char* type, int64_t time) {
	sqlite3_stmt* statement;
	sqlite3_prepare_v3(
		reinterpret_cast<sqlite3*>(database),
		"INSERT INTO assets (uuid, name, path, type, updated_at) VALUES (?, ?, ?, ?, ?)",
		-1,
		SQLITE_PREPARE_PERSISTENT,
		&statement,
		nullptr
	);
	sqlite3_bind_text(statement, 1, uuid, -1, SQLITE_STATIC);
	sqlite3_bind_text(statement, 2, name, -1, SQLITE_STATIC);
	sqlite3_bind_text(statement, 3, path, -1, SQLITE_STATIC);
	sqlite3_bind_text(statement, 4, type, -1, SQLITE_STATIC);
	sqlite3_bind_int64(statement, 5, time);

	sqlite3_step(statement);
	sqlite3_finalize(statement);

	return sqlite3_errcode(reinterpret_cast<sqlite3*>(database));
}

void AssetDatabase_UpdateAsset(void* database, const char* uuid, int64_t time) {
	// Update the asset's modified time
	sqlite3_stmt* statement;

	sqlite3_prepare_v3(
		reinterpret_cast<sqlite3*>(database),
		"UPDATE assets SET updated_at = ? WHERE uuid = ?",
		-1,
		SQLITE_PREPARE_PERSISTENT,
		&statement,
		nullptr
	);

	sqlite3_bind_int64(statement, 1, time);
	sqlite3_bind_text(statement, 2, uuid, -1, SQLITE_STATIC);

	sqlite3_step(statement);
	sqlite3_finalize(statement);
}

void AssetDatabase_RemoveAsset(void* database, const char* uuid) {
	sqlite3_stmt* statement;
	sqlite3_prepare_v3(
		reinterpret_cast<sqlite3*>(database),
		"DELETE FROM assets WHERE uuid = ? OR path = ?",
		-1,
		SQLITE_PREPARE_PERSISTENT,
		&statement,
		nullptr
	);
	sqlite3_bind_text(statement, 1, uuid, -1, SQLITE_STATIC);

	sqlite3_step(statement);
	sqlite3_finalize(statement);
}

int64_t AssetDatabase_GetModifiedTime(void* database, const char* uuid) {
	sqlite3_stmt* statement;
	sqlite3_prepare_v3(
		reinterpret_cast<sqlite3*>(database),
		"SELECT updated_at FROM assets WHERE uuid = ? LIMIT 1",
		-1,
		SQLITE_PREPARE_PERSISTENT,
		&statement,
		nullptr
	);

	sqlite3_bind_text(statement, 1, uuid, -1, SQLITE_STATIC);

	sqlite3_step(statement);
	auto time = sqlite3_column_int64(statement, 0);
	sqlite3_finalize(statement);

	return time;
}

void AssetDatabase_GetUuidForPath(void* database, const char* path, const char** uuid) {
	sqlite3_stmt* statement;
	sqlite3_prepare_v3(
		reinterpret_cast<sqlite3*>(database),
		"SELECT uuid FROM assets WHERE path = ? LIMIT 1",
		-1,
		SQLITE_PREPARE_PERSISTENT,
		&statement,
		nullptr
	);

	sqlite3_bind_text(statement, 1, path, -1, SQLITE_STATIC);

	while (sqlite3_step(statement) == SQLITE_ROW) {
		const char* uuidPtr = reinterpret_cast<const char*>(sqlite3_column_text(statement, 0));
		if (uuidPtr != nullptr) {
			// Copy the UUID so it can be used outside of this function
			const char* uuidStr = new char[37];
			strcpy((char*)uuidStr, uuidPtr);

			*uuid = uuidStr;
			break; // Assuming only one UUID is expected
		}
	}

	sqlite3_finalize(statement);
}

void AssetDatabase_FreeUuid(const char* uuid) {
	delete[] uuid;
}

void* AssetDatabase_CreateOrLoad(const char* path) {
	sqlite3* db;
	int rc = sqlite3_open(path, &db);
	Create(db);

	rc = sqlite3_errcode(db);
	if(rc != SQLITE_OK) {
		throw std::runtime_error("Failed to open database");
	}

	return db;
}

void AssetDatabase_Close(void* database) {
	sqlite3* db = (sqlite3*)database;
	sqlite3_close(db);
}
