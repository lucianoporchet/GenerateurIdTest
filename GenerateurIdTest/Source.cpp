#include <iostream>
#include "GenerateurId.h"

using namespace std;


int main() {


	GenerateurId gen{ aleatoire };
	cout << gen.prendre() << '\n';
	cout << gen.prendre() << '\n';
	cout << gen.prendre() << '\n';
	cout << gen.prendre() << '\n';
	cout << gen.prendre() << '\n';

}

