#ifndef _MARKOV_HPP_    
#define _MARKOV_HPP_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include "Eigen/Dense"

class Markov {
    public:
        Markov(Eigen::MatrixXd tpm, Eigen::RowVectorXd initial_state);
        
        Eigen::MatrixXd get_tpm();
        Eigen::VectorXd get_initial_state();
        Eigen::VectorXd get_next_state(Eigen::RowVectorXd current_state);
        Eigen::VectorXd simulate(int steps);
        Eigen::VectorXd steady_state();
        std::vector<std::vector<int>> get_communication_classes();
        int get_period(int state);
        std::vector<int> get_periods();
    
    private:
        int N;
        Eigen::MatrixXd tpm;
        Eigen::VectorXd initial_state;
        std::vector<std::vector<int>> communication_classes;
        std::vector<int> periods;

        std::vector<std::vector<int>> _get_communication_classes();
        void _dfs(int v, std::vector<bool>& visited, std::vector<int>* scc, std::stack<int>* stack, const Eigen::MatrixXd& tpm);
        int Markov::_get_period(int state);
        std::vector<int> Markov::_get_periods();
};

#endif
