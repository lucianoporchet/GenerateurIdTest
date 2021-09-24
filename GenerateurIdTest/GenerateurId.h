#pragma once

#include <memory>

class sequentiel_t {};
inline sequentiel_t sequentiel;
class recycleur_t {};
inline recycleur_t recycleur;
class aleatoire_t {};
inline aleatoire_t aleatoire;


class banque_vide {};
class jamais_donne {};
class deja_rendu {};

class GenerateurId {

public:
	struct Impl;
private:
	std::unique_ptr<Impl> p;
public:
	using value_type = unsigned short;

	GenerateurId(sequentiel_t);
	GenerateurId(recycleur_t);
	GenerateurId(aleatoire_t);

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
	~GenerateurId();
	value_type prendre();
	void rendre(value_type);
};
