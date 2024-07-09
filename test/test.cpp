#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <filesystem>
#include <fstream>
#include <random>

#include "my_grep_api/IActionExecutor.h"
#include "my_grep_api/MyGrep.h"

struct MockExecutor : public IActionExecutor
{
    MOCK_METHOD(void, execute, (const std::string&, const std::string&), (override));
};

struct MyGrepTest : public ::testing::Test
{
    const std::string fsRoot = "./my_fs";

    void deleteFs()
    {
        if (std::filesystem::exists(fsRoot))
        {
            std::filesystem::remove_all(fsRoot);
        }
    }
    void createFs()
    {
        deleteFs();
        std::filesystem::create_directory(fsRoot);
    }


    std::ofstream createFile(const std::string& file, const std::string& path = "")
    {
        std::string filepath = fsRoot;
        if (!path.empty())
        {
            filepath += ("/" + path);
            std::filesystem::create_directories(filepath);
        }
        filepath += ("/" + file);
        
        return std::ofstream(filepath);
    } 


    void SetUp() override
    {
        createFs();
    }
    void TearDown() override
    {
        deleteFs();
    }

    MockExecutor* executor_{ new MockExecutor };
    MyGrep grep_{executor_};

    static size_t getRandom(size_t start, size_t end)
    {
        static std::random_device dev;
        static std::mt19937 rng{dev()};
        std::uniform_int_distribution<std::mt19937::result_type> dist6{start, end}; // distribution in range [start, end]
        return dist6(rng);
    }
    static inline constexpr std::array<const char*, 5U> WORDS = {
        "aksdjaslkdjaksljdlkfklaksd",
        "aksjdlaksdjaksljdidfu90e87r3024oijdjf943ojdfsf9843",
        "kjlklk;lf908987dfspkjd;slkfadsjfhad7s98as7dakjsdlkajsdklausd98u",
        "11231564030a458asd01asdas1d3as1d56a4s6d8a7sd6541asd321a3s2d156",
        "888dkkkksadlasdlmvc,cxvmcxvoaqwed9adsfkj;;;;asdasdas",
    };
};

TEST_F(MyGrepTest, FoundGrep)
{
    const std::string keyword = "KEYWORD";

    for (size_t i = 0U; i < 10U; i++)
    {
        const std::string input = "myFile" + std::to_string(i) + ".txt";
        auto file = createFile(input);
        ASSERT_TRUE(file.is_open());

        size_t random =  getRandom(0U, 4U);
        file << WORDS[random] << keyword << WORDS[random] << std::endl;

        EXPECT_CALL(*executor_, execute(testing::HasSubstr(input), testing::_)).Times(1); // expect 1 call for each input/filepath
    }

    grep_.grep(keyword, fsRoot);
}

TEST_F(MyGrepTest, NoGrepFound)
{
    const std::string keyword = "KEYWORD";

    for (size_t i = 0U; i < 10U; i++)
    {
        const std::string input = "myFile" + std::to_string(i) + ".txt";
        auto file = createFile(input);
        ASSERT_TRUE(file.is_open());

        size_t random = getRandom(0U, 4U);
        file << WORDS[random] << std::endl;

        EXPECT_CALL(*executor_, execute(testing::HasSubstr(input), testing::_)).Times(0); // expect 0 calls for each input/filepath
    }

    grep_.grep(keyword, fsRoot);
}

TEST_F(MyGrepTest, GrepOnBadPath)
{
    const std::string keyword = "KEYWORD";
    const std::string badPath = fsRoot + "/" + "badPath.txt";
    if (std::filesystem::exists(badPath))
    {
        std::filesystem::remove(badPath);
    }
    ASSERT_FALSE(std::filesystem::exists(badPath));

    EXPECT_CALL(*executor_, execute(testing::_, testing::_)).Times(0);
    grep_.grep(keyword, badPath);
}
