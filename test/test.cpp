#include "../brainfuck/bfmachine.hpp"
#include <string>   // std::string
#include <iostream> // std::cout
#include <sstream>
#include <fstream>
#include <cassert>

std::string get_cout_string(bfmachine & c)
{
    std::stringstream buffer;
    std::streambuf* prevcoutbuf = std::cout.rdbuf(buffer.rdbuf());
    c.execute();
    std::cout.rdbuf(prevcoutbuf);
    std::string text = buffer.str();
    return text;
}

std::string read_file_to_string(const std::string& file_name) {
    std::ifstream f(file_name);
    if (!f.good())
        throw std::invalid_argument("Such file doesn't exist\n");
    std::string s = std::string((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    return s;
}

int main()
{
    bfmachine b;
    b.init(read_file_to_string("golden.bf"));
    assert(read_file_to_string("golden.out").compare(get_cout_string(b))==0);
    b.init(read_file_to_string("Mandelbrot.bf"));
    assert(read_file_to_string("Mandelbrot.out").compare(get_cout_string(b))==0);
    b.init(read_file_to_string("counter.bf"));
    assert(read_file_to_string("counter.out").compare(get_cout_string(b))==0);

}

