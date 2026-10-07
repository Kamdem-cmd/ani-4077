#include <iostream>

namespace MonRhi {
    struct EnregistreurAuto {
        EnregistreurAuto() {
            std::cout << "[MonRhi] MODULE D'ENREGISTREMENT AUTOMATIQUE INITIALISÉ !" << std::endl;
        }
    };

    // Variable globale : son constructeur doit s'exécuter avant d'entrer dans main()
    static EnregistreurAuto g_autoRegister;
}