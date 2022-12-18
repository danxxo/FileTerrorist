#include <gtest/gtest.h>
#include "Terrorist/Terrorist.h"

void startTerror(fs::path &folderName) {
    FolderTerrorist::_myHandlerWork();
    FolderTerrorist::_getInstance(folderName)._start();
}

TEST(Terrorist, createAnotherTerrorist) {
    std::string folderName = "folderTerror";
    std::string anotherFolder = "anotherFolder";

    fs::path path = fs::current_path() / folderName;
    fs::path anotherPath = fs::current_path() / anotherFolder;

    fs::create_directory(path);
    fs::create_directory(anotherPath);

    fillFolder(path);
    fillFolder(anotherPath);

    std::thread thr_1(startTerror, std::ref(path));
    std::thread thr_2(startTerror, std::ref(anotherPath));
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::this_thread::sleep_for(std::chrono::seconds(2));

    sleep(2);

    std::raise(15);

    thr_1.join();
    thr_2.join();

    bool ifFilesTerrorised = ifSuccess(path);

    fs::remove_all(path);
    fs::remove_all(anotherPath);

    EXPECT_TRUE(ifFilesTerrorised);
}

TEST(Terrorist, wrongPath) {
    fs::path wrongPath = "wrongPath";

    sleep(1);

    try {
        startTerror(wrongPath);
    } catch (std::logic_error &logic_error) {
        EXPECT_STREQ(logic_error.what(), "wrong path");
    }
}

TEST(Terrorist, addFilesWhileRunning) {
    std::string folderName = "folderTerror";
    std::string fileName = "addedFile.txt";

    fs::path path = fs::current_path() / folderName;
    fs::path addedFilePath = path / fileName;

    fs::create_directory(path);
    fillFolder(path);

    std::thread thr_1(startTerror, std::ref(path));
    std::this_thread::sleep_for(std::chrono::seconds(2));

    sleep(2);
    std::raise(15);

    addFile(addedFilePath);
    thr_1.join();
    bool findAddedFile = fs::exists(addedFilePath.string() + ".terror");
    fs::remove_all(path);

    EXPECT_TRUE(findAddedFile);
}
