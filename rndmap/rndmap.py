import sys
from math import floor, ceil
import random as rnd

default_width = 60    # preferably two times of default_height
default_height = 30

default_bug_ratio = 0.03                # default ratio of bugs to map size
default_obstacle_ratio = 0.1            # default ratio of obstructed cells to map size
default_food_bug_ratio = 10.0           # default ratio of total food to total bugs
default_average_food = 5                # default average food among cells with food

verbose = 2    # 0 disables printing logs, 1 prints important logs, 2 additionally prints food logs
log = False

log_str = ""


def write_log(str, food=False, force=False):
    global log_str
    if log:
        log_str += str + '\n'
    global verbose
    if force or verbose == 2 or (verbose == 1 and not food):
        print(str, file=sys.stderr)


def generate_map(width, height, bug=None, food=None, seed=None):
    size = width*height
    if bug is None:
        bug = round(size*default_bug_ratio)

    if food is None:
        food = round(default_food_bug_ratio * (2 * bug))

    if seed is None:
        seed = rnd.randint(1000, 9999)
    rnd.seed(seed)

    obstacle = round(size*default_obstacle_ratio)
    food_spread = ceil(food / default_average_food)

    if food_spread*9 < food:
        print("Error: Not enough food spread ({}), must be at least {}"
              .format(food_spread, ceil(food/9)), file=sys.stderr)
        return None

    available = (width-2)*(height-2)            # number of available cells
    occupied = 2*bug + obstacle + food_spread   # number of occupied cells
    if available < occupied:
        print("Error: Not enough available space={} in the map for food_spread={}"
              .format(available, food_spread), file=sys.stderr)
        return None

    write_log("Generating map with width={}, height={}, food={}, \
food_spread={}, bug={}, obstacle={}, seed={}"
             .format(width, height, food, food_spread, bug, obstacle, seed))

    #     dimension header                          top wall
    map = str(width) + '\n' + str(height) + '\n' + '#' * width + '\n'

    bugred = bugblack = bug
    bug += bug    # total number of bugs
    for i in range(1, height-1):
        line = "#"                                   # left wall
        for j in range(1, width-1):
            # decision process
            r = rnd.random()
            if r < bug/available:                             # add a bug
                color = None
                if rnd.random() < bugred/bug:                           # a red bug
                    color = "red"
                    line += '+'
                    bugred -= 1
                else:                                               # a black bug
                    color = "black"
                    line += '-'
                    bugblack -= 1
                write_log("A {} bug placed at ({}, {})".format(color, i, j))
                bug -= 1

            elif r < (bug+food_spread)/available:  # add food
                mean = food / food_spread
                # food/food_spread <= 9 necessary to not be left with remaining food
                # (food-add_food)/(food_spread-1) <= 9 necessary to have a valid next step
                min_food = food - 9*(food_spread-1)

                if min_food > 9:    # this should not be possible when the previous step leaves a valid step
                    print("Error: min_food={:.2f} is bigger than 9 (at ({}, {}))"
                          .format(min_food, i, j), file=sys.stderr)
                    return None

                deviation = None
                if mean < min_food:
                    write_log("mean={} is below min_food={:.2f}".format(mean, min_food), food=True, force=True)
                    deviation = min(min_food, food-min_food)    # a large deviation to relax following states
                    mean = ceil(min_food + deviation)    # at least min_food, at most food will be assigned
                    write_log("Recovering: new mean={}, new deviation={}".format(mean, deviation), food=True, force=True)

                else:
                    deviation = min(mean - 1, food - mean)    # never assign food less than one or more than available
                    deviation = min(deviation, mean-min_food)    # never assign food less than min_food

                add_food = rnd.randint(ceil(mean-deviation), round(mean+deviation))    # do not let below min_food
                if add_food > 9:    # cannot assign more than 9 food
                    add_food = 9
                line += str(add_food)
                write_log("{} food placed at ({}, {}) with food={}, mean={:.2f}, deviation={:.2f}, min_food={}"
                         .format(add_food, i, j, food, mean, deviation, min_food), food=True)
                food_spread -= 1
                food -= add_food

            elif r < (bug+food_spread+obstacle)/available:    # add an obstacle
                line += "#"
                obstacle -= 1

            else:                                               # add empty space
                line += "."

            available -= 1

        line += "#"                                  # right wall
        map += line + '\n'
    map += '#' * width

    write_log("Final values food={}, food_spread={}, bug={}, obstacle={}"
             .format(food, food_spread, bug, obstacle))

    return map


if __name__ == "__main__":
    width = default_width
    height = default_height
    bug = None
    food = None
    seed = None

    if len(sys.argv) > 1:
        width = int(sys.argv[1])
    if len(sys.argv) > 2:
        height = int(sys.argv[2])
    if len(sys.argv) > 3:
        bug = int(sys.argv[3])
    if len(sys.argv) > 4:
        food = int(sys.argv[4])
    if len(sys.argv) > 5:
        seed = int(sys.argv[5])

    count = 10
    m = generate_map(width, height, bug, food, seed)
    while m is None and count > 1:    # in case of an error
        count -= 1
        print("Error detected. Trying again ({})".format(count))
        m = generate_map(width, height, bug, food, seed)
    if m is None:
        raise Exception("Failed to generate a valid map")
    print(m)
