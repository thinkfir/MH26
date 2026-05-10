#pragma once
#include "Project.hpp"

namespace pp {

// one song
struct Track {
	std::string filePath;
	tx::u32 id;
};
// group of songs
struct Album {
	std::vector<tx::u32> ids;
	tx::u32 id;
};

// collection
class TrackSourceManager {
public:
	TrackSourceManager(const std::filesystem::path& configFilePath)
	    : m_configFilePath(configFilePath) {
		init_impl();
	}

	void addTrack(std::string_view filePath) {
		m_tracks.push_back(Track{
		    std::string(filePath),
		    static_cast<tx::u32>(m_tracks.size()) });
		m_root["tracks"].get<tx::JsonArray>().push_back(tx::JsonValue(makeTrackObject(filePath)));
	}
	Track& track(tx::u32 id) { return m_tracks[id]; }
	const Track& track(tx::u32 id) const { return m_tracks[id]; }



private:
	std::filesystem::path m_configFilePath;
	tx::JsonObject m_root;
	std::vector<Track> m_tracks;
	std::vector<Album> m_albums;
	bool m_valid = false;

	void init_impl() {
		m_valid = false;

		m_root = tx::parseJson(tx::readWholeFileText(m_configFilePath));

		auto it = m_root.find("tracks");
		if (it == m_root.end() || !it->v().is<tx::JsonArray>()) {
			return;
		}
		// read tracks
		for (const tx::JsonValue& i : it->v().get<tx::JsonArray>()) {
			if (i.is<tx::JsonObject>()) {
				if (!readObject(i.get<tx::JsonObject>()))
					return;
			} else
				return;
		}


		m_valid = true;
	}

	bool readObject(const tx::JsonObject& obj) {
		const tx::JsonValue& value = obj["path"];
		if (value.is<std::string>())
			m_tracks.push_back(Track{
			    value.get<std::string>(),
			    static_cast<tx::u32>(m_tracks.size()) });
		else
			return false;
		return true;
	}

	tx::JsonObject makeTrackObject(std::string_view str) {
		tx::JsonObject obj;
		obj.insertSingle("path", tx::JsonValue(std::string(str)));
		return obj;
	}
};







class Player {
public:
private:
};
} // namespace pp