#include "bfmachine.hpp"
#include <stack>
#include <utility>

void cmd::set_next_cmd(std::unique_ptr<cmd> nxt)
{
    next_cmd = std::move(nxt);
}

cmd *cmd::get_next_cmd()
{
    return next_cmd.get();
}

void cmd::execute()
{
    if (next_cmd)
        next_cmd->execute();
}

void decrement_cmd::execute()
{
    buf->at(*head) -= am;
    cmd::execute();
}
decrement_cmd::decrement_cmd(std::shared_ptr<int> head, std::vector<char> *buf, int am) : cmd(std::move(head), buf, am)
{
}

void increment_cmd::execute()
{
    buf->at(*head) += am;
    cmd::execute();
}

increment_cmd::increment_cmd(std::shared_ptr<int> head, std::vector<char> *buf, int am) : cmd(std::move(head), buf, am)
{
}

void move_left_cmd::execute()
{
    if (((*head) -= am) < 0)
        throw std::logic_error("An attempt to move head pointer to cell which index is less than zero\n");
    cmd::execute();
}

move_left_cmd::move_left_cmd(std::shared_ptr<int> head, std::vector<char> *buf, int am) : cmd(std::move(head), buf, am)
{
}

void move_right_cmd::execute()
{
    if (((*head) += am) >= BUF_SIZE)
        throw std::logic_error("An attempt to move head pointer to cell which index is more than number of cells\n");
    if (*head >= buf->size())
    {
        while (buf->size() <= *head)
            buf->push_back(0);
    }
    cmd::execute();
}

move_right_cmd::move_right_cmd(std::shared_ptr<int> head, std::vector<char> *buf, int am)
    : cmd(std::move(head), buf, am)
{
}

loop_cmd::loop_cmd(const std::shared_ptr<int> &head, std::vector<char> *buf, int am) : cmd(head, buf, am)
{
}

void loop_cmd::execute()
{
    while (buf->at(*head) != 0)
    {
        if (first_inner_cmd)
            first_inner_cmd->execute();
    }
    if (next_cmd)
        next_cmd->execute();
}

void loop_cmd::set_next_cmd(std::unique_ptr<cmd> nxt)
{
    if (inner_cmd)
    {
        next_cmd = std::move(nxt);
    }
    else
    {
        first_inner_cmd = std::move(nxt);
        inner_cmd = true;
    }
}

void loop_cmd::set_inner_cmd_flag(bool val)
{
    inner_cmd = val;
}

cmd *loop_cmd::get_next_cmd()
{
    if (inner_next)
    {
        return next_cmd.get();
    }
    else
    {
        inner_next = true;
        return first_inner_cmd.get();
    }
}

void loop_cmd::set_inner_next_flag(bool val)
{
    inner_next = val;
}

void out_cmd::execute()
{
    for (auto i = 0; i < am; ++i)
        std::cout << buf->at(*head);
    cmd::execute();
}

out_cmd::out_cmd(std::shared_ptr<int> head, std::vector<char> *buf, int am) : cmd(std::move(head), buf, am)
{
}

std::vector<std::pair<char, size_t>> bfmachine::string_to_char_pairs(std::string str)
{
    if (str.empty())
        throw std::invalid_argument("Code string is empty");

    std::vector<std::pair<char, size_t>> sps;
    size_t c = 1;
    for (auto i = ++str.begin(); i != ++str.end(); ++i)
    {
        if ((*i) == *(i - 1) && (*i) != LEFT_BRACKET && (*i) != RIGHT_BRACKET)
            ++c;
        else
        {
            sps.emplace_back(*(i - 1), c);
            c = 1;
        }
    }
    return sps;
}

void bfmachine::init(std::string str)
{
    used = false;
    std::shared_ptr<int> head_ptr = std::make_shared<int>(head);
    *head_ptr = 0;
    memory_cells.clear();
    memory_cells.push_back(0);
    std::stack<loop_cmd *> stack;
    auto ps = string_to_char_pairs(str);
    cmd *current_ptr;
    if (first_cmd)
        first_cmd = std::move(first_cmd);
    else
        first_cmd = std::make_unique<cmd>(cmd(head_ptr, &memory_cells, 1));
    current_ptr = first_cmd.get();
    for (auto p : ps)
    {
        switch (p.first)
        {
        case MINUS: {
            current_ptr->set_next_cmd(std::make_unique<decrement_cmd>(head_ptr, &memory_cells, p.second));
            current_ptr = current_ptr->get_next_cmd();
            break;
        }
        case PLUS: {
            current_ptr->set_next_cmd(std::make_unique<increment_cmd>(head_ptr, &memory_cells, p.second));
            current_ptr = current_ptr->get_next_cmd();
            break;
        }
        case LEFT: {
            current_ptr->set_next_cmd(std::make_unique<move_left_cmd>(head_ptr, &memory_cells, p.second));
            current_ptr = current_ptr->get_next_cmd();
            break;
        }
        case RIGHT: {
            current_ptr->set_next_cmd(std::make_unique<move_right_cmd>(head_ptr, &memory_cells, p.second));
            current_ptr = current_ptr->get_next_cmd();
            break;
        }
        case LEFT_BRACKET: {

            auto blc_ = std::make_unique<loop_cmd>(head_ptr, &memory_cells, 1);
            stack.push(blc_.get());
            current_ptr->set_next_cmd(std::move(blc_));
            current_ptr = current_ptr->get_next_cmd();
            break;
        }
        case RIGHT_BRACKET: {
            if (current_ptr == stack.top())
            {
                stack.top()->set_inner_cmd_flag(true);
                stack.top()->set_inner_next_flag(true);
            }
            current_ptr = stack.top();
            stack.pop();
            break;
        }
        case POINT: {
            current_ptr->set_next_cmd(std::make_unique<out_cmd>(head_ptr, &memory_cells, p.second));
            current_ptr = current_ptr->get_next_cmd();
            break;
        }
        default:
            break;
        }
    }
    if (!stack.empty())
        throw std::logic_error("Right bracket ']' is missing\n");
    if (current_ptr == first_cmd.get())
        throw std::logic_error("There isn't brainfuck code in string/file\n");
}

void bfmachine::run()
{
    if (used)
        throw std::logic_error("You should reinit your bfmachine before next execution");
    used = true;
    if (first_cmd && first_cmd->get_next_cmd() != nullptr)
        first_cmd->execute();
}

bfmachine::bfmachine()
{
}
