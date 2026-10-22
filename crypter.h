#include <string>
#include <cstring>
#include "librandom--/random.h"


class Crypter {
	public: 
		Crypter(std::string password);
		std::string crypt(std::string input);
		std::string uncrypt(std::string input);
		void reset();
		~Crypter();
	private:
		std::string password;
		Random ** random;
		int random_number;
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

Crypter::~Crypter() {
	for (int i = 0 ; i < this->random_number ; i ++) {
		free(&(this->random[i]));
	}
}


void Crypter::init_randoms_by_password () {
	int max = ceil (this->password.length()/4.0);
	this->random = (Random**)malloc(sizeof(Random*)*max);
	
	this->random_number = max;
	
	for (int i = 1 ; i <=  max; i ++) {
		std::string p = this->password.substr(4*(i-1), 4);
		while (p.length() < 4) {
			p += ' ';
		}
		const char *s = p.c_str();
		int _t;
		memcpy(&_t, s, 4*sizeof(char));
		
		
		double t2 = _t ? 1.0 / _t : 0;
		
		Random *rnd = new Random(t2);
		
		rnd->rand();
		
		int y = _t % 256;
	
		for (int j = 0 ; j < y ; j ++ ) {
			rnd->rand();
		}
		
		char buff2[256];
		sprintf(buff2, "%.8g", rnd->get_seed());

		this->random[i-1] = new Random(atof(buff2));
	}
}


std::string Crypter::crypt(std::string input) {
	std::string out = "";
	for (int i = 0 ; i < input.length() ; i ++) {
		char chr = input[i];
		unsigned ord = (short)chr;
		
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
			short r = (short) this->random[j]->rand(256);
			ord ^= r;
			
		}
		out += (char) ord;		
	}
	
	return out;
}

std::string Crypter::uncrypt(std::string input) {
	std::string out = "";
	for (int i = 0 ; i < input.length() ; i ++) {
		char chr = input[i];
		unsigned ord = (short)chr;
		
		Random *r = this->random[this->n];
		if (++this->n >= this->random_number) {
			this->n = 0;
		}

		if (this->obfuscate) {
			if (this->uncrypt_obfuscate_overstep) {
				if (input.length() > this->uncrypt_obfuscate_overstep) {
					i += this->uncrypt_obfuscate_overstep;
					chr = input[i];
					ord = (short) chr;
				} else {
					this->uncrypt_obfuscate_overstep -= input.length();
					return out;
				}
			} else {
				unsigned nr = r->rand(this->obfuscate);
				for (int j = 1 ; j <= nr ; j ++) {
					out += (char) r->rand(256);
				}
				
				i += nr;
				if (i >= input.length()) {
					this->uncrypt_obfuscate_overstep += i - input.length();
					return out;
				}
				chr = input[i];
				ord = (short) chr;				
			}
		}
		
		for (int j = 0 ; j < this->random_number ; j ++) {
			short r = (short) this->random[j]->rand(256);
			ord ^= r;
			
		}
		out += (char) ord;		
	}
	
	return out;
}

void Crypter::reset() {
	this->n = 0;
	this->uncrypt_obfuscate_overstep = 0;
	this->init_randoms_by_password();
}
