#include "MeshLoader.h"

bool weightCompare(BoneIndex a, BoneIndex b) {
   return a.weight > b.weight;
}

void MrModel::writeModFile(const std::string & path, uint maxVerts, uint meshCount, uint flags) {
   bool reduce = flags & REDUCE;
   bool resize = flags & RESIZE;
   bool center = flags & CENTER;

   Assimp::Importer importer;
   const aiScene* scene = importer.ReadFile(path,
      aiProcess_CalcTangentSpace       |
      aiProcess_Triangulate            |
      aiProcess_JoinIdenticalVertices  |
      aiProcess_SortByPType);
   if (!scene) {
      std::cerr << "File: " << path << " doesn't exist. Quitting...\n";
      exit(EXIT_FAILURE);
   }

   std::string outPath = path.substr(0, path.find(".") + 1) + "mod";
   std::ofstream outFile;
   outFile.open(outPath);
   std::cerr << "Writing mod file to: " << outPath << std::endl;

   MrMesh mrMesh;
   aiMesh& aimesh = *scene->mMeshes[0];
   std::vector<glm::mat4> offMats; // array of bone matrix transforms
   std::vector<std::string> boneNames; // name of bone to bone index

   std::cerr << "Reading in Vertices, Normals, Texture Coordinates, Tangents, and Bitangents\n";
   // Todo, check if has normals, and if has positions?
   for (uint i = 0; i < aimesh.mNumVertices; i++) {
      glm::vec3 pos = glm::vec3(aimesh.mVertices[i][0], aimesh.mVertices[i][1], aimesh.mVertices[i][2]);
      glm::vec3 norm = glm::vec3(aimesh.mNormals[i][0], aimesh.mNormals[i][1], aimesh.mNormals[i][2]);
      glm::vec2 uv = glm::vec2(0.0f);
      if (aimesh.HasTextureCoords(0))
         uv = glm::vec2(aimesh.mTextureCoords[0][i].x, aimesh.mTextureCoords[0][i].y);
      glm::vec3 tangent = glm::vec3(aimesh.mTangents[i][0], aimesh.mTangents[i][1], aimesh.mTangents[i][2]);
      glm::vec3 bitangent = glm::vec3(aimesh.mBitangents[i][0], aimesh.mBitangents[i][1], aimesh.mBitangents[i][2]);
      mrMesh.verts.push_back(new Vert(i, pos, norm, uv, tangent, bitangent));
   }

   if (aimesh.HasFaces()) {
      std::cerr << "Reading in Faces\n";
      for (uint i = 0; i < aimesh.mNumFaces; i++) {
         Vert * vert0 = mrMesh.verts[aimesh.mFaces[i].mIndices[0]];
         Vert * vert1 = mrMesh.verts[aimesh.mFaces[i].mIndices[1]];
         Vert * vert2 = mrMesh.verts[aimesh.mFaces[i].mIndices[2]];

         // These are sometimes the same... What the heck... maya is crazy?
         if (vert0 != vert1 && vert1 != vert2 && vert2 != vert0) {
            Face * face = new Face(vert0, vert1, vert2);
            face->computeNormal();

            vert0->faces.push_back(face);
            vert1->faces.push_back(face);
            vert2->faces.push_back(face);
            if (!vert0->hasNeighbor(vert1)) vert0->neighbors.push_back(vert1);
            if (!vert0->hasNeighbor(vert2)) vert0->neighbors.push_back(vert2);
            if (!vert1->hasNeighbor(vert0)) vert1->neighbors.push_back(vert0);
            if (!vert1->hasNeighbor(vert2)) vert1->neighbors.push_back(vert2);
            if (!vert2->hasNeighbor(vert0)) vert2->neighbors.push_back(vert0);
            if (!vert2->hasNeighbor(vert1)) vert2->neighbors.push_back(vert1);

            mrMesh.faces.push_back(face);
         }
      }
   }
   else
      std::cerr << "Has no faces\n";

   if (aimesh.HasBones()) {
      std::cerr << "Reading in Bones\n";
      for (uint i = 0; i < aimesh.mNumBones; i++) {
         // fill in a boneName entry
         boneNames.push_back(aimesh.mBones[i]->mName.C_Str());

         // fill in an offMat entry
         aiMatrix4x4 mat = aimesh.mBones[i]->mOffsetMatrix;
         offMats.push_back(aiToGlmMat4(mat));

         // add BoneIndices to bIndices of each referenced vertex
         for (uint j = 0; j < aimesh.mBones[i]->mNumWeights; j++) {
            uint vNdx = aimesh.mBones[i]->mWeights[j].mVertexId;
            float bWeight = aimesh.mBones[i]->mWeights[j].mWeight;
            BoneIndex bi;
            bi.id = i;
            bi.weight = bWeight;
            mrMesh.verts[vNdx]->bIndices.push_back(bi);
         }
      }
      

      // sort the bone indices by their weights (in decending order)
      for (uint i = 0; i < aimesh.mNumVertices; i++) {
         std::sort (mrMesh.verts[i]->bIndices.begin(),
                    mrMesh.verts[i]->bIndices.end(),
                    weightCompare);
               //std::cerr << "!!!!!!!!! numverts = " << mesh.mNumVertices << ", numweights = " << mrMesh.verts[i]->bIndices.size() << std::endl;
      }
   }
   else 
      std::cerr << "Has no bones\n";

   // Fill in edge collapsing costs and candidate vertices for all vertices
   std::cerr << "Filling in Edge Costs\n";
   for (uint i = 0; i < mrMesh.verts.size(); i++)
      mrMesh.verts[i]->computeEdgeCost();

   float minX = 1000, minY = 1000, minZ = 1000, maxX = -1000, maxY = -1000, maxZ = -1000;
   for (uint i = 0; i < mrMesh.verts.size(); i++) {
      glm::vec3 pos = mrMesh.verts[i]->position;
      minX = std::min(pos.x, minX);
      minY = std::min(pos.y, minY);
      minZ = std::min(pos.z, minZ);
      maxX = std::max(pos.x, maxX);
      maxY = std::max(pos.y, maxY);
      maxZ = std::max(pos.z, maxZ);
   }

   glm::vec3 modCent = glm::vec3((minX + maxX)/2, (minY + maxY)/2,(minZ + maxZ)/2);
   if (center) {
      std::cerr << "CENTER flag is on. Centering the model on (0,0,0).\n";
      for (uint i = 0; i < mrMesh.verts.size(); i++)
         mrMesh.verts[i]->position -= modCent;
      
      modCent = glm::vec3(0);
   }

   glm::vec3 modLen = glm::vec3(maxX - minX, maxY - minY, maxZ - minY);
   if (resize) {
      std::cerr << "RESIZE flag is on. Moving to standard {-1,1} space\n";
      float maxDiff = std::max(maxX - minX, std::max(maxY - minY, maxZ - minZ));
      for (uint i = 0; i < mrMesh.verts.size(); i++)
         mrMesh.verts[i]->position = mrMesh.verts[i]->position / glm::vec3(maxDiff);
      
      modLen /= glm::vec3(maxDiff);
   }

   maxVerts = std::min((int)maxVerts, (int)(mrMesh.verts.size()));
   if (reduce)
      std::cerr << "REDUCE flag is on. Creating multiple meshes.\n";
   else {
      std::cerr << "REDUCE flag is off. Only creating the base mesh.\n";
      maxVerts = mrMesh.verts.size();
      meshCount = 1;
   }

   // Write meshCount and maxVerts to file
   if (meshCount == 0)
      meshCount = 1;
   outFile << "meshcount " << meshCount << std::endl;
   outFile << "maxverts " << maxVerts << std::endl;
   outFile << "center " << modCent.x << " " << modCent.y << " " << modCent.z << std::endl;
   outFile << "dimensions " << modLen.x << " " << modLen.y << " " << modLen.z << std::endl;

   // Write bone offsets to file
   for (uint i = 0; i < offMats.size(); i++) {
      glm::mat4 m = offMats[i];
      outFile << "bm " << m[0][0] << " " << m[1][0] << " " << m[2][0] << " " << m[3][0] << " "
                    << m[0][1] << " " << m[1][1] << " " << m[2][1] << " " << m[3][1] << " "
                    << m[0][2] << " " << m[1][2] << " " << m[2][2] << " " << m[3][2] << " "
                    << m[0][3] << " " << m[1][3] << " " << m[2][3] << " " << m[3][3] << std::endl;
   }
   for (uint i = 0; i < boneNames.size(); i++)
      outFile << "bn " << i << " " << boneNames[i] << std::endl;
   outFile << std::endl;

   std::cerr << "Creating BASE mesh with " << maxVerts << " vertices." << std::endl;

   // Write the base mesh to file
   while (mrMesh.verts.size() > maxVerts) {
      Vert * least = mrMesh.findLowestCostVertex();
      mrMesh.collapseEdge(least, least->collapse);
   }
   mrMesh.writeToFile(outFile, 0, mrMesh.verts.size(), mrMesh.faces.size());

   if (reduce) {
      // initially Collapse/merge all vertices that lie on top of each other...
      for (int i = mrMesh.verts.size()-1; i >= 0; i--)
         for (int j = i - 1; j >= 0; j--)
            if (glm::length(mrMesh.verts[i]->position == mrMesh.verts[j]->position)) {
               mrMesh.collapseEdge(mrMesh.verts[i], mrMesh.verts[j]);
               break;
            }
   }

   // Write the reduced meshes to file
   for (uint i = 1; i < meshCount; i++) { // don't redo the mrMesh
      uint desired = (int)(1.0f * (meshCount - i) / (meshCount) * maxVerts);
      std::cerr << "Creating reduced mesh with " << desired << " vertices." << std::endl;
      
      while (mrMesh.verts.size() > desired) {
         Vert * least = mrMesh.findLowestCostVertex();
         mrMesh.collapseEdge(least, least->collapse);
      }

      mrMesh.writeToFile(outFile, i, mrMesh.verts.size(), mrMesh.faces.size());
   }

   outFile.close();
}

