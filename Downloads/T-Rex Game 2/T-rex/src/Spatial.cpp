#include "Spatial.h"
#include "Utils.h"
#include <algorithm>

// references:
// http://en.wikipedia.org/wiki/Kd-tree
// http://en.wikipedia.org/wiki/Binary_space_partitioning
// http://en.wikipedia.org/wiki/Quadtree

// technically this is actually a Binary Space Partitioning Tree
// it was first made as a kd-Tree but that is just for points, not spaces
// so we made a few small tweaks to the kd-Tree that makes it a
// hybrid between a BSP and a Quadtree 

#define LOW(l)  KD_Z_AXIS ? l->getPosition().z - l->getEdibleScale().z : l->getPosition().x - l->getEdibleScale().x
#define HIGH(l) KD_Z_AXIS ? l->getPosition().z + l->getEdibleScale().z : l->getPosition().x + l->getEdibleScale().x

bool KDNode::entCmpX(Entity *l, Entity *r) {
  return l->getPosition().x < r->getPosition().x;
}

bool KDNode::entCmpZ(Entity *l, Entity *r) {
  return l->getPosition().z < r->getPosition().z;
}

KDNode* KDNode::kdTree(std::vector<Entity*, std::allocator<Entity*> >&entities) {
  return KDNode::kdTree(entities, 0);
}

KDNode* KDNode::kdTree(std::vector<Entity*, std::allocator<Entity*> >&entities, int depth) {
  char axis = depth % 2;
  int mid  = entities.size() / 2;

  if (entities.size() == 0)
    return NULL;
  else if (entities.size() == 1) {
    return new KDNode(NULL, NULL, new std::vector<Entity *>(entities), axis);
  }

  sort(entities.begin(), entities.end(), axis == KD_Z_AXIS ? KDNode::entCmpZ : KDNode::entCmpX);

  std::vector<Entity *> *cons= new std::vector<Entity *>();
  std::vector<Entity *> bot;
  std::vector<Entity *> top;

  double pl = LOW(entities[mid]);
  double ph = HIGH(entities[mid]);

  for (auto e : entities) {
    double l = LOW(e);
    double h = HIGH(e);

    if (l >= pl && h <= ph) {
      cons->push_back(e);
    }
    else {
      if (l <= pl)
        bot.push_back(e);
      if (h >= pl)
        top.push_back(e);
    }
  }

  return new KDNode(KDNode::kdTree(bot, depth+1), KDNode::kdTree(top, depth+1), cons, axis);
}

void KDNode::printTree(KDNode *&node) {
  KDNode::printTree(node, 0);
}

void KDNode::printTree(KDNode *&node, int parent) {
  static int num = 0;

  int _this = num++;
  for (auto e : *node->ents)
    printf("%d/%d: %lf %lf\n", _this, parent, e->getPosition().x, e->getPosition().z);
  if (node->left)
    printTree(node->left, _this);
  if (node->right)
    printTree(node->right, _this);
}