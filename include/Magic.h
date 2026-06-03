#ifndef MAGIC_H
#define MAGIC_H

#include "Bitboard.h"
#include "MaskGen.h"

Bitboard rookAttacksForBlockers(Bitboard blockers, int sq);
Bitboard bishopAttacksForBlockers(Bitboard blockers, int sq);
#endif
