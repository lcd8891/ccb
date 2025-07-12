#include "gtk_app.hpp"
#include "builder.hpp"

namespace ccb{
    void initialize(int agrc, char* argv[]){
        gtk_init(&agrc,&argv);
        Builder::Start();
        gtk_main();
    }
    void finalize(){
        Builder::stop();
    }
}
