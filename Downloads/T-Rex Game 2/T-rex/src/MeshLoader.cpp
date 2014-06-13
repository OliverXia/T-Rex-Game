#include "MeshLoader.h"

Model::Model(const std::string & path, uint meshCount, uint maxVerts, uint flags) {
   bool remake = REMAKE & flags;
   bool reduce = REDUCE & flags;

   std::ifstream file;
   std::string pathNoExt = path.substr(0, path.find(".") + 1);
   std::string pathMOD = pathNoExt + "mod";
   std::string pathDAE = pathNoExt + "dae";
   std::string readString;
   std::string label;
   std::stringstream stream;

   if (meshCount == 0 || !reduce)
      meshCount = 1;
   
   std::cerr << "********* READING MODEL '" << pathDAE << "' *********\n";

   file.open(pathMOD);

   if (remake) {
      std::cerr << "REMAKE flag is on. Generating mod file\n";
      MrModel::writeModFile(pathDAE, maxVerts, meshCount, flags);
   }
   else if (!file.is_open()) {
      std::cerr << "Could not find " << pathMOD << ". Generating it\n";
      MrModel::writeModFile(pathDAE, maxVerts, meshCount, flags);
   }

   if (!file.is_open())
      file.open(pathMOD);

     //////////////////////////////////////
    ////// LOAD THE MODEL MESHES IN //////
   //////////////////////////////////////

   rotAdjust = MAT4_ID;
   Mesh mesh;
   numMeshes = 0;
   uint faceCount;
   std::vector<float> buffVerts, buffNorms, buffUVs, buffBitangents, buffTangents, boneWeights;
   std::vector<uint> buffIndxs, boneIndices;

   std::cerr << "Loading mod file: " << pathMOD << std::endl;

   std::getline(file, readString); // skip meshcount
   std::getline(file, readString); // skip maxverts

   std::string stroodle;
   float fun;
   std::getline(file, readString);
   std::stringstream stream1 (readString);
   stream1 >> stroodle; // read in the coordinates for the center
   stream1 >> fun; center.x = fun;
   stream1 >> fun; center.y = fun;
   stream1 >> fun; center.z = fun;

   std::getline(file, readString);
   std::stringstream stream2 (readString);
   stream2 >> stroodle; // read in the dimensions of the model
   stream2 >> fun; dimensions.x = fun;
   stream2 >> fun; dimensions.y = fun;
   stream2 >> fun; dimensions.z = fun;

   while (file) {
      std::getline(file, readString);

      std::stringstream stream (readString);
      std::string label;
      std::string name;
      uint inum;
      stream >> label;
      float fnum;

      if ("bm" == label) {
         glm::mat4 mat;
         stream >> fnum; mat[0][0] = fnum;
         stream >> fnum; mat[1][0] = fnum;
         stream >> fnum; mat[2][0] = fnum;
         stream >> fnum; mat[3][0] = fnum;
         stream >> fnum; mat[0][1] = fnum;
         stream >> fnum; mat[1][1] = fnum;
         stream >> fnum; mat[2][1] = fnum;
         stream >> fnum; mat[3][1] = fnum;
         stream >> fnum; mat[0][2] = fnum;
         stream >> fnum; mat[1][2] = fnum;
         stream >> fnum; mat[2][2] = fnum;
         stream >> fnum; mat[3][2] = fnum;
         stream >> fnum; mat[0][3] = fnum;
         stream >> fnum; mat[1][3] = fnum;
         stream >> fnum; mat[2][3] = fnum;
         stream >> fnum; mat[3][3] = fnum;
         boneMats.push_back(mat);
      }
      else if ("bn" == label) {
         stream >> inum;
         stream >> name;
         boneTable.insert(BoneMap::value_type(name, inum));
      }
      else
         break;
   }


   while (file) {
      std::getline(file, readString);

      std::stringstream stream (readString);
      std::string label;
      std::string junk;
      std::string str;
      float fnum;
      uint inum;
      uint ndx;

      stream >> label;

      if (label.find("#") != std::string::npos) {
         // Comment, skip
      }
      else if ("" == label) {
         // just skip the new line
      }
      else if ("mesh" == label) {
         // No info here
      }
      else if ("vertex" == label) {
         // Don't care about number of verts
      }
      else if ("face" == label) {
         stream >> junk;
         stream >> fnum;
         faceCount = fnum;
      }
      else if ("v" == label) {
         stream >> fnum; buffVerts.push_back(fnum);
         stream >> fnum; buffVerts.push_back(fnum);
         stream >> fnum; buffVerts.push_back(fnum);
      }
      else if ("n" == label) {
         stream >> fnum; buffNorms.push_back(fnum);
         stream >> fnum; buffNorms.push_back(fnum);
         stream >> fnum; buffNorms.push_back(fnum);
      }
      else if ("uv" == label) { 
         stream >> fnum; buffUVs.push_back(fnum);
         stream >> fnum; buffUVs.push_back(fnum);
      }
      else if ("ta" == label) {
         stream >> fnum; buffTangents.push_back(fnum);
         stream >> fnum; buffTangents.push_back(fnum);
         stream >> fnum; buffTangents.push_back(fnum);
      }
      else if ("bt" == label) {
         stream >> fnum; buffBitangents.push_back(fnum);
         stream >> fnum; buffBitangents.push_back(fnum);
         stream >> fnum; buffBitangents.push_back(fnum);
      }
      else if ("f" == label) {
         stream >> ndx; buffIndxs.push_back(ndx);
         stream >> ndx; buffIndxs.push_back(ndx);
         stream >> ndx; buffIndxs.push_back(ndx);
      }
      else if ("bi" == label) {
         for (uint i = 0; i < MAX_WEIGHTS; i++) {
            stream >> inum; boneIndices.push_back(inum);
            stream >> fnum; boneWeights.push_back(fnum); 
            //std::cerr << "weight[" << i << "] = " << inum << ", " << fnum << std::endl;
         }
      }
      else if ("end" == label) {
         glGenBuffers(1, & mesh.vbo);
         glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
         glBufferData(GL_ARRAY_BUFFER, buffVerts.size() * sizeof(float),
            & buffVerts.front(), GL_STATIC_DRAW);
         
         glGenBuffers(1, & mesh.nbo);
         glBindBuffer(GL_ARRAY_BUFFER, mesh.nbo);
         glBufferData(GL_ARRAY_BUFFER, buffNorms.size() * sizeof(float),
            & buffNorms.front(), GL_STATIC_DRAW);

         glGenBuffers(1, & mesh.uvbo);
         glBindBuffer(GL_ARRAY_BUFFER, mesh.uvbo);
         glBufferData(GL_ARRAY_BUFFER, buffUVs.size() * sizeof(float),
            & buffUVs.front(), GL_STATIC_DRAW);

         glGenBuffers(1, & mesh.tabo);
         glBindBuffer(GL_ARRAY_BUFFER, mesh.tabo);
         glBufferData(GL_ARRAY_BUFFER, buffTangents.size() * sizeof(float),
            & buffTangents.front(), GL_STATIC_DRAW);

         glGenBuffers(1, & mesh.btbo);
         glBindBuffer(GL_ARRAY_BUFFER, mesh.btbo);
         glBufferData(GL_ARRAY_BUFFER, buffBitangents.size() * sizeof(float),
            & buffBitangents.front(), GL_STATIC_DRAW);

         glGenBuffers(1, & mesh.bibo);
         glBindBuffer(GL_ARRAY_BUFFER, mesh.bibo);
         glBufferData(GL_ARRAY_BUFFER, boneIndices.size() * sizeof(uint),
            & boneIndices.front(), GL_STATIC_DRAW);

         //std::cerr << "len = " << boneWeights.size() << " verts = " << buffVerts.size() << std::endl;
         glGenBuffers(1, & mesh.bwbo);
         glBindBuffer(GL_ARRAY_BUFFER, mesh.bwbo);
         glBufferData(GL_ARRAY_BUFFER, boneWeights.size() * sizeof(float),
            & boneWeights.front(), GL_STATIC_DRAW);

         glGenBuffers(1, & mesh.ibo);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
         glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffIndxs.size() * sizeof(uint),
            & buffIndxs.front(), GL_STATIC_DRAW);

         mesh.numFaces = faceCount;

         meshTable.insert(MeshMap::value_type(numMeshes, mesh));
         if (numMeshes == 0)
            base = &(meshTable.find(0)->second);
         numMeshes++;

         buffVerts.clear();
         buffNorms.clear();
         buffUVs.clear();
         buffTangents.clear();
         buffBitangents.clear();
         boneIndices.clear();
         boneWeights.clear();
         buffIndxs.clear();
      }
      else {
         std::cerr << "Unexpected '" << label << "' in " << path << ". Quitting\n";
         exit(0);
      }
   }
   file.close();
   numAnimations = 0;
}

