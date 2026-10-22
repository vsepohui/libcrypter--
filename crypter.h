#include <string>
#include <cstring>
#include "librandom--/random.h"

#include <iostream>

class Crypter {
	public: 
		Crypter(std::string password);
		std::string crypt(std::string input);
		std::string uncrypt(std::string input);
		void reset();
	private:
		std::string password;
		Random ** random;
		int random_number;
		std::string * seeds;
		int n;
		int obfuscate;
		int uncrypt_obfuscate_overstep;
		
		void init_randoms_by_password();
};



Crypter::Crypter(std::string password) {
	this->password = password;
	this->n = 0;
	this->obfuscate = 0;
	this->uncrypt_obfuscate_overstep = 0;
	this->init_randoms_by_password();
}


void Crypter::init_randoms_by_password () {
	int max = ceil (this->password.length()/4.0);
	this->seeds = (std::string*)malloc(sizeof(std::string)*max);
	this->random = (Random**)malloc(sizeof(Random*)*max);
	
	this->random_number = max;
	
	for (int i = 1 ; i <=  max; i ++) {
		std::string p = this->password.substr(4*(i-1), 4);
		while (p.length() < 4) {
			p += ' ';
		}
		const char *s = p.c_str();
		float t;
		memcpy(&t, s, 4*sizeof(char));
		
		
		
		char buff[256];
		sprintf(buff, "%.49f", t);

		std::cout << buff << " --- " << t << std::endl;		
		
		
		t = atof(buff);
		t = t ? 1 / t : 0;
		
		
		
		Random *rnd = new Random(t);
		
		unsigned y;
		memcpy(&y, s, 4);
		for (int j = 0 ; j <= y % 256 ; j ++ ) {
			rnd->rand();
		}
		
		sprintf(buff, "%.8f", rnd->get_seed());
		
		std::string seed = buff;
		
		this->seeds[i-1] = seed;
		this->random[i-1] = rnd;
	}
}

std::string Crypter::crypt(std::string input) {
	std::string out = "";
	for (int i = 0 ; i < input.length() ; i ++) {
		char chr = input[i];
		short ord = (short)chr;
		
		Random *r = this->random[this->n];
		if (++this->n >= this->random_number) {
			this->n = 0;
		}
		
		if (this->obfuscate) {
			unsigned nr = r->rand(this->obfuscate);
			for (int j = 1 ; j <= nr ; j ++) {
				out += (char) r->rand(256);
			}
		}
		
		for (int j = 0 ; j < this->random_number ; j ++) {
			ord ^= (short) this->random[j]->rand(256);
			
		}
		out += (char) ord;		
	}
	
	return out;
}
