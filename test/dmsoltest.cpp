
#include "dmsol.h"

int main( int argc, char* argv[] ) {

    Idmsol* module = dmsolGetModule();
    if (module)
    {
        module->Test();
        module->Release();
    }
    return 0;
}
