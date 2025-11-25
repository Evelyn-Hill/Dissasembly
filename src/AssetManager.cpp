#include <AssetManager.hpp>
#include <memory>
#include <sstream>
#include <fstream>
#include <ranges>
#include <vector>
#include <iostream>
#include <thread>


AssetManager* AssetManager::m_Instance;
std::mutex AssetManager::m_Mtx;

AssetManager::AssetManager() {
	m_StartupLoadQueue = std::make_unique<std::queue<AssetQueueItem>>();
	m_BackgroundLoadQueue = std::make_unique<std::queue<AssetQueueItem>>();
	ParseAssetFile(m_StartupAssetFile, *m_StartupLoadQueue);
	LoadQueue(*m_StartupLoadQueue);

	ParseAssetFile(m_BackgroundAssetFile, *m_BackgroundLoadQueue);
	LoadQueueThreaded(*m_BackgroundLoadQueue);
}

AssetManager::~AssetManager() {

}

void AssetManager::QueueAsset(AssetQueueItem qi, std::queue<AssetQueueItem>& loadQueue) {
	if(qi.name.empty() || qi.path.empty()) {
		std::cout << "AssetQueueItem cannot be enqueued because it is not full\n";
	}

	loadQueue.emplace(qi);
}

void AssetManager::LoadQueue(std::queue<AssetQueueItem>& queue) {
	while (!queue.empty()) {
		auto qi = queue.front();
		
		auto ext = qi.path.extension();

		if (ext == ".png") {
			Texture2D tex = LoadTexture(qi.path.string().c_str());
			if (m_TexMap == nullptr) {
				m_TexMap = std::make_unique<TexMap>();
			}

			m_TexMap->emplace(std::pair<std::string, Texture2D>(qi.name, tex));
			std::cout << "Loaded texture: " << qi.name << "\n";
		} 
		else {
			std::cout << "Unsupported file format: " << ext << "\n";
		}

		queue.pop();
	}
}

void AssetManager::LoadQueueThreaded(std::queue<AssetQueueItem>& queue) {
	// NOTE: I'd rather send a copy of the queue over to the new thread,
	// this way we dont have to use sync primitives to modify the actual queue
	// and we have a clean queue each time we call LoadQueueThreaded, meaning we never get
	// a duplication of assets loaded 
	std::queue<AssetQueueItem> q;
	while (!queue.empty()) {
		q.emplace(queue.front());
		queue.pop();
	}
	
	std::thread t(&AssetManager::ThreadSafeLoadQueue, this, q);
	
	t.join();
}

void AssetManager::ThreadSafeLoadQueue(std::queue<AssetQueueItem> queue) {
	std::mutex mtx;
	
	mtx.lock();
	loadState = BackgroundLoadState::LOADING;
	mtx.unlock();

	while (!queue.empty()) {
		auto qi = queue.front();
		
		auto ext = qi.path.extension();

		if (ext == ".png") {
			Texture2D tex = LoadTexture(qi.path.string().c_str());
			if (m_TexMap == nullptr) {
				mtx.lock();
				m_TexMap = std::make_unique<TexMap>();
				mtx.unlock();
			}
			
			mtx.lock();
			m_TexMap->emplace(std::pair<std::string, Texture2D>(qi.name, tex));
			mtx.unlock();
			std::cout << "Loaded texture: " << qi.name << "\n";
		} 
		else {
			std::cout << "Unsupported file format: " << ext << "\n";
		}

		queue.pop();
	}

	mtx.lock();
	loadState = BackgroundLoadState::FINISHED;
	mtx.unlock();
	std::cout << "Finished background loading" << std::endl;
}

Texture2D AssetManager::GetTexture(std::string name) {
	return m_TexMap->at(name);
}

void AssetManager::ParseAssetFile(std::string path, std::queue<AssetQueueItem>& loadQueue) {

	std::ifstream file(path);
	if (!file.is_open()) {
		std::cout << "Could not open file: " << path << "\n";
		return;
	}	
	
	std::string line;

	std::string delimiter = " ";
	std::string basePath = "";
	
	int lineCount = 1;

	while (std::getline(file, line)) {
		if (line[0] == '#') {
			continue;
		}
		if (line.empty()) {
			continue;
		}

		if(line.starts_with("BASE_PATH:")) {
			std::vector<std::string> tokens;

			for(const auto& view : line | std::views::split(delimiter)) {
				tokens.push_back(std::string(view.begin(), view.end()));
			}

			basePath = tokens.at(1);
		}

		std::vector<std::string> tokens;
	
		for(const auto& view : line | std::views::split(delimiter)) {
			tokens.push_back(std::string(view.begin(), view.end()));
		}
		
		if (tokens.size() != 2) {
			std::cout << "Could not parse file: " << path << "\n line: " << lineCount << "\n";
			std::cout << "Continuing...\n";
		}

		
		std::filesystem::path p(basePath + tokens.at(0));
		AssetQueueItem qi(p, tokens.at(1));
		std::cout << "Queued: " << qi.name << " to load.\n";
		QueueAsset(qi, loadQueue);
		lineCount++;
	}
}
