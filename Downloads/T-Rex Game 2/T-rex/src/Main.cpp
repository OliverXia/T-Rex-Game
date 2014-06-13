#define GL_GLEXT_PROTOTYPES
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <thread>       
#include <chrono>  
#include <irrklang/irrKlang.h>
#include <iostream>     
#include <FTGL/ftgl.h>
#include <FTGL/FTGLPixmapFont.h>

#include "Game.h" 

#define WIN_HEIGHT 600
#define WIN_WIDTH  1000

irrklang::ISoundEngine *engine;

static void error_callback(int error, const char* description) {
   fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
   if (game->onKey(key, scancode, action, mods))
      glfwSetWindowShouldClose(window, GL_TRUE);
}

static void cursor_pos_callback(GLFWwindow* window, double x, double y) {
   game->onMouseMove(window, x, y);
}

int main(int argc, char ** argv) {
   GLFWwindow* window;
   glfwSetErrorCallback(error_callback);
  
   if (!glfwInit())
      exit(EXIT_FAILURE);

   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

   window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Super Galaxy X-Rex Simulator: Gaiden. FPS: 0", NULL, NULL);
   if (!window) {
      glfwTerminate();
      exit(EXIT_FAILURE);
   }

   glfwMakeContextCurrent(window);

   FTGLPixmapFont font("./resources/fonts/FreeMono.ttf");
   font.FaceSize(72);
   font.Render("Loading...", -1, FTPoint(300, 300));
   glfwSwapBuffers(window);

   game = new Game(argc, argv);
   engine = irrklang::createIrrKlangDevice();
   engine->play2D("./audio/jpr.wav", true);

   glfwSetKeyCallback(window, key_callback);
   glfwSetCursorPosCallback(window, cursor_pos_callback);
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

   double cumDelta = 0;
   unsigned int numFrames = 0;

   int gstatus = 0;

   double lastTime = glfwGetTime(); 
   while (!glfwWindowShouldClose(window) && !gstatus) {
      int w_width, w_height;
      glfwGetWindowSize(window, &w_width, &w_height);

      double newTime = glfwGetTime();

      double delta = newTime - lastTime;
      cumDelta += delta;
      if (cumDelta > 1.0) {
         char buf[255];
         sprintf(buf, "Super Galaxy X-Rex Simulator: Gaiden. FPS: %d", numFrames);
         glfwSetWindowTitle(window, buf);
         numFrames = 0;
         cumDelta = 0;
      }
      numFrames++;

      gstatus = game->doFrame(newTime, w_width, w_height);
      if (gstatus == 1)
         break;

      glfwSwapBuffers(window);
      glfwPollEvents();

      lastTime = newTime;
   }

   glClearDepth(1.0f);
   glDepthFunc(GL_LEQUAL);
   glEnable(GL_DEPTH_TEST);
   glClearColor(0.4,0.2,0.2,1.0);
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   if (gstatus == 2) {
      font.Render("Game Over. You win!", -1, FTPoint(60, 330));
      font.Render("(dinos always win)",  -1, FTPoint(90, 230));      
   }
   else {
      font.Render("RIP - You lose :(", -1, FTPoint(60, 330));
   }
   glfwSwapBuffers(window);

   while (!glfwWindowShouldClose(window))
      glfwPollEvents();

   glfwDestroyWindow(window);
   glfwTerminate();
   exit(EXIT_SUCCESS);
}
