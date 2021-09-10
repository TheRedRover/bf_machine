#include "bfmachine.hpp"
#include <stack>
#include <utility>

void cmd::set_next(cmd* nxt)
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

command_types cmd::get_cmd_type() {
    return c;
}

cmd::~cmd()
{
    delete _next;
}

cmd *cmd::get_constr_next() {
    return _next;
}

int decrement_cmd::execute(int cell_value)
{
    return -am;
}

decrement_cmd::decrement_cmd(int am) : cmd(am)
{
}

command_types decrement_cmd::get_cmd_type() {
    return c;
}

int increment_cmd::execute(int cell_value)
{
    return am;
}

increment_cmd::increment_cmd(int am) : cmd(am)
{
}

command_types increment_cmd::get_cmd_type() {
    return c;
}

int move_left_cmd::execute(int cell_value)
{
    return -am;
}

move_left_cmd::move_left_cmd(int am) : cmd(am)
{
}

command_types move_left_cmd::get_cmd_type() {
    return c;
}

int move_right_cmd::execute(int cell_value)
{
    return am;
}

move_right_cmd::move_right_cmd(int am) : cmd(am)
{
}

command_types move_right_cmd::get_cmd_type() {
    return c;
}

int out_cmd::execute(int cell_value)
{
    return am;
}

out_cmd::out_cmd(int am) : cmd(am)
{
}

command_types out_cmd::get_cmd_type() {
    return c;
}

loop_cmd::loop_cmd(int am) : cmd(am) {
}


loop_cmd::~loop_cmd()
{
    delete after_loop_next;
    delete inner_next;
}

int loop_cmd::execute(int cell_value) {
    if(cell_value==0) {
        std::cout<<"After_loop\n";
        _next = after_loop_next;
    }
    else {
        std::cout<<"Into loop\n";
        _next = inner_next;
    }
}

command_types loop_cmd::get_cmd_type() {
    return c;
}

void loop_cmd::set_next(cmd * next) {
    if(inner_next!=nullptr) {
        std::cerr<<"AFTER\n";
        after_loop_next = next;
    }
    else {
        std::cerr<<"INNER\n";
        inner_next = next;
    }

}

cmd *loop_cmd::get_constr_next() {
    if(inner_next_flag)
        return after_loop_next;
    else
        return inner_next;
}


std::vector<std::pair<char, size_t>> bfmachine::s_to_ps(std::string str)
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
    head = 0;
    cells.clear();
    cells.push_back(0);
    delete first_cmd;
    first_cmd = new cmd(1);
    cmd * current_cmd = first_cmd;
    std::stack<loop_cmd *> stack;
    auto ps = s_to_ps(str);

    for (auto p : ps)
    {
        switch (p.first)
        {
            case MINUS: {
                if(current_cmd)
                    std::cerr<<(char)current_cmd->get_cmd_type();
                current_cmd->set_next(new decrement_cmd(p.second));
                current_cmd = current_cmd->get_constr_next();
                if(current_cmd)
                    std::cerr<<(char)current_cmd->get_cmd_type()<<"\n";
                break;
            }
            case PLUS: {
                if(current_cmd)
                    std::cerr<<(char)current_cmd->get_cmd_type();
                current_cmd->set_next(new increment_cmd(p.second));
                current_cmd = current_cmd->get_constr_next();
                if(current_cmd)
                    std::cerr<<(char)current_cmd->get_cmd_type()<<"\n";
                break;
            }
            case LEFT: {
                if(current_cmd)
                    std::cerr<<(char)current_cmd->get_cmd_type();
                current_cmd->set_next(new move_left_cmd(p.second));
                current_cmd = current_cmd->get_constr_next();
                if(current_cmd)
                    std::cerr<<(char)current_cmd->get_cmd_type()<<"\n";
                break;
            }
            case RIGHT: {
                if(current_cmd)
                    std::cerr<<(char)current_cmd->get_cmd_type();
                current_cmd->set_next(new move_right_cmd(p.second));
                current_cmd = current_cmd->get_constr_next();
                if(current_cmd)
                    std::cerr<<(char)current_cmd->get_cmd_type()<<"\n";
                break;
            }
            case LEFT_BRACKET: {
                if(current_cmd)
                    std::cerr<<(char)current_cmd->get_cmd_type();
                auto lp_cmd = new loop_cmd(1);
                stack.push(lp_cmd);
                current_cmd->set_next(lp_cmd);
                current_cmd = current_cmd->get_constr_next();
                if(current_cmd)
                    std::cerr<<(char)current_cmd->get_cmd_type()<<"\n";
                break;
            }
            case RIGHT_BRACKET: {
                if(current_cmd)
                    std::cerr<<(char)current_cmd->get_cmd_type();

                current_cmd->set_next(stack.top());

                current_cmd = stack.top();

                stack.pop();
                if(current_cmd)
                    std::cerr<<(char)current_cmd->get_cmd_type()<<"\n";
                break;

            }
            case POINT: {
                if(current_cmd)
                    std::cerr<<(char)current_cmd->get_cmd_type()<<"\n";
                current_cmd->set_next(new out_cmd(p.second));

                current_cmd = current_cmd->get_constr_next();
                if(current_cmd)
                    std::cerr<<(char)current_cmd->get_cmd_type()<<"\n";
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
    delete first_cmd;

}

void bfmachine::run()
{
    if(used)
        throw std::runtime_error("You should reinitialize bfmachine before using one more time\n");
    auto ptr = first_cmd;
    while(ptr)
    {
        runtime_dispatch(ptr->get_cmd_type(),ptr->execute(cells[head]));
        ptr = ptr->get_next();
    }
}

template<command_types c>
void bfmachine::change_state(int param)
{
    if constexpr (c==CHANGE_CELL)
    {
        cells[head] += param;
    }
    if constexpr (c==MOVE_HEAD) {
        if (head + param > BUF_SIZE || head + param < 0) {
            throw std::out_of_range("Head pointer out of cells range\n");
        } else if ((head += param) > cells.size()) {
            while (cells.size() < head+1)
                cells.push_back(0);
        }
    }
    if constexpr (c==OUT)
    {
        for (auto i = 0; i < param; ++i)
            std::cout << cells[head];
    }
    if constexpr (c==LOOP)
    {
    }
}

void bfmachine::runtime_dispatch(command_types c, int param) {
    if ( c == CHANGE_CELL ) { change_state<CHANGE_CELL>(param); }
    if ( c == MOVE_HEAD ) { change_state<MOVE_HEAD>(param); }
    if ( c == LOOP ) { change_state<LOOP>(param); }
    if ( c == OUT ) { change_state<OUT>(param); }
}





