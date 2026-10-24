#include <string>
#include "librandom--/random++.h"


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
		int n;
		int obfuscate;
		int uncrypt_obfuscate_overstep;
		
		void init_randoms_by_password();
};


