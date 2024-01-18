#include "EventParser.h"
#include "Logger.h"
EventParser::EventParser()
{

}

std::vector<std::string> EventParser::FindEventFileNames()
{
	std::vector<std::string> fileNames;

	if (!std::filesystem::exists(EVENTS_PATH)) {
		Logger::Debug(Logger::ERROR, "Directory does not exist:", EVENTS_PATH);
		// TODO handle error
		return {};
	}

	auto it = std::filesystem::directory_iterator(EVENTS_PATH);
	for (const auto& entry : std::filesystem::directory_iterator(EVENTS_PATH)) {
		fileNames.push_back(entry.path().filename().string());
	}

	return fileNames;
}

std::vector<Event<EnemyType>> EventParser::ParseEvents()
{
	std::vector<Event<EnemyType>> events;

	std::vector<std::string> fileNames = FindEventFileNames();
	for (std::string& fileName : fileNames) {
		std::ifstream file(EVENTS_PATH + fileName);
		if (!file.is_open()) {
			Logger::Log(Logger::ERROR, "Failed to open events file");
			// TODO: return error and let handler handle error
			return events;
		}

		std::string line;
		while (std::getline(file, line)) {
			if (line.find("EVENT") != std::string::npos) {
				std::istringstream iss(line);
				std::string eventName;
				iss >> eventName; // Skip "EVENT"
				iss >> eventName; // Read the event name

				// Parse the event body
				std::string timeStr, spawnStr;
				while (std::getline(file, line) && line.find("END") == std::string::npos) {
					if (line.find("TIME=") != std::string::npos) {
						timeStr = line.substr(line.find("=") + 1);
					}
					else if (line.find("SPAWN=") != std::string::npos) {
						spawnStr = line.substr(line.find("=") + 1);
					}
				}

				int time = std::stoi(timeStr);
				std::map<EnemyType, std::vector<int>> spawnMap = ParseSpawnStr(spawnStr);

				Event<EnemyType> e;
				e.id = eventId++;
				e.name = eventName;
				e.time = time;
				e.enemies = spawnMap;

				events.push_back(e);

				Logger::Debug("Loaded event", eventName, ", occurs at", time, "and spawns");
				for (auto& item : spawnMap) {
					EnemyType value = item.first;
					std::vector<int> nums = item.second;
					for (int num : nums) {
						Logger::Debug(value, num);
					}
				}
			}

		}
	}

	return events;
}

std::map<EnemyType, std::vector<int>> EventParser::ParseSpawnStr(std::string& spawnStr)
{
	try {
		std::map<EnemyType, std::vector<int>> spawnMap = {};
		std::vector<int> commaIdxs;
		std::vector<std::string> keys;
		std::vector<int> values;

		for(int i = 0; i < spawnStr.size(); ++i) {
			if (spawnStr[i] == ',') {
				commaIdxs.push_back(i);
			}
		}

		if(commaIdxs.size() > 0) {
			// get everything between each comma
			std::string subSpawn = spawnStr.substr(0, commaIdxs[0]);
			std::string key = subSpawn.substr(0, subSpawn.find(":"));
			keys.push_back(key);
			int value = std::atoi(subSpawn.substr(subSpawn.find(":") + 1).c_str());
			values.push_back(value);
			for (int i = 0; i < commaIdxs.size(); ++i) {
				std::string subSpawn;
				if (i == commaIdxs.size() - 1) {
					subSpawn = spawnStr.substr(commaIdxs[i] + 2);
				}
				else {
					// Does not work
					subSpawn = spawnStr.substr(commaIdxs[i] + 2, commaIdxs[i + 1] - commaIdxs[i] - 2);
				}
				std::string key = subSpawn.substr(0, subSpawn.find(":"));
				keys.push_back(key);
				int value = std::atoi(subSpawn.substr(subSpawn.find(":") + 1).c_str());
				values.push_back(value);
			}
		}
		else {
			std::string key = spawnStr.substr(0, spawnStr.find(":"));
			keys.push_back(key);
			int value = std::atoi(spawnStr.substr(spawnStr.find(":") + 1).c_str());
			values.push_back(value);
		}

		for (int i = 0; i < keys.size(); ++i) {
			EnemyType key = ConvertStringToEnemyType(keys[i]);
			if (key == ENEMY_ERROR) continue;
			if (spawnMap.find(key) != spawnMap.end()) {
				spawnMap[key].push_back(values[i]);
			}
			else {
				spawnMap[key] = std::vector<int>{ values[i] };
			}
		}
		
		return spawnMap;

	}
	catch (std::exception e) {
		Logger::Log(Logger::ERROR, "Something went wrong when parsing event file", e.what());
		return {};
	}
}

EnemyType EventParser::ConvertStringToEnemyType(std::string enemyStr)
{
	if (enemyStr == "ZOMBIE") {
		return ENEMY_ZOMBIE;
	}
	// TODO handle errors
	return ENEMY_ERROR;
}
