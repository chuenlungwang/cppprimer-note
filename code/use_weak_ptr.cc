#include <memory>
#include <iostream>

int main()
{
    auto p = std::make_shared<int>(42);
    std::weak_ptr<int> wp(p);
    //p.reset(); //reset p will delete object that it points to.
    std::cout << wp.expired() << std::endl;
    if (std::shared_ptr<int> np = wp.lock())
    {
        std::cout << "inside the if, np shares its object with p" << std::endl;
    }

    return 0;
}