Mesh * Model::getMesh(uint meshIndex) {
   Mesh * mesh = &(meshTable.find(meshIndex)->second);
   return mesh;
}
   
  /////////////////////////////////////////////////
 //////////////// Animation Setup ////////////////
/////////////////////////////////////////////////

void Model::loadAnimation(const std::string & path) {
   std::cerr << "Reading animation " << path << " into slot " << animations.size() << std::endl;
   Assimp::Importer importer;
   const aiScene * scene = importer.ReadFile(path,
                           aiProcess_CalcTangentSpace       |
                           aiProcess_Triangulate            |
                           aiProcess_JoinIdenticalVertices  |
                           aiProcess_SortByPType);

   if (!scene->HasAnimations()) {
      std::cerr << "This animation file does not have any animations... Exiting.\n";
      exit(0);
   }

   Animation anim;
   anim.ticksPerSecond = scene->mAnimations[0]->mTicksPerSecond != 0 ?
    scene->mAnimations[0]->mTicksPerSecond : 25.0f;
   anim.duration = scene->mAnimations[0]->mDuration;

   aiNode * root = findRoot(scene->mRootNode);
   anim.root = makeBoneTree(NULL, root, scene); // Recursively fill out the bone tree
   assert(anim.root != NULL);

   animations.push_back(anim);
   numAnimations = animations.size();
}

