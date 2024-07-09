#ifndef E0AD69C8_D084_463C_AC8C_84B0AC915571
#define E0AD69C8_D084_463C_AC8C_84B0AC915571

#include <string>
#include <vector>
#include <future>
#include <memory>

class IActionExecutor;

class MyGrep
{
public:
    using fsVector = std::vector<std::string>;
    using grepFuture = std::future<void>;

    void grep(const std::string& keyword, const std::string& path);

    MyGrep(IActionExecutor* executor) : executor_(executor) {}
    MyGrep(std::unique_ptr<IActionExecutor> executor) : executor_(std::move(executor)) {}

private:
    void collectFs(const std::string& path);
    void search(fsVector::const_iterator begin, fsVector::const_iterator end);

    std::string keyword_;

    std::vector<std::string> fs_;
    std::vector<grepFuture> tasks_;
    std::unique_ptr<IActionExecutor> executor_;
    
};

#endif /* E0AD69C8_D084_463C_AC8C_84B0AC915571 */
