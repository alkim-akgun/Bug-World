#include "Instruction.hpp"

#include <iostream>
#include "World.hpp"
#include "Bug.hpp"
#include "utility.hpp"

// call this before every execute
bool Instruction::pre_execute_check(Bug & b) // return whether execution should stop
{
    if (b.is_dead()) return true;
    if (!b.rested())
    {
        b.rest();
        return true;
    }
    return false;
}

// check if the end of line is reached
bool Instruction::EOL_check(std::istringstream & liss)
{
    std::string temp;
    if (liss >> temp)
    {
        std::string msg = "expected end of line, received '" + temp + "'";
        throw std::runtime_error(msg);
        return false;
    }
    return true;
}


// sense sensedir success_state fail_state condition
void I_sense::parse(std::string line)
{
    std::istringstream liss(line);
    std::string dummy; // skip 'sense'
    liss >> dummy >> this->sensedir >> this->success
         >> this->fail >> this->cond;
    Instruction::EOL_check(liss);
}

void I_sense::execute(Bug & b, World & this_world)
{
    if (Instruction::pre_execute_check(b)) return;
    bool success = true;
    // success is always over-written
    // if success is set false, then 'goto update' is executed

    tposition target_pos = sensed_position(b.get_position(),
                                     b.get_direction(), this->sensedir);
    if (!this_world.is_valid_position(target_pos))
    {
        success = false;
        goto update;
    }
    else
    {
        Cell & target_cell = this_world.get_cell(target_pos);
        switch (this->cond)
        {
            // occupant related
            case tcondition::FRIEND:
            case tcondition::FOE:
            case tcondition::FRIENDWITHFOOD:
            case tcondition::FOOWITHFOOD:
            {
                Bug* target_b_ptr = target_cell.get_occupant_ptr();
                if (target_b_ptr == nullptr)
                    success = false;
                else
                {
                    switch (this->cond)
                    {
                        case tcondition::FRIENDWITHFOOD:
                            // check food
                            success = target_b_ptr->get_food();
                            if (!success)
                                goto update;
                            // else dont break, also check friend
                            [[fallthrough]];
                        case tcondition::FRIEND:
                            // check friend
                            success = are_friends(b, *target_b_ptr);
                            break;
                        case tcondition::FOOWITHFOOD:
                            // check food
                            success = target_b_ptr->get_food();
                            if (!success)
                                goto update;
                            // else dont break, also check foe
                            [[fallthrough]];
                        case tcondition::FOE:
                            // check foe
                            success = !(are_friends(b, *target_b_ptr));
                            break;
                        default:
                            unexpected_error("instruction.cpp: I_sense::execute");
                            break;
                    }
                }
                break;
            }
            // cell related
            case tcondition::FOOD:
                success = (target_cell.get_food() > 0);
                break;
            case tcondition::ROCK:
                success = target_cell.get_obstructed();
                break;
            case tcondition::FOEHOME:
                success = target_cell.is_home_to_color(other_color(b.get_color()));
                break;
            case tcondition::HOME:
                success = target_cell.is_home_to_color(b.get_color());
                break;
            case tcondition::FOEMARKER:
                success = target_cell.marks.check_other_marker(b.get_color());
                break;
            default: // MARKERX
                // first 6 tconditions correspond to marker values
                tmark m = static_cast<tmark>((int)this->cond);
                success = target_cell.marks.check_marker(m, b.get_color());
                break;
        }
    }
    
    update:
    std::string msg = "Bug" + std::to_string(b.get_id())
                    + " at " + std::to_string(b.get_position())
                    + " at state " + std::to_string(b.get_state());
            
    if (success)
    {
        b.set_state(this->success);
        msg += " sensed " + std::to_string(this->cond) + " "
            + std::to_string(this->sensedir) + "\n";
    }
    else
    {
        b.set_state(this->fail);
        msg += " did not sense " + std::to_string(this->cond) + " "
            + std::to_string(this->sensedir) + "\n";
    }
    this_world.log(msg);
    
        
}

