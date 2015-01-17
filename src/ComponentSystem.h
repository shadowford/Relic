#ifndef __COMPONENTSYSTEM_H__
#define __COMPONENTSYSTEM_H__

#include <map>
#include <string>

#include <SFML/System.hpp>

#include "EventSystem.h"

#define GET_COMPONENT_TYPE(TYPE) #TYPE
#define ADD_COMPONENT(TYPE, ...) addComponent(#TYPE, new TYPE(__VA_ARGS__))
#define GET_COMPONENT(ENTITY, TYPE) ((TYPE *)ENTITY->getComponent(#TYPE))

class Entity;

class Component {
    public:
        Entity * entity;
        bool initialised = false;
        void update();
    protected:
        virtual void runStart () = 0;
        virtual void runUpdate () = 0;
};

typedef std::string ComponentType;

struct Transform {
    sf::Vector2<double> pos = sf::Vector2<double>(0, 0);
    sf::Vector2<double> scale = sf::Vector2<double>(0, 0);
    double rotation = 0;
};

class Entity : public EventDispatcher {
    public:
        Entity(EventSystem *);
        Component * addComponent(ComponentType, void *);
        Component * removeComponent(ComponentType);
        Component * getComponent(ComponentType);

        bool destroyed = false;

        EventSystem * eventSystem;

        // Transform
        Transform transform;

        virtual void update();

    private:
        std::map<std::string, Component *> components;
};

#endif