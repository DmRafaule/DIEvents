#include "event.hpp"
#include "DIWindow/win.hpp"
#include "DIWindow/viewport.hpp"
#include "DIDebuger/log.hpp"
#include "DIDebuger/debuger.hpp"

namespace DI{

   extern WinData* eg_winData;
   EventData* eg_eventData;
  
   void KeyButtonCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
   void WinCloseCallback(GLFWwindow* window);
   void WinResizeCallback(GLFWwindow* window, int width, int height);
   void WinMovedCallback(GLFWwindow* window, int xpos, int ypos);
   void WinFocusCallback(GLFWwindow* window, int focused);
   void WinMinimizedCallback(GLFWwindow* window, int iconified);
   void MouseMovedCallback(GLFWwindow* window, double xpos, double ypos);
   void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
   void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

   void EventHandler::Init(){
      eg_eventData = new EventData();
      DI_LOG_TRACE("EventHandler say: Init Events"); 
      glfwSetKeyCallback(eg_winData->win,KeyButtonCallback);
      glfwSetWindowCloseCallback(eg_winData->win,WinCloseCallback);
      glfwSetWindowSizeCallback(eg_winData->win,WinResizeCallback);
      glfwSetWindowPosCallback(eg_winData->win,WinMovedCallback);
      glfwSetWindowFocusCallback(eg_winData->win,WinFocusCallback);
      glfwSetWindowIconifyCallback(eg_winData->win,WinMinimizedCallback);
      glfwSetCursorPosCallback(eg_winData->win,MouseMovedCallback);
      glfwSetMouseButtonCallback(eg_winData->win,MouseButtonCallback);
      glfwSetScrollCallback(eg_winData->win,MouseScrollCallback);
   }
   void EventHandler::Kill(){
     delete eg_eventData;
     DI_LOG_TRACE("EventHandler say: Kill Events"); 
   }
   void EventHandler::Set(EventType type,std::function<void()> func){
     eg_eventData->callbacks.emplace(type,func);
   }
   void EventHandler::UnSet(EventType type){
     eg_eventData->callbacks.erase(type);
   }
   void EventHandler::Use(EventType type){
     // Find callback, check if it exist
     // And call it
     auto callbacks = eg_eventData->callbacks.equal_range(type);
     for (auto callback = callbacks.first; callback != callbacks.second; callback++){
       callback->second(); 
     }
   }

   void EventHandler::Update(){
      glfwWaitEvents();
      EventHandler::Use(APP_FRAME);
   }

   void KeyButtonCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
      eg_eventData->key = key;
      action == GLFW_REPEAT ? eg_eventData->isKeyHold = true : eg_eventData->isKeyHold = false; 
      if (action == GLFW_PRESS || action == GLFW_REPEAT){
         EventHandler::Use(KEY_PRESSED);
      }
      if (action == GLFW_RELEASE){
         EventHandler::Use(KEY_RELEASED);
      }
   }
   void WinCloseCallback(GLFWwindow* window){
      eg_winData->isOpen = false;
      EventHandler::Use(WIN_CLOSED);
   }
   void WinResizeCallback(GLFWwindow* window, int width, int height){
      eg_winData->size = glm::vec2(width,height);
      ViewportHandler::UpdateOnWindowResize(0,0,width,height);
      EventHandler::Use(WIN_RESIZED);
   }
   void WinMovedCallback(GLFWwindow* window, int xpos, int ypos){
      eg_winData->pos = glm::vec2(xpos,ypos);
      EventHandler::Use(WIN_MOVED);
   }
   void WinFocusCallback(GLFWwindow* window, int focused){
      eg_winData->isFocued = (bool)focused;
      EventHandler::Use(WIN_FOCUS);
   }
   void WinMinimizedCallback(GLFWwindow* window, int iconified){
      eg_winData->isMinimized = (bool)iconified;
      EventHandler::Use(WIN_MINIMIZED);
   }
   void MouseMovedCallback(GLFWwindow* window, double xpos, double ypos){
      // Translate mouse coord so that left bottom corner has 0,0 coord
      eg_eventData->mousePos = glm::vec2(xpos,eg_winData->size.y - ypos);
      EventHandler::Use(MOUSE_MOVED);
   }
   void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
      if (action == GLFW_PRESS){
         eg_eventData->key = button;
         EventHandler::Use(MOUSE_PRESSED);
      }
      else{
         eg_eventData->key = button;
         EventHandler::Use(MOUSE_RELEASED);
      }
   }
   void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset){
      eg_eventData->mouseWheelOffset = glm::vec2(xoffset,yoffset); 
      EventHandler::Use(MOUSE_SCROLLED);
   }
}
