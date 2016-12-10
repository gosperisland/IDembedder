#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <cassert>
#include <exception>
#include <time.h>
#include "armadillo"
#include "IDpair.hpp"
//#include "Learning.hpp"
//#include "GridGroup.hpp"
#include <ctime>

using namespace std;
using namespace arma;

#define EPOCH_TIMES 3
static int prunePairsFactor = 1;


std::vector<double> init(const std::vector<std::vector<double> >& examples,
		const std::vector<std::vector<size_t> >& indecies_of_pairs,
		const std::vector<short>& tags,
		const std::vector<std::vector<double> >& discrete_points,
		const double C, double& thold);

void SGD_similar(std::vector<double>& W, const std::vector<double>& Wreg,
		const std::vector<Pair>& volume, short tag, double & thold,
		const double C, size_t etha) {

	std::vector<double> W_old(W);
	size_t size = W.size();
	size_t sparse_size = volume.size();
	double dotProd = 0;

	//ID(X_pi_1, X_pi_2) * W
	for (size_t i = 0; i < sparse_size; i++)
		dotProd += volume[i]._weight * W[volume[i]._index];

	// 1 - { (ID(X_pi_1, X_pi_2) * W) - threshold } * y_i
	if ((1 - ((dotProd - thold) * tag)) > 0) {
		for (auto& simplex_point : volume) {
			//cout<<"debug simplex point"<<simplex_point._index/11<<","<<simplex_point._index%11<<endl;
			W[simplex_point._index] += (1.0 / (double) etha)
					* (C * tag * simplex_point._weight);
			W[simplex_point._index] =
					W[simplex_point._index] < 0 ? 0 : W[simplex_point._index];
		}
		//thold -= ((1.0 / (double) etha) * (C * tag));
	}

	for (size_t i = 0; i < size; i++) {
		W[i] -= (1.0 / (double) etha) * ((W_old[i] - Wreg[i]));
		W[i] = W[i] < 0 ? 0 : W[i];

	}
	//cout<<"debug W"<<W<<endl;
}

std::vector<double> learn_similar(
		const std::vector<std::vector<double> >& examples, const IDpair& idpair,
		const std::vector<std::vector<size_t> >& indecies_of_pairs,
		const std::vector<short>& tags, const std::vector<double>& Wreg,
		const double C, double& thold) {

	assert(tags.size() == indecies_of_pairs.size());

	size_t W_size = idpair.get_num_of_vertices();

	std::vector<double> W(Wreg.size(), 0);
	assert(Wreg.size() == W.size());

	size_t num_of_pairs = indecies_of_pairs.size();

	for (int j = 0; j < EPOCH_TIMES; ++j) {

		std::vector<int> random_indexes(num_of_pairs);
		std::iota(std::begin(random_indexes), std::end(random_indexes), 0); // Fill with 0, 1, ..., n.
		std::random_shuffle(random_indexes.begin(), random_indexes.end());

		for (size_t i = 0; i < num_of_pairs / prunePairsFactor; i++) {

			//get random index
			size_t random_index = random_indexes.back();
			//cout<<random_index<<endl; consistant
			random_indexes.pop_back();

			const std::vector<Pair>& volume = idpair(
					examples[indecies_of_pairs[random_index][0]],
					examples[indecies_of_pairs[random_index][1]]);

			SGD_similar(W, Wreg, volume, tags[random_index], thold, C, i + 1);
		}

	}

	cout << "W.size(): " << W.size() << " \nW:" << endl;
	for (int i = 0; i < W.size(); i++) {
		cout << W[i] << " , ";
		if ((i + 1) % (int) sqrt(W.size()) == 0) {
			cout << endl;
		}
	}
	//cout << W << endl;

	return Wreg;
}

void sanityTest1Dim() {

}

int main() {

	return 0;
}
