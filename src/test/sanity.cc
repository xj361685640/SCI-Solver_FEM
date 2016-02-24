#include "gtest/gtest.h"
#include "FEMSolver.h"
TEST(SanityTests, EggCarton) {
  //make sure there is a command interpreter
  ASSERT_EQ(0, (int)!(std::system(NULL)));
  //test the egg carton
  FEMSolver cfg;
  cfg.filename_ = std::string(TEST_DATA_DIR) + "/simple";
  cfg.verbose_ = true;
  cfg.tetMesh_ = TetMesh::read(
    (cfg.filename_ + ".node").c_str(),
    (cfg.filename_ + ".ele").c_str(), true, cfg.verbose_);
  cfg.topSize_ = 275;
  size_t num_vert = cfg.tetMesh_->vertices.size();
  float lambda = 1.f;
  //create the A matrix
  Matrix_ell_h A_h(num_vert, num_vert, num_vert, 1);
  for (int i = 0; i < num_vert; i++) {
    A_h.column_indices(i, 0) = i;
    A_h.values(i, 0) = 8. * M_PI * M_PI + lambda;
  }
  //create the b vector
  Vector_h_CG b_h(num_vert, 1.0), x_h(num_vert, 0.0);
  for (int i = 0; i < num_vert; i++) {
    b_h[i] = 8. * M_PI * M_PI * std::sin(2. * M_PI * cfg.tetMesh_->vertices[i][0]) *
      std::sin(2. * M_PI * cfg.tetMesh_->vertices[i][1]) + lambda *
      std::sin(2. * M_PI * cfg.tetMesh_->vertices[i][0]) *
      std::sin(2. * M_PI * cfg.tetMesh_->vertices[i][1]);
  }
  //create the b vector
  std::vector<float> x_answer;
  for (int i = 0; i < num_vert; i++) {
    x_answer.push_back(
      std::sin(2. * M_PI * cfg.tetMesh_->vertices[i][0]) *
      std::sin(2. * M_PI * cfg.tetMesh_->vertices[i][1]));
  }
  cfg.checkMatrixForValidContents(&A_h);
  cfg.solveFEM(&A_h, &x_h, &b_h);
  //look for error
  float error = 0.f;
  std::vector<float> x_actual;
  for (int i = 0; i < num_vert; i++) {
    error += (x_h[i] - x_answer[i]) * (x_h[i] - x_answer[i]);
    x_actual.push_back(x_h[i]);
  }
  cfg.writeVTK(x_actual, "test_egg_carton2");
  ASSERT_TRUE(std::sqrt(error) < 1.);
  std::cout << "The error is : " << std::sqrt(error) << std::endl;
}