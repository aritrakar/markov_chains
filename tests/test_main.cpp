#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "../src/markov.hpp"

template<typename T>
bool AreInnerVectorsEqualIgnoringOrder(const std::vector<T>& vec1, const std::vector<T>& vec2) {
    if (vec1.size() != vec2.size()) {
        return false;
    }

    std::vector<T> sortedVec1 = vec1; // Copy because we'll sort these
    std::vector<T> sortedVec2 = vec2;
    std::sort(sortedVec1.begin(), sortedVec1.end());
    std::sort(sortedVec2.begin(), sortedVec2.end());

    return sortedVec1 == sortedVec2;
}

template<typename T>
bool AreVectorsOfVectorsEqualIgnoringOrder(const std::vector<std::vector<T>>& vecOfVec1, const std::vector<std::vector<T>>& vecOfVec2) {
    if (vecOfVec1.size() != vecOfVec2.size()) {
        return false;
    }

    std::vector<bool> matched(vecOfVec2.size(), false); // Track matched inner vectors in vecOfVec2

    for (const auto& innerVec1 : vecOfVec1) {
        bool foundMatch = false;
        for (size_t i = 0; i < vecOfVec2.size(); ++i) {
            if (!matched[i] && AreInnerVectorsEqualIgnoringOrder(innerVec1, vecOfVec2[i])) {
                matched[i] = true; // Mark as matched
                foundMatch = true;
                break;
            }
        }
        if (!foundMatch) {
            return false; // No matching inner vector found for an innerVec1
        }
    }

    // Ensure all inner vectors in vecOfVec2 are matched
    return std::all_of(matched.begin(), matched.end(), [](bool match) { return match; });
}

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

// ---------------------------------------------------------------------

// Read a transition probability matrix
TEST(MarkovTest, ReadTransitionMatrix) {
    Eigen::MatrixXd transition_matrix = read_tpm_file("../etc/tpm1.txt");
    Eigen::Matrix3d expected;
    expected << 0.2, 0.3, 0.5,
                0.4, 0.1, 0.5,
                0.3, 0.4, 0.3;
    ASSERT_TRUE(transition_matrix.isApprox(expected, 1e-6));
}

// Test the communication classes
TEST(MarkovTest, CommunicationClasses) {
    Eigen::MatrixXd transition_matrix = read_tpm_file("../etc/tpm1.txt");
    Eigen::RowVectorXd initial_state(3);
    initial_state << 1, 0, 0;
    Markov markov(transition_matrix, initial_state);
    std::vector<std::vector<int>> communication_classes = markov.get_communication_classes();
    std::vector<std::vector<int>> expected = {{0, 1, 2}};

    ASSERT_TRUE(AreVectorsOfVectorsEqualIgnoringOrder(communication_classes, expected));
}
