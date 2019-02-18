#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>

void
format_of_boolean_values()
{
    std::cout << "default bool values: " << true << " " << false
              << "\nalpha bool values: " << std::boolalpha
              << true << " " << false << std::noboolalpha << std::endl;
}

void
base_for_integral_values()
{
    std::cout << std::showbase << std::uppercase;
    std::cout << "default: " << 20 << " " << 1024 << std::endl;
    std::cout << "octal: " << std::oct << 20 << " " << 1024 << std::endl;
    std::cout << "hex: " << std::hex << 20 << " " << 1024 << std::endl;
    std::cout << "decimal: " << std::dec << 20 << " " << 1024 << std::endl;
    std::cout << std::nouppercase << std::noshowbase << std::dec;
}

void
control_precision_of_float_point()
{
    int precision = std::cout.precision();
    std::cout << "Precision: " << std::cout.precision()
              << ", Value: " << sqrt(2.0) << std::endl;
    std::cout.precision(12);
    //std::cout << std::scientific;
    std::cout << "Precision: " << std::cout.precision()
              << ", Value: " << sqrt(2.0) << std::endl;
    std::cout << std::setprecision(3);
    std::cout << std::defaultfloat;
    std::cout << "Precision: " << std::cout.precision()
              << ", Value: " << sqrt(2.0) << std::endl;
    std::cout.precision(precision);
}

void
another_control_precision_of_float_point()
{
    double num = 100 * sqrt(2.0);
    std::cout << "default format: " << num << '\n'
              << "scientific: " << std::scientific << num << '\n'
              << "fixed decimal: " << std::fixed << num << '\n'
              << "hexadecimal: " << std::hexfloat << num << '\n'
              << "use defaults: " << std::defaultfloat << num << "\n\n";
}

void
showpoint_of_io()
{
    std::cout << 10.0 << std::endl;
    std::cout << std::showpoint << 10.0
              << std::noshowpoint << std::endl;
}

void
padding_the_output()
{
    int i = -16;
    double d = 3.14159;
    std::cout << "i: " << std::setw(12) << i << " | next col" << '\n'
              << "d: " << std::setw(12) << d << " | next col" << '\n';

    std::cout << std::left;
    std::cout << "i: " << std::setw(12) << i << " | next col" << '\n'
              << "d: " << std::setw(12) << d << " | next col" << '\n';

    std::cout << std::right;
    std::cout << "i: " << std::setw(12) << i << " | next col" << '\n'
              << "d: " << std::setw(12) << d << " | next col" << '\n';

    std::cout << std::internal;
    std::cout << "i: " << std::setw(12) << i << " | next col" << '\n'
              << "d: " << std::setw(12) << d << " | next col" << '\n';

    std::cout << std::setfill('#');
    std::cout << "i: " << std::setw(12) << i << " | next col" << '\n'
              << "d: " << std::setw(12) << d << " | next col" << '\n';
    std::cout << std::setfill(' ');
}

void
skip_input_whitespace()
{
    std::istringstream is("ab c\nd\n");
    is >> std::noskipws;
    char ch;
    while (is >> ch)
        std::cout << ch;
    is >> std::skipws;
}

void
single_byte_operation()
{
    char ch;
    std::istringstream is("ab c\nd\n");
    while (is.get(ch))
        std::cout.put(ch);
}

void
get_return_int_value()
{
    int ch;
    std::istringstream is("ab c\nd\n");
    while ((ch = is.get()) != EOF)
        std::cout.put(ch);
}

int main()
{
    format_of_boolean_values();
    base_for_integral_values();
    control_precision_of_float_point();
    another_control_precision_of_float_point();
    showpoint_of_io();
    padding_the_output();
    skip_input_whitespace();
    single_byte_operation();
    return 0;
}
