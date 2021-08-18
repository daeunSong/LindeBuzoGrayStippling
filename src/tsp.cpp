//This file solves a TSP using concorde given a symmetric 2D distance matrix

//Info about concorde functions: www.math.uwaterloo.ca/tsp/concorde/DOC/concorde_org.html
#include "lbgstippling.h"
#include "tsp.h"
#include <iostream>
#include <vector>
extern "C" {
#include <concorde.h>
}

TSP::TSP () {

};

// Write .tsp file
void TSP::writeDataFile (const std::vector<Stipple> &stipples) {
  FILE *out = fopen (m_TSPSolver.name, "w");
  m_TSPSolver. ncount = stipples.size();

  fprintf (out, "NAME: concorde%d\n",m_TSPSolver. ncount);
  fprintf (out, "TYPE: TSP\n");
  fprintf (out, "DIMENSION: %d\n", m_TSPSolver.ncount);
  fprintf (out, "EDGE_WEIGHT_TYPE: EUC_2D\n");
  fprintf (out, "NODE_COORD_SECTION\n");
  for (int i = 0; i < m_TSPSolver.ncount; i ++) {
    fprintf(out, "%d %lf %lf\n", i, stipples[i].pos.x()*1000, stipples[i].pos.y()*1000);
  }
  fprintf(out, "EOF\n");
  fclose(out);
}

//Receive a symmetric 2D distance matrix (dist) and create a TSP optimal tour (tour)
//void solve(vector<vector<int> > * dist, vector<int> * tour){
std::tuple<std::vector<int>,double> TSP::solve(const std::vector<Stipple> &stipples) {

  int rval = 0; //Concorde functions return 1 if something fails
  double szeit; //Measure cpu time

  std::string str = "temp.tsp";
  m_TSPSolver.name = new char[str.size() + 1];
  std::copy(str.begin(), str.end(), m_TSPSolver.name);
  m_TSPSolver.name[str.size()] = '\0';

  // TODO: properly figure out Concorde's CCdatagroup format and
  // initialize this object directly instead of going via file.
  writeDataFile(stipples);

  CCrandstate rstate;
  int seed = rand();
  CCutil_sprand(seed, &rstate); //Initialize the portable random number generator
  m_TSPSolver.ncount = stipples.size(); //Number of nodes (cities)
  m_TSPSolver.out_tour = CC_SAFE_MALLOC (m_TSPSolver.ncount, int);


  CCdatagroup dat;
  //Initialize a CCdatagroup
  CCutil_init_datagroup (&dat);
  CCutil_gettsplib (m_TSPSolver.name, &m_TSPSolver.ncount, &dat);

  clock_t begin_time = clock();
  //Solves the TSP over the data specified in the datagroup
  rval = CCtsp_solve_dat (m_TSPSolver.ncount, &dat, m_TSPSolver.in_tour, m_TSPSolver.out_tour,
                              m_TSPSolver.in_val, &m_TSPSolver.optval, &m_TSPSolver.success,
                              &m_TSPSolver.foundtour, m_TSPSolver.name, m_TSPSolver.timebound,
                              &m_TSPSolver.hit_timebound, m_TSPSolver.silent, &rstate);

  clock_t end_time = clock();
  double solvetime = double(end_time - begin_time)/CLOCKS_PER_SEC*1000;
//  std::cout << "TIME: " << solvetime << " ms" << std::endl;

  std::vector<int> solution;
  solution.assign(m_TSPSolver.out_tour, m_TSPSolver.out_tour + m_TSPSolver.ncount);

  CC_IFFREE (m_TSPSolver.out_tour, int);

  return {solution,solvetime};
}

