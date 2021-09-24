#include "GenerateurId.h"
#include <algorithm>
#include <iterator>
#include <numeric>
#include <random>
#include <limits>
#include <list>
#include <string>
#include <vector>
#include <fstream>

using namespace std;


struct GenerateurId::Impl {
	using value_type = GenerateurId::value_type;
	virtual value_type prendre() = 0;
	virtual void rendre(value_type) = 0;

	virtual ~Impl() = default;
};

class GenSequentiel : public GenerateurId::Impl {
	value_type cur{};

	value_type prendre() override {
		auto id = cur++;
		if (cur < numeric_limits<value_type>::max())
			return id;
		throw banque_vide{};

	}

	void rendre(value_type id) override {
		if (id >= cur) throw jamais_donne{};
	}
};

class GenRecycleur : public GenerateurId::Impl {
	value_type cur{};
	vector<value_type> recycles;

	value_type prendre() override {
		if (!recycles.empty()) {
			auto id = recycles.back();
			recycles.pop_back();
			return id;
		}
		return cur++;
	}

	void rendre(value_type id) override {

		if (id > cur) throw jamais_donne{};
		if (any_of(begin(recycles), end(recycles), [&](auto x) { return x == id; }))
			throw deja_rendu{};
		recycles.push_back(id);
	}
};


class GenAleatoir : public GenerateurId::Impl {

	vector<value_type> disponibles;
	mt19937 prng{ random_device{}() };

public:
	GenAleatoir() : disponibles(numeric_limits<value_type>::max()) {
		iota(begin(disponibles), end(disponibles), 0);
	}

private:


	value_type prendre() override {

		if (disponibles.empty()) throw banque_vide{};

		uniform_int_distribution<> de{ 0, static_cast<int>(disponibles.size()) - 1 };
		auto n = de(prng);
		auto id = disponibles[n];
		disponibles.erase(begin(disponibles) + n);
		return id;
	}

	void rendre(value_type id) override {
		if (any_of(begin(disponibles), end(disponibles), [&](auto x) { return x == id; }))
			throw deja_rendu{};

		disponibles.push_back(id);
	}
};

GenerateurId::GenerateurId(sequentiel_t) : p{ new GenSequentiel } {}
GenerateurId::GenerateurId(recycleur_t) : p{ new GenRecycleur } {}
GenerateurId::GenerateurId(aleatoire_t) : p{ new GenAleatoir } {}

GenerateurId::~GenerateurId() = default;

auto GenerateurId::prendre() -> value_type {
	return p->prendre();
}
void GenerateurId::rendre(value_type id) {
	p->rendre(id);
}




/*GenerateurId(sorte_gen sorte) {
	switch (sorte)
	{
	case sequentiel: p = new GenSequentiel; break;
	case recycleur: p = new GenRecycleur; break;
	case aleatoire: p = new GenAleatoir; break;
	default:
		throw sorte_invalide{};
	}
}*/

/*
*
template<class T> auto generateur(T init) {
	return [cur = init]() mutable { return cur++; };
}

//main
	//generateur de nombres
	auto gen = generateur(0.5);





*/

//enum sorte_gen{ sequentiel, recycleur, aleatoire};
//class sorte_invalide {};