int I_flip::prng_state = PRNG_SEED;

// flip p success_state fail_state
void I_flip::parse(std::string line)
{
    std::istringstream liss(line);
    std::string dummy; // skip 'flip'
    liss >> dummy >> this->p >> this->success >> this->fail;
    Instruction::EOL_check(liss);
}

void I_flip::execute(Bug & b, World & this_world)
{
    (void) this_world; // silence unused parameter
    if (Instruction::pre_execute_check(b)) return;
    int rnd = I_flip::randomint(this->p);
    if (rnd == 0)
        b.set_state(this->success);
    else
        b.set_state(this->fail);
}

I_flip::I_flip(void)
{
    static bool initialized = false;
    if (initialized) return; // initialize only once
    initialized = true;
    I_flip::prng_init();
}

void I_flip::prng_init(void) // executed once in constructor
{
    // because x_i is a function of s_{i+4}
    for (int i = 0; i < 3; i++) // do this 3 times
        I_flip::prng_next_state();
    // the 4'th time will be achieved in randomint
}
void I_flip::prng_next_state(void)
{
    // s_{i+1} = (s_i x 22695477  +  1) mod 2^30
    I_flip::prng_state = (I_flip::prng_state*22695477 + 1) % (1U << 30U);
}
int I_flip::randomint(int n)
{
    I_flip::prng_next_state(); // after prng_init, this will be the 4th time
    // x_i = floor(s_{i+4} / 2^16) mod 2^14
    int x = (I_flip::prng_state >> 16U) % (1U << 14U);

    return x % n;
}


// direction dir success_state fail_state
void I_direction::parse(std::string line)
{
    std::istringstream liss(line);
    std::string dummy; // skip 'direction'
    liss >> dummy >> this->dir >> this->success >> this->fail;
    Instruction::EOL_check(liss); 
}

void I_direction::execute(Bug & b, World & this_world)
{
    (void) this_world; // silence unused parameter
    if (Instruction::pre_execute_check(b)) return;

    if (b.get_direction() == this->dir)
        b.set_state(this->success);
    else
        b.set_state(this->fail);
}


// turn dir next_state
void I_turn::parse(std::string line)
{
    std::istringstream liss(line);
    std::string dummy; // skip 'turn'
    liss >> dummy >> this->lr >> this->next;
    Instruction::EOL_check(liss);
}

void I_turn::execute(Bug & b, World & this_world)
{
    if (Instruction::pre_execute_check(b)) return;
    tdirection new_dir = turn(this->lr, b.get_direction());
    b.set_direction(new_dir);
    b.set_state(this->next);
    std::string msg = "Bug" + std::to_string(b.get_id())
                    + " at " + std::to_string(b.get_position())
                    + " at state " + std::to_string(b.get_state())
                    + " turned " + std::to_string(new_dir) + "\n";
    this_world.log(msg);
}


// pickup success_state fail_state
void I_pickup::parse(std::string line)
{
    std::istringstream liss(line);
    std::string dummy; // skip 'pickup'
    liss >> dummy >> this->success >> this->fail;
    Instruction::EOL_check(liss);
}

void I_pickup::execute(Bug & b, World & this_world)
{
    if (Instruction::pre_execute_check(b)) return;
    if (b.get_food()) return; // cannot carry more than 1
    Cell & c = this_world.get_cell(b.get_position());
    int f = c.get_food();
    std::string msg;
    if(f > 0) // if cell has food?
    {
        c.set_food(f-1);
        b.set_food(true);
        b.set_state(this->success);
        msg = "Bug" + std::to_string(b.get_id()) + " at " + std::to_string(b.get_position())
            + " at state " + std::to_string(b.get_state()) + " picked up food\n";
    }
    else
    {
        b.set_state(this->fail);
    }
    this_world.log(msg);        
}


