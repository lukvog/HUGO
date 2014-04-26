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

#include "glushkow.h"
#include "error.h"
#include "str-dict.h"
#include "canonize.h"
#include "collections.h"

struct GNode
{
   char*  label;
   GNode* left;
   GNode* right;
   int    leaf_id;

   GNode(RawTree* expr);
   ~GNode();
   bool leaf();
   bool nullable();
   bool first(int i);
   bool last(int i);
   bool follows(int i, int j);
};

GNode::GNode(RawTree* t)
{
   this->label = t->label;
   if (t->left != NULL) {
      this->left = new GNode(t->left);
   } else {
      this->left = NULL;
   }
   if (t->right != NULL) {
      this->right = new GNode(t->right);
   } else {
      this->right = NULL;
   }
};

GNode::~GNode()
{
   if (this->left != NULL) {
      delete this->left;
   }
   if (this->right != NULL) {
      delete this->right;
   }
}

bool GNode::leaf() 
{
   return left == NULL;
}

bool GNode::nullable()
{
   if (leaf()) {
      return label == EMPTY;
   } else if (label == AST) {
      return true;
   } else if (label == COMMA) {
      return left->nullable() && right->nullable();
   } else if (label == PLUS) {
      return left->nullable() || right->nullable();
   } else {
      ERROR("Bad label of an inner node %s",label);
   }
}

bool GNode::first(int i)
{
   if (leaf()) {
      return label != EMPTY && leaf_id == i;
   } else if (label == AST) {
      return left->first(i);
   } else if (label == COMMA) {
      return left->first(i) || (left->nullable() && right->first(i));
   } else if (label == PLUS) {
      return left->first(i) || right->first(i);
   } else {
      ERROR("Bad label of an inner node %s",label);
   }
}

bool GNode::last(int i)
{
   if (leaf()) {
      return label != EMPTY && leaf_id == i;
   } else if (label == AST) {
      return left->last(i);
   } else if (label == COMMA) {
      return right->last(i) || (right->nullable() && left->last(i));
   } else if (label == PLUS) {
      return left->last(i) || right->last(i);
   } else {
      ERROR("Bad label of an inner node %s",label);
   }
}

bool GNode::follows(int i, int j)
{
   if (leaf()) {
      return false;
   } else if (label == AST) {
      return left->follows(i,j) || (left->last(i) && left->first(j));
   } else if (label == COMMA) {
      return left->follows(i,j) || right->follows(i,j) || (left->last(i) && right->first(j));
   } else if (label == PLUS) {
      return left->follows(i,j) || right->follows(i,j);
   } else {
      ERROR("Bad label of an inner node %s",label);
   }
}


void gather_leaves(GNode* node, int& leaf_count, List<GNode*>& leaf_store);

Glushkow::Glushkow(RawTree* expr)
{
   ASSERT(expr != NULL);
   this->expr = new GNode(expr);
   List<GNode*> leaf_store;
   int leaf_count = 0;
   gather_leaves(this->expr,leaf_count,leaf_store);
   this->leaf = leaf_store;
}

Glushkow::~Glushkow()
{
   delete this->expr;
}

void gather_leaves(GNode* node, int& counter, List<GNode*>& store)
{
   if (node == NULL) {
      return;
   }

   char* label = node->label;

   if (node->leaf()) {
      ASSERT(label != PLUS && 
             label != AST && 
             label != COMMA);

      
      if (label != EMPTY) {
         node->leaf_id = counter;
         counter++;
         store.add(node);
      }
   } else {
      //an inner node
      ASSERT(label == PLUS || 
             label == AST ||
             label == COMMA," where label is %s",label);

      gather_leaves((GNode*) node->left, counter, store);
      gather_leaves((GNode*) node->right, counter, store);
   }
}


bool Glushkow::is_deterministic()
{

   for (int i = 0; i < leaf.size; i++) {
      for (int j = i+1; j < leaf.size; j++) {
         if (expr->first(i) && expr->first(j) &&
             leaf[i]->label == leaf[j]->label) {
            return false;
         }
      }
   }

   for (int k = 0; k < leaf.size; k++) {
      for (int i = 0; i < leaf.size; i++) {
         for (int j = i+1; j < leaf.size; j++) {
            if (expr->follows(k,i) && expr->follows(k,j) &&
                leaf[i]->label == leaf[j]->label) {
               return false;
            }
         }
      }
   }
   
   return true;
}

static void clean(bool* array, int length)
{
   for (int i = 0; i < length; i++) {
      array[i] = false;
   }
}

bool Glushkow::match(Vector<char*>& token)
{
   if (token.size == 0) {
      return expr->nullable();
   }


   bool* curr = new bool[leaf.size];
   bool* next = new bool[leaf.size];
   
   clean(curr,leaf.size);
   clean(next,leaf.size);
   
   // First transition
   for (int i = 0; i < leaf.size; i++) {
      curr[i] = expr->first(i) 
         && leaf[i]->label == token[0];
   }

   // Subsequent transitions
   for (int k = 1; k < token.size; k++) {
      for (int i = 0; i < leaf.size; i++) {
         for (int j = 0; j < leaf.size; j++) {
            next[j] = next[j] || 
               (curr[i] 
               && leaf[j]->label == token[k] 
               && expr->follows(i,j));
         }
      }

      bool* tmp = curr;
      curr = next;
      next = tmp;
      clean(next,leaf.size);
   }
   
   bool result = false;
   // Testing final states
   for (int i = 0; i < leaf.size; i++) {
      result = result || curr[i] && expr->last(i);
   }
   
   delete[] curr;
   delete[] next;

   return result;
}



