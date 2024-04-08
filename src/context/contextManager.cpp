#include "context/contextManager.hpp"



void ContextManager::setup(p6::Context& ctx, Camera& camera)
{
    ctx.mouse_dragged = [&](p6::MouseDrag) {
        glm::vec2 deltamouse = ctx.mouse_delta();

        camera.rotateLeft(deltamouse.x * 50);
        camera.rotateUp(deltamouse.y * 50);
    };

    ctx.mouse_scrolled = [&](p6::MouseScroll e) {
        camera.moveFront(-e.dy);
    };

    glEnable(GL_DEPTH_TEST);
}


void ContextManager::check_keys(p6::Context& ctx) {
    if (ctx.key_is_pressed(GLFW_KEY_ESCAPE)) {
        ctx.stop();
    }
}