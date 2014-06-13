#ifndef SPATIAL__H
#define SPATIAL__H

#include "Game.h"
#include <vector>

#define KD_Z_AXIS 1
#define KD_X_AXIS 0

class KDNode {
private:
  KDNode *left;
  KDNode *right;
  std::vector<Entity *>* ents;
  char axis;

  KDNode(KDNode *l, KDNode *r, std::vector<Entity*>* e, char a) 
    : left(l), right(r), ents(e), axis(a) {}

  static bool entCmpX(Entity *l, Entity *r);
  static bool entCmpZ(Entity *l, Entity *r);
  static void printTree(KDNode *&node, int parent);
  static KDNode* kdTree(std::vector<Entity*, std::allocator<Entity*> >&entities, int depth);

public:
  std::vector<Entity *>* getEnts() { return ents; }
  KDNode *getLeft() { return left; }
  KDNode *getRight() { return right; }
  char    getAxis()  { return axis;  }

  static KDNode* kdTree(std::vector<Entity*, std::allocator<Entity*> >&entities);
  static void printTree(KDNode *&node);
  ~KDNode() { if (left) delete left; if (right) delete right; ents->clear(); delete ents; }
};

#endif
