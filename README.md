# Bongo Mountain Engine

C++ game engine

## Table of contents
 * [Introduction](#introduction)
 * [Dependencies](#dependencies)
 * [Feature](#feature)
 * [Plan Feature](#plan-feature)

## Introduction

Watch branch [dev](//github.com/binaire10/BongoMountain/tree/dev) to see the current state of the engine

The purpose of this engine is to try to implement a game / graphic engine.
Inspire by the [hazel engine](//github.com/TheCherno/Hazel) and my knowledge from university.
This engine should inline call of graphic module engine command with compiler optimisation.

## Dependencies

Most dependencies is downloaded automaticaly by the cmake project
at configuration time with git.

## Feature
 * Core Module
   - Assertion customised
   - Create and free resource (Handle)
   - Logger with spdlog
   - Event system
 * Graphic Module
   - Load image with stb
   - Create module graphic with window
   - Basic Windows Event
   - High-level Vector/Array for Buffer Object
   - Agnostic API
     * OpenGL api (Shader, Buffer Object, Vertex Array Object, Texture)
 * Render Module
   - Particle system (send point to gpu to draw quad generate by the geometry shader)

## Plan Feature
 * Render Module
   - Voxel render system
   - Manage GPU memory
