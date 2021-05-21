#ifndef EDITOR_HPP
#define EDITOR_HPP

class RenderWindow;

class FrameBuffer;

class Camera;

template <class ENTITY>
class EntityManager;

class EntityBase;

struct EditorContext {
    RenderWindow *window;
    FrameBuffer *frameBuffer;
    Camera *camera;
    EntityManager<EntityBase> *entities;
};

class Editor {
   public:
    static Editor &instance();

    void render();

    void close();

    EditorContext context;
   private:
    Editor();

    int m_id;
};

#endif /* EDITOR_HPP */