aiNode * Model::findRoot(aiNode * start) {
   std::queue<aiNode *> que;
   que.push(start);

   while (!que.empty()) {
      aiNode * node = que.front();
      que.pop();
      if (!strcmp(node->mName.C_Str(), "root"))
         return node;
      else
         for (uint i = 0; i < node->mNumChildren; i++)
            que.push(node->mChildren[i]);
   }

   std::cerr << "Could not find 'root'. Quitting...\n";
   exit(0);
}

Bone * Model::makeBoneTree(Bone * pBone, aiNode * ainode, const aiScene * scene) {
   std::string boneName = ainode->mName.C_Str();
   // std::cerr << "Name = " << boneName << std::endl;

   assert(scene->HasAnimations());
   aiNodeAnim * nodeAnim = findNodeAnim(scene->mAnimations[0], boneName);

   if (!nodeAnim) {
      std::cerr << "Bone '" << boneName << "' is not in the tree... Exiting.\n";
      exit(0);
   }

   Bone * bone = new Bone();

   if (pBone)
      pBone->children.push_back(bone);
   
   bone->name = boneName;
   bone->index = boneTable.find(boneName)->second;
   assert(bone->index < 50);
   bone->offset = boneMats[bone->index];
   
   for (uint i = 0; i < nodeAnim->mNumRotationKeys; i++)
      bone->rotationKeys.push_back(nodeAnim->mRotationKeys[i]);
   for (uint i = 0; i < nodeAnim->mNumPositionKeys; i++)
      bone->positionKeys.push_back(nodeAnim->mPositionKeys[i]);
   for (uint i = 0; i < nodeAnim->mNumScalingKeys; i++)
      bone->scaleKeys.push_back(nodeAnim->mScalingKeys[i]);

   // Recursively fill in the bone tree
   for (uint i = 0 ; i < ainode->mNumChildren; i++)
      makeBoneTree(bone, ainode->mChildren[i], scene);

   return bone;
}

aiNodeAnim * Model::findNodeAnim(aiAnimation * anim, std::string name) {
   for (uint i = 0; i < anim->mNumChannels; i++)
      if (!strcmp(anim->mChannels[i]->mNodeName.C_Str(), name.c_str()))
         return anim->mChannels[i];
   return NULL;
}

  /////////////////////////////////////////////////
 //////////////// Animation Usage ////////////////
/////////////////////////////////////////////////

glm::mat4 * Model::getAnimationTransforms(uint animNum, double timePassed, double wantedDuration, glm::mat4 * allMats) {
   Animation * anim = &animations[animNum];

   float tickTime = timePassed * anim->ticksPerSecond * anim->duration / wantedDuration;
   tickTime = fmod(tickTime, anim->duration);

   // Iterate through each bone/Node recursively and update its matrix
   computeBoneTransform(anim->root, allMats, tickTime, glm::mat4(1.0));

   return allMats;
}

void Model::computeBoneTransform(Bone * bone, glm::mat4 * allMats, float tickTime, glm::mat4 parentTransform) {
   glm::mat4 globalTransform = parentTransform * getAnimTransform(bone, tickTime);     
   glm::mat4 boneOffset = bone->offset;
   allMats[bone->index] = globalTransform * boneOffset;

   for (uint i = 0; i < bone->children.size(); i++)
      computeBoneTransform(bone->children[i], allMats, tickTime, globalTransform);
}

glm::mat4 Model::getAnimTransform(Bone * bone, float tickTime) { 
   glm::quat rotateQuat = calcInterpolatedRotation(bone, tickTime);
   glm::vec3 positionVec = calcInterpolatedPosition(bone, tickTime);
   glm::vec3 scalingVec = calcInterpolatedScaling(bone, tickTime);

   glm::mat4 rotationM = glm::toMat4(rotateQuat);
   glm::mat4 translationM = glm::translate(glm::mat4(1.0), positionVec);
   glm::mat4 scalingM = glm::scale(glm::mat4(1.0), scalingVec);

   return translationM * rotationM * scalingM;
}

