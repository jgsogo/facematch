
#pragma once

#include <string>
#include <map>
#include <fstream>

inline std::map<std::string, std::string> parse_config(const std::string& filename) {
	std::ifstream cfg(filename, std::ifstream::in);
	if (!cfg) {
		std::cout << "cannot read config file: '" << filename << "'" << std::endl;
		exit(1);
	}
	std::map<std::string, std::string> ret;
	std::string line;
	while (std::getline(cfg, line)) {
		std::istringstream is_line(line);
		std::string key;
		if (std::getline(is_line, key, '=')) {
			std::string value;
			if (std::getline(is_line, value)) {
				ret[key] = value;
			}
		}
	}
	return ret;
}