void MrMesh::writeToFile(std::ofstream & file, uint meshNum,
 uint vertCount, uint faceCount) {
   file << "mesh " << meshNum << std::endl;
   file << "vertex count " << vertCount << std::endl;
   file << "face count " << faceCount << std::endl;
   for (uint i = 0; i < verts.size(); i++)
      file << "v " << verts[i]->position.x << " " << verts[i]->position.y << " " << verts[i]->position.z << std::endl;
   for (uint i = 0; i < verts.size(); i++)
      file << "n " << verts[i]->normal.x << " " << verts[i]->normal.y << " " << verts[i]->normal.z << std::endl;
   for (uint i = 0; i < verts.size(); i++)
      file << "uv " << verts[i]->uv.x << " " << verts[i]->uv.y << std::endl;
   for (uint i = 0; i < verts.size(); i++)
      file << "ta " << verts[i]->tangent.x << " " << verts[i]->tangent.y << " " << verts[i]->tangent.z << std::endl;
   for (uint i = 0; i < verts.size(); i++)
      file << "bt " << verts[i]->bitangent.x << " " << verts[i]->bitangent.y << " " << verts[i]->bitangent.z << std::endl;
   for (uint i = 0; i < faces.size(); i++)
      file << "f " << faces[i]->vertices[0]->id << " " << faces[i]->vertices[1]->id << " " << faces[i]->vertices[2]->id << std::endl;
   //for (uint i = 0; i < )
   for (uint i = 0; i < verts.size(); i++) {
      file << "bi";
      for (uint j = 0; j < MAX_WEIGHTS; j++) {
         if (j >= verts[i]->bIndices.size())
            file << " 0 0";
         else
            file << " " << verts[i]->bIndices[j].id << " " << verts[i]->bIndices[j].weight;
      }
      file << std::endl;
   }

   file << "end\n";
}


