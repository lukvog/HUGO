#pragma once
//
// This file is part of Chipmunk, a free, experimental library for
// efficient testing of determinism and membership for regular
// expressions.
//
// Chipmunk is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation, version 3 of the License, or any later
// version.
//
// Chipmunk is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License
// along with Chipmunk. If not, see <http://www.gnu.org/licenses/>.
//
// Authors: Sławek Staworko <staworko@gmail.com>
//          Benoît Groz <benoit.groz@yahoo.fr>
//          Sebastian Maneth <sebastian.maneth@gmail.com>
// Date: 20 June 2011.
//
#ifndef RE_GLUSHKOW_H_
#define RE_GLUSHKOW_H_

#include "raw-tree.h"
#include "collections.h"

struct GNode;
struct Glushkow
{
   GNode*  expr;        // regular expression 
   Vector<GNode*> leaf; // positions in the regular expression (non-epsilon leaves)

   Glushkow(RawTree* expr);
   ~Glushkow();

   bool is_deterministic();
   bool match(Vector<char*>& token);
};


#endif // RE_GLUSHKOW_H_
