#include "MyGrep.h"

#include <fstream>
#include <filesystem>
#include <cstdint>
#include <cassert>

#include <iostream>
#include <mutex>

#include "IActionExecutor.h"

void MyGrep::grep(const std::string& keyword, const std::string& path)
{
    assert(executor_ != nullptr);

    fs_.clear();
    tasks_.clear();
    keyword_ = keyword;

    if (std::filesystem::is_directory(path))
    {
        collectFs(path);
    }
    else
    {
        fs_.push_back(path);
    }

    constexpr size_t MAX_THREADS = 10U;
    constexpr size_t MIN_FILES_FOR_SEPARATE_TH = 5U;

    const bool separateThread = fs_.size() > MIN_FILES_FOR_SEPARATE_TH;
    const size_t numOfThreadsInCaseOfMin = (fs_.size() / MIN_FILES_FOR_SEPARATE_TH);
    const size_t numOfThreads = (separateThread) ? (numOfThreadsInCaseOfMin > MAX_THREADS) ? MAX_THREADS : numOfThreadsInCaseOfMin : 1U;

    const size_t filesPerThread = (fs_.size() / numOfThreads);
    const size_t extraFiles = (fs_.size() % numOfThreads);

    std::vector<grepFuture> tasks_;
    tasks_.reserve(numOfThreads);

    // iterators
    const auto END = fs_.cend();
    auto start = fs_.cbegin();
    auto end = start;
    auto step = start;

    for (size_t i = 1U; i <= numOfThreads; i++)
    {
        step = start + (i * filesPerThread);
        if (i <= extraFiles)
        {
            step++;
        }
        end = (step > END) ? END : step;
        tasks_.emplace_back(std::async(std::launch::async, &MyGrep::search, this, start, end));
        start = end;
    }

    for (auto& g : tasks_)
    {
        g.get();
    }
}

void MyGrep::collectFs(const std::string& path)
{
    constexpr size_t MIN_CAPACITY = 5U;
    constexpr size_t ADDITIONAL_CAPACITY = 10U;
    if (fs_.capacity() - fs_.size() <= MIN_CAPACITY)
    {
        fs_.reserve(fs_.capacity() + ADDITIONAL_CAPACITY);
    }

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (std::filesystem::is_directory(entry.path()))
        {
            collectFs(entry.path());
        }
        else
        {
            fs_.push_back(entry.path());
        }
    }
}

void MyGrep::search(fsVector::const_iterator start, fsVector::const_iterator end)
{
    std::ifstream f;
    std::string line;
    for (auto it = start; it != end; it++)
    {
        f.open(*it);
        if (f.is_open())
        {
            while(!f.eof())
            {
                std::getline(f, line);
                if (line.find(keyword_) != std::string::npos)
                {
                    executor_->execute(*it, line);
                }
            }
            f.close();
        }
    }
}
