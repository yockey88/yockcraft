/**
 * \file scene/scene.hpp
*/
#ifndef YOCKCRAFT_SCENE_HPP
#define YOCKCRAFT_SCENE_HPP

#include <string>

#include <entt/entt.hpp>

#include "core/uuid.hpp"

namespace yockcraft {
  
  class Entity;

  class Scene {
    public:
      Scene();
      ~Scene();
      
      void AddEntity(const std::string& name); 

      void Start(); 
      void Stop(); 

      const std::map<UUID , Entity*>& SceneEntities() const;

    protected:
      virtual void OnStart() {}
      virtual void OnStop() {}

    private:
      friend class Entity;
      // Octree space_partition;

      bool corrupt = true;

      entt::registry registry;

      std::map<UUID , Entity*> entities{};
  };

} // namespace yockcraft

#endif // !YOCKCRAFT_SCENE_HPP