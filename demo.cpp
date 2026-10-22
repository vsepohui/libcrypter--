#include <string>
#include <iostream>
#include "crypter.h"

int main () {
	Crypter * c = new Crypter("123456");
	std::cout << c->crypt("Hello") << std::endl;
}
