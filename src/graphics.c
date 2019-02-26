#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>

#include "graphics.h"
#include "config.h"
#include "utils.h"


void
agent_vbo_setup(Agent_graphics* ag)
{
  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, ag->vert_data_len, ag->vert_data, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  ag->vbo = vbo;
}

void
agent_shader_setup(Agent_graphics* ag)
{
  GLuint vs = 0;
  GLuint fs = 0;
  GLuint shader = 0;

  vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &(ag->vert_shader), NULL);
  glCompileShader(vs);

  fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &(ag->frag_shader), NULL);
  glCompileShader(fs);

  shader = glCreateProgram();
  glAttachShader(shader, fs);
  glAttachShader(shader, vs);
  glLinkProgram(shader);
  ag->shader = shader;

}

void
agents_draw(Agent_graphics* ag)
{
  int agent_dims = 4;
  size_t color_offset = ag->no_agents * agent_dims * sizeof(float);

  glUseProgram(ag->shader);
  glBindBuffer(GL_ARRAY_BUFFER, ag->vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, ag->vert_data_len, ag->vert_data);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, AGENT_VERT_DIMS, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribPointer(1, AGENT_VERT_DIMS, GL_FLOAT, GL_FALSE, 0, (void*) color_offset);
  glDrawArrays(GL_POINTS, 0, ag->no_agents);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glUseProgram(0);
}


void
ui_gfx_setup(Ui_graphics* uig)
{
  uig->height = 0.05;
  uig->show_sel = 0;
  uig->sel_x1 = 0.0f;
  uig->sel_y1 = 0.0f;
  uig->sel_x2 = 0.0f;
  uig->sel_y2 = 0.0f;
  float tmp_v[] = {
    -1.0f, -1.0f,
    1.0f, -1.0f,
    1.0f, -1.0f + uig->height,
    -1.0f, -1.0f+ uig->height};
  size_t tmp_v_size = sizeof(tmp_v);
  uig->cmd_txt = malloc(sizeof(char) * 64);
  strcpy(uig->cmd_txt, " ");

  uig->vertex_data = malloc(tmp_v_size);
  memcpy(uig->vertex_data, tmp_v, tmp_v_size);
}

void
ui_draw(Ui_graphics* uig)
{

  if(uig->show_sel)
  {
    glColor3f(0.5, 0.5, 0.9);
    glBegin(GL_LINE_LOOP);
    glVertex3f(uig->sel_x2, uig->sel_y1, 0.0);
    glVertex3f(uig->sel_x1, uig->sel_y1, 0.0);
    glVertex3f(uig->sel_x1, uig->sel_y2, 0.0);
    glVertex3f(uig->sel_x2, uig->sel_y2, 0.0);
    glEnd();
  }

  float* tmp_ptr = uig->vertex_data;
  glColor3f(0.5, 0.5, 0.5);
  glBegin(GL_QUADS);
  for(int i = 0; i < 4; i++, tmp_ptr += 2)
    glVertex3f(*tmp_ptr, *(tmp_ptr + 1), 0.0f);
  glEnd();
  draw_text(-0.98, -0.98,(const unsigned char *) uig->cmd_txt);


}

void
ui_gfx_update(Ui* ui, Ui_graphics* uig)
{
  if(ui->resp == (UI_RESP_UPDATE_TEXT || UI_RESP_RUN_CMD))
    strcpy(uig->cmd_txt, ui->out_buff);
  if(ui->resp == (UI_RESP_SEL_MODE + UI_RESP_UPDATE_TEXT))
  {
    strcpy(uig->cmd_txt, ui->out_buff);
    uig->show_sel = 1;
    printf("sel mode\n");
    uig->sel_x1 = ui->sel_x1;
    uig->sel_y1 = ui->sel_y1;
    uig->sel_x2 = ui->sel_x2;
    uig->sel_y2 = ui->sel_y2;
  }
  else uig->show_sel = 0;
}


void
draw_text(float x, float y, const unsigned char* txt)
{
  glColor3f(0.0f, 0.0f, 0.0f);
  glRasterPos3d(x, y, 0.0);
  glutBitmapString(GLUT_BITMAP_9_BY_15, txt);
}


