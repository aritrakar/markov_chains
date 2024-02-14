headers = markov.hpp
files = markov.cpp main.cpp

mc: $(files) $(headers)
	g++ -O3 -std=c++17 -o markov $(files)
