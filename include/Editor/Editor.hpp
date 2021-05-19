#ifndef EDITOR_HPP
#define EDITOR_HPP

class FrameBuffer;

class Camera;

struct EditorContext {
    FrameBuffer *frameBuffer;
    Camera *camera;
};

class Editor {
   public:
    Editor() = default;

    void intialize();

    void render(EditorContext &context);

    void close();
};

#endif /* EDITOR_HPP */
