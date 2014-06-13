#include "ParticleSpawner.h"


ParticleSpawner::ParticleSpawner(){
}

void ParticleSpawner::startParticles(glm::vec3 pos, int amount, float size, Material* material, glm::vec3 direction, Shader *shader){
   position = pos;
   pntSize = size;
   
   //glGenBuffers(1, &posBuff);
   //assigns initial particle positions
   for(GLuint i = 0; i < amount; i++){
      velocity.push_back(glm::vec3((float) rand() / (float) RAND_MAX,(float) rand() / (float) RAND_MAX, (float) rand() / (float) RAND_MAX));
      points.push_back(position);
      //printf("%f\n", velocity[i]);
   }
   //printf("ran the spawner\n");
   
}//end constructor

void ParticleSpawner::drawParticles(Shader *shaderover){
   
   GLenum err;
   //initial position
   glGenBuffers(1, &posBuff);
   glBindBuffer(GL_ARRAY_BUFFER, posBuff);     
   glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*points.size(), NULL, GL_DYNAMIC_DRAW); 
   glm::vec3* data = (glm::vec3*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE); //in GPU
   
   //might not need
   glGenBuffers(1, &movBuff);
   glBindBuffer(GL_ARRAY_BUFFER, movBuff);     
   glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*points.size(), NULL, GL_DYNAMIC_DRAW); 
   glm::vec3* transPoint = (glm::vec3*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE); //in GPU
   
   //printf("before loop\n");
   for(GLuint i = 0; i < points.size(); i++){
      data[i] = points[i];      
      transPoint[i] = points[i];
   }
   
   //printf("positions: %f, %f, %f\n", points[5].x, points[5].y, points[5].z);
   glUnmapBuffer(GL_ARRAY_BUFFER);
   glPointSize(pntSize);
   
   glVertexAttribPointer (1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray (0);
   glEnableVertexAttribArray (1);
   
   
   shaderover->render_particles(new Material(0.2, 0.0, 0.2, 0.5, 0.1, 0.6, 0.8, 0.1, 0.9, 200.f),
	           posBuff,
	           //movBuff,
	           points.size());
   
}

void ParticleSpawner::setPosition(glm::vec3 pos){
   position = pos;
}

void ParticleSpawner::updateParticles(int num){
   if(num == 0){
      for(GLuint i=0; i < points.size(); i++){
         //points[i].y -= velocity[i].y;
         //points[i].z += velocity[i].z;
         //points[i].x += velocity[i].x;
         
         points[i] -= glm::vec3((float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
         if(points[i].y < -1.0)
            points[i] = position;
      }
   }
   if(num == 1){
      for(GLuint i=0; i < points.size(); i++){
         points[i].y += velocity[i].y;
         points[i].z += (float)rand() / (float)RAND_MAX;
         points[i].x += (float)rand() / (float)RAND_MAX;
         
         if(points[i].y > position.y + 500.0 || points[i].x > position.x + 500.0 || points[i].x > position.z + 500.0)
            points[i] = position;
      }
   }   
}//end updateParticles
