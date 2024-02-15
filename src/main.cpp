#include <iostream>
#include <fstream>
#include <sstream>

#include "markov.hpp"

Eigen::MatrixXd read_tpm_file(std::string tpm_filepath) {
    // Read the TPM file
    std::ifstream tpm_file (tpm_filepath);

    // Construct the NxN TPM matrix
    int N = 0;
    std::string line;
    while (std::getline(tpm_file, line)) {
        ++N;
    }

    // Reset the file
    tpm_file.clear();
    tpm_file.seekg(0, std::ios::beg);

    // Construct the TPM matrix
    Eigen::MatrixXd tpm(N, N);
    int i = 0;
    while (std::getline(tpm_file, line)) {
        std::istringstream iss(line);
        int j = 0;
        while (iss) {
            std::string token;
            iss >> token;
            if (token != "") {
                tpm(i, j) = std::stod(token);
                ++j;
            }
        }
        ++i;
    }

    return tpm;
}

// Some testing code.
int main(int argc, char* argv[]) {
    std::cout << "Reading TPM file: " << argv[1] << std::endl;
    Eigen::MatrixXd tpm = read_tpm_file(argv[1]);
    std::cout << tpm << std::endl;

    // Initialize Markov chain
    Eigen::RowVectorXd initial_state(4);
    initial_state << 1, 0, 0, 0;

    Markov markov(tpm, initial_state);

    // std::cout << "Initial state: " << markov.get_initial_state() << std::endl;
    // std::cout << "Next state: " << markov.get_next_state(initial_state) << std::endl;

    // Get communication classes
    std::cout << "Communication classes: " << std::endl;
    std::vector<std::vector<int>> communication_classes = markov.get_communication_classes();
    for (auto& scc : communication_classes) {
        for (auto& state : scc) {
            std::cout << state << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
