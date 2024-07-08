#include <iostream>
#include <string>
#include <cctype>
#include <filesystem>
#include <vector>
#include <fstream>
#include <mutex>
#include <future>

#include <chrono>

using fsVector = std::vector<std::string>;
using grepFuture = std::future<void>;

void collectFs(fsVector& fs, const std::string& path)
{
    constexpr size_t MIN_CAPACITY = 5U;
    constexpr size_t ADDITIONAL_CAPACITY = 10U;
    if (fs.capacity() - fs.size() <= MIN_CAPACITY)
    {
        fs.reserve(fs.capacity() + ADDITIONAL_CAPACITY);
    }

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        const auto& ep = entry.path();
        if (std::filesystem::is_directory(ep))
        {
            collectFs(fs, ep);
        }
        else
        {
            fs.push_back(ep);
        }
    }
}

void do_grep(fsVector::const_iterator start, fsVector::const_iterator end, const std::string& keyword)
{
    std::ifstream f;
    std::string line;
    for (auto it = start; it != end; it++)
    {
        f.open(*it);
        if (f.is_open())
        {
            static std::mutex coutMtx;
            while(!f.eof())
            {
                std::getline(f, line);
                if (line.find(keyword) != std::string::npos)
                {
                    std::lock_guard<std::mutex> coutGuard(coutMtx); 
                    std::cout << *it << ": " << line << std::endl;
                }
            }
            f.close();
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: $ ./my_grep \"KEYWORD\" \"PATH\"" << std::endl;
        return 1;
    }

    const std::string keyword = argv[1];
    const std::string path = argv[2];
    if (!std::filesystem::exists(path))
    {
        std::cerr << "Cannot find path: \"" << path << "\"" << std::endl;
        return 1;
    }

    using std::chrono::high_resolution_clock;
    using std::chrono::duration;
    auto t1 = high_resolution_clock::now();

    fsVector fs;
    collectFs(fs, path);

    constexpr size_t MAX_THREADS = 10U;
    constexpr size_t MIN_FILES_FOR_SEPARATE_TH = 5U;

    const bool separateThread = fs.size() > MIN_FILES_FOR_SEPARATE_TH;
    const size_t numOfThreadsInCaseOfMin = (fs.size() / MIN_FILES_FOR_SEPARATE_TH);
    const size_t numOfThreads = (separateThread) ? (numOfThreadsInCaseOfMin > MAX_THREADS) ? MAX_THREADS : numOfThreadsInCaseOfMin : 1U;

    const size_t filesPerThread = (fs.size() / numOfThreads);
    const size_t extraFiles = (fs.size() % numOfThreads);

    // iterators
    const auto END = fs.cend();
    auto start = fs.cbegin();
    auto end = start;
    auto step = start;

    std::vector<grepFuture> greps;
    greps.reserve(numOfThreads);

    for (size_t i = 1U; i <= numOfThreads; i++)
    {
        step = start + (i * filesPerThread);
        if (i <= extraFiles)
        {
            step++;
        }
        end = (step > END) ? END : step;
        // do_grep(start, end, keyword);
        greps.emplace_back(std::async(std::launch::async, &do_grep, start, end, keyword));
        start = end;
    }

    for (auto& g : greps)
    {
        g.get();
    }

    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_double.count() << "ms" << std::endl;

    return 0;
}