Vert::Vert(uint ndx, glm::vec3 p, glm::vec3 n, glm::vec2 uvCoord, glm::vec3 tang, glm::vec3 bitang) {
   id = ndx;
   position = p;
   normal = n;
   uv = uvCoord;
   tangent = tang;
   bitangent = bitang;
}

Vert::~Vert() {}

Face::Face(Vert * v0, Vert * v1, Vert * v2) {
   vertices[0] = v0;
   vertices[1] = v1;
   vertices[2] = v2;
}

Face::~Face() {}

void Face::computeNormal() {
   //printf("begin\n");
   normal = glm::vec3(0.0f);
   for (int i = 0; i < 3; i++)
      normal += vertices[i]->normal;
   if (glm::length(normal) > 0)
      normal = glm::normalize(normal);
   //printf("end\n");
}

bool Vert::hasNeighbor(Vert * v) {
   for (uint i = 0; i < neighbors.size(); i++)
      if (v == neighbors[i])
         return true;
   return false;
}

bool Face::hasVertex(Vert * v) {
   for (uint i = 0; i < 3; i++)
      if (v == vertices[i]) 
         return true;
   return false;
}

float computeEdgeCollapseCost(Vert * u, Vert * v) {
   // if we collapse edge uv by moving u to v then how 
   // much different will the model change, i.e. the "error".
   float edgelength = glm::distance(v->position, u->position);
   float curvature = 0;

   // find the "sides" triangles that are on the edge uv
   std::vector<Face *> sides;
   for (uint i = 0; i < u->faces.size(); i++) {
      if (u->faces[i]->hasVertex(v)) {
         sides.push_back(u->faces[i]);
      }
   }

   // use the triangle facing most away from the sides 
   // to determine our curvature term
   for (uint i = 0; i < u->faces.size(); i++) {
      float mincurv = 1; 
      for (uint j = 0; j < sides.size(); j++) {
         // use dot product of face normals.
         float dotprod = glm::dot(u->faces[i]->normal, sides[j]->normal);
         mincurv = std::min(mincurv, (1 - dotprod) / 2.0f);
      }
      curvature = std::max(curvature, mincurv);
   }
   //printf("Curvature = %f\n", curvature);
   return edgelength * curvature;
}

