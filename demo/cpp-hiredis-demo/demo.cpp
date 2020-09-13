#include "redis.hpp"

int main()
{
    Redis *r = new Redis();
    if(!r->connect("127.0.0.1", 6379))
    {
        printf("connect error!\n");
        return 0;
    }


    r->set("name", "Hello Redis");
    std::string read_val = r->get("name");
    printf("Get the name is %s\n", read_val.c_str());


    delete r;
    return 0;
}