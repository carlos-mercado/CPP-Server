#include <fstream>
#include <string>
#include <winsock2.h>
#include <chrono>

class Music
{
public:
    Music()
    {
        dir = L"G:\\now_playing.txt";
        updateMusicInfo();
        lastCheckTime = std::chrono::steady_clock::now();
    }
    ~Music()
    {
    }

    std::string getMusicInfo() { return info; }
    void checkAndUpdate();  // New polling method
private:
    std::string info;
    std::wstring dir;
    std::chrono::steady_clock::time_point lastCheckTime;
    void updateMusicInfo();
};