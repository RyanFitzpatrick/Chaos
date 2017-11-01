#include "Num.h"

ch_num CH_NumPlus2(ch_num augend, ch_num addend)
{
    return augend + addend;
}

void CH_NumPlus(ch_num * num, ch_num summand)
{
    (*num) += summand;
}
