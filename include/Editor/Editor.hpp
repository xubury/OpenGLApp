#ifndef EDITOR_HPP
#define EDITOR_HPP

class RenderWindow;

class FrameBuffer;

class Camera;

class TransformComp;

struct EditorContext {
    RenderWindow *window;
    FrameBuffer *frameBuffer;
    Camera *camera;
    TransformComp *trans;
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
