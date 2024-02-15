#include <queue>
#include <utility>
#include <map>

#include "markov.hpp"

Markov::Markov(Eigen::MatrixXd tpm, Eigen::RowVectorXd initial_state) {
    this->N = tpm.rows();
    this->tpm = tpm;
    this->initial_state = initial_state;
}

Eigen::MatrixXd Markov::get_tpm() {
    return this->tpm;
}

Eigen::VectorXd Markov::get_initial_state() {
    return this->initial_state;
}

Eigen::VectorXd Markov::get_next_state(Eigen::RowVectorXd current_state) {
    return current_state * this->tpm;
}

Eigen::VectorXd Markov::simulate(int steps) {
    Eigen::VectorXd state = this->initial_state;
    for (int i = 0; i < steps; ++i) {
        state = state * this->tpm;
    }
    return state;
}

Eigen::VectorXd Markov::steady_state() {
    Eigen::EigenSolver<Eigen::MatrixXd> es(this->tpm);
    Eigen::VectorXd steady_state = es.eigenvectors().col(0).real();
    return steady_state / steady_state.sum();
}

std::vector<std::vector<int>> Markov::get_communication_classes() {
    if (this->communication_classes.empty()) {
        this->communication_classes = this->_get_communication_classes();
    }   
    return this->communication_classes;
}

void Markov::_dfs(int v, std::vector<bool>& visited, std::vector<int>* scc, std::stack<int>* stack, const Eigen::MatrixXd& tpm) {
    visited[v] = true;
    for (int i = 0; i < this->N; ++i) {
        if (tpm(v, i) > 0 && !visited[i]) {
            this->_dfs(i, visited, scc, stack, tpm);
        }
    }
    // For the first pass, use the stack to track the finish order
    if (stack != nullptr) {
        stack->push(v);
    }
    // For the second pass, collect the SCC members
    if (scc != nullptr) {
        scc->push_back(v);
    }
}

/*
We can treat the Markov chain as a graph. In that case, finding the
communication classes is equivalent to finding the strongly connected
components. We can use Kosaraju's algorithm to find the SCCs by treating
the transition probability matrix as an adjacency matrix.
*/
std::vector<std::vector<int>> Markov::_get_communication_classes() {
    std::vector<std::vector<int>> communication_classes;
    std::vector<bool> visited(this->N, false);
    std::stack<int> stack;

    // First pass
    for (int i = 0; i < this->N; ++i) {
        if (!visited[i]) {
            this->_dfs(i, visited, nullptr, &stack, this->tpm);
        }
    }

    // Transpose the TPM
    Eigen::MatrixXd tpmTransposed = this->tpm.transpose();

    // Second DFS pass
    // Reset the visited vector
    std::fill(visited.begin(), visited.end(), false);
    
    // Note that the stack now contains the finish order of the first pass
    while (!stack.empty()) {
        int v = stack.top();
        stack.pop();
        if (!visited[v]) {
            std::vector<int> scc;
            this->_dfs(v, visited, &scc, nullptr, tpmTransposed);
            communication_classes.push_back(scc);
        }
    }

    return communication_classes;
}

int gcd(int a, int b) {
    int c = 0;
    while (b != 0) {
        c = a % b;
        a = b;
        b = c;
    }
    return a;
}

int Markov::get_period(int state) {
    if (this->periods.empty()) {
        this->periods = this->_get_periods();
    }
    return this->periods[state];
}

int Markov::_get_period(int state) {
    std::queue<std::pair<int, int>> q;
    q.emplace(std::make_pair(state, 0));

    std::map<int, int> visited;
    visited[state] = 0;
    std::vector<int> periods_of_this_state;

    while (!q.empty()) {
        int current_state = q.front().first;
        int current_depth = q.front().second;
        q.pop();

        for (int i = 0; i < this->N; ++i) {
            // If there is a probability to transition to state i
            if (this->tpm(current_state, i) > 0) {
                if (visited.find(i) == visited.end()) {
                    // If we haven't visited this state yet, visit it
                    visited[i] = current_depth + 1;
                    q.emplace(std::make_pair(i, current_depth + 1));
                } else {
                    // Otherwise, increase the period/depth and store it
                    periods_of_this_state.push_back(current_depth - visited[i] + 1);
                }
            }
        }
    }

    // Now, we calculate the GCD of all the periods
    int period = periods_of_this_state[0];
    for (int i = 1; i < periods_of_this_state.size(); ++i) {
        period = gcd(period, periods_of_this_state[i]);
    }

    return period;
}

std::vector<int> Markov::get_periods() {
    return this->periods;
}

std::vector<int> Markov::_get_periods() {
    std::vector<int> periods(this->N, 1);
    
    // Since communication classes share the same period,
    // we can calculate the period for each state in the
    // communication class and then assign the period to
    // all the states in the communication class.
    if (this->communication_classes.empty()) {
        throw std::runtime_error("No communication classes found!");
    }

    for (auto& scc : this->communication_classes) {
        int period = this->_get_period(scc[0]);
        for (auto& state : scc) {
            periods[state] = period;
        }
    }

    return periods;
}
