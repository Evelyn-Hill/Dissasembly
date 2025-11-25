#pragma once
#include <unordered_map>
#include <raylib.h>
#include <mutex>
#include <queue>
#include <memory>
#include <filesystem>

// Singleton Asset Manager class used to load and retrieve assets.
// How the asset manager works:
// The AssetManager is a fully thread safe singleton that can be accessed from anywhere in the project.
// In your project root you must create two files. Startup.asset.txt and Background.asset.txt
// the asset manager reads these files on startup and queues each of the listed assets to load.
//
// How the asset files work:
// Each asset file should begin with a BASE_PATH: <path> line. This line tells the asset manager where to look
// for the following assets. This variable can be changed throughout the lifetime of the file.
//
// Each asset in the file is a path (with no spaces) and a name (with no spaces) that you can retrieve it by.
// Neither the asset path nor name can have a space in them. DO NOT PUT SPACES!
//
// Example:
// <myAsset/Path AssetName>
//
// At startup, the asset manager will read in first the startup file and load all of those assets,
// then on a background thread read the background asset file and begin loading each of those assets in a queue.
//
// You can call `GetBackgroundLoadProgress()` to retrieve the load progress of the background thread.


struct AssetQueueItem {
	AssetQueueItem(std::filesystem::path path, std::string name) : path(path), name(name) {}
	std::filesystem::path path;
	std::string name;
};

class AssetManager {

using TexMap = std::unordered_map<std::string, Texture2D>;
using MusicMap = std::unordered_map<std::string, Music>;

public:
	// Thread safe Singleton instance.
	static AssetManager* GetInstance() {
		std::lock_guard<std::mutex> lock(m_Mtx);
		if (m_Instance == nullptr) {
			m_Instance = new AssetManager();
		}
		return m_Instance;
	}

	// Thread Safe method to load an asset.
	void LoadAsset(std::string path, std::string name);
	
	float GetBackgroundLoadProgress();

	Texture2D GetTexture(std::string name);
	Music* GetMusic(std::string name);
	
	enum class BackgroundLoadState {
		LOADING,
		FINISHED,
	};

	BackgroundLoadState loadState = BackgroundLoadState::FINISHED;
private:
	AssetManager();
	~AssetManager();

	AssetManager(const AssetManager& other) = delete;
	AssetManager& operator=(const AssetManager& other) = delete;

	AssetManager(AssetManager&&) = delete;
	AssetManager& operator=(AssetManager&&) = delete;

	static AssetManager* m_Instance;
	static std::mutex m_Mtx;

	const std::string m_StartupAssetFile = "Startup.asset.txt";
	const std::string m_BackgroundAssetFile = "Background.asset.txt";
	
	std::unique_ptr<TexMap>   m_TexMap;
	std::unique_ptr<MusicMap> m_MusicMap;
	
	std::unique_ptr<std::queue<AssetQueueItem>> m_StartupLoadQueue;
	std::unique_ptr<std::queue<AssetQueueItem>> m_BackgroundLoadQueue;

	float backgroundLoadProgress;
	
	void ParseAssetFile(std::string path, std::queue<AssetQueueItem>& loadQueue);
	void ThreadSafeLoadQueue(std::queue<AssetQueueItem> queue);
	void QueueAsset(AssetQueueItem qi, std::queue<AssetQueueItem>& loadQueue);
	void LoadQueue(std::queue<AssetQueueItem>& queue);
	void LoadQueueThreaded(std::queue<AssetQueueItem>& queue);
	std::unique_ptr<std::queue<AssetQueueItem>>& GetBackgroundQueue();
};
