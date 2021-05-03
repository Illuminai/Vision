#version 460

#extension GL_GOOGLE_include_directive : require
#extension GL_EXT_ray_tracing : require

#include "core/ray_payload.glsl"

layout(location = 0) rayPayloadEXT RayPayload ray;

void main() {
    ray.colorAndDistance = vec4(0,0,0,-1);
}