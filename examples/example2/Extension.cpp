#include <extension_system/Extension.hpp>
#include "Interface.hpp"
#include <iostream>

class Extension : public Interface2
{
public:
	virtual void test1() override {
		std::cout<<"Hello from Interface2 Extension"<<std::endl;
	}
};

// export extension and add user defined metadata
EXTENSION_SYSTEM_EXTENSION(Interface2, Extension, "Example2Extension", 100, "Example 2 extension",
						   EXTENSION_SYSTEM_DESCRIPTION_ENTRY("author", "Alice Bobbens")
						   EXTENSION_SYSTEM_DESCRIPTION_ENTRY("vendor", "42 inc.")
						   EXTENSION_SYSTEM_DESCRIPTION_ENTRY("target_product", "example2")
						   )