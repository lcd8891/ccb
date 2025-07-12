#include "gtk_app.hpp"

int main(int argc, char* argv[]){
    ccb::initialize(argc,argv);
    ccb::finalize();
    return 0;
}