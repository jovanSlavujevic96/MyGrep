#ifndef E4F33F12_D5A0_4951_9180_3A752B8796E7
#define E4F33F12_D5A0_4951_9180_3A752B8796E7

#include <string>

class IActionExecutor
{
public:
    virtual ~IActionExecutor() = default;

    virtual void execute(const std::string& file, const std::string& line) = 0;
};

#endif /* E4F33F12_D5A0_4951_9180_3A752B8796E7 */