// mark mark_m next_state
void I_mark::parse(std::string line)
{
    std::istringstream liss(line);
    std::string dummy; // skip 'mark'
    liss >> dummy >> this->mark >> this->next; 
    Instruction::EOL_check(liss);
}

void I_mark::execute(Bug & b, World & this_world)
{
    if (Instruction::pre_execute_check(b)) return;
    Cell & c = this_world.get_cell(b.get_position());
    c.marks.set_marker(this->mark, b.get_color());
    b.set_state(this->next);
    std::string msg = "Bug" + std::to_string(b.get_id()) + " at " + std::to_string(b.get_position())
                    + " at state " + std::to_string(b.get_state())
                    + " marked cell with marker " 
                    + std::to_string(this->mark) + "\n";
    this_world.log(msg);
}


// unmark mark_m next_state
void I_unmark::parse(std::string line)
{
    std::istringstream liss(line);
    std::string dummy; // skip 'unmark'
    liss >> dummy >> this->mark >> this->next;
    Instruction::EOL_check(liss);
}

void I_unmark::execute(Bug & b, World & this_world)
{
    if (Instruction::pre_execute_check(b)) return;
    Cell & c = this_world.get_cell(b.get_position());
    c.marks.clear_marker(this->mark, b.get_color());
    b.set_state(this->next);
    std::string msg = "Bug" + std::to_string(b.get_id()) + " at " + std::to_string(b.get_position())
                    + " at state " + std::to_string(b.get_state())
                    + " unmarked cell of marker " 
                    + std::to_string(this->mark) + "\n";
    this_world.log(msg);
}


// move success_state fail_state
void I_move::parse(std::string line)
{
    std::istringstream liss(line);
    std::string dummy; // skip 'move'
    liss >> dummy >> this->success >> this->fail;
    Instruction::EOL_check(liss);
}

void I_move::execute(Bug & b, World & this_world)
{
    if (Instruction::pre_execute_check(b)) return;
    tposition current_pos = b.get_position();
    tposition candidate_pos = adjacent(current_pos, b.get_direction());
    std::string msg = "Bug" + std::to_string(b.get_id()) + " at " + std::to_string(current_pos)
                    + " at state " + std::to_string(b.get_state())  + " ";
    // check validity
    if (!this_world.is_valid_position(candidate_pos)
        || this_world.get_cell(candidate_pos).is_occupied()
        || this_world.get_cell(candidate_pos).get_obstructed())
    {
        b.set_state(this->fail);
        msg += "could not move to " + std::to_string(candidate_pos) + "\n";
        this_world.log(msg);
    }
    else
    {
        // update
        Cell & current_cell = this_world.get_cell(current_pos);
        Cell & candidate_cell = this_world.get_cell(candidate_pos);
        current_cell.set_occupant_ptr(nullptr);
        candidate_cell.set_occupant_ptr(&b);
        b.set_position(candidate_pos);
        msg += "moved to " + std::to_string(candidate_pos) + "\n";
        this_world.log(msg);
        // martial arts
        if (!this_world.kill_if_surrounded(candidate_pos))
        {
            // if not killed
            this_world.check_for_surrounded_bugs(candidate_pos);
            b.start_resting();
            b.set_state(this->success);
        }
    }
}


// drop next_state
void I_drop::parse(std::string line)
{
    std::istringstream liss(line);
    std::string dummy; // skip 'drop'
    liss >> dummy >> this->next;
    Instruction::EOL_check(liss);
}

void I_drop::execute(Bug & b, World & this_world)
{
    if (Instruction::pre_execute_check(b)) return;
    if (!b.get_food()) return; // has no food to drop
    Cell & c = this_world.get_cell(b.get_position());
    int f = c.get_food();
    c.set_food(f+1);
    b.set_food(false);
    b.set_state(this->next);
    std::string msg = "Bug" + std::to_string(b.get_id()) + " at " + std::to_string(b.get_position())
                    + " at state " + std::to_string(b.get_state())
                    + " dropped food\n";
    this_world.log(msg);
}