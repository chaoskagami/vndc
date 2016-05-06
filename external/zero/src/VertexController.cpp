#include <Zero.hpp>
        VertexController::VertexController(ContextManager* cx) {
            ctx = cx;
        }

        // Storage initializers.
        void VertexController::LoadModel() {

        }

        // Programmatic Model Creation.
        void VertexController::PushModel(int mode) {
            ++modelscount;
            models = NULL;

            models = (ModelData*)realloc(models, sizeof(ModelData) * modelscount);

            models[modelscount-1].pts = mode;
            models[modelscount-1].count = 0;
            models[modelscount-1].v = NULL;
        }

        void VertexController::PushTexture(char* fname) {
            void* ptr = ctx->LoadImage(fname);
            models[modelscount-1].t = ((GLuint*)ctx->GLTexImage(ptr))[0];
        }

        void VertexController::PushQuad(    GLfloat x1, GLfloat y1, GLfloat z1, GLfloat u1, GLfloat v1,
                                            GLfloat x2, GLfloat y2, GLfloat z2, GLfloat u2, GLfloat v2,
                                            GLfloat x3, GLfloat y3, GLfloat z3, GLfloat u3, GLfloat v3,
                                            GLfloat x4, GLfloat y4, GLfloat z4, GLfloat u4, GLfloat v4  ) {
            _AllocateVertex();

            models[modelscount-1].v[models[modelscount-1].count-1].pts[0].x = x1;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[0].y = y1;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[0].z = z1;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[0].u = u1;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[0].v = v1;

            models[modelscount-1].v[models[modelscount-1].count-1].pts[1].x = x2;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[1].y = y2;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[1].z = z2;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[1].u = u2;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[1].v = v2;

            models[modelscount-1].v[models[modelscount-1].count-1].pts[2].x = x3;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[2].y = y3;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[2].z = z3;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[2].u = u3;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[2].v = v3;

            models[modelscount-1].v[models[modelscount-1].count-1].pts[3].x = x4;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[3].y = y4;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[3].z = z4;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[3].u = u4;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[3].v = v4;


        }

        void VertexController::PushTri(       GLfloat x1, GLfloat y1, GLfloat z1, GLfloat u1, GLfloat v1,
                                            GLfloat x2, GLfloat y2, GLfloat z2, GLfloat u2, GLfloat v2,
                                            GLfloat x3, GLfloat y3, GLfloat z3, GLfloat u3, GLfloat v3  ) {
            _AllocateVertex();

            models[modelscount-1].v[models[modelscount-1].count-1].pts[0].x = x1;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[0].y = y1;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[0].z = z1;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[0].u = u1;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[0].v = v1;

            models[modelscount-1].v[models[modelscount-1].count-1].pts[1].x = x2;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[1].y = y2;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[1].z = z2;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[1].u = u2;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[1].v = v2;

            models[modelscount-1].v[models[modelscount-1].count-1].pts[2].x = x3;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[2].y = y3;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[2].z = z3;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[2].u = u3;
            models[modelscount-1].v[models[modelscount-1].count-1].pts[2].v = v3;
        }

        void VertexController::_AllocateVertex() {
            models[modelscount-1].count += 1;
            models[modelscount-1].v = (PrimData*)realloc(models[modelscount-1].v, sizeof(PrimData) * models[modelscount-1].count);
        }

        void VertexController::PushDone() {

        }
