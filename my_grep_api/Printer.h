#ifndef FFB8E7E6_97DE_4035_AC4A_A05363E0397F
#define FFB8E7E6_97DE_4035_AC4A_A05363E0397F

#include "IActionExecutor.h"

#include <iostream>
#include <mutex>

class Printer : public IActionExecutor
{
public:
    void execute(const std::string& file, const std::string& line) override
    {
        std::lock_guard guard(coutMtx_);
        std::cout << file << ": " << line << std::endl;
    }
private:
    std::mutex coutMtx_;
};

#endif /* FFB8E7E6_97DE_4035_AC4A_A05363E0397F */
