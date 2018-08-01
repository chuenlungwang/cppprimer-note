#include <memory>
#include <iostream>

struct destination {
};

struct connection {
};

connection connect(destination *dest)
{
    std::cout << "connect" << std::endl;
    return connection();
}

void disconnect(connection con)
{
    std::cout << "disconnect" << std::endl;
}

void end_connection(connection *p)
{
    disconnect(*p);
}

void f(destination &d)
{
    connection c = connect(&d);
    std::shared_ptr<connection> p(&c, end_connection);
}

void f2(destination &d)
{
    connection c = connect(&d);
    std::unique_ptr<connection, decltype(end_connection)*> p(&c, end_connection);
}

int main()
{
    destination dest;
    f(dest);
    f2(dest);
    return 0;
}