void Vert::computeEdgeCost() {
   if (neighbors.size() == 0) {
      collapse = NULL;
      cost = -0.01f;
      return;
   }
   cost = 1000000;
   collapse = NULL;
   // search all neighboring edges for "least cost" edge
   for (uint i = 0; i < neighbors.size(); i++) {
      float c;
      c = computeEdgeCollapseCost(this, neighbors[i]);
      if (c < cost) {
         collapse = neighbors[i];
         cost = c;
      }
      if (this == neighbors[i])
         printf("same as neighbor %d\n", this->id);
   }
}

Vert * MrMesh::findLowestCostVertex() {
   Vert * leastCostVert = verts[0];
   for (uint i = 1; i < verts.size(); i++)
      if (verts[i]->cost < leastCostVert->cost)
         leastCostVert = verts[i];
   return leastCostVert;
}

void MrMesh::removeVertex(Vert * vert) {
   // remove self from neighboring neighbors lists
   for (uint i = 0; i < vert->neighbors.size(); i++) {
      Vert * neigh = vert->neighbors[i];
      for (uint j = 0; j < neigh->neighbors.size(); j++)
         if (neigh->neighbors[j] == vert) {
            neigh->neighbors.erase(neigh->neighbors.begin() + j);
            break;
         }
   }

   // remove self from mesh's vertices list   
   int ndx = -1;
   while (verts[++ndx] != vert);
   verts.erase(verts.begin() + ndx);

   // update all id's after the one removed.
   while (ndx < (int)(verts.size())) {
      verts[ndx]->id = verts[ndx]->id - 1;
      ndx++;
   }

   delete vert;
}

void MrMesh::removeFace(Face * face) {
   // Remove all surrounding vertices references to face
   for (uint i = 0; i < 3; i++)
      for (uint j = 0; j < face->vertices[i]->faces.size(); j++)
         if (face->vertices[i]->faces[j] == face) {
            face->vertices[i]->faces.erase(face->vertices[i]->faces.begin() + j);
            break;
         }

   for (uint i = 0; i < faces.size(); i++)
      if (faces[i] == face) {
         faces.erase(faces.begin() + i);
         break;
      }

   delete face;
}

// Collapse the edge uv by moving vertex u onto v
void MrMesh::collapseEdge(Vert * u, Vert * v) {
   if (!v) {  // u is a vertex all by itself so just delete it
      removeVertex(u);
      return;
   }

   // delete triangles on edge uv:
   for (int i = u->faces.size() - 1; i >= 0; i--)
      if (u->faces[i]->hasVertex(v))
         removeFace(u->faces[i]);

   for (uint i = 0; i < u->faces.size(); i++) {
      // replace u with v for each face connected to u
      for (uint j = 0; j < 3; j++)
         if (u->faces[i]->vertices[j] == u) {
            u->faces[i]->vertices[j] = v;
            break;
         }
      // add each face of u to v's faces list
      v->faces.push_back(u->faces[i]);
   }

   for (uint i = 0; i < u->neighbors.size(); i++) {
      Vert * neigh = u->neighbors[i];
      // update v to contain the neighbors of u
      if (!v->hasNeighbor(neigh) && neigh != v)
         v->neighbors.push_back(neigh);
      // update the neighbors of u to contain v
      if (!neigh->hasNeighbor(v) && neigh != v)
         neigh->neighbors.push_back(v);
   }

   // make uNeighs a temporary list of all the neighbors of u
   std::vector<Vert *> uNeighs;
   for (uint i = 0; i < u->neighbors.size(); i++)
      uNeighs.push_back(u->neighbors[i]);

   removeVertex(u); // remove u and all references to it

   // recompute the edge collapse costs in neighborhood
   for (uint i = 0; i < uNeighs.size(); i++)
      uNeighs[i]->computeEdgeCost();

   // recompute the normal for v by averaging surrounding face normals
   v->normal = glm::vec3(0.0f);
   for (uint i = 0; i < v->faces.size(); i++)
      v->normal += v->faces[i]->normal;
   v->normal = glm::normalize(v->normal);
}