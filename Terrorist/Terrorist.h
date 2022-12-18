#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <list>
#include <mutex>
#include <fstream>
#include <csignal>
#include <filesystem>
#include <unistd.h>

namespace fs = std::filesystem;

/// true when terrorism is working, false when terrorism need to be stopped
static bool _isAppStarted;

class FolderTerrorist {
public:
    static FolderTerrorist &_getInstance(fs::path &folderName);

    void _start();

    static void _myHandlerWork();

private:
    static inline FolderTerrorist *_imp;
    fs::path _folderName;
    std::mutex _door;
    std::vector<std::thread> _threads;
    std::list<fs::path> _files;

    FolderTerrorist(fs::path &folderName);

    const fs::path &_getFolderName() const;

    std::vector<fs::path> _getCurrentFiles();

    void _updateQueue();

    void _releaseQueue();

    void _fileTerror(const fs::path &fileName);

    FolderTerrorist() = delete;

    static void _myHandler(int signal);
};


void fillFolder(fs::path &folderName);

void clearFolder(fs::path &folderName);

bool ifSuccess(fs::path &folderName);

void addFile(fs::path &fileName);
