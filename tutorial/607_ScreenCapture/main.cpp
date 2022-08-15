#include <igl/readOFF.h>
#include <igl/opengl/glfw/Viewer.h>
#include <iostream>
#include "tutorial_shared_path.h"
#include <igl/png/writePNG.h>
#include <igl/png/readPNG.h>
#if 1
Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> maskR,maskG,maskB,maskA;
Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> fgR,fgG,fgB,fgA;
Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> bgR,bgG,bgB,bgA;

double anim_t = 1.0;
double anim_t_dir = -0.03;
int bg_mesh = 0;
int fg_mesh = 0;
Eigen::MatrixXd V_bg(4,3);


bool key_down(igl::opengl::glfw::Viewer &viewer, unsigned char key, int mods)
{
  switch(key)
  {
    case ' ':
      viewer.core().is_animating = !viewer.core().is_animating;
      break;
  }
    return true;
}
bool pre_draw(igl::opengl::glfw::Viewer & viewer)
{
  using namespace Eigen;
  using namespace std;
  if(viewer.core().is_animating)
  {
      
      Eigen::MatrixXd V = V_bg;
      V.col(0).array() += anim_t;
      viewer.data(fg_mesh).set_vertices(V);
    
      anim_t += anim_t_dir;
    anim_t_dir *= (anim_t>=1.0 || anim_t<=0.0?-1.0:1.0);
  }
  return false;
}


int main(int argc, char *argv[])
{
    const std::string img_fg = "/Users/vbatra/Desktop/motion/fg.png";
    const std::string img_bg = "/Users/vbatra/Desktop/motion/bg.png";
    const std::string img_mask = "/Users/vbatra/Desktop/motion/mask.png";
    
    

    // Read the PNG
    igl::png::readPNG(img_fg,fgR,fgG,fgB,fgA);
    igl::png::readPNG(img_bg,bgR,bgG,bgB,bgA);
    igl::png::readPNG(img_mask,maskR,maskG,maskB,maskA);
    
    
    V_bg <<
      -1.0,-1.0,0,
       1.0,-1.0,0,
       1.0, 1.0,0,
      -1.0, 1.0,0;
    Eigen::MatrixXi F_bg(2,3);
    F_bg <<
      0,1,2,
      2,3,0;
    Eigen::MatrixXd UV_bg(4,2);
    UV_bg <<
      0,0,
      1,0,
      1,1,
      0,1;
    
    igl::opengl::glfw::Viewer viewer;
    viewer.core().depth_test = false;
    viewer.data().show_overlay = false;
    viewer.data().double_sided = true;

    bg_mesh = viewer.append_mesh();
    
    viewer.data(bg_mesh).set_mesh(V_bg, F_bg);
    viewer.data(bg_mesh).set_uv(UV_bg);
    viewer.core(bg_mesh).align_camera_center(V_bg);
    viewer.data(bg_mesh).show_texture = true;
    viewer.data(bg_mesh).show_overlay = false;
    viewer.data(bg_mesh).show_lines = false;
    // Use the image as a texture
    viewer.data(bg_mesh).set_texture(bgR,bgG,bgB, bgA);
    viewer.data(bg_mesh).set_colors(Eigen::RowVector3d(1,1,1));
    viewer.core().align_camera_center(viewer.data().V,viewer.data().F);
    

    fg_mesh = viewer.append_mesh();
    
    viewer.data(fg_mesh).set_mesh(V_bg, F_bg);
    viewer.data(fg_mesh).set_uv(UV_bg);
    viewer.core(fg_mesh).align_camera_center(V_bg);
    viewer.data(fg_mesh).show_texture = true;
    viewer.data(fg_mesh).show_lines = false;
    viewer.data(fg_mesh).show_overlay = false;
    viewer.data(fg_mesh).set_colors(Eigen::RowVector3d(1,1,1));
    viewer.data(fg_mesh).set_texture(fgR,fgG,fgB, maskR);

    viewer.callback_pre_draw = &pre_draw;
    viewer.callback_key_down = &key_down;
    viewer.core().is_animating = false;

    viewer.launch();


}


#endif



#if 0

// This function is called every time a keyboard button is pressed
bool key_down(igl::opengl::glfw::Viewer& viewer, unsigned char key, int modifier)
{
  if (key == '1')
  {
    // Allocate temporary buffers
    Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> R(1280,800);
    Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> G(1280,800);
    Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> B(1280,800);
    Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> A(1280,800);

    // Draw the scene in the buffers
    viewer.core().draw_buffer(
      viewer.data(),false,R,G,B,A);

    // Save it to a PNG
    igl::png::writePNG(R,G,B,A,"out.png");
  }

  if (key == '2')
  {
    // Allocate temporary buffers
    Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> R,G,B,A;

    // Read the PNG
    igl::png::readPNG("out.png",R,G,B,A);

    // Replace the mesh with a triangulated square
    Eigen::MatrixXd V(4,3);
    V <<
      -0.5,-0.5,0,
       0.5,-0.5,0,
       0.5, 0.5,0,
      -0.5, 0.5,0;
    Eigen::MatrixXi F(2,3);
    F <<
      0,1,2,
      2,3,0;
    Eigen::MatrixXd UV(4,2);
    UV <<
      0,0,
      1,0,
      1,1,
      0,1;

    viewer.data().clear();
    viewer.data().set_mesh(V,F);
    viewer.data().set_uv(UV);
    viewer.core().align_camera_center(V);
    viewer.data().show_texture = true;

    // Use the image as a texture
    viewer.data().set_texture(R,G,B);

  }


  return false;
}

int main(int argc, char *argv[])
{
  // Load a mesh in OFF format
  Eigen::MatrixXd V;
  Eigen::MatrixXi F;
  igl::readOFF(TUTORIAL_SHARED_PATH "/bunny.off", V, F);

  std::cerr << "Press 1 to render the scene and save it in a png." << std::endl;
  std::cerr << "Press 2 to load the saved png and use it as a texture." << std::endl;

  // Plot the mesh and register the callback
  igl::opengl::glfw::Viewer viewer;
  viewer.callback_key_down = &key_down;
  viewer.data().set_mesh(V, F);
  viewer.launch();
}
#endif
