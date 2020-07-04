#pragma once
#include "src/controller/Top.h"

unsigned short* Play_Winners(
    const card_list yours,           // your cards
    const card_list hist_cards[],    // the history of cards played
    const unsigned short hist_size,  // the size of Last_cards
    unsigned short& size);

unsigned short* Play_Hearts(
    const card_list Tyours,         // your cards
    const card_list Thist_cards[],  // the history of cards played
    const unsigned short hist_size  // the size of Last_cards
);

unsigned short* Exchange_Hearts(const card_list yours);

