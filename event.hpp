#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>
#include <functional>


namespace DI{
   enum EventType : unsigned int{
        WIN_CLOSED,
        WIN_RESIZED,
        WIN_MOVED,
        WIN_FOCUS,
        WIN_MINIMIZED,
        APP_FRAME,
        KEY_PRESSED,
        KEY_RELEASED,
        MOUSE_MOVED,
        MOUSE_PRESSED,
        MOUSE_RELEASED,
        MOUSE_SCROLLED,
        CUSTOM_EVENT,
   };


   struct EventData{
        int key;
        int isKeyHold;
        glm::vec2 mousePos;
        glm::vec2 mouseWheelOffset;
        std::unordered_multimap<EventType,std::function<void()>> callbacks;
   };

  namespace EventHandler{
          void Init();
          void Kill();
          // Check all possible events, only then when it occures
          void Update();
          void Set(EventType type,std::function<void()> func);
          void UnSet(EventType type);
          // Call callback
          void Use(EventType type);
  }

}
