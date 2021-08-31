//
// Created by theredrover on 31/08/2021.
//

#ifndef BF_MACHINE_BFMACHINE_HPP
#define BF_MACHINE_BFMACHINE_HPP
#include <iostream>
#include <memory>
#include <vector>


enum commands
{
    MINUS = '-',
    PLUS = '+',
    RIGHT = '>',
    LEFT = '<',
    POINT = '.',
    COMA = ',',
    RIGHT_BRACKET = ']',
    LEFT_BRACKET = '['
};

const size_t BUF_SIZE = 30000;

class cmd
{
public:
    cmd(std::shared_ptr<int> head_, char * buf_, int am_):head(std::move(head_)), buf(buf_), am(am_){};
    void nxt(std::unique_ptr<cmd>);
    cmd * get_nxt();
    virtual void fn();
protected:
    std::unique_ptr<cmd> nxt_;
    std::shared_ptr<int> head;
    char * buf;
    int am=0;
};

class decc : public cmd
{
public:
    decc( std::shared_ptr<int> head, char *buf, int am);

    void fn() override;
};

class incc : public cmd
{
public:
    void fn() override;

    incc( std::shared_ptr<int> head, char *buf, int am);
};

class mvlc : public cmd
{
public:
    mvlc( std::shared_ptr<int> head, char *buf, int am);

    void fn() override;
};

class mvrc : public cmd
{
public:
    mvrc( std::shared_ptr<int> head, char *buf, int am);

    void fn() override;
};

class blc : public cmd
{
private:
    cmd * elc;
public:
    blc( std::shared_ptr<int> head, char *buf, int am);

    void set_elc(cmd * elc_);
    void fn() override;
};

class elc : public cmd
{
private:
    cmd * blc;
public:
    elc( std::shared_ptr<int> head, char *buf, int am);

    void set_blc(cmd * blc_);
    void fn() override;
};

class outc : public cmd
{
public:
    outc( std::shared_ptr<int> head, char *buf, int am);

    void fn() override;
};

class bfmachine
{
private:
    std::unique_ptr<cmd> first_cmd;
    int head=0;
    std::unique_ptr<cmd> fst_cmd;
    static std::vector<std::pair<char, size_t>> s_to_ps( const std::string & str);

public:
    char cpu[BUF_SIZE] = {0};
    char * cpu_first = cpu;
    void init( const std::string & str);
    void execute();
};
#endif //BF_MACHINE_BFMACHINE_HPP
