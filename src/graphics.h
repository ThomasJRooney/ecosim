#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <GL/glew.h>
#include "agents.h"
#include "quadtree.h"
#include "ui_response.h"

typedef struct RGB_ RGB;

struct _Ui_graphics {
  float height;
  float* vertex_data;
  RGB* color;
  char* cmd_txt;
  int show_sel;
  float selection[4];
};

typedef struct _Ui_graphics Ui_graphics;

GLuint gfx_agent_shader();
void gfx_agents_draw_new(Agent_verts* av, GLuint shader);

void gfx_quad_draw(Quadtree_verts* qv);

Ui_graphics* ui_gfx_setup(void);
void ui_draw(Ui_graphics* uig);
void ui_gfx_update(Ui_resp* resp, Ui_graphics* uig);

void gfx_text_draw(float x, float y, const unsigned char* txt);




#endif
