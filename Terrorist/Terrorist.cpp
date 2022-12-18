#include "Terrorist.h"

const fs::path &FolderTerrorist::_getFolderName() const {
    return _folderName;
}

FolderTerrorist &FolderTerrorist::_getInstance(fs::path &folderName) {
    if (_imp == nullptr) {
        _imp = new FolderTerrorist(folderName);
        return *_imp;
    }
}

FolderTerrorist::FolderTerrorist(fs::path &folderName) {
    _myHandlerWork();
    if (folderName.string().empty() || !fs::exists(folderName)) {
        throw std::logic_error("wrong path");
    } else {
        _folderName = folderName;
    }
}

void FolderTerrorist::_start() {
    _isAppStarted = true;

    std::cout << " [FolderTerrorist] is be in " << _getFolderName().string();

    _threads.push_back(std::thread(&FolderTerrorist::_updateQueue, this));

    for (int threadNum = std::thread::hardware_concurrency() - 1; threadNum > 0; threadNum--) {
        _threads.push_back(std::thread(&FolderTerrorist::_releaseQueue, this));
    }

    for (auto &t: _threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

std::vector<fs::path> FolderTerrorist::_getCurrentFiles() {
    std::cout << "[Scan directory]\n";
    std::vector<fs::path> files;
    for (const auto &file: fs::recursive_directory_iterator(_getFolderName())) {
        if (!file.is_directory()) {
            files.push_back(file.path());
        }
    }
    return files;
}

void FolderTerrorist::_updateQueue() {
    while (_isAppStarted) {
        sleep(3);
        auto files = _getCurrentFiles();
        for (const auto &file: files) {
            if (std::find(_files.begin(), _files.end(), file) == _files.end() && !(file.extension() == ".terror")) {
                {
                    std::lock_guard<std::mutex> lock(_door);
                    std::cout << "Add queue " << file.string() << "\n";
                    _files.push_front(file);
                }
            }
        }
    }

}

void FolderTerrorist::_releaseQueue() {
    fs::path filename;
    while (_isAppStarted || !_files.empty()) {
        {
            std::lock_guard<std::mutex> lock(_door);
            if (!_files.empty()) {
                filename = _files.back();
                _files.pop_back();
            }
        }
        if (fs::exists(fs::path(filename)) && std::filesystem::is_regular_file(fs::path(filename))) {
            _fileTerror(filename);
        }
        sleep(1);
    }
}

void FolderTerrorist::_fileTerror(const fs::path &fileName) {
    std::cout << "Terror on file " << fileName.string() << "\n";
    std::ifstream ifs(fileName, std::ios::binary);
    ifs.seekg(0, std::ios::end);
    auto filesize = ifs.tellg();
    std::vector<char> data(filesize);
    ifs.seekg(0, std::ios::beg);
    ifs.read(reinterpret_cast<char *>(data.data()), filesize);
    std::transform(data.begin(), data.end(), data.begin(), [](char byte) {
        return byte+100;
    });
    ifs.close();
    fs::remove(fileName);
    fs::path new_filename = fileName.string() + ".terror";
    fs::path p = fs::path(fileName).parent_path() / new_filename;
    std::ofstream ofs(p, std::ios::binary);
    ofs.write(reinterpret_cast<char *>(data.data()), data.size());
    ofs.close();
}


void FolderTerrorist::_myHandler(int signal) {
    std::cout << "Warning! Recieved signal : " << signal << std::endl;
    if (signal == 15) {
        std::cout << "--------------------------INFO-------------------------" << std::endl;
        std::cout << "\t\t\tFolder Terrorist will be ended" << std::endl;
        std::cout << "--------------------------INFO-------------------------" << std::endl;
    }
    _isAppStarted = false;
}

void FolderTerrorist::_myHandlerWork() {
    std::signal(1, _myHandler);
    std::signal(2, _myHandler);
    std::signal(15, _myHandler);
}

//////////////////////////////////////////////////////////////////////////

void fillFolder(fs::path &folderName) {
    for (int i = 1; i < 10; i++) {
        fs::path file_name = folderName.string() + "/file_" + std::to_string(i) + ".txt";
        std::ofstream ofs(file_name);
        ofs << "abcdefghijklmnopqrstuvwxyz";
        ofs.close();
    }
    fs::path path = folderName / "another_folder";
    fs::create_directory(path);
    fs::path file_nam = path.string() + "/file.txt";
    std::ofstream ofs_1(file_nam);
    ofs_1 << "abcdef";
    ofs_1.close();
    sleep(1);
}

void clearFolder(fs::path &folderName) {
    fs::remove_all(folderName);
    fs::create_directory(folderName);
}

bool ifSuccess(fs::path &folderName) {
    bool success = false;
    for (auto &file: fs::recursive_directory_iterator(folderName)) {
        if (!file.is_directory()) {
            if (file.path().extension() == ".terror") {
                success = true;
            } else {
                return false;
            }
        }
    }
    return success;
}

void addFile(fs::path &fileName) {
    std::ofstream ofs(fileName);
    ofs << "abcdefghijklmnopqrstuvwxyz";
    ofs.close();
    sleep(1);
}

