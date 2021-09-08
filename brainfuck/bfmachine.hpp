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
    cmd(std::shared_ptr<int> head_, std::vector<char> *buf_, int am_) : head(std::move(head_)), buf(buf_), am(am_){};
    virtual void set_next_cmd(std::unique_ptr<cmd>);
    virtual cmd *get_next_cmd();
    virtual void execute();

  protected:
    std::unique_ptr<cmd> next_cmd;
    std::shared_ptr<int> head;
    std::vector<char> *buf;
    int am = 0;
};

class decrement_cmd : public cmd
{
  public:
    decrement_cmd(std::shared_ptr<int> head, std::vector<char> *buf, int am);
    void execute() override;
};

class increment_cmd : public cmd
{
  public:
    void execute() override;
    increment_cmd(std::shared_ptr<int> head, std::vector<char> *buf, int am);
};

class move_left_cmd : public cmd
{
  public:
    move_left_cmd(std::shared_ptr<int> head, std::vector<char> *buf, int am);
    void execute() override;
};

class move_right_cmd : public cmd
{
  public:
    move_right_cmd(std::shared_ptr<int> head, std::vector<char> *buf, int am);
    void execute() override;
};

class loop_cmd : public cmd
{
  private:
    bool inner_cmd = false;
    bool inner_next = false;
    std::unique_ptr<cmd> first_inner_cmd;

  public:
    void set_inner_cmd_flag(bool val);
    void set_inner_next_flag(bool val);
    loop_cmd(const std::shared_ptr<int> &head, std::vector<char> *buf, int am);
    void set_next_cmd(std::unique_ptr<cmd>) override;
    cmd *get_next_cmd() override;
    void execute() override;
};

class out_cmd : public cmd
{
  public:
    out_cmd(std::shared_ptr<int> head, std::vector<char> *buf, int am);
    void execute() override;
};

class bfmachine
{
  private:
  public:
    bfmachine();

  private:
    std::unique_ptr<cmd> first_cmd;
    int head = 0;
    static std::vector<std::pair<char, size_t>> string_to_char_pairs(std::string str);
    std::vector<char> memory_cells;
    bool used = false;

  public:
    void init(std::string str);
    void run();
};
#endif // BF_MACHINE_BFMACHINE_HPP
