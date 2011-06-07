#define HERMES_REPORT_ALL


// This example shows how to solve a first simple PDE:
//   - load the mesh,
//   - perform initial refinements,
//   - create a H1 space over the mesh,
//   - define weak formulation,
//   - initialize matrix solver,
//   - assemble and solve the matrix system,
//   - output the solution and element orders in VTK format 
//     (to be visualized, e.g., using Paraview),
//   - visualize the solution using Hermes' native OpenGL-based functionality.
//
// PDE: Poisson equation -Laplace u = CONST_F with homogeneous (zero)
//      Dirichlet boundary conditions.
//
// You can change the constant right-hand side CONST_F, the
// initial polynomial degree P_INIT, and play with various initial
// mesh refinements at the beginning of the main() function.
const double CONST_F = 2.0;
const bool HERMES_VISUALIZATION = true;           // Set to "false" to suppress Hermes OpenGL visualization. 
const bool VTK_VISUALIZATION = true;              // Set to "true" to enable VTK output.
const int P_INIT = 3;                             // Uniform polynomial degree of mesh elements.
Hermes::MatrixSolverType matrix_solver = Hermes::SOLVER_UMFPACK;  // Possibilities: Hermes::SOLVER_AMESOS, Hermes::SOLVER_AZTECOO, Hermes::SOLVER_MUMPS,
// Hermes::SOLVER_PETSC, Hermes::SOLVER_SUPERLU, Hermes::SOLVER_UMFPACK.

// Weak forms.
#include "definitions.cpp"

int main(int argc, char* argv[])
{
  // Load the mesh.
  Mesh mesh;
  H2DReader mloader;
  mloader.load("domain.mesh", &mesh);

  // Perform initial mesh refinements (optional).
  //mesh.refine_all_elements();

  // Initialize the weak formulation.
  CustomWeakFormPoisson wf(CONST_F);

  // Initialize boundary conditions.
  DefaultEssentialBCConst<double> bc_essential("Dirichlet", 0.0);
  EssentialBCs<double> bcs(&bc_essential);

  // Create an H1 space with default shapeset.
  H1Space<double> space(&mesh, &bcs, P_INIT);
  int ndof = space.get_num_dofs();
  info("ndof = %d", ndof);

  // Initialize the FE problem.
  bool is_linear = true;
  DiscreteProblem<double> dp(&wf, &space, is_linear);

  // Set up the solver, matrix, and rhs according to the solver selection.
  SparseMatrix<double>* matrix = create_matrix<double>(matrix_solver);
  Vector<double>* rhs = create_vector<double>(matrix_solver);
  Solver<double>* solver = create_linear_solver<double>(matrix_solver, matrix, rhs);

  // Initialize the solution.
  Solution<double> sln;

  // Assemble the stiffness matrix and right-hand side vector.
  info("Assembling the stiffness matrix and right-hand side vector.");
  dp.assemble(matrix, rhs);

  // Solve the linear system and if successful, obtain the solution.
  info("Solving the matrix problem.");
  if(solver->solve()) Solution<double>::vector_to_solution(solver->get_solution(), &space, &sln);
  else error ("Matrix solver failed.\n");

  // VTK output.
  if (VTK_VISUALIZATION) 
  {
    // Output solution in VTK format.
    Linearizer lin;
    bool mode_3D = true;
    lin.save_solution_vtk(&sln, "sln.vtk", "Temperature", mode_3D);
    info("Solution in VTK format saved to file %s.", "sln.vtk");

    // Output mesh and element orders in VTK format.
    Orderizer ord;
    ord.save_orders_vtk(&space, "ord.vtk");
    info("Element orders in VTK format saved to file %s.", "ord.vtk");
  }

  // Visualize the solution.
  if (HERMES_VISUALIZATION) 
  {
    ScalarView<double> view("Solution", new WinGeom(0, 0, 440, 350));
    view.show(&sln);
    View::wait();
  }

  // Clean up.
  delete solver;
  delete matrix;
  delete rhs;

  return 0;
}
