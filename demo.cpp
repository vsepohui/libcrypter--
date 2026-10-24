#include <string>
#include <iostream>
#include "crypter++.h"

int main () {
	Crypter * c = new Crypter("123456");
	std::string crypted = c->crypt("Hello");
	c->reset();
	std::cout << c->uncrypt(crypted) << std::endl;
}
