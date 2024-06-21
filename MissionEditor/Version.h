#pragma once

#define PRODUCT_VERSION_MAJOR 2
#define PRODUCT_VERSION_MINOR 1
#define PRODUCT_VERSION_REVISION 0

#if !defined(_str)
	#define _str(x) #x
#endif
#define str(x) _str(x)

#define PRODUCT_VERSION_STRING str(PRODUCT_VERSION_MAJOR) \
	"." str(PRODUCT_VERSION_MINOR) \
	"." str(PRODUCT_VERSION_REVISION)


