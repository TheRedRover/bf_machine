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

enum command_types
{
    START = '0',
    CHANGE_CELL = '+',
    MOVE_HEAD = '>',
    OUT = '.',
    LOOP = ']'
};


const size_t BUF_SIZE = 30000;

class cmd
{

  public:
    cmd(int am_) :am(am_){};
    cmd * get_clear_next();
    virtual void set_next(cmd  * next);
    virtual cmd *get_next();
    virtual int execute(int cell_value);
    virtual command_types get_cmd_type();
    virtual ~cmd();
  protected:
    command_types c = START;
    cmd* _next = nullptr;
    int am = 0;
};

class decrement_cmd : public cmd
{
    command_types c  = CHANGE_CELL;
  public:
    decrement_cmd(int am);
    command_types get_cmd_type() override;
    int execute(int cell_value) override;
};

class increment_cmd : public cmd
{
    command_types c = CHANGE_CELL;
  public:
    int execute(int cell_value) override;
    increment_cmd( int am);
    command_types get_cmd_type() override;
};

class move_left_cmd : public cmd
{
    command_types c = MOVE_HEAD;
  public:
    move_left_cmd( int am);
    int execute(int cell_value) override;
    command_types get_cmd_type() override;
};

class move_right_cmd : public cmd
{
    command_types c = MOVE_HEAD;
  public:
    move_right_cmd( int am);
    int execute(int cell_value) override;
    command_types get_cmd_type() override;
};

class loop_cmd : public cmd
{
    bool inner_flag = true;
    command_types c = LOOP;
    cmd * inner_next = nullptr;
public:
    ~loop_cmd();
    void set_flag(bool par);
    loop_cmd(int am);
    void set_next(cmd * next) override;
    int execute(int cell_value) override;
    cmd * get_next() override;
};

class out_cmd : public cmd
{
    command_types c = OUT;
  public:
    out_cmd( int am);
    int execute(int cell_value) override;
    command_types get_cmd_type() override;
};

class bfmachine
{
  private:
    cmd * first_cmd = nullptr;
    std::vector<char> cells;
    int head=0;
    bool used = false;
public:
    void init(std::string str);
    std::vector<std::pair<char, size_t>> string_to_pairs(std::string str);
    template<command_types c>
    void change_state(int param);
    void run();
    void runtime_dispatch(command_types c, int param);
    virtual ~bfmachine();

};
#endif // BF_MACHINE_BFMACHINE_HPP
