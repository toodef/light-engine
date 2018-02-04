#pragma once

#include <string>

namespace shaders
{
   std::string default_vs("#version 420\n"
                             "\n"
                             "layout ( location = 0 )in vec3 pos;\n"
                             "layout ( location = 3 )in vec3 norm;\n"
                             "layout ( location = 6 )in vec3 tex;\n"
                             "layout ( location = 9 )in vec4 col;\n"
                             "\n"
                             "out vec4 out_col;\n"
                             "\n"
                             "uniform mat4 m_mvp;\n"
                             "\n"
                             "void main()\n"
                             "{\n"
                             "   out_col = col;\n"
                             "\n"
                             "   gl_Position = m_mvp * vec4(pos, 1.0f);\n"
                             "}");

   std::string default_fs("#version 420\n"
                         "\n"
                         "in vec4 out_col;\n"
                         "\n"
                         "void main()\n"
                         "{\n"
                         "   gl_FragColor = out_col;\n"
                         "}");
}
