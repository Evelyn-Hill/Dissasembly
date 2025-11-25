#pragma once
#include <unordered_map>
#include <raylib.h>
#include <mutex>
#include <queue>
#include <memory>
#include <filesystem>

// Singleton Asset Manager class used to load and retrieve assets.
// How the asset files work:
// Each asset in the file is given a path (with no spaces) and a name that you can retrieve it by, space delimited.
// Neither the asset path nor name can have a space in them.
//
// Example:
// <myAsset/Path AssetName>
//
// At startup, the asset manager will read in first the startup file and load all of those assets, 
// then on a background thread read the background asset file and begin loading 
// each of those assets in a queue.
//
// You can also call `AppendBackgroundAsset(path, name)` at any tme to load an asset in the background.


struct AssetQueueItem {
	AssetQueueItem(std::filesystem::path path, std::string name) : path(path), name(name) {}
	std::filesystem::path path;
	std::string name;
};

class AssetManager {

using TexMap = std::unordered_map<std::string, Texture2D>;
using MusicMap = std::unordered_map<std::string, Music>;

public:
	static AssetManager* GetInstance() {
		std::lock_guard<std::mutex> lock(m_Mtx);
		if (m_Instance == nullptr) {
			m_Instance = new AssetManager();
		}
		return m_Instance;
	}
	
	
	void QueueAsset(AssetQueueItem qi, std::queue<AssetQueueItem>& loadQueue);
	
	void LoadQueue(std::queue<AssetQueueItem>& queue);
	
	Texture2D GetTexture(std::string name);
	Music* GetMusic(std::string name);
	
	enum class BackgroundLoadState {
		LOADING,
		FINISHED,
	};


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
	
	void ParseAssetFile(std::string path, std::queue<AssetQueueItem>& loadQueue);
};

