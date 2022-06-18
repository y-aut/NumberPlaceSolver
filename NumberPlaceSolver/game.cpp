#include "game.h"

string to_string(const Group g)
{
    switch (type_of(g)) {
    case GRT_FILE: return to_string(to_file(g));
    case GRT_RANK: return to_string(to_rank(g));
    default: return to_string(to_block(g));
    }
}
