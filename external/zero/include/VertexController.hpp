#ifndef VTEXCONTROL_HPP
#define VTEXCONTROL_HPP

typedef struct {
    GLfloat x, y, z;
    GLfloat u, v;
} VtexData;

typedef struct {
    VtexData pts[4];
} PrimData;

typedef struct {
    PrimData* v; // Vertex data
    int count;   // How many vertexes
    int pts;     // 4 or 3
    GLuint t;    // Texture ptr
} ModelData;

class VertexController {
    public:
        VertexController(ContextManager* cx);

        // Storage initializers.
        void LoadModel();

        // Programmatic Model Creation.
        void PushModel(int mode);

        void PushTexture(char* fname);

        void PushQuad(    GLfloat x1, GLfloat y1, GLfloat z1, GLfloat u1, GLfloat v1,
                        GLfloat x2, GLfloat y2, GLfloat z2, GLfloat u2, GLfloat v2,
                        GLfloat x3, GLfloat y3, GLfloat z3, GLfloat u3, GLfloat v3,
                        GLfloat x4, GLfloat y4, GLfloat z4, GLfloat u4, GLfloat v4  );

        void PushTri(   GLfloat x1, GLfloat y1, GLfloat z1, GLfloat u1, GLfloat v1,
                        GLfloat x2, GLfloat y2, GLfloat z2, GLfloat u2, GLfloat v2,
                        GLfloat x3, GLfloat y3, GLfloat z3, GLfloat u3, GLfloat v3  );

        void PushDone();


    private:
        ModelData* models;
        ContextManager* ctx;
        int modelscount;

        void _AllocateVertex();
};

#endif
