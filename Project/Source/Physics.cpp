#include "GameDefn.hpp"
#include <iostream>
#include "Physics.hpp"

Physics::Physics(std::shared_ptr<Object> parent) : Object2D(std::move(parent)) {}

void Physics::Enter() {

    AEGfxMeshStart();

    AEGfxTriAdd(-0.5f, -0.5f, color, 0.0f, 1.0f,
        0.5f, -0.5f, color, 1.0f, 1.0f,
        -0.5f, 0.5f, color, 0.0f, 0.0f);

    AEGfxTriAdd(0.5f, -0.5f, color, 1.0f, 1.0f,
        0.5f, 0.5f, color, 1.0f, 0.0f,
        -0.5f, 0.5f, color, 0.0f, 0.0f);

    aeMesh = AEGfxMeshEnd();

    Object2D::Enter();
}

void Physics::PreUpdate() {

    Object2D::PreUpdate();
}

void Physics::Gravity(f32 velocityY, f32 positionY)
{
    velocityY += gravity_force * dt;
    positionY -= velocityY * dt;
}

void Physics::Update() {
    //gravity(velocityY, positionY);

    
    Object2D::Update();
}

void Physics::PostUpdate() {
    AEMtx33 scale_matrix{ 0.0f };
    AEMtx33Identity(&scale_matrix);
    AEMtx33Scale(&scale_matrix, local_scale.x, local_scale.y);

    AEMtx33 rotation_matrix{ 0.0f };
    AEMtx33Identity(&rotation_matrix);
    AEMtx33Rot(&rotation_matrix, rotation * (PI / 180.0f));

    AEMtx33 translate{ 0.0f };
    AEMtx33Identity(&translate);
    AEMtx33Trans(&translate, position.x, position.y);

    AEMtx33 transform = { 0 };
    AEMtx33Concat(&transform, &rotation_matrix, &scale_matrix);
    AEMtx33Concat(&transform, &translate, &transform);

    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

    AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(transparency);

    AEGfxTextureSet(pTex, 0, 0);

    AEGfxSetTransform(transform.m);

    AEGfxMeshDraw(aeMesh, AE_GFX_MDM_TRIANGLES);

    Object2D::PostUpdate();
}

void Physics::Exit() {

    AEGfxMeshFree(aeMesh);

    if (pTex) {
        AEGfxTextureUnload(pTex);
    }

    Object2D::Exit();
}