glm::quat Model::calcInterpolatedRotation(Bone * bone, float tickTime) {
   // we need at least two values to interpolate...
   if (bone->rotationKeys.size() == 1)
      return aiToGlmQuat(bone->rotationKeys[0].mValue);

   uint earlyNdx = findEarlyRotationKeyIndex(bone, tickTime);
   uint lateNdx = earlyNdx + 1;
   assert(lateNdx < bone->rotationKeys.size());
   float deltaTime = bone->rotationKeys[lateNdx].mTime - bone->rotationKeys[earlyNdx].mTime;
   float ratio = (tickTime - (float)bone->rotationKeys[earlyNdx].mTime) / deltaTime;
   ratio = std::min(1.0f, std::max(0.0f, ratio));
   const aiQuaternion& earlyQuat = bone->rotationKeys[earlyNdx].mValue;
   const aiQuaternion& lateQuat = bone->rotationKeys[lateNdx].mValue;
   // glm::slerp()
   aiQuaternion out;
   aiQuaternion::Interpolate(out, earlyQuat, lateQuat, ratio);
   return glm::normalize(aiToGlmQuat(out));
}

glm::vec3 Model::calcInterpolatedPosition(Bone * bone, float tickTime) {
    // we need at least two values to interpolate...
   if (bone->positionKeys.size() == 1)
      return aiToGlmVec3(bone->positionKeys[0].mValue);

   uint earlyNdx = findEarlyPositionKeyIndex(bone, tickTime);
   uint lateNdx = earlyNdx + 1;
   assert(lateNdx < bone->positionKeys.size());
   float deltaTime = bone->positionKeys[lateNdx].mTime - bone->positionKeys[earlyNdx].mTime;
   float ratio = (tickTime - (float)bone->positionKeys[earlyNdx].mTime) / deltaTime;
   ratio = std::min(1.0f, std::max(0.0f, ratio));
   glm::vec3 earlyPos = aiToGlmVec3(bone->positionKeys[earlyNdx].mValue);
   glm::vec3 latePos = aiToGlmVec3(bone->positionKeys[lateNdx].mValue);
   return earlyPos + ratio * (latePos - earlyPos); // linear interpolation
}

glm::vec3 Model::calcInterpolatedScaling(Bone * bone, float tickTime) {
   // we need at least two values to interpolate...
   if (bone->scaleKeys.size() == 1)
      return aiToGlmVec3(bone->scaleKeys[0].mValue);

   uint earlyNdx = findEarlyScalingKeyIndex(bone, tickTime);
   uint lateNdx = earlyNdx + 1;
   assert(lateNdx < bone->scaleKeys.size());
   float deltaTime = bone->scaleKeys[lateNdx].mTime - bone->scaleKeys[earlyNdx].mTime;
   float ratio = (tickTime - (float)bone->scaleKeys[earlyNdx].mTime) / deltaTime;
   ratio = std::min(1.0f, std::max(0.0f, ratio));
   glm::vec3 earlyScale = aiToGlmVec3(bone->scaleKeys[earlyNdx].mValue);
   glm::vec3 lateScale = aiToGlmVec3(bone->scaleKeys[lateNdx].mValue);
   return earlyScale + ratio * (lateScale - earlyScale); // linear interpolation
}

uint Model::findEarlyRotationKeyIndex(Bone * bone, float tickTime) {
   assert(bone->rotationKeys.size() > 0);

   for (uint i = 0; i < bone->rotationKeys.size() - 1; i++)
      if (tickTime < (float)bone->rotationKeys[i + 1].mTime)
         return i;

   assert(false); // should never get here
}

uint Model::findEarlyPositionKeyIndex(Bone * bone, float tickTime) {
   assert(bone->positionKeys.size() > 0);

   for (uint i = 0; i < bone->positionKeys.size() - 1; i++)
      if (tickTime < (float)bone->positionKeys[i + 1].mTime)
         return i;

   assert(false); // should never get here
}

uint Model::findEarlyScalingKeyIndex(Bone * bone, float tickTime) {
   assert(bone->scaleKeys.size() > 0);

   for (uint i = 0; i < bone->scaleKeys.size() - 1; i++)
      if (tickTime < (float)bone->scaleKeys[i + 1].mTime)
         return i;

   assert(false); // should never get here
}

Bone::Bone() {};

Animation * Model::getAnimation(uint animNum) {
   return &(animations[animNum]);
}

