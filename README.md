# Bongo Mountain Engine

C++ game engine

## Table of contents
 * [Introduction](#introduction)
 * [Dependencies](#dependencies)
 * [Feature](#feature)
 * [Plan Feature](#plan-feature)

## Introduction

Watch branch [dev](//github.com/binaire10/BongoMountain/tree/dev) to see the current state of the engine

## Dependencies

Most dependencies is handle by conan.

## Feature

 - OpenGL primitive object
   - Vertex and Index Buffer
   - Texture 2D
   - Shader
   - Allow memory dump for OpenGL buffer
   - Vertex Layout
   - Dynamic allocation of memory for `BufferVectorObjectGeneric`
 - Dynamic Extension loader
 - Core and App Logger
 - resource handler (shared, not shared)
 - Current extension available :
    - glfw loader with glad initializing
    - utility extension with image loader
 - Default debug helper in debug binary version

## Plan Feature
 - Draw by chosen primitive type in static
 - Draw manager in separate module
   - particle system
   - quad batchRender