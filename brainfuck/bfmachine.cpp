#include "bfmachine.hpp"
#include <stack>
#include <utility>

void cmd::set_next(cmd *nxt)
{
    _next = nxt;
}

cmd *cmd::get_next()
{
    return _next;
}

int cmd::execute(int cell_value)
{
    return 0;
}

command_types cmd::get_cmd_type()
{
    return c;
}

cmd::~cmd()
{
}

cmd *cmd::get_clear_next()
{
    return _next;
}

int decrement_cmd::execute(int cell_value)
{
    return -am;
}

decrement_cmd::decrement_cmd(int am) : cmd(am)
{
}

command_types decrement_cmd::get_cmd_type()
{
    return c;
}

int increment_cmd::execute(int cell_value)
{
    return am;
}

increment_cmd::increment_cmd(int am) : cmd(am)
{
}

command_types increment_cmd::get_cmd_type()
{
    return c;
}

int move_left_cmd::execute(int cell_value)
{
    return -am;
}

move_left_cmd::move_left_cmd(int am) : cmd(am)
{
}

command_types move_left_cmd::get_cmd_type()
{
    return c;
}

int move_right_cmd::execute(int cell_value)
{
    return am;
}

move_right_cmd::move_right_cmd(int am) : cmd(am)
{
}

command_types move_right_cmd::get_cmd_type()
{
    return c;
}

int out_cmd::execute(int cell_value)
{
    return am;
}

out_cmd::out_cmd(int am) : cmd(am)
{
}

command_types out_cmd::get_cmd_type()
{
    return c;
}

loop_cmd::loop_cmd(int am) : cmd(am)
{
}

int loop_cmd::execute(int cell_value)
{
    if (cell_value == 0)
        inner_flag = false;
    else
        inner_flag = true;
    return 0;
}

cmd *loop_cmd::get_next()
{
    if (inner_next == _next && !inner_flag)
        return nullptr;
    if (inner_flag)
        return inner_next;
    else
        return _next;
}

void loop_cmd::set_flag(bool par)
{
    inner_flag = par;
}

void loop_cmd::set_next(cmd *next)
{
    if (inner_flag)
    {
        inner_next = next;
    }
    else
    {
        _next = next;
    }
}

loop_cmd::~loop_cmd()
{

    cmd *ptr = this->inner_next;
    if (ptr && this != ptr)
        while (ptr->get_next() != this)
        {
            auto temp = ptr;
            ptr = temp->get_next();
            delete temp;
        }
}

std::vector<std::pair<char, size_t>> bfmachine::string_to_pairs(std::string str)
{
    if (str.empty())
        throw std::invalid_argument("Code string is empty\n");

    std::vector<std::pair<char, size_t>> sps;
    size_t c = 1;
    for (std::string::iterator iter = str.begin(); iter != str.end(); ++iter)
    {
        if (*iter == *(std::next(iter)) && *iter != LEFT_BRACKET && *iter != RIGHT_BRACKET)
        {
            ++c;
        }
        else
        {
            sps.emplace_back(*iter, c);
            c = 1;
        }
    }
    return sps;
}

void bfmachine::init(std::string str)
{
    used = false;
    head = 0;
    cells.clear();
    cells.push_back(0);
    delete first_cmd;
    first_cmd = new cmd(1);
    cmd *current_cmd = first_cmd;
    std::stack<loop_cmd *> stack;
    auto ps = string_to_pairs(str);

    for (auto p : ps)
    {
        switch (p.first)
        {
        case MINUS: {
            current_cmd->set_next(new decrement_cmd(p.second));
            current_cmd = current_cmd->get_next();
            break;
        }
        case PLUS: {
            current_cmd->set_next(new increment_cmd(p.second));
            current_cmd = current_cmd->get_next();
            break;
        }
        case LEFT: {
            current_cmd->set_next(new move_left_cmd(p.second));
            current_cmd = current_cmd->get_next();
            break;
        }
        case RIGHT: {
            current_cmd->set_next(new move_right_cmd(p.second));
            current_cmd = current_cmd->get_next();
            break;
        }
        case POINT: {
            current_cmd->set_next(new out_cmd(p.second));
            current_cmd = current_cmd->get_next();
            break;
        }
        case LEFT_BRACKET: {
            stack.push(new loop_cmd(1));
            current_cmd->set_next(stack.top());
            current_cmd = current_cmd->get_next();
            stack.top()->set_flag(true);
            break;
        }
        case RIGHT_BRACKET: {
            if (stack.empty())
                throw std::logic_error("Left bracket '[' is missing\n");
            current_cmd->set_next(stack.top());
            stack.top()->set_flag(false);
            current_cmd = stack.top();
            stack.pop();
            break;
        }
        default:
            break;
        }
    }
    if (!stack.empty())
        throw std::logic_error("Right bracket ']' is missing\n");
    if (current_cmd == first_cmd)
        throw std::logic_error("There isn't brainfuck code in string/file\n");
}

bfmachine::~bfmachine()
{
    auto ptr = first_cmd;
    while (ptr)
    {
        auto temp = ptr;
        ptr = ptr->get_clear_next();
        delete temp;
    }
}

void bfmachine::run()
{
    if (used)
        throw std::runtime_error("You should reinitialize bfmachine before using one more time\n");
    auto ptr = first_cmd;
    while (ptr)
    {
        runtime_dispatch(ptr->get_cmd_type(), ptr->execute(cells[head]));
        if (!ptr->get_next())
            ptr = nullptr;
        else
            ptr = ptr->get_next();
    }
    used = true;
}

template <command_types c> void bfmachine::change_state(int param)
{
    if constexpr (c == CHANGE_CELL)
    {
        cells[head] += param;
    }
    if constexpr (c == MOVE_HEAD)
    {
        if (head + param > BUF_SIZE || head + param < 0)
        {
            throw std::out_of_range("Head pointer out of cells range\n");
        }
        while (cells.size() <= head + param + 1)
            cells.push_back(0);
        head += param;
    }
    if constexpr (c == OUT)
    {
        for (auto i = 0; i < param; ++i)
            std::cout << cells[head];
    }
    if constexpr (c == LOOP)
    {
    }
}

void bfmachine::runtime_dispatch(command_types c, int param)
{
    if (c == CHANGE_CELL)
    {
        change_state<CHANGE_CELL>(param);
    }
    if (c == MOVE_HEAD)
    {
        change_state<MOVE_HEAD>(param);
    }
    if (c == LOOP)
    {
        change_state<LOOP>(param);
    }
    if (c == OUT)
    {
        change_state<OUT>(param);
    }
}
