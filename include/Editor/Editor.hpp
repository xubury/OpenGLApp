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
    static Editor &instance();

    void render(EditorContext &context);

    void close();

   private:
    Editor();
};

#endif /* EDITOR_HPP */
