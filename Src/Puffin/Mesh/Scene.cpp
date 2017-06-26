//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Mesh/Scene.h"

using namespace puffin;

Scene::Scene(std::string name)
{
    if (!name.empty())
        name_ = name;

    logDebug(name_, "Scene::Scene()", "Scene created.");
}

Scene::~Scene()
{
    logDebug(name_, "Scene::~Scene()", "Scene destroyed.");
}

void Scene::enable(GLboolean state)
{
    enabled_ = state;
}

void Scene::addSkybox(SkyboxPtr src_skybox, GLboolean activate)
{
    if (!src_skybox)
        logErrorAndThrow(name_, "Scene::addSkybox()",
            "Object [Skybox] pointer not set.");

    skybox_container_.push_back(src_skybox);

    if (activate)
        active_skybox_ = src_skybox;

    logInfo(name_, "Scene::addSkybox()", "Skybox [" + src_skybox->getName() +
        "] added to scene.");
}

void Scene::addObject3D(Object3DPtr src_object)
{
    if (!src_object)
        logErrorAndThrow(name_, "Scene::addObject3D()",
            "Object [Object3DPtr] pointer not set.");

    object3d_container_.push_back(src_object);

    logInfo(name_, "Scene::addModel3D()", "Model 3D [" + src_object->getName() +
        "] added to scene.");
}

void Scene::addWaterTile(WaterTilePtr water_tile_src)
{
    if (!water_tile_src)
        logErrorAndThrow(name_, "Scene::addWaterTile()",
            "Object [WaterTile] pointer not set.");

    water_tile_container_.push_back(water_tile_src);

    logInfo(name_, "Scene::addWaterTile()", "Water tile [" +
        water_tile_src->getName() + "] added to scene.");
}

void Scene::addParticleSystem(ParticleSystemPtr particle_system_src)
{
    if (!particle_system_src)
        logErrorAndThrow(name_, "Scene::addParticleSystem()",
            "Object [ParticleSystem] pointer not set.");

    particle_system_container_.push_back(particle_system_src);

    logInfo(name_, "Scene::addParticleSystem()", 
        "Particle system added to scene.");
}

void Scene::addText(TextPtr text_src)
{
    if (!text_src)
        logErrorAndThrow(name_, "Scene::addText()",
            "Object [Text] pointer not set.");

    text_container_.push_back(text_src);

    logInfo(name_, "Scene::addText()", "Text [" + text_src->getName() +
        "] added to scene.");
}